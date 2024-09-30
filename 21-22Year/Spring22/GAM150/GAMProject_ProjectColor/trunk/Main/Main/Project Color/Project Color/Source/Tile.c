#include "stdafx.h"		// Should be included first in every .c file!
#include "Tile.h"
#include "Paint.h"
#include "Level.h"
#include "GameObject.h"
#include "Transform.h"
#include "Physics.h"
#include "Sprite.h"
#include "AABB.h"
#include "Vector2D.h"
#include "GameplayInstance.h"
#include "Mesh.h"
#include "Debug.h"
#include "EnemyAI.h"
#include "Camera.h"
#include "World.h"
#include "Brush.h"
#include "Player Controller.h"
#include "Player Movement.h"
#include "SpriteSource.h"
#include "Serialization.h"
#include "Audio.h"
#include "Player.h"

// Tile mesh.
AEGfxVertexList* tileMesh;

// Array of all tile types.
Tile allTiles[TILE_COUNT];

static Tilemap currTilemap;
static Tilemap nextTilemap;

// Create a new tile and add it to the allTiles array.
Tile NewTile(char, enum PaintColor, char, char, char, AEGfxTexture*, TileInfoFunctionPtr, TileInfoFunctionPtr, TileInfoFunctionPtr, TileInfoFunctionPtr, TileInfoFunctionPtr, TileInfoFunctionPtr, TileInfoFunctionPtr, TileInfoFunctionPtr);

// Create all tile types.
void DefineAllTiles(void);

void TileBob(unsigned char, unsigned char);
void TileToIce(unsigned char, unsigned char);
void TileToWater(unsigned char, unsigned char);
void TileDamagePlayer(unsigned char, unsigned char);
void TileToggleEyeBlock(unsigned char, unsigned char);
void TileToCheckpoint(unsigned char, unsigned char);
void TileToGem(unsigned char, unsigned char);
void TileCollectGem(unsigned char, unsigned char);
void TileSpawnPaintbrush(unsigned char, unsigned char);
void TileDespawnPaintbrush(unsigned char, unsigned char);
void TileSpawnGreenSlime(unsigned char, unsigned char);
void TileSpawnYellowSlime(unsigned char, unsigned char);
void TileSpawnBlueSlime(unsigned char, unsigned char);
void TileSpawnPurpleSlime(unsigned char, unsigned char);
void TileSpawnFleye(unsigned char, unsigned char);
void TileSpawnPaintKnight(unsigned char, unsigned char);
void TileDespawnEnemy(unsigned char, unsigned char);
void TileSpawnCrate(unsigned char, unsigned char);
void TileDespawnCrate(unsigned char, unsigned char);
void TileToGrownFlower(unsigned char, unsigned char);
void TileHideVinePath(unsigned char, unsigned char);
void TileActivateCheckpoint(unsigned char, unsigned char);
void TileSlowPlayer(unsigned char, unsigned char);
void TileSpeedPlayer(unsigned char, unsigned char);
void TileSlowPlayerAndMakeFall(unsigned char, unsigned char);
void TileCollectHeartContainer(unsigned char, unsigned char);
void TileCollectPaintContainer(unsigned char, unsigned char);
void TileGrowVine(unsigned char, unsigned char);
void TileUngrowVine(unsigned char, unsigned char);
void TileGenerateElectricity(unsigned char, unsigned char);
void TileDisappearAfterTime(unsigned char, unsigned char);
void TileCollectRedPaint(unsigned char, unsigned char);
void TileCollectBluePaint(unsigned char, unsigned char);
void TileCollectGreenPaint(unsigned char, unsigned char);
void TileCollectYellowPaint(unsigned char, unsigned char);
void TileCollectOrangePaint(unsigned char, unsigned char);
void TileCollectPurplePaint(unsigned char, unsigned char);

// Init tile system.
void InitTileSystem(void)
{
	// Create a tile mesh.
	tileMesh = MeshCreateQuad(TILE_SIZE * 0.5F, TILE_SIZE * 0.5F, 1.0F, 1.0F, "Tile");

	// Define all tiles.
	DefineAllTiles();

	LoadSound("./Assets/SFX/sample_beep.wav", sfx);
	LoadSound("./Assets/SFX/visblock_appear.mp3", sfx);
}

// Shutdown tile system.
void ShutdownTileSystem(void)
{
	// Free the mesh.
	AEGfxMeshFree(tileMesh);

	// Unload all textures.
	for (int i = 0; i < TILE_COUNT; ++i)
	{
		AEGfxTextureUnload(allTiles[i].texture);
	}
}

// Gets a tile from allTiles[].
Tile* GetTileAt(int index)
{
	return &allTiles[index];
}

// Create a new tile and add it to the allTiles array.
Tile NewTile(char id, enum PaintColor paintColor, char hasCollider, char isTrigger, char canBePainted, AEGfxTexture* texture, TileInfoFunctionPtr onInit, TileInfoFunctionPtr onUpdate, TileInfoFunctionPtr onShutdown, TileInfoFunctionPtr onEnterCollision, TileInfoFunctionPtr onStayCollision, TileInfoFunctionPtr onExitCollision, TileInfoFunctionPtr onApplyPaint, TileInfoFunctionPtr onRemovePaint)
{
	// Define new tile.
	Tile newTile =
	{
		.id = id,

		.paintColor = (char)paintColor,
		.canBePainted = canBePainted,

		.hasCollider = hasCollider,
		.isTrigger = isTrigger,

		.texture = texture,

		.onInit = onInit,
		.onUpdate = onUpdate,
		.onShutdown = onShutdown,

		.onEnterCollision = onEnterCollision,
		.onStayCollision = onStayCollision,
		.onExitCollision = onExitCollision,

		.onApplyPaint = onApplyPaint,
		.onRemovePaint = onRemovePaint
	};

	// Store and return new tile.
	allTiles[id] = newTile;
	return newTile;
}

// Create all tile types.
void DefineAllTiles(void)
{
	// Create an instance of each tile.
	NewTile(TILE_AIR, PAINT_NONE, 0, 0, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/1.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_2, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/2.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_3, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/3.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_4, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/4.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_6, PAINT_NONE, 0, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/6.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_7, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/7.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_8, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/8.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_9, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/9.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_10, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/10.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_11, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/11.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_12, PAINT_NONE, 0, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/12.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_13, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/13.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_14, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/14.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_15, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/15.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_16, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/16.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_17, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/17.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_18, PAINT_NONE, 0, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/18.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_19, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/19.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_20, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/20.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_21, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/21.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_22, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/22.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_23, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/23.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_24, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/24.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_25, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/25.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROUND_26, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/26.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_SOLID_WATER, PAINT_BLUE, 1, 1, 1, AEGfxTextureLoad("./Assets/Tiles/solid_water.png"), NULL, TileBob, NULL, TileSlowPlayer, NULL, TileSpeedPlayer, TileToIce, NULL);
	NewTile(TILE_FLOOR_SPIKES, PAINT_NONE, 1, 1, 1, AEGfxTextureLoad("./Assets/Tiles/floor_spikes.png"), NULL, NULL, NULL, TileDamagePlayer, NULL, NULL, NULL, NULL);
	NewTile(TILE_DOTTED_BLOCK, PAINT_RED, 0, 0, 1, AEGfxTextureLoad("./Assets/Tiles/dotted_block.png"), NULL, NULL, NULL, NULL, NULL, NULL, TileToggleEyeBlock, NULL);
	NewTile(TILE_VINE_STEM_UP, PAINT_GREEN, 0, 0, 1, AEGfxTextureLoad("./Assets/Tiles/vine_stem_up.png"), NULL, NULL, NULL, NULL, NULL, NULL, TileGrowVine, TileUngrowVine);
	NewTile(TILE_BATTERY, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Tiles/battery.png"), NULL, TileGenerateElectricity, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_POWERED_DOOR_OFF, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Tiles/powered_door_off.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GEM, PAINT_NONE, 1, 1, 0, AEGfxTextureLoad("./Assets/Tiles/gem.png"), NULL, TileBob, NULL, TileCollectGem, NULL, NULL, NULL, NULL);
	NewTile(TILE_SAPLING, PAINT_GREEN, 0, 0, 1, AEGfxTextureLoad("./Assets/Tiles/sapling.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_ICE, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Tiles/ice.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, TileToWater);
	NewTile(TILE_FILLED_BLOCK, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Level_Tiles/vis-6.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, TileToggleEyeBlock);
	NewTile(TILE_RED_PAINT_UNLOCK, PAINT_NONE, 1, 1, 0, AEGfxTextureLoad("./Assets/Tiles/red_paint_unlock.png"), NULL, TileBob, NULL, TileCollectRedPaint, NULL, NULL, NULL, NULL);
	NewTile(TILE_UPGRADE_PART, PAINT_NONE, 0, 0, 0, AEGfxTextureLoad("./Assets/Tiles/upgrade_part.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_WOOD, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Tiles/wood.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GREEN_SLIME_SPAWN, PAINT_NONE, 0, 0, 0, AEGfxTextureLoad("./Assets/Tiles/green_slime_spawn.png"), TileSpawnGreenSlime, NULL, TileDespawnEnemy, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_CHECKPOINT, PAINT_NONE, 1, 1, 0, AEGfxTextureLoad("./Assets/Tiles/checkpoint.png"), NULL, NULL, NULL, TileActivateCheckpoint, NULL, NULL, NULL, NULL);
	NewTile(TILE_CLOUD, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Tiles/cloud.png"), NULL, NULL, NULL, NULL, TileDisappearAfterTime, NULL, NULL, NULL);
	NewTile(TILE_DOTTED_CHECKPOINT, PAINT_RED, 0, 0, 1, AEGfxTextureLoad("./Assets/Tiles/dotted_checkpoint.png"), NULL, NULL, NULL, NULL, NULL, NULL, TileToCheckpoint, NULL);
	NewTile(TILE_DOTTED_GEM, PAINT_RED, 0, 0, 1, AEGfxTextureLoad("./Assets/Tiles/dotted_gem.png"), NULL, TileBob, NULL, NULL, NULL, NULL, TileToGem, NULL);
	NewTile(TILE_RED_EASEL, PAINT_RED, 0, 0, 1, AEGfxTextureLoad("./Assets/Tiles/red_easel.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GROWN_FLOWERS, PAINT_NONE, 0, 0, 1, AEGfxTextureLoad("./Assets/Tiles/grown_flowers.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_FLOWING_WATER, PAINT_BLUE, 1, 1, 1, AEGfxTextureLoad("./Assets/Tiles/flowing_water.png"), NULL, NULL, NULL, NULL, TileSlowPlayerAndMakeFall, NULL, TileToIce, NULL);
	NewTile(TILE_KNIGHT_SPAWN, PAINT_NONE, 0, 0, 0, AEGfxTextureLoad("./Assets/Tiles/knight_spawn.png"), TileSpawnPaintKnight, NULL, TileDespawnEnemy, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_LEAF_BLOCK, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Tiles/leaf_block.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_POWER_SWITCH_ON, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Tiles/power_switch_on.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_POWER_SWITCH_OFF, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Tiles/power_switch_off.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_POWERED_DOOR_ON, PAINT_NONE, 1, 1, 1, AEGfxTextureLoad("./Assets/Tiles/powered_door_on.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_BLUE_SLIME_SPAWN, PAINT_NONE, 0, 0, 0, AEGfxTextureLoad("./Assets/Tiles/blue_slime_spawn.png"), TileSpawnBlueSlime, NULL, TileDespawnEnemy, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_YELLOW_SLIME_SPAWN, PAINT_NONE, 0, 0, 0, AEGfxTextureLoad("./Assets/Tiles/yellow_slime_spawn.png"), TileSpawnYellowSlime, NULL, TileDespawnEnemy, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_PURPLE_SLIME_SPAWN, PAINT_NONE, 0, 0, 0, AEGfxTextureLoad("./Assets/Tiles/purple_slime_spawn.png"), TileSpawnPurpleSlime, NULL, TileDespawnEnemy, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_GREEN_EASEL, PAINT_GREEN, 0, 0, 1, AEGfxTextureLoad("./Assets/Tiles/green_easel.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_YELLOW_EASEL, PAINT_YELLOW, 0, 0, 1, AEGfxTextureLoad("./Assets/Tiles/yellow_easel.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_BLUE_EASEL, PAINT_BLUE, 0, 0, 1, AEGfxTextureLoad("./Assets/Tiles/blue_easel.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_ORANGE_EASEL, PAINT_ORANGE, 0, 0, 1, AEGfxTextureLoad("./Assets/Tiles/orange_easel.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_PURPLE_EASEL, PAINT_PURPLE, 0, 0, 1, AEGfxTextureLoad("./Assets/Tiles/purple_easel.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_CEILING_SPIKES, PAINT_NONE, 1, 1, 1, AEGfxTextureLoad("./Assets/Tiles/ceiling_spikes.png"), NULL, NULL, NULL, TileDamagePlayer, NULL, NULL, NULL, NULL);
	NewTile(TILE_SNOW_BLOCK, PAINT_NONE, 0, 0, 1, AEGfxTextureLoad("./Assets/Tiles/snow_block.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_STALACTITE, PAINT_NONE, 0, 0, 0, AEGfxTextureLoad("./Assets/Tiles/stalactite.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_UNGROWN_FLOWER, PAINT_GREEN, 0, 0, 1, AEGfxTextureLoad("./Assets/Tiles/ungrown_flowers.png"), NULL, NULL, NULL, NULL, NULL, NULL, TileToGrownFlower, NULL);
	NewTile(TILE_STALACTITE_TRIGGER, PAINT_BLUE, 0, 0, 0, AEGfxTextureLoad("./Assets/Tiles/stalactite_trigger.png"), NULL, NULL, NULL, NULL, NULL,NULL, NULL, NULL);
	NewTile(TILE_WALL_SPIKES_LEFT, PAINT_NONE, 1, 1, 1, AEGfxTextureLoad("./Assets/Tiles/wall_spikes_left.png"), NULL, NULL, NULL, TileDamagePlayer, NULL, NULL, NULL, NULL);
	NewTile(TILE_WALL_SPIKES_RIGHT, PAINT_NONE, 1, 1, 1, AEGfxTextureLoad("./Assets/Tiles/wall_spikes_right.png"), NULL, NULL, NULL, TileDamagePlayer, NULL, NULL, NULL, NULL);
	NewTile(TILE_VINE_PATH, PAINT_NONE, 0, 0, 0, AEGfxTextureLoad("./Assets/Tiles/vine_path.png"), TileHideVinePath, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_VINE_BLOCK, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Tiles/vine_block.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_EYEBAT_SPAWN, PAINT_NONE, 0, 0, 0, AEGfxTextureLoad("./Assets/Tiles/eyebat_spawn.png"), TileSpawnFleye, NULL, TileDespawnEnemy, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_CRATE, PAINT_NONE, 0, 0, 0, AEGfxTextureLoad("./Assets/Tiles/crate.png"), TileSpawnCrate, NULL, TileDespawnCrate, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_VINE_STEM_DOWN, PAINT_GREEN, 0, 0, 1, AEGfxTextureLoad("./Assets/Tiles/vine_stem_down.png"), NULL, NULL, NULL, NULL, NULL, NULL, TileGrowVine, TileUngrowVine);
	NewTile(TILE_VINE_STEM_LEFT, PAINT_GREEN, 0, 0, 1, AEGfxTextureLoad("./Assets/Tiles/vine_stem_left.png"), NULL, NULL, NULL, NULL, NULL, NULL, TileGrowVine, TileUngrowVine);
	NewTile(TILE_VINE_STEM_RIGHT, PAINT_GREEN, 0, 0, 1, AEGfxTextureLoad("./Assets/Tiles/vine_stem_right.png"), NULL, NULL, NULL, NULL, NULL, NULL, TileGrowVine, TileUngrowVine);
	NewTile(TILE_HEART_CONTAINER, PAINT_NONE, 1, 1, 0, AEGfxTextureLoad("./Assets/Tiles/heart_container.png"), NULL, TileBob, NULL, TileCollectHeartContainer, NULL, NULL,NULL, NULL);
	NewTile(TILE_SEMI_SOLID_PLATFORM, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Tiles/semi_solid_platform.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_PAINT_CONTAINER, PAINT_NONE, 1, 1, 0, AEGfxTextureLoad("./Assets/Tiles/paint_container.png"), NULL, TileBob, NULL, TileCollectPaintContainer, NULL, NULL, NULL, NULL);
	NewTile(TILE_FLOWING_ICE, PAINT_NONE, 1, 0, 1, AEGfxTextureLoad("./Assets/Tiles/ice.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, TileToWater);
	NewTile(TILE_GRASS, PAINT_NONE, 0, 0, 0, AEGfxTextureLoad("./Assets/Level_Tiles/grass.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(TILE_BLUE_PAINT_UNLOCK, PAINT_NONE, 1, 1, 0, AEGfxTextureLoad("./Assets/Tiles/blue_paint_unlock.png"), NULL, TileBob, NULL, TileCollectBluePaint, NULL, NULL, NULL, NULL);
	NewTile(TILE_GREEN_PAINT_UNLOCK, PAINT_NONE, 1, 1, 0, AEGfxTextureLoad("./Assets/Tiles/green_paint_unlock.png"), NULL, TileBob, NULL, TileCollectGreenPaint, NULL, NULL, NULL, NULL);
	NewTile(TILE_YELLOW_PAINT_UNLOCK, PAINT_NONE, 1, 1, 0, AEGfxTextureLoad("./Assets/Tiles/yellow_paint_unlock.png"), NULL, TileBob, NULL, TileCollectYellowPaint, NULL, NULL, NULL, NULL);
	NewTile(TILE_ORANGE_PAINT_UNLOCK, PAINT_NONE, 1, 1, 0, AEGfxTextureLoad("./Assets/Tiles/orange_paint_unlock.png"), NULL, TileBob, NULL, TileCollectOrangePaint, NULL, NULL, NULL, NULL);
	NewTile(TILE_PURPLE_PAINT_UNLOCK, PAINT_NONE, 1, 1, 0, AEGfxTextureLoad("./Assets/Tiles/purple_paint_unlock.png"), NULL, TileBob, NULL, TileCollectPurplePaint, NULL, NULL, NULL, NULL);
	NewTile(TILE_PAINTBRUSH_UNLOCK, PAINT_NONE, 0, 0, 0, AEGfxTextureLoad("./Assets/Tiles/paintbrush_unlock.png"), TileSpawnPaintbrush, TileBob, TileDespawnPaintbrush, NULL, NULL, NULL, NULL, NULL);
}

// Get tile tile at (x, y) in a level.
Tile* GetTile(const Tilemap* tilemap, unsigned char x, unsigned char y)
{
	if (tilemap == NULL ||
		x > tilemap->width ||
		y > tilemap->height)
		return NULL;

	// Return the tile at x, y in level level.
	return tilemap->tiles[x][y];
}

// Place a tile at (x, y) in a level.
void PlaceTile(Tilemap* tilemap, Tile* tile, unsigned char x, unsigned char y)
{
	if (tilemap == NULL ||
		x > tilemap->width ||
		y > tilemap->height)
		return;

	// Place a tile in the level tilemap.
	tilemap->tiles[x][y] = tile;
}

// Erase a tile at (x, y) in a level.
void EraseTile(Tilemap* tilemap, unsigned char x, unsigned char y)
{
	if (tilemap == NULL ||
		x > tilemap->width ||
		y > tilemap->height)
		return;

	// Set the tile to air.
	tilemap->tiles[x][y] = &allTiles[TILE_AIR];
}

// Draw a tile at x, y (translated from screen coords).
void DrawTile(const Tile* tile, float x, float y, float alpha)
{
	if (tile == NULL || tile->texture == NULL)
		return;

	// Set render mode.
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	// Set the position.
	AEGfxSetPosition(x, y);

	// Render the texture.
	AEGfxTextureSet(tile->texture, 0.0F, 0.0F);

	// Set transparency.
	AEGfxSetTransparency(alpha);

	// Draw the mesh.
	AEGfxMeshDraw(tileMesh, AE_GFX_MDM_TRIANGLES);
}

// Converts a tile position to a screen position.
void TileToScreenPos(unsigned char tilePosX, unsigned char tilePosY, long* screenPosX, long* screenPosY)
{
	// Define new vector with translated values.
	*screenPosX = (long)(tilePosX * TILE_SIZE) - 768;
	*screenPosY = (long)(-tilePosY * TILE_SIZE) + 448;
}

// Converts a screen position to a tile position.
void ScreenToTilePos(long screenPosX, long screenPosY, unsigned char* tilePosX, unsigned char* tilePosY)
{
	// Translate values to snap to a grid.
	*tilePosX = (unsigned char)(screenPosX / TILE_SIZE);
	*tilePosY = (unsigned char)(screenPosY / TILE_SIZE);
}

// Returns current tilemap.
Tilemap* GetCurrentTilemap(void)
{
	return &currTilemap;
}

// Sets a new tilemap.
void SetCurrentTilemap(Tilemap* tilemap)
{
	currTilemap = *tilemap;
}

// Returns current tilemap.
Tilemap* GetNextTilemap(void)
{
	return &nextTilemap;
}

// Sets a new tilemap.
void SetNextTilemap(Tilemap* tilemap)
{
	nextTilemap = *tilemap;
}

void TileBob(unsigned char x, unsigned char y)
{
	float yOffset = sinf((AEFrameRateControllerGetFrameCount() * 0.8F + x * 10) * 0.0375F) * 16;

	GetCurrentTilemap()->tileOffsets[x][y].x = 0;
	GetCurrentTilemap()->tileOffsets[x][y].y = yOffset;
}
void TileToIce(unsigned char x, unsigned char y)
{
	for (int x2 = -1; x2 < 2; ++x2)
	{
		for (int y2 = -1; y2 < 2; ++y2)
		{
			if (GetCurrentTilemap()->tiles[x + x2][y + y2]->id == TILE_SOLID_WATER ||
				GetCurrentTilemap()->tiles[x + x2][y + y2]->id == TILE_FLOWING_WATER)
			{
				Vector2D pos = {
					(float)x * TILE_SIZE - 768,
					448 - (float)y * TILE_SIZE + (GetCurrentTilemap()->height - 13) * TILE_SIZE
				};

				// Init bounding box memory.
				GetGame()->tileCollisions[x][y] = FromPoints(pos, TILE_SIZE, TILE_SIZE, 0, 0, false);

				if (GetCurrentTilemap()->tiles[x + x2][y + y2]->id == TILE_SOLID_WATER)
				{
					// Set block to ice block.
					GetCurrentTilemap()->tiles[x + x2][y + y2] = &allTiles[TILE_ICE];
				}
				else
				{
					// Set block to ice block.
					GetCurrentTilemap()->tiles[x + x2][y + y2] = &allTiles[TILE_FLOWING_ICE];
				}

				// Freeze next block.
				TileToIce((unsigned char)(x + x2), (unsigned char)(y + y2));
			}
		}
	}
}
void TileToWater(unsigned char x, unsigned char y)
{
	for (int x2 = -1; x2 < 2; ++x2)
	{
		for (int y2 = -1; y2 < 2; ++y2)
		{
			if (GetCurrentTilemap()->tiles[x + x2][y + y2]->id == TILE_ICE ||
				GetCurrentTilemap()->tiles[x + x2][y + y2]->id == TILE_FLOWING_ICE)
			{
				// Free bounding box memory.
				FreeAABB(GetGame()->tileCollisions[x][y]);
				GetGame()->tileCollisions[x][y] = NULL;

				if (GetCurrentTilemap()->tiles[x + x2][y + y2]->id == TILE_ICE)
				{
					// Set block to solid water block.
					GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_SOLID_WATER];
				}
				else
				{
					// Set block to solid water block.
					GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_FLOWING_WATER];
				}

				// Unfreeze next block.
				TileToWater((unsigned char)(x + x2), (unsigned char)(y + y2));
			}
		}
	}
}
void TileDamagePlayer(unsigned char x, unsigned char y)
{
	Vector2D pos = { x, y };
	PlayerHurt(&pos);
}
void TileToggleEyeBlock(unsigned char x, unsigned char y)
{
	Vector2D pos = { 0, 0 };

	switch (GetCurrentTilemap()->tiles[x][y]->id)
	{
		case TILE_DOTTED_BLOCK:
			// Get position of block.
			pos.x = (float)x * TILE_SIZE - 768;
			pos.y = 448 - (float)y * TILE_SIZE + (GetCurrentTilemap()->height - 13) * TILE_SIZE;

			// Init bounding box memory.
			GetGame()->tileCollisions[x][y] = FromPoints(pos, TILE_SIZE, TILE_SIZE, 0, 0, false);

			// Set block to solid block.
			GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_FILLED_BLOCK];
			PlaySFX("./Assets/SFX/visblock_appear.mp3");
			break;

		case TILE_FILLED_BLOCK:
			// Free bounding box memory.
			FreeAABB(GetGame()->tileCollisions[x][y]);
			GetGame()->tileCollisions[x][y] = NULL;

			// Set block to ghost block.
			GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_DOTTED_BLOCK];
			break;
	}
}
void TileToCheckpoint(unsigned char x, unsigned char y)
{
	GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_CHECKPOINT];

	Vector2D pos = {
		(float)x * TILE_SIZE - 768,
		448 - (float)y * TILE_SIZE + (GetCurrentTilemap()->height - 13) * TILE_SIZE
	};

	// Init bounding box memory.
	GetGame()->tileCollisions[x][y] = FromPoints(pos, TILE_SIZE, TILE_SIZE, 0, 0, false);
}
void TileToGem(unsigned char x, unsigned char y)
{
	GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_GEM];

	Vector2D pos = {
		(float)x * TILE_SIZE - 768,
		448 - (float)y * TILE_SIZE + (GetCurrentTilemap()->height - 13) * TILE_SIZE
	};

	// Init bounding box memory.
	GetGame()->tileCollisions[x][y] = FromPoints(pos, TILE_SIZE, TILE_SIZE, 0, 0, false);
}
void TileCollectGem(unsigned char x, unsigned char y)
{
	GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_AIR];
	PlaySFX("./Assets/SFX/sample_beep.wav");

	++GetPlayer()->gemCount;
}
void TileSpawnPaintbrush(unsigned char x, unsigned char y)
{
	GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_AIR];

	if (GetPlayer()->hasBrush)
		return;

	GameObjectPtr paintbrush = GameObjectCreate();

	TransformPtr transform = TransformCreate();
	GameObjectAddTransform(paintbrush, transform);

	Vector2D pos = {
		(float)x * TILE_SIZE - 768,
		448 - (float)y * TILE_SIZE + (GetCurrentTilemap()->height - 13) * TILE_SIZE
	};

	Vector2D scale = { 1.2F, 1.5F };
	TransformSetTranslation(transform, &pos);
	TransformSetRotation(transform, 0);
	TransformSetScale(transform, &scale);

	SpritePtr sprite = SpriteCreate();
	GameObjectAddSprite(paintbrush, sprite);

	AABB AABB = FromPoints(pos, scale.x + TILE_SIZE * 1.5F, scale.y + TILE_SIZE * 1.5F, 0, 0, true);
	GameObjectAddCollider(paintbrush, AABB);

	SpriteSourcePtr spriteSource = SpriteSourceCreate(1, 1, SpriteSourceGetTexture(SpriteGetSpriteSource(GameObjectGetSprite(GetBrush()))));

	SpriteSetMesh(sprite, tileMesh);
	SpriteSetSpriteSource(sprite, spriteSource);
	SpriteSetFrame(sprite, 0);

	GameObjectSetName(paintbrush, "Paintbrush");
	AddGameObject(paintbrush);
}
void TileDespawnPaintbrush(unsigned char x, unsigned char y)
{

}
void TileSpawnGreenSlime(unsigned char x, unsigned char y)
{
	GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_AIR];

	// Get enemy's position on-screen.
	Vector2D pos = {
		(float)x * TILE_SIZE - 768,
		448 - (float)y * TILE_SIZE + (GetCurrentTilemap()->height - 13) * TILE_SIZE
	};

	// Spawn the enemy.
	SpawnEnemy(Slime, Green, pos, "Green Slime");
}
void TileSpawnYellowSlime(unsigned char x, unsigned char y)
{
	GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_AIR];

	// Get enemy's position on-screen.
	Vector2D pos = {
		(float)x * TILE_SIZE - 768,
		448 - (float)y * TILE_SIZE + (GetCurrentTilemap()->height - 13) * TILE_SIZE
	};

	// Spawn the enemy.
	SpawnEnemy(Slime, Yellow, pos, "Yellow Slime");
}
void TileSpawnBlueSlime(unsigned char x, unsigned char y)
{
	GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_AIR];

	// Get enemy's position on-screen.
	Vector2D pos = {
		(float)x * TILE_SIZE - 768,
		448 - (float)y * TILE_SIZE + (GetCurrentTilemap()->height - 13) * TILE_SIZE
	};

	// Spawn the enemy.
	SpawnEnemy(Slime, Blue, pos, "Blue Slime");
}
void TileSpawnPurpleSlime(unsigned char x, unsigned char y)
{
	GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_AIR];

	// Get enemy's position on-screen.
	Vector2D pos = {
		(float)x * TILE_SIZE - 768,
		448 - (float)y * TILE_SIZE + (GetCurrentTilemap()->height - 13) * TILE_SIZE
	};

	// Spawn the enemy.
	SpawnEnemy(Slime, Purple, pos, "Purple Slime");
}
void TileSpawnFleye(unsigned char x, unsigned char y)
{
	GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_AIR];

	// Get enemy's position on-screen.
	Vector2D pos = {
		(float)x * TILE_SIZE - 768,
		448 - (float)y * TILE_SIZE + (GetCurrentTilemap()->height - 13) * TILE_SIZE
	};

	// Spawn the enemy.
	SpawnEnemy(Fleye, Blue, pos, "Fleye");
}
void TileSpawnPaintKnight(unsigned char x, unsigned char y)
{
	GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_AIR];

	// Get enemy's position on-screen.
	Vector2D pos = {
		(float)x * TILE_SIZE - 768,
		448 - (float)y * TILE_SIZE + (GetCurrentTilemap()->height - 13) * TILE_SIZE
	};

	// Spawn the enemy.
	//SpawnEnemy(Fleye, Blue, spawnPos, "Fleye");
}
void TileDespawnEnemy(unsigned char x, unsigned char y)
{

}
void TileSpawnCrate(unsigned char x, unsigned char y)
{
	GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_AIR];

	GameObjectPtr crate = GameObjectCreate();

	TransformPtr transform = TransformCreate();
	GameObjectAddTransform(crate, transform);

	// Get enemy's position on-screen.
	Vector2D pos = {
		(float)x * TILE_SIZE - 768,
		448 - (float)y * TILE_SIZE + (GetCurrentTilemap()->height - 13) * TILE_SIZE
	};

	Vector2D scale = { 1, 1 };
	TransformSetTranslation(transform, &pos);
	TransformSetRotation(transform, 0);
	TransformSetScale(transform, &scale);

	SpritePtr sprite = SpriteCreate();
	GameObjectAddSprite(crate, sprite);

	AABB AABB = FromPoints(pos, scale.x + TILE_SIZE, scale.y + TILE_SIZE, 0, 0, false);
	GameObjectAddCollider(crate, AABB);

	SpriteSourcePtr spriteSource = SpriteSourceCreate(1, 1, allTiles[TILE_CRATE].texture);

	SpriteSetMesh(sprite, tileMesh);
	SpriteSetSpriteSource(sprite, spriteSource);
	SpriteSetFrame(sprite, 0);

	GameObjectSetName(crate, "Crate");
	AddGameObject(crate);
}
void TileDespawnCrate(unsigned char x, unsigned char y)
{

}
void TileToGrownFlower(unsigned char x, unsigned char y)
{
	GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_GROWN_FLOWERS];
}
void TileHideVinePath(unsigned char x, unsigned char y)
{
	AEGfxTextureUnload(allTiles[TILE_VINE_PATH].texture);
	allTiles[TILE_VINE_PATH].texture = NULL;
}
void TileActivateCheckpoint(unsigned char x, unsigned char y)
{
	SaveAllData();
}
void TileSlowPlayer(unsigned char x, unsigned char y)
{
	SetInWater(true);
}
void TileSpeedPlayer(unsigned char x, unsigned char y)
{
	SetInWater(false);
}
void TileSlowPlayerAndMakeFall(unsigned char x, unsigned char y)
{

}
void TileCollectHeartContainer(unsigned char x, unsigned char y)
{
	IncreaseMaxHealth(1);

	GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_AIR];
}
void TileCollectPaintContainer(unsigned char x, unsigned char y)
{
	IncreaseMaxPaint(2.5F);

	GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_AIR];
}
void TileGrowVine(unsigned char x, unsigned char y)
{
	for (int x2 = -1; x2 < 2; ++x2)
	{
		for (int y2 = -1; y2 < 2; ++y2)
		{
			if (GetCurrentTilemap()->tiles[x + x2][y + y2]->id == TILE_VINE_PATH)
			{
				Vector2D pos = {
					(float)x* TILE_SIZE - 768,
					448 - (float)y * TILE_SIZE + (GetCurrentTilemap()->height - 13) * TILE_SIZE
				};

				// Init bounding box memory.
				GetGame()->tileCollisions[x + x2][y + y2] = FromPoints(pos, TILE_SIZE, TILE_SIZE, 0, 0, false);

				// Set block to solid block.
				GetCurrentTilemap()->tiles[x + x2][y + y2] = &allTiles[TILE_VINE_BLOCK];

				// Grow next block.
				TileGrowVine((unsigned char)(x + x2), (unsigned char)(y + y2));
			}
		}
	}
}
void TileUngrowVine(unsigned char x, unsigned char y)
{
	for (int x2 = -1; x2 < 2; ++x2)
	{
		for (int y2 = -1; y2 < 2; ++y2)
		{
			if (GetCurrentTilemap()->tiles[x + x2][y + y2]->id == TILE_VINE_BLOCK)
			{
				// Free bounding box memory.
				FreeAABB(GetGame()->tileCollisions[x + x2][y + y2]);
				GetGame()->tileCollisions[x + x2][y + y2] = NULL;

				// Set block to vine path block.
				GetCurrentTilemap()->tiles[x + x2][y + y2] = &allTiles[TILE_VINE_PATH];

				// Retract next block.
				TileUngrowVine((unsigned char)(x + x2), (unsigned char)(y + y2));
			}
		}
	}
}
void TileGenerateElectricity(unsigned char x, unsigned char y)
{

}
void TileDisappearAfterTime(unsigned char x, unsigned char y)
{

}
void TileCollectRedPaint(unsigned char x, unsigned char y)
{
	GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_AIR];
	UnlockPaint(PAINT_RED);
}
void TileCollectBluePaint(unsigned char x, unsigned char y)
{
	GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_AIR];
	UnlockPaint(PAINT_BLUE);
}
void TileCollectGreenPaint(unsigned char x, unsigned char y)
{
	GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_AIR];
	UnlockPaint(PAINT_GREEN);
}
void TileCollectYellowPaint(unsigned char x, unsigned char y)
{
	GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_AIR];
	UnlockPaint(PAINT_YELLOW);
}
void TileCollectOrangePaint(unsigned char x, unsigned char y)
{
	GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_AIR];
	UnlockPaint(PAINT_ORANGE);
}
void TileCollectPurplePaint(unsigned char x, unsigned char y)
{
	GetCurrentTilemap()->tiles[x][y] = &allTiles[TILE_AIR];
	UnlockPaint(PAINT_PURPLE);
}
