#include "AEEngine.h"

struct Level;

#define TILE_SIZE_SMALL 8
#define MAX_ROOM_COUNT 128

#define MAX_GEM_COUNT 20 // Change later.
#define MAX_HEART_CONTAINER_COUNT 7
#define MAX_PAINT_CONTAINER_COUNT 6
#define MAX_CHECKPOINT_FLAG_COUNT 12

// Area definition. Map version of level.
typedef struct Room
{
	// File path for where to find the world and level num.
	char filePath[MAX_PATH];
	int roomNum;

	// x and y position of an area in tile coordinates.
	unsigned int x, y;

	// The width and height of an area in tile coordinates.
	int width, height;

	// ID of tile to display.
	char tileID[72][42];

	// Whether or not the area should be visible to the map/editor.
	char isVisible;

	// Pointers to levels in each direction.
	struct Room* topLeft;
	struct Room* top;
	struct Room* topRight;
	struct Room* right;
	struct Room* bottomRight;
	struct Room* bottom;
	struct Room* bottomLeft;
	struct Room* left;
} Room;

// World definition. Connects different levels and stores information.
typedef struct World
{
	// File path for where to find the world.
	char filePath[MAX_PATH];

	// Number of rooms in the world.
	int roomCount;

	// Array of rooms in the map.
	struct Room rooms[MAX_ROOM_COUNT];

	// Count of each collectible type.
	char gems[MAX_GEM_COUNT];
	char heartContainers[MAX_HEART_CONTAINER_COUNT];
	char paintContainers[MAX_PAINT_CONTAINER_COUNT];
	char checkpointFlags[MAX_CHECKPOINT_FLAG_COUNT];
} World;

// Init world system.
void InitWorldSystem(void);

// Shutdown world system.
void ShutdownWorldSystem(void);

// Draws an area.
void DrawArea(const struct Room*, float, float);

// Draws a world.
void DrawWorld(const struct World*, float, float);

// Draws a minimap.
void DrawMinimap(float, float);

// Creates a new area from a level.
void FromLevel(struct Room*, struct Tilemap*);

// Converts a screen position to a small tile position.
void ScreenToSmallTilePos(long, long, unsigned int*, unsigned int*);

// Returns the world.
struct World* GetWorld(void);

// Sets the world.
void SetWorld(struct World*);

// Returns current room.
struct Room* GetCurrentRoom(void);

// Sets a current room.
void SetCurrentRoom(struct Room*);

// Returns new room.
struct Room* GetNextRoom(void);

// Sets a new room.
void SetNextRoom(struct Room*);

// Sets a new room as the current room.
void SwitchRoom(struct Room*);

// Sets a room.
void SetRoom(struct Room**, struct Room*);
