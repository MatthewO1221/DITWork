#include "AEEngine.h"
#include "Vector2D.h"

// Level info.
#define MIN_LEVEL_WIDTH 24
#define MIN_LEVEL_HEIGHT 14
#define MAX_LEVEL_WIDTH 72
#define MAX_LEVEL_HEIGHT 42

// Tilemap definition. Stores information about a level tilemap.
typedef struct Tilemap
{
	// The width and height of the level.
	unsigned char width, height;

	// 2D array of all the tiles in the level.
	struct Tile* tiles[MAX_LEVEL_WIDTH][MAX_LEVEL_HEIGHT];

	// 2D array of all tile offsets in the level.
	Vector2D tileOffsets[MAX_LEVEL_WIDTH][MAX_LEVEL_HEIGHT];
} Tilemap;

// Level definition. Stores information about a level.
typedef struct Level
{
	// File path for where to find the level.
	char filePath[MAX_PATH];

	// Level number.
	unsigned int levelNum;
} Level;

// Draws a level.
void DrawLevel(const struct Tilemap*);

// Sets all the level's values back to defaults.
void ResetLevel(struct Level*, struct Tilemap*, int*, const char*);

// Loads memory for level.
void InitLevel(struct Tilemap*);

// Updates level.
void UpdateLevel(struct Tilemap*);

// Unloads memory for level.
void ShutdownLevel(struct Tilemap*);

// Returns current level.
struct Level* GetCurrentLevel(void);

// Sets a new level.
void SetCurrentLevel(struct Level*);

// Returns current level.
struct Level* GetNextLevel(void);

// Sets a new level.
void SetNextLevel(struct Level*);
