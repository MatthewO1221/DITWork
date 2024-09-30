#include "AEEngine.h"

#define TILE_SIZE_WORLD 8
#define MAX_AREA_COUNT 128

typedef int SAVELOADMODE;
typedef struct Tile Tile;
typedef struct GameplayInstance GameplayInstance;

enum Direction
{
	TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT
};

typedef struct AreaBox
{
	// The level name for the editor.
	char areaName[32];

	// x and y position of an area in tile coordinates.
	unsigned int x, y;

	// The width and height of an area in tile coordinates.
	int width, height;

	// ID of tile to display.
	char tileID[72][42];
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

typedef struct AreaConnection
{
	// The level name for the editor.
	char name[32];

	// Names of levels in each direction.
	struct AreaConnection* topLeft;
	struct AreaConnection* topRight;
	struct AreaConnection* bottomLeft;
	struct AreaConnection* bottomRight;
} AreaConnection;

typedef struct Player
{
	float x;
	float y;
} Player;

typedef struct Level Level;

void DrawArea(struct AreaBox*, AEGfxVertexList*, struct Tile[]);
void DrawAreas(struct World*, AEGfxVertexList*, struct Tile[]);
int SaveWorld(const World*);
int SaveWorldAtPath(const World*, const char*);
int LoadWorld(struct World*);
int LoadWorldAtPath(struct World*, const char*);
BOOL ChooseWorld(TCHAR[], SAVELOADMODE);
struct AreaBox FromLevel(struct Level*);
void ScreenToSmallTilePos(long, long, unsigned int*, unsigned int*);
void EnterRoom(struct GameplayInstance*, struct Level*, enum Direction);
