#include "AEEngine.h"

#define TILE_SIZE_WORLD 8
#define MAX_AREA_COUNT 128

typedef int SAVELOADMODE;
typedef struct Tile Tile;

typedef struct AreaBox
{
	// The level name for the editor.
	char areaName[32];

	// x and y position of an area in tile coordinates.
	unsigned int x, y;

	// The width and height of an area in tile coordinates.
	int width, height;

	// ID of tile to display.
	char tileID[99][59];
} AreaBox;
typedef struct World
{
	// The level name for the editor.
	char fileName[32];

	// Number of areas in the world.
	int areaCount;

	// Array of areas in the map.
	AreaBox areas[MAX_AREA_COUNT];
} World;

typedef struct Level Level;

void DrawArea(AreaBox*, AEGfxVertexList*, Tile[]);
void DrawAreas(World*, AEGfxVertexList*, Tile[]);
int SaveWorld(const World*);
int SaveWorldAtPath(const World*, const char*);
int LoadWorld(World*);
int LoadWorldAtPath(World*, const char*);
BOOL ChooseWorld(TCHAR[], SAVELOADMODE);
AreaBox FromLevel(Level*);
void ScreenToSmallTilePos(long, long, unsigned int*, unsigned int*);