#include "AEEngine.h"

enum PaintColor;

struct Tilemap;
typedef struct GameObject GameObject, * GameObjectPtr;

typedef void (*TileInfoFunctionPtr)(unsigned char, unsigned char);

// Tile info.
#define TILE_SIZE 64
#define PIXELS_PER_TILE 16
#define TILE_COUNT 85
#define MAX_TILE_ENTITIES 8

// All tiles.
#define TILE_AIR 0
#define TILE_GROUND 1
#define TILE_SOLID_WATER 2
#define TILE_FLOOR_SPIKES 3
#define TILE_DOTTED_BLOCK 4
#define TILE_VINE_STEM_UP 5
#define TILE_BATTERY 6
#define TILE_POWERED_DOOR_OFF 7
#define TILE_GEM 8
#define TILE_SAPLING 9
#define TILE_ICE 10
#define TILE_FILLED_BLOCK 11
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
#define TILE_CEILING_SPIKES 36
#define TILE_SNOW_BLOCK 37
#define TILE_STALACTITE 38
#define TILE_UNGROWN_FLOWER 39
#define TILE_STALACTITE_TRIGGER 40
#define TILE_WALL_SPIKES_LEFT 41
#define TILE_WALL_SPIKES_RIGHT 42
#define TILE_VINE_PATH 43
#define TILE_VINE_BLOCK 44
#define TILE_EYEBAT_SPAWN 45
#define TILE_CRATE 46
#define TILE_VINE_STEM_DOWN 47
#define TILE_VINE_STEM_LEFT 48
#define TILE_VINE_STEM_RIGHT 49
#define TILE_HEART_CONTAINER 50
#define TILE_SEMI_SOLID_PLATFORM 51
#define TILE_PAINT_CONTAINER 52
#define TILE_FLOWING_ICE 53
#define TILE_GROUND_2 54
#define TILE_GROUND_3 55
#define TILE_GROUND_4 56
#define TILE_GROUND_6 57
#define TILE_GROUND_7 58
#define TILE_GROUND_8 59
#define TILE_GROUND_9 60
#define TILE_GROUND_10 61
#define TILE_GROUND_11 62
#define TILE_GROUND_12 63
#define TILE_GROUND_13 64
#define TILE_GROUND_14 65
#define TILE_GROUND_15 66
#define TILE_GROUND_16 67
#define TILE_GROUND_17 68
#define TILE_GROUND_18 69
#define TILE_GROUND_19 70
#define TILE_GROUND_20 71
#define TILE_GROUND_21 72
#define TILE_GROUND_22 73
#define TILE_GROUND_23 74
#define TILE_GROUND_24 75
#define TILE_GROUND_25 76
#define TILE_GROUND_26 77
#define TILE_GRASS 78
#define TILE_BLUE_PAINT_UNLOCK 79
#define TILE_GREEN_PAINT_UNLOCK 80
#define TILE_YELLOW_PAINT_UNLOCK 81
#define TILE_ORANGE_PAINT_UNLOCK 82
#define TILE_PURPLE_PAINT_UNLOCK 83
#define TILE_PAINTBRUSH_UNLOCK 84

// Tile definition. Stores information about a tile.
typedef struct Tile
{
	// ID for the tile. Same as index in allTiles array.
	char id;

	// Color of paint needed for painted() function to be ran.
	char paintColor;
	char canBePainted;

	// Whether or not the tile is collidable, and whether it's a trigger or not.
	char hasCollider;
	char isTrigger;

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

// Init tile system.
void InitTileSystem(void);

// Shutdown tile system.
void ShutdownTileSystem(void);

// Gets a tile from allTiles[].
Tile* GetTileAt(int);

// Get tile tile at (x, y) in a level.
Tile* GetTile(const struct Tilemap*, unsigned char, unsigned char);

// Place a tile at (x, y) in a level.
void PlaceTile(struct Tilemap*, struct Tile*, unsigned char, unsigned char);

// Erase a tile at (x, y) in a level.
void EraseTile(struct Tilemap*, unsigned char, unsigned char);

// Draws a tile at (x, y).
void DrawTile(const struct Tile*, float, float, float);

// Converts a tile position to a screen position.
void TileToScreenPos(unsigned char, unsigned char, long*, long*);

// Converts a screen position to a tile position.
void ScreenToTilePos(long, long, unsigned char*, unsigned char*);

// Returns current tilemap.
struct Tilemap* GetCurrentTilemap(void);

// Sets a new tilemap.
void SetCurrentTilemap(struct Tilemap*);

// Returns current tilemap.
struct Tilemap* GetNextTilemap(void);

// Sets a new tilemap.
void SetNextTilemap(struct Tilemap*);
