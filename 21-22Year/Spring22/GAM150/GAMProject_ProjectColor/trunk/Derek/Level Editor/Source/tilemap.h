#include "AEEngine.h"

#define TILE_SIZE 64
#define PIXELS_PER_TILE 16
#define TILE_COUNT 50
#define MAX_TILE_ENTITIES 8
#define MIN_AREA_WIDTH 20
#define MIN_AREA_HEIGHT 12
#define MAX_AREA_WIDTH 60
#define MAX_AREA_HEIGHT 36

typedef int SAVELOADMODE;
#define SAVE_MODE 1
#define LOAD_MODE 2
#define NEW_MODE 3

// Does nothing. Acts as a NULL tile.
#define TILE_AIR 0

// Allows collisions with the player.
#define TILE_GROUND 1

// Slows the player's momentum and can be frozen into TILE_ICE with PAINT_BLUE.
#define TILE_SOLID_WATER 2

// Kills the player and causes them to respawn at a checkpoint.
#define TILE_FLOOR_SPIKES 3

// Can be changed into a solid block with PAINT_RED which then acts like a TILE_GROUND block.
#define TILE_DOTTED_BLOCK 4

// Grows into a long vine following a TILE_VINE_PATH using TILE_VINE_BLOCK with PAINT_RED which then acts like a TILE_GROUND block.
#define TILE_VINE_STEM_UP 5

// Puts out an electrical signal which can be conducted with PAINT_YELLOW.
#define TILE_BATTERY 6

// Acts as a TILE_GROUND block until powered by a TILE_POWER_SWITCH_ON.
#define TILE_POWERED_DOOR_OFF 7

// Gives the player a gem.
#define TILE_GEM 8

// Grows into a tall tree with PAINT_GREEN using TILE_WOOD and TILE_LEAF_BLOCK.
#define TILE_SAPLING 9

// Ice from water block. Slippery and acts as a TILE_GROUND until paint expires.
#define TILE_ICE 10

// Filled block from dotted block. Acts as a TILE_GROUND until paint expires.
#define TILE_FILLED_BLOCK 11

//
#define TILE_RED_PAINT_UNLOCK 12
#define TILE_UPGRADE_PART 13
#define TILE_WOOD 14
#define TILE_GREEN_SLIME_SPAWN 15
#define TILE_CHECKPOINT 16
#define TILE_CLOUD 17
#define TILE_DOTTED_CHECKPOINT 18
#define TILE_DOTTED_GEM 19
#define TILE_RED_EASEL 20
#define TILE_GROWN_FLOWERS 21
#define TILE_FLOWING_WATER 22
#define TILE_KNIGHT_SPAWN 23
#define TILE_LEAF_BLOCK 24
#define TILE_POWER_SWITCH_ON 25
#define TILE_POWER_SWITCH_OFF 26
#define TILE_POWERED_DOOR_ON 27
#define TILE_BLUE_SLIME_SPAWN 28
#define TILE_YELLOW_SLIME_SPAWN 29
#define TILE_PURPLE_SLIME_SPAWN 30
#define TILE_GREEN_EASEL 31
#define TILE_YELLOW_EASEL 32
#define TILE_BLUE_EASEL 33
#define TILE_ORANGE_EASEL 34
#define TILE_PURPLE_EASEL 35

// Kills the player and causes them to respawn at a checkpoint.
#define TILE_CEILING_SPIKES 36

// 
#define TILE_SNOW_BLOCK 37

// Falls when a TILE_STALACTITE_TRIGGER below it is triggered.
#define TILE_STALACTITE 38

// Grows into a TILE_GROWN_FLOWER when with PAINT_GREEN. Drops upgrade parts and gems.
#define TILE_UNGROWN_FLOWER 39

// Trigger for TILE_STALACTITE to interact with when player crosses.
#define TILE_STALACTITE_TRIGGER 40

// Kills the player and causes them to respawn at a checkpoint.
#define TILE_WALL_SPIKES_LEFT 41

// Kills the player and causes them to respawn at a checkpoint.
#define TILE_WALL_SPIKES_RIGHT 42

// Path for TILE_VINE_BLOCK to follow.
#define TILE_VINE_PATH 43

// Vine block from vine stem block. Acts as TILE_GROUND until paint expires.
#define TILE_VINE_BLOCK 44

//
#define TILE_EYEBAT_SPAWN 45

//
#define TILE_CRATE 46

#define TILE_VINE_STEM_DOWN 47
#define TILE_VINE_STEM_LEFT 48
#define TILE_VINE_STEM_RIGHT 49
#define TILE_HEART_CONTAINER 50

enum PaintColor;
struct GameplayInstance;
typedef void (*TileInfoFunctionPtr)(struct GameplayInstance*, unsigned char, unsigned char);

typedef struct Tile
{
	// ID for the tile. Same as index in allTiles array.
	unsigned char id;

	// Color of paint needed for painted() function to be ran.
	// Needs to be converted to enum PaintColor.
	// -1 means none.
	char paintColor;
	unsigned char requiresFullPaint;

	// Decides whether or not to create an entity version at runtime.
	unsigned char isEntity;

	// Texture for the tile.
	AEGfxTexture* texture;

	// Game loop related functions.
	TileInfoFunctionPtr onInit;
	TileInfoFunctionPtr onUpdate;
	TileInfoFunctionPtr onShutdown;

	// Collision related functions.
	TileInfoFunctionPtr onEnterCollision;
	TileInfoFunctionPtr onStayCollision;
	TileInfoFunctionPtr onExitCollision;

	// Paint related functions.
	TileInfoFunctionPtr onApplyPaint;
	TileInfoFunctionPtr onRemovePaint;
} Tile;
typedef struct TileEntity
{
	// Pointer to the original tile.
	struct Tile* tile;

	// Information about the entity.
	float xPos, yPos;

	// Information about original tile.
	unsigned char originalXPos, originalYPos;
} TileEntity;
typedef struct Tilemap
{
	// The width and height of the level.
	// Automatically figured out when saving to file.
	unsigned char width, height;

	// 2D array of all the tiles in the level. Max size is 256x256.
	struct Tile* tiles[MAX_AREA_WIDTH][MAX_AREA_HEIGHT];
} Tilemap;
typedef struct Level
{
	// The level name for the editor.
	char levelName[32];
	int levelNum;

	// Tilemap for the level.
	Tilemap* tilemap;
} Level;
typedef struct GameplayInstance
{
	// Pointer to level.
	struct Level* level;

	// List of all tile entities.
	struct TileEntity tileEntities[MAX_TILE_ENTITIES];

	// Current room the player's in.
	struct AreaConnection* currentRoom;

	// All tiles.
	struct Tile allTiles[TILE_COUNT];
} GameplayInstance;

Tile NewTile(Tile[], unsigned char, enum PaintColor, unsigned char, unsigned char, AEGfxTexture*, TileInfoFunctionPtr, TileInfoFunctionPtr, TileInfoFunctionPtr, TileInfoFunctionPtr, TileInfoFunctionPtr, TileInfoFunctionPtr, TileInfoFunctionPtr, TileInfoFunctionPtr);
TileEntity NewTileEntity(TileEntity[], Tile*, float, float, unsigned char, unsigned char, int);
void DefineTiles(Tile[]);
void DrawTile(Tile*, float, float, AEGfxVertexList*, float);
void DrawTileEntity(TileEntity*, AEGfxVertexList*, float);
void DrawLevel(Level*, AEGfxVertexList*);
void DrawTileEntities(GameplayInstance*, AEGfxVertexList*);
Tile* GetTile(const Level*, unsigned char, unsigned char);
void PlaceTile(Tile*, Level*, unsigned char, unsigned char);
void EraseTile(Level*, unsigned char, unsigned char, Tile*);
int SaveLevel(const Level*);
int SaveLevelAtPath(const Level*, const char*);
int LoadLevel(Tile[], Level*);
int LoadLevelAtPath(Tile[], Level*, const char*);
int NewLevel(Level*);
void ConvertSize(const char*);
void InitLevel(Tilemap*, Level*, Tile[], int*, const char*);
BOOL ChooseLevel(TCHAR[], SAVELOADMODE);
void TileToScreenPos(unsigned char, unsigned char, long*, long*);
void ScreenToTilePos(long, long, unsigned char*, unsigned char*);
void InitPlaymodeLevel(GameplayInstance*);
void UpdatePlaymodeLevel(GameplayInstance*);
void ShutdownPlaymodeLevel(GameplayInstance*);
void ToFilledBlock(GameplayInstance*, unsigned char, unsigned char);
void ToDottedBlock(GameplayInstance*, unsigned char, unsigned char);
void ToGem(GameplayInstance*, unsigned char, unsigned char);
void ToCheckpoint(GameplayInstance*, unsigned char, unsigned char);
void ToIce(GameplayInstance*, unsigned char, unsigned char);
void ToSolidWater(GameplayInstance*, unsigned char, unsigned char);
void ToVinePath(GameplayInstance*, unsigned char, unsigned char);
void TriggerDespawn(GameplayInstance*, unsigned char, unsigned char);
void TriggerStalactiteSpawn(GameplayInstance*, unsigned char, unsigned char);
void TriggerVinePathSpawn(GameplayInstance*, unsigned char, unsigned char);
void GrowVine(GameplayInstance*, unsigned char, unsigned char);
void DropStalactite(GameplayInstance*, unsigned char, unsigned char);
