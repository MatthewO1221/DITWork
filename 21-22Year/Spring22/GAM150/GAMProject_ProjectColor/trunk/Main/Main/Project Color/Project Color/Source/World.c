#include "stdafx.h"		// Should be included first in every .c file!
#include "World.h"
#include "Level.h"
#include "Tile.h"
#include "GameplayInstance.h"
#include "Serialization.h"
#include "Mesh.h"

// Tile mesh.
AEGfxVertexList* smallTileMesh;

static World currWorld;

static Room currRoom;
static Room nextRoom;

void LoadWorldStats(void);

// Loads levels from a file and connects them to a world.
void PopulateWorld(struct World*);

// Connects two rooms together based on positions in a world map.
void SetNeighbors(struct Room*);

// Connects all rooms together based on positions in a world map.
void ConnectRooms(struct World*);

// Init world system.
void InitWorldSystem(void)
{
	// Load information about collectibles.
	LoadWorldStats();

	// Create a small tile mesh.
	smallTileMesh = MeshCreateQuad(TILE_SIZE_SMALL * 0.5F, TILE_SIZE_SMALL * 0.5F, 1.0F, 1.0F, "Small Tile");

	// Load world from a file.
	LoadWorldAtPath(&currWorld, "./Levels/main.wld");

	// Populate the current world and connect all the rooms together.
	PopulateWorld(&currWorld);
	ConnectRooms(&currWorld);

	// Set the first room to the first room that gets loaded.
	SetCurrentRoom(&GetWorld()->rooms[GetCurrentLevel()->levelNum]);
}

// Shutdown world system.
void ShutdownWorldSystem(void)
{
	// Free the mesh.
	AEGfxMeshFree(smallTileMesh);
}

// Draws an area.
void DrawArea(const Room* area, float xOffset, float yOffset)
{
	if (area == NULL)
		return;

	// Iterate through each tile in the area.
	for (int y = 0; y <= area->height; ++y)
	{
		for (int x = 0; x <= area->width; ++x)
		{
			if (area->tileID[x][y] == TILE_AIR)
				continue;

			// Set render mode.
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

			// Set the position.
			AEGfxSetPosition((area->x + x) * (float)TILE_SIZE_SMALL + xOffset,
							 (area->height + area->y - y) * (float)TILE_SIZE_SMALL + yOffset);

			// Render the texture.
			AEGfxTextureSet(GetTileAt(area->tileID[x][y])->texture, 0.0F, 0.0F);

			// Set transparency.
			AEGfxSetTransparency(1.0F);

			// Draw the mesh.
			AEGfxMeshDraw(smallTileMesh, AE_GFX_MDM_TRIANGLES);
		}
	}
}

// Draws a world.
void DrawWorld(const World* world, float xOffset, float yOffset)
{
	if (world == NULL)
		return;

	// Iterate through each area in the world.
	for (int i = 0; i < world->roomCount; ++i)
	{
		// Draw the level.
		DrawArea(&world->rooms[i], xOffset, yOffset);
	}
}

// Draws a minimap.
void DrawMinimap(float xOffset, float yOffset)
{
	// Iterate through each area in the world.
	for (int i = 0; i < GetWorld()->roomCount; ++i)
	{
		// Draw the level.
		DrawArea(&GetWorld()->rooms[i], xOffset, yOffset);
	}
}


// Creates a new area from a level.
void FromLevel(Room* room, Tilemap* tilemap)
{
	if (room == NULL || tilemap == NULL)
		return;

	// Apply width and height.
	room->width = tilemap->width;
	room->height = tilemap->height;

	// Iterate through each of the tiles.
	for (int y = 0; y < MAX_LEVEL_HEIGHT; ++y)
	{
		for (int x = 0; x < MAX_LEVEL_WIDTH; ++x)
		{
			room->tileID[x][y] = tilemap->tiles[x][y]->id;
		}
	}
}

// Converts a screen position to a small tile position.
void ScreenToSmallTilePos(long screenPosX, long screenPosY, unsigned int* tilePosX, unsigned int* tilePosY)
{
	// Translate values to snap to a grid.
	*tilePosX = (unsigned int)(screenPosX / TILE_SIZE_SMALL);
	*tilePosY = (unsigned int)(screenPosY / TILE_SIZE_SMALL);
}

void LoadWorldStats(void)
{
	if (LoadCollectibleData("collectible_data.svd") == -1)
	{
		for (int i = 0; i < MAX_GEM_COUNT; ++i)
		{
			GetWorld()->gems[i] = 1;
		}

		for (int i = 0; i < MAX_HEART_CONTAINER_COUNT; ++i)
		{
			GetWorld()->heartContainers[i] = 1;
		}

		for (int i = 0; i < MAX_PAINT_CONTAINER_COUNT; ++i)
		{
			GetWorld()->paintContainers[i] = 1;
		}

		for (int i = 0; i < MAX_CHECKPOINT_FLAG_COUNT; ++i)
		{
			GetWorld()->checkpointFlags[i] = 1;
		}

		// Write new file.
		SaveCollectibleData("collectible_data.svd");
	}
}

// Loads levels from a file and connects them to a world.
void PopulateWorld(World* world)
{
	if (world == NULL)
		return;

	// Create temp level and tilemap values.
	Tilemap tilemap = { 0 };
	Level level = { 0 };

	// Iterate through each level in a file.
	for (int i = 0, j = 0; i < MAX_ROOM_COUNT; ++i)
	{
		// Check if level exists.
		if (LoadLevelAtPath(&level, &tilemap, world->rooms[i].filePath) == PICKERRESULT_SUCCESS)
		{
			// Set filename for level.
			strcpy_s(GetWorld()->rooms[j].filePath, sizeof(GetWorld()->rooms[j].filePath), world->rooms[i].filePath);

			// Iterate through tilemap.
			FromLevel(&world->rooms[j], &tilemap);
			world->rooms[j].roomNum = j;

			// Increment room index variable.
			++j;
		}
	}
}

// Connects two rooms together based on positions in a world map.
void SetNeighbors(Room* currentRoom)
{
	if (currentRoom == NULL)
		return;

	int r1x1 = currentRoom->x;
	int r1x2 = currentRoom->x + currentRoom->width;
	int r1y1 = currentRoom->y;
	int r1y2 = currentRoom->y + currentRoom->height;

	// Iterate through each level in the world.
	for (int i = 0; i < MAX_ROOM_COUNT; ++i)
	{
		// Get the next room.
		Room* nextRoom = &currWorld.rooms[i];

		// Skip iteration if rooms are the same.
		if (nextRoom == currentRoom)
			continue;

		int r2x1 = nextRoom->x;
		int r2x2 = nextRoom->x + nextRoom->width;
		int r2y1 = nextRoom->y;
		int r2y2 = nextRoom->y + nextRoom->height;

		if (r2y2 == r1y1 - 1 &&
			r2x1 <= r1x2 &&
			r2x2 >= r1x1)
		{
			
			if (r2x1 > r1x1 && r2x2 > r1x2)
			{
				currentRoom->bottomRight = nextRoom;
			}
			else if (r2x1 < r1x1 && r2x2 < r1x2)
			{
				currentRoom->bottomLeft = nextRoom;
			}
			else
			{
				currentRoom->bottom = nextRoom;
			}
		}
		else if (r2y1 == r1y2 + 1 &&
				 r2x1 <= r1x2 &&
				 r2x2 >= r1x1)
		{
			if (r2x1 > r1x1 && r2x2 > r1x2)
			{
				currentRoom->topRight = nextRoom;
			}
			else if (r2x1 < r1x1 && r2x2 < r1x2)
			{
				currentRoom->topLeft = nextRoom;
			}
			else
			{
				currentRoom->top = nextRoom;
			}
		}
		else if (r2x2 == r1x1 - 1 &&
				 r2y1 <= r1y2 &&
				 r2y2 >= r1y1)
		{
			if (r2y1 > r1y1 && r2y2 > r1y2)
			{
				currentRoom->topLeft = nextRoom;
			}
			else if (r2y1 < r1y1 && r2y2 < r1y2)
			{
				currentRoom->bottomLeft = nextRoom;
			}
			else
			{
				currentRoom->left = nextRoom;
			}
		}
		else if (r2x1 == r1x2 + 1 &&
				 r2y1 <= r1y2 &&
				 r2y2 >= r1y1)
		{
			if (r2y1 > r1y1 && r2y2 > r1y2)
			{
				currentRoom->topRight = nextRoom;
			}
			else if (r2y1 < r1y1 && r2y2 < r1y2)
			{
				currentRoom->bottomRight = nextRoom;
			}
			else
			{
				currentRoom->right = nextRoom;
			}
		}
	}
}

// Connects all rooms together based on positions in a world map.
void ConnectRooms(World* world)
{
	if (world == NULL)
		return;

	// Iterate through each level in the world.
	for (int i = 0; i < MAX_ROOM_COUNT; ++i)
	{
		// Get the current room.
		Room* currentRoom = &currWorld.rooms[i];

		// Set the neighbor for the current room.
		SetNeighbors(currentRoom);
	}
}

// Returns the world.
World* GetWorld(void)
{
	return &currWorld;
}

// Sets the world.
void SetWorld(World* world)
{
	currWorld = *world;
}

// Returns current room.
Room* GetCurrentRoom(void)
{
	return &currRoom;
}

// Sets a new room.
void SetCurrentRoom(Room* room)
{
	currRoom = *room;
}

// Returns new room.
struct Room* GetNextRoom(void)
{
	return &nextRoom;
}

// Sets a new room.
void SetNextRoom(struct Room* room)
{
	nextRoom = *room;
}

// Sets a new room as the current room.
void SwitchRoom(Room* newRoom)
{
	if (newRoom == NULL)
		return;

	// Shutdown old level.
	ShutdownLevel(GetCurrentTilemap());

	// Load new level.
	LoadLevelAtPath(GetCurrentLevel(), GetCurrentTilemap(), newRoom->filePath);

	// Initialize new level.
	InitLevel(GetCurrentTilemap());

	// Set new room as current room and set next area.
	SetCurrentRoom(newRoom);
}

// Sets a room.
void SetRoom(Room** destination, Room* source)
{
	if (source == NULL)
		return;

	// Load new level.
	LoadLevelAtPath(GetNextLevel(), GetNextTilemap(), source->filePath);
	SetNextRoom(source);

	*destination = source;
}
