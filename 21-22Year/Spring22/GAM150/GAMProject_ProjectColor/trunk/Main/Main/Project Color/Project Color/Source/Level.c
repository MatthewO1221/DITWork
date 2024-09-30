#include "stdafx.h"		// Should be included first in every .c file!
#include "Level.h"
#include "Tile.h"
#include "GameplayInstance.h"
#include "GameObject.h"
#include "AABB.h"
#include "Debug.h"
#include "Mesh.h"
#include "Paint.h"
#include "EnemyAI.h"
#include "Vector2D.h"
#include <math.h>

static Level currLevel;
static Level nextLevel;

// Draws a level.
void DrawLevel(const Tilemap* tilemap)
{
	if (tilemap == NULL)
		return;

	// Iterate through y dimension.
	for (int y = 0; y < tilemap->height + 1; ++y)
	{
		// Iterate through x dimension.
		for (int x = 0; x < tilemap->width + 1; ++x)
		{
			// Convert tile position to screen position.
			long screenPosX, screenPosY;
			TileToScreenPos((unsigned char)x, (unsigned char)y, &screenPosX, &screenPosY);

			// Set the tile based on the tiles in the tile list.
			DrawTile(tilemap->tiles[x][y],
					(float)screenPosX + tilemap->tileOffsets[x][y].x,
					(float)screenPosY + tilemap->tileOffsets[x][y].y + (tilemap->height * TILE_SIZE) - ((MIN_LEVEL_HEIGHT - 1) * TILE_SIZE),
					1.0F);
		}
	}
}

// Sets all the level's values back to defaults.
void ResetLevel(Level* level, Tilemap* tilemap, int* worldNum, const char* name)
{
	if (tilemap == NULL || level == NULL || worldNum == NULL)
		return;

	// Set level to default values.
	tilemap->width = MIN_LEVEL_WIDTH;
	tilemap->height = MIN_LEVEL_HEIGHT;

	// Increment level count.
	level->levelNum = *worldNum++;

	// Set level name.
	strcpy_s(level->filePath, sizeof(level->filePath), name);

	// Iterate through all tiles.
	for (int y = 0; y < MAX_LEVEL_HEIGHT; ++y)
	{
		for (int x = 0; x < MAX_LEVEL_WIDTH; ++x)
		{
			// Set to air tile.
			tilemap->tiles[x][y] = GetTileAt(TILE_AIR);
		}
	}
}

// Loads memory for level.
void InitLevel(Tilemap* tilemap)
{
	if (tilemap == NULL)
		return;

	// Iterate through all tiles.
	for (int y = 0; y < MAX_LEVEL_HEIGHT; ++y)
	{
		for (int x = 0; x < MAX_LEVEL_WIDTH; ++x)
		{
			GetCurrentTilemap()->tileOffsets[x][y].x = 0;
			GetCurrentTilemap()->tileOffsets[x][y].y = 0;

			switch (tilemap->tiles[x][y]->id)
			{
				// Up.
				case TILE_GROUND_2:
					GetGame()->tileNormals[x][y] = 1;
					break;

				// Down.
				case TILE_GROUND_9:
					GetGame()->tileNormals[x][y] = 3;
					break;

				// Left.
				case TILE_GROUND_7:
					GetGame()->tileNormals[x][y] = 0;
					break;

				// Right.
				case TILE_GROUND_4:
					GetGame()->tileNormals[x][y] = 2;
					break;
			}

			if (tilemap->tiles[x][y]->hasCollider)
			{
				Vector2D pos = { (float)x * TILE_SIZE - 768,
								 448 - (float)y * TILE_SIZE + (tilemap->height * TILE_SIZE) - ((MIN_LEVEL_HEIGHT - 1) * TILE_SIZE) };

				// Init bounding box memory.
				GetGame()->tileCollisions[x][y] = FromPoints(pos, TILE_SIZE, TILE_SIZE, 0, 0, tilemap->tiles[x][y]->isTrigger);
			}
			else
			{
				// Don't init bounding box memory.
				GetGame()->tileCollisions[x][y] = NULL;
			}

			// Call the tile's init function if it exists.
			if (tilemap->tiles[x][y]->onInit != NULL)
			{
				tilemap->tiles[x][y]->onInit((unsigned char)x, (unsigned char)y);
			}
		}
	}
}

// Updates level.
void UpdateLevel(Tilemap* tilemap)
{
	if (tilemap == NULL)
		return;

	// Iterate through all tiles.
	for (int y = 0; y < MAX_LEVEL_HEIGHT; ++y)
	{
		for (int x = 0; x < MAX_LEVEL_WIDTH; ++x)
		{
			// Call the tile's update function if it exists.
			if (tilemap->tiles[x][y]->onUpdate != NULL)
			{
				tilemap->tiles[x][y]->onUpdate((unsigned char)x, (unsigned char)y);
			}
		}
	}
}

// Unloads memory for level.
void ShutdownLevel(Tilemap* tilemap)
{
	if (tilemap == NULL)
		return;

	// Clears all paint in level.
	RemoveAllPaint();

	// Iterate through all tiles.
	for (int y = 0; y < MAX_LEVEL_HEIGHT; ++y)
	{
		for (int x = 0; x < MAX_LEVEL_WIDTH; ++x)
		{
			// Call the tile's shutdown function if it exists.
			if (tilemap->tiles[x][y]->onShutdown != NULL)
			{
				tilemap->tiles[x][y]->onShutdown((unsigned char)x, (unsigned char)y);
			}

			// Free bounding box memory.
			FreeAABB(GetGame()->tileCollisions[x][y]);

			GetCurrentTilemap()->tileOffsets[x][y].x = 0;
			GetCurrentTilemap()->tileOffsets[x][y].y = 0;
		}
	}

	// Iterate through all gameobjects in gameplay instance and destroy them.
	int count = GetGame()->gameObjectCount;
	for (int i = 0; i < count; ++i)
	{
		GameObjectPtr gameObject = GetGame()->gameObjects[0];

		// Delete the gameobject.
		RemoveGameObject(gameObject);
		GameObjectFree(&gameObject);
	}

	// Despawn all enemies.
	DespawnAll();
}

// Returns current level.
Level* GetCurrentLevel(void)
{
	return &currLevel;
}

// Sets a new level.
void SetCurrentLevel(Level* level)
{
	currLevel = *level;
}

// Returns current level.
Level* GetNextLevel(void)
{
	return &nextLevel;
}

// Sets a new level.
void SetNextLevel(Level* level)
{
	nextLevel = *level;
}
