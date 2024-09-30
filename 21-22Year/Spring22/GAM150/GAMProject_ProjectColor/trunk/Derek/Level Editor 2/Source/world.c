#include "stdafx.h"		// Should be included first in every .c file!
#include "AEEngine.h"
#include "world.h"
#include "tilemap.h"

void DrawArea(AreaBox* area, AEGfxVertexList* mesh, Tile allTiles[])
{
	for (int y = 0; y <= area->height; ++y)
	{
		for (int x = 0; x <= area->width; ++x)
		{
			if (area->tileID[x][y] == TILE_AIR)
				continue;

			// Set render mode.
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

			// Set the position.
			AEGfxSetPosition((area->x + x) * (float)TILE_SIZE_WORLD, (area->height + area->y - y) * (float)TILE_SIZE_WORLD);

			// Render the texture.
			AEGfxTextureSet(allTiles[area->tileID[x][y]].texture, 0.0F, 0.0F);

			// Set transparency.
			AEGfxSetTransparency(1.0F);

			// Draw the mesh.
			AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
		}
	}
}

void DrawAreas(World* world, AEGfxVertexList* mesh, Tile allTiles[])
{
	for (int i = 0; i < world->areaCount; ++i)
	{
		if (world->areas[i].width != 0)
		{
			DrawArea(&world->areas[i], mesh, allTiles);
		}
	}
}

int SaveWorld(const World* world)
{
	TCHAR buffer_w[MAX_PATH] = { 0 };

	if (!ChooseWorld(buffer_w, SAVE_MODE))
		return 0;

	char buffer[MAX_PATH] = { 0 };
	size_t size;
	wcstombs_s(&size, buffer, sizeof(buffer), buffer_w, sizeof(buffer));
	
	return SaveWorldAtPath(world, buffer);
}
int SaveWorldAtPath(const World* world, const char* path)
{
	// Open a file at filePath.
	FILE* file_ptr;
	fopen_s(&file_ptr, path, "w");

	// Check if file was successfully opened.
	if (file_ptr != NULL)
	{
		// Save info about how many levels are in the world.
		fprintf_s(file_ptr, "%d\n", world->areaCount);

		// Save information about each of the areas.
		for (int i = 0; i < MAX_AREA_COUNT; ++i)
		{
			fprintf_s(file_ptr, "%u %u\n", world->areas[i].x, world->areas[i].y);
			fprintf_s(file_ptr, "%d %d\n", world->areas[i].width, world->areas[i].height);
		}

		// Close the file.
		fclose(file_ptr);

		return 0;
	}

	return -1;
}
int LoadWorld(World* world)
{
	TCHAR buffer_w[MAX_PATH] = { 0 };

	if (!ChooseWorld(buffer_w, LOAD_MODE))
		return 0;

	char buffer[MAX_PATH] = { 0 };
	size_t size;
	wcstombs_s(&size, buffer, sizeof(buffer), buffer_w, sizeof(buffer));
	
	return LoadWorldAtPath(world, buffer);
}
int LoadWorldAtPath(World* world, const char* path)
{
	// Open a file at filePath.
	FILE* file_ptr;
	fopen_s(&file_ptr, path, "r");

	// Check if file was successfully opened.
	if (file_ptr != NULL)
	{
		// Load the world's name.
		strcpy_s(world->fileName, sizeof(world->fileName), path);

		// Save info about how many levels are in the world.
		fscanf_s(file_ptr, "%d\n", &world->areaCount);

		// Save information about each of the areas.
		for (int i = 0; i < MAX_AREA_COUNT; ++i)
		{
			fscanf_s(file_ptr, "%u %u\n", &world->areas[i].x, &world->areas[i].y);
			fscanf_s(file_ptr, "%d %d\n", &world->areas[i].width, &world->areas[i].height);
		}

		// Close the file.
		fclose(file_ptr);

		return 0;
	}

	return -1;
}

// Choose a world with the file picker dialogue.
BOOL ChooseWorld(TCHAR buffer[], SAVELOADMODE mode)
{
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = L"World Files (.wld)\0*.wld\0\0";
	ofn.lpstrTitle = mode == SAVE_MODE ? L"Save As" : L"Open";
	ofn.lpstrFile = buffer;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_NONETWORKBUTTON | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	switch (mode)
	{
		case SAVE_MODE:
			return GetSaveFileName(&ofn);
			break;
		case LOAD_MODE:
			return GetOpenFileName(&ofn);
			break;
		default:
			return 0;
			break;
	}
}

AreaBox FromLevel(Level* level)
{
	AreaBox newArea =
	{
		.x = 0,
		.y = 0,
		.width = level->tilemap->width,
		.height = level->tilemap->height
	};
	strcpy_s(newArea.areaName, sizeof(newArea.areaName), level->levelName);

	for (int y = 0; y < MAX_AREA_HEIGHT; ++y)
	{
		for (int x = 0; x < MAX_AREA_WIDTH; ++x)
		{
			newArea.tileID[x][y] = level->tilemap->tiles[x][y]->id;
		}
	}

	return newArea;
}

// Convert a screen position to a small tile position.
void ScreenToSmallTilePos(long screenPosX, long screenPosY, unsigned int* tilePosX, unsigned int* tilePosY)
{
	// Translate values to snap to a grid.
	*tilePosX = (unsigned int)(screenPosX / TILE_SIZE_WORLD);
	*tilePosY = (unsigned int)(screenPosY / TILE_SIZE_WORLD);
}

// Set a new room as the current room.
void EnterRoom(GameplayInstance* game, Level* newLevel, enum Direction direction)
{
	switch (direction)
	{
		case TOP_LEFT:
			if (game->currentRoom->topLeft != NULL)
			{
				game->currentRoom = game->currentRoom->topLeft;
			}
			break;
		case TOP_RIGHT:
			if (game->currentRoom->topRight != NULL)
			{
				game->currentRoom = game->currentRoom->topRight;
			}
			break;
		case BOTTOM_LEFT:
			if (game->currentRoom->bottomLeft != NULL)
			{
				game->currentRoom = game->currentRoom->bottomLeft;
			}
			break;
		case BOTTOM_RIGHT:
			if (game->currentRoom->bottomRight != NULL)
			{
				game->currentRoom = game->currentRoom->bottomRight;
			}
			break;
	}

	// Load level with new name.
	if (game->currentRoom != NULL)
	{
		LoadLevelAtPath(game->allTiles, newLevel, game->currentRoom->name);
	}
}
