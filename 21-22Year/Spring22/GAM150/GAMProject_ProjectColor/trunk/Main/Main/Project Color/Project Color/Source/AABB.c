// ---------------------------------------------------------------------------
// Project Name		:	project
// File Name		:	AABB.c
// Author			:	Brody Endstrasser
// Creation Date	:	01-21-22
// Purpose			:	Source code for all collision functions
// History			:
// - 2012/01/21		:	- initial implementation
// - 
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// includes
#include "stdafx.h"		// Should be included first in every .c file!
#include "AEEngine.h"

#include "GameplayInstance.h"

#include "AABB.h"
#include "Transform.h"
#include "Level.h"
#include "Tile.h"
#include "GameObject.h"
#include "Debug.h"
#include "Paint.h"
#include "Physics.h"
#include "Player Movement.h"


tileInst DefineSquareMesh(AEGfxVertexList* mesh, float width, float height)
{
	/*x1, y1: top left coordinate
	x2, y2: top right coordinate
	x3, y3: bottom right coordinate
	x4, y4: bottom left coordinate*/
	float halfwidth = width / 2;
	float halfheight = height / 2;
	f32 x1, y1, x2, y2, x3, y3, x4, y4;

	x1 = -halfwidth;
	x4 = -halfwidth;
	x2 = halfwidth;
	x3 = halfwidth;

	y1 = halfheight;
	y2 = halfheight;
	y3 = -halfheight;
	y4 = -halfheight;

	// 0xAARRGGBB - alpha, red green blue
	AEGfxMeshStart();
	// This shape has 2 triangles
	AEGfxTriAdd(
		/*-30.0f, -30.0f*/x4, y4, 0x00FF00FF, 0.0f, 1.0f,
		/*30.0f, -30.0f*/x3, y3, 0x00FFFF00, 1.0f, 1.0f,
		/*-30.0f, 30.0f */x1, y1, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		/*30.0f, -30.0f*/x3, y3, 0x00FFFFFF, 1.0f, 1.0f,
		/*30.0f, 30.0f*/ x2, y2, 0x00FFFFFF, 1.0f, 0.0f,
		/*-30.0f, 30.0f*/x1, y1, 0x00FFFFFF, 0.0f, 0.0f);

	mesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(mesh, "Failed to create rectangle mesh!!");

	tileInst newTile = malloc(sizeof(struct tileData));
	if (newTile) {
		newTile->vx1 = x1;
		newTile->vx2 = x2;
		newTile->vx3 = x3;
		newTile->vx4 = x4;

		newTile->vy1 = y1;
		newTile->vy2 = y2;
		newTile->vy3 = y3;
		newTile->vy4 = y4;

		newTile->xpos = 0;
		newTile->ypos = 0;

		newTile->Mesh = mesh;
	}
	else
		return NULL;
	
	return newTile;

}

void FreeTileInst(tileInst tile)
{
	if (tile->aabb)
		free(tile->aabb);

	if (tile) {
		AEGfxMeshFree(tile->Mesh);
		free(tile);
	}

}

AABB FreeAABB(AABB aabb)
{
	if (aabb)
	{
		free(aabb);
	}

	aabb = NULL;
	return NULL;
}

void updateAABBPosition(AABB aabb, PhysicsPtr phys)
{
	if (!aabb || !phys)
		return; 

	aabb->WorldPos = *PhysicsGetOldTranslation(phys);
}

AABB CreateAABBFromTile(tileInst tile)
{
	AABB newCol = malloc(sizeof(struct collisionData));

	tile->aabb = newCol;

	UpdateTileCollision(tile);
	
	return newCol;
}

AABB FromPoints(Vector2D pos, float w, float h, float x_, float y_, bool isTrigger)
{
	AABB newAABB = calloc(1, sizeof(struct collisionData));

	if (newAABB)
	{
		newAABB->WorldPos = pos;
		newAABB->width = w;
		newAABB->height = h;
		newAABB->xOff = x_;
		newAABB->yOff = y_;
		newAABB->isTrigger = isTrigger;

		return newAABB;
	}
	else
	{
		return NULL;
	}
}

void UpdateTileCollision(tileInst tile)
{
	if (tile)
	{

		if (tile->vy1 + tile->ypos > tile->vy2 + tile->ypos)
			tile->aabb->WorldPos.y = tile->vy1 + tile->ypos;
		else
			tile->aabb->WorldPos.y = tile->vy2 + tile->ypos;

		if (tile->vx4 + tile->xpos < tile->vx1 + tile->xpos)
			tile->aabb->WorldPos.x = tile->vx4 + tile->xpos;
		else
			tile->aabb->WorldPos.x = tile->vx1 + tile->xpos;

	}

}

int AABBPointContained(AABB col, float _x, float _y)
{
	if (_y <= col->WorldPos.y + col->height &&
		_y >= col->WorldPos.y &&
		_x <= col->WorldPos.x + col->width &&
		_x >= col->WorldPos.x)
		return 1;
	else
		return 0;

}

int CheckAABBCollision(AABB t1, AABB t2)
{
	if (t1 == NULL || t2 == NULL)
		return 0;

	return (t1->WorldPos.x + t1->width >= t2->WorldPos.x &&
			t1->WorldPos.x <= t2->WorldPos.x + t2->width &&
			t1->WorldPos.y + t1->height >= t2->WorldPos.y &&
			t1->WorldPos.y <= t2->WorldPos.y + t2->height);
}

enum collision_direction CheckCollisionDirect(AABB prime, AABB t)
{

	if (CheckAABBCollision(prime, t))
	{

		//float midprimex = (prime->left + prime->right) / 2;
		float midprimex = (prime->WorldPos.x + (prime->WorldPos.x + prime->width)) / 2;
		float midprimey = (prime->WorldPos.y + (prime->WorldPos.y - prime->height)) / 2;

		float midtx = (t->WorldPos.x + (t->WorldPos.x + t->width)) / 2;
		float midty = (t->WorldPos.y + (t->WorldPos.y - t->height)) / 2;

		if (midprimex < midtx)
			return Right;
		else if (midprimex > midtx)
			return Left;

		if (midprimey < midty)
			return Top;
		else if (midprimey > midty)
			return Bottom;

	}
	return Not;
}

int AABBX(AABB col) {
	return (int)(col->WorldPos.x + 768) / TILE_SIZE;
}

int AABBY(AABB col) {
	return (int)(448 - col->WorldPos.y + (GetCurrentTilemap()->height - MIN_LEVEL_HEIGHT + 1) * TILE_SIZE) / TILE_SIZE;
}

float AABBgetXDistance(AABB a, AABB b)
{
	//x coordinate for a
	int ax = AABBX(a);
	//x coordinate for b
	int bx = AABBX(b);

	if (ax < bx) {
		return b->WorldPos.x - a->WorldPos.x;
	}
	else if (ax > bx) {
		return a->WorldPos.x - b->WorldPos.x;
	}

	return 0;
}

float AABBgetYDistance(AABB a, AABB b)
{
	//y coordinate for a
	int ay = AABBY(a);
	//y coordinate for b
	int by = AABBY(b);

	if (ay < by) {
		return b->WorldPos.y - a->WorldPos.y;
	}
	else if (ay > by) {
		return a->WorldPos.y - b->WorldPos.y;
	}

	return 0;
}

int AABBReturnColNumber(AABB col)
{
	GameplayInstance* game = GetGame();
	int count = 0;

	//height
	for (int y = 0; y < GetCurrentTilemap()->height; ++y)
	{
		//width
		for (int x = 0; x < GetCurrentTilemap()->width; ++x)
		{
			if (CheckAABBCollision(col, game->tileCollisions[x][y]) == 1 &&
				!GetCurrentTilemap()->tiles[x][y]->isTrigger)
				++count;
		}
	}

	return count;

}

GameObjectPtr ReturnGameObjectAABBCollision(AABB col)
{
	for (int i = 0; i < GetGame()->gameObjectCount; ++i)
	{
		if (GetGame()->gameObjects[i])
		{
			AABB collider = GameObjectGetCollider(GetGame()->gameObjects[i]);
			if (collider && CheckAABBCollision(col, GameObjectGetCollider(GetGame()->gameObjects[i])))
			{
				return GetGame()->gameObjects[i];
			}
		}
	}

	return NULL;
}

int AABBReturnColNumberinRange(GameObjectPtr parent, AABB col)
{
	GameplayInstance* game = GetGame();
	int count = 0;

	int x_pos = AABBX(col);
	int y_pos = AABBY(col);

	TransformPtr transform = GameObjectGetTransform(parent);

	for (int y = (y_pos - 2); y <= (y_pos + 2); ++y)
	{
		//3 tiles left and right
		for (int x = (x_pos - 2); x <= (x_pos + 2); ++x)
		{
			if (x < 0 ||
				y < 0 ||
				x >= GetCurrentTilemap()->width ||
				y >= GetCurrentTilemap()->height ||
				!GetCurrentTilemap()->tiles[x][y]->hasCollider)
				continue;

			float objectY = TransformGetTranslation(transform)->y;

			if (GetCurrentTilemap()->tiles[x][y]->id == TILE_SEMI_SOLID_PLATFORM)
			{
				PhysicsPtr physics = GameObjectGetPhysics(parent);

				if (physics != NULL &&
					PhysicsGetVelocity(physics)->y > 0)
				{
					continue;
				}
			}

#if DEBUG_MODE
			// Call the tile's update function if it exists.
			DrawBoundingBox(GetGame()->tileCollisions[x][y]);
#endif

			if (CheckAABBCollision(col, game->tileCollisions[x][y]) == 1)
			{
				if (!GetCurrentTilemap()->tiles[x][y]->isTrigger)
				{
					switch (game->tileColors[x][y])
					{
						case PAINT_ORANGE: {
							PhysicsPtr physics = GameObjectGetPhysics(parent);
							Vector2D newVel = *PhysicsGetVelocity(physics);

							OverrideVelocity(true);

							newVel.y *= -0.75F;
							PhysicsSetVelocity(physics, &newVel);
						} break;

						case PAINT_PURPLE: {
							float rotation = (float)game->tileNormals[x][y] * (PI / 2);
							Vector2D newGrav = { 0, 0 };

							Vector2DFromAngleRad(&newGrav, rotation);
							Vector2DScale(&newGrav, &newGrav, -1500);

							SetGravity(&newGrav);
							TransformSetRotation(transform, rotation - (PI / 2));
						} break;
					}

					++count;
				}

				if (GetCurrentTilemap()->tiles[x][y]->onEnterCollision != NULL)
				{
					GetCurrentTilemap()->tiles[x][y]->onEnterCollision(x, y);
				}
			}
		}
	}

	return count;

}

AABB** ReturnAABBCollisions(AABB col)
{
	//this scans all tiles in gameplayInstance, so be careful.
	//i'll probably make another function like this for the player that only scans a couple tiles around them down the line when we need it
	int colNum = AABBReturnColNumber(col);
	GameplayInstance* game = GetGame();

	if (colNum == 0)
		return NULL;

	AABB** colisions = (AABB**)malloc(colNum * sizeof(AABB*));

	//for everything that the AABB colides with
	for (int i = 0; i < colNum; ++i) {

		if (colisions) {
			//height
			for (int y = 0; y < GetCurrentTilemap()->width; ++y)
			{
				//width
				for (int x = 0; x < GetCurrentTilemap()->height; ++x)
				{
					if (game->tileCollisions[x][y] != NULL &&
						CheckAABBCollision(col, game->tileCollisions[x][y]) == 1) {

						if (GetCurrentTilemap()->tiles[x][y]->onEnterCollision != NULL)
						{
							GetCurrentTilemap()->tiles[x][y]->onEnterCollision(x, y);
						}
						colisions[i] = &game->tileCollisions[x][y];
					}
				}
			}
		}	
	}

	return colisions;

}

AABB ReturnAABBCollisionsInRange(AABB col, int* colCount)
{
	//int colNum = AABBReturnColNumber(col);
	GameplayInstance* game = GetGame();

	int x_pos = AABBX(col);
	int y_pos = AABBY(col);

	//if (colNum == 0)
		//return NULL;

	AABB collisions = (AABB)calloc(16, sizeof(struct collisionData));

	//for everything that the AABB colides with
	//for (int i = 0; i < colNum; ++i) {

		if (collisions) {
			int colNum = 0;
			//height
			//3 tiles above and below the x and y midpoint for col
			for (int y = (y_pos - 2); y <= (y_pos + 2); ++y)
			{
				//3 tiles left and right
				for (int x = (x_pos - 2); x <= (x_pos + 2); ++x)
				{
					if (x >= 0 && x <= GetCurrentTilemap()->width && y >= 0 && y <= GetCurrentTilemap()->height &&
						CheckAABBCollision(col, game->tileCollisions[x][y]) == 1 &&
						!GetCurrentTilemap()->tiles[x][y]->isTrigger) {

						if (GetCurrentTilemap()->tiles[x][y]->onEnterCollision != NULL)
						{
							GetCurrentTilemap()->tiles[x][y]->onEnterCollision(x, y);
						}
						collisions[colNum] = *game->tileCollisions[x][y];
						++colNum;
					}
				}
			}

			*colCount = colNum;
			return collisions;
		}
	//}

	*colCount = 0;
	return NULL;

}

AABB* BoxCast(AABB col, int dir)
{
	AABB* List = (AABB*)calloc(5, sizeof(AABB));

	if (List) {
		List[0] = col;

		//make it easier to type
		float colx = col->WorldPos.x;
		float coly = col->WorldPos.y;


		Vector2D LeftPos = { 0, 0 };
		Vector2D RightPos = { 0, 0 };
		Vector2D TopPos = { 0, 0 };
		Vector2D BottomPos = { 0, 0 };

		switch (dir)
		{
			// down
			case 4:
				LeftPos.x = colx - col->width * 0.05F;
				LeftPos.y = coly;
				RightPos.x = colx + col->width * 0.75F;
				RightPos.y = coly;
				TopPos.x = colx;
				TopPos.y = coly + col->height * 0.5F;
				BottomPos.x = colx;
				BottomPos.y = coly - col->height * 0.265F;

				//left
				List[Right] = FromPoints(LeftPos, BOX_SIZE, col->height * 0.5F, 0, 0, false);
				//right
				List[Left] = FromPoints(RightPos, BOX_SIZE, col->height * 0.5F, 0, 0, false);
				//top
				List[Bottom] = FromPoints(TopPos, col->width * 0.5F, BOX_SIZE, 0, 0, false);
				//bottom
				List[Top] = FromPoints(BottomPos, col->width * 0.5F, BOX_SIZE, 0, 0, false);
				break;

			// up.
			default:
				LeftPos.x = colx - col->width * 0.05F;
				LeftPos.y = coly;
				RightPos.x = colx + col->width * 0.75F;
				RightPos.y = coly;
				TopPos.x = colx;
				TopPos.y = coly + col->height * 0.5F;
				BottomPos.x = colx;
				BottomPos.y = coly - col->height * 0.265F;

				//left
				List[Left] = FromPoints(LeftPos, BOX_SIZE, col->height * 0.5F, 0, 0, false);
				//right
				List[Right] = FromPoints(RightPos, BOX_SIZE, col->height * 0.5F, 0, 0, false);
				//top
				List[Top] = FromPoints(TopPos, col->width * 0.5F, BOX_SIZE, 0, 0, false);
				//bottom
				List[Bottom] = FromPoints(BottomPos, col->width * 0.5F, BOX_SIZE, 0, 0, false);
				break;
		}
		List[Not] = col;

		return List;
	}
	else
		return NULL;

}

void FreeBoxCast(AABB* ar)
{
	for (int i = 1; i < 5; ++i)
		free(ar[i]);

	free(ar);
}

void FreeAABBarray(AABB** ar)
{
	free(ar);

}

