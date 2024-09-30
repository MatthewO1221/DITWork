// ---------------------------------------------------------------------------
// Project Name		:	project
// File Name		:	AABB.c
// Author			:	Brody Endstrasser
// Creation Date	:	01-21-22
// Purpose			:	Demonstrates Alpha Engine features.
// History			:
// - 2012/01/21		:	- initial implementation
// - 
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// includes
#include "stdafx.h"		// Should be included first in every .c file!
#include "AEEngine.h"
#include "AABB.h"

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

void FreeAABB(AABB aabb)
{
	if (aabb)
	{
		free(aabb);
	}
}

AABB CreateAABBFromTile(tileInst tile)
{
	AABB newCol = malloc(sizeof(struct collisionData));

	tile->aabb = newCol;

	UpdateTileCollision(tile);
	
	return newCol;
}

AABB FromPoints(float left, float right, float top, float bottom)
{
	AABB newAABB = calloc(1, sizeof(struct collisionData));

	if (newAABB)
	{
		newAABB->left = left;
		newAABB->top = top;
		newAABB->right = right;
		newAABB->bottom = bottom;

		return newAABB;
	}
	else
	{
		return NULL;
	}
}

void UpdateTileCollision(tileInst tile)
{
	if (tile) {

		if (tile->vy1 + tile->ypos > tile->vy2 + tile->ypos)
			tile->aabb->top = tile->vy1 + tile->ypos;
		else
			tile->aabb->top = tile->vy2 + tile->ypos;

		if (tile->vx2 + tile->xpos > tile->vx3 + tile->xpos)
			tile->aabb->right = tile->vx2 + tile->xpos;
		else
			tile->aabb->right = tile->vx3 + tile->xpos;

		if (tile->vy3 + tile->ypos < tile->vy4 + tile->ypos)
			tile->aabb->bottom = tile->vy3 + tile->ypos;
		else
			tile->aabb->bottom = tile->vy4 + tile->ypos;

		if (tile->vx4 + tile->xpos < tile->vx1 + tile->xpos)
			tile->aabb->left = tile->vx4 + tile->xpos;
		else
			tile->aabb->left = tile->vx1 + tile->xpos;


	}

}

int AABBPointContained(AABB col, float _x, float _y)
{
	if (_y <= col->top &&
		_y >= col->bottom &&
		_x <= col->right &&
		_x >= col->left)
		return 1;
	else
		return 0;

}

int CheckAABBCollision(AABB t1, AABB t2)
{
	if (t1 == NULL || t2 == NULL)
		return 0;

	if (t1->left   <   t2->right &&
		t1->right  >   t2->left &&
		t1->bottom <   t2->top &&
		t1->top    >   t2->bottom)
		return 1;
	else
		return 0;	
}

enum collision_direction CheckCollisionDirect(AABB prime, AABB t)
{

	if (CheckAABBCollision(prime, t)) {

		float midprimex = (prime->top + prime->bottom) / 2;
		float midprimey = (prime->left + prime->right) / 2;

		float midtx = (t->top + t->bottom) / 2;
		float midty = (t->left + t->right) / 2;

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

