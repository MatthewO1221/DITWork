#include "stdafx.h"		// Should be included first in every .c file!
#include "tilemap.h"
#include "paint.h"

Tile NewTile(Tile allTiles[],
			 unsigned char id,
			 enum PaintColor paintColor,
			 unsigned char requiresFullPaint,
			 unsigned char isEntity,
			 AEGfxTexture* texture, 
			 TileInfoFunctionPtr onInit,
			 TileInfoFunctionPtr onUpdate,
			 TileInfoFunctionPtr onShutdown,
			 TileInfoFunctionPtr onEnterCollision,
			 TileInfoFunctionPtr onStayCollision,
			 TileInfoFunctionPtr onExitCollision,
			 TileInfoFunctionPtr onApplyPaint,
			 TileInfoFunctionPtr onRemovePaint)
{
	Tile newTile =
	{
		.id = id,

		.paintColor = (char)paintColor,
		.requiresFullPaint = requiresFullPaint,

		.isEntity = isEntity,

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
	allTiles[id] = newTile;

	return newTile;
}

TileEntity NewTileEntity(TileEntity tileEntities[], Tile* template, float xPos, float yPos, unsigned char ogXPos, unsigned char ogYPos, int index)
{
	TileEntity newTileEntity =
	{
		.tile = template,

		.xPos = xPos,
		.yPos = yPos,

		.originalXPos = ogXPos,
		.originalYPos = ogYPos
	};
	tileEntities[index] = newTileEntity;

	return newTileEntity;
}

// Create all the tile types.
void DefineTiles(Tile allTiles[])
{
	NewTile(allTiles, TILE_AIR,					PAINT_NONE,		0, 0, NULL,															NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_GROUND,				PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/ground.png"),				NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_SOLID_WATER,			PAINT_BLUE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/solid_water.png"),			NULL, NULL, NULL, NULL, NULL, NULL, ToIce, NULL);
	NewTile(allTiles, TILE_FLOOR_SPIKES,		PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/floor_spikes.png"),			NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_DOTTED_BLOCK,		PAINT_RED,		0, 0, AEGfxTextureLoad("./Assets/Tiles/dotted_block.png"),			NULL, NULL, NULL, NULL, NULL, NULL, ToFilledBlock, NULL);
	NewTile(allTiles, TILE_VINE_STEM_UP,		PAINT_GREEN,	0, 0, AEGfxTextureLoad("./Assets/Tiles/vine_stem_up.png"),			NULL, NULL, NULL, NULL, NULL, NULL, GrowVine, NULL);
	NewTile(allTiles, TILE_BATTERY,				PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/battery.png"),				NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_POWERED_DOOR_OFF,	PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/powered_door_off.png"),		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_GEM,					PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/gem.png"),					NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_SAPLING,				PAINT_GREEN,	0, 0, AEGfxTextureLoad("./Assets/Tiles/sapling.png"),				NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_ICE,					PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/ice.png"),					NULL, NULL, ToSolidWater, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_FILLED_BLOCK,		PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/filled_block.png"),			NULL, NULL, ToDottedBlock, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_RED_PAINT_UNLOCK,	PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/red_paint_unlock.png"),		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_UPGRADE_PART,		PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/upgrade_part.png"),			NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_WOOD,				PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/wood.png"),					NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_GREEN_SLIME_SPAWN,	PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/green_slime_spawn.png"),		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_CHECKPOINT,			PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/checkpoint.png"),			NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_CLOUD,				PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/cloud.png"),					NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_DOTTED_CHECKPOINT,	PAINT_RED,		0, 0, AEGfxTextureLoad("./Assets/Tiles/dotted_checkpoint.png"),		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_DOTTED_GEM,			PAINT_RED,		0, 0, AEGfxTextureLoad("./Assets/Tiles/dotted_gem.png"),			NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_RED_EASEL,			PAINT_RED,		0, 0, AEGfxTextureLoad("./Assets/Tiles/red_easel.png"),				NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_GROWN_FLOWERS,		PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/grown_flowers.png"),			NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_FLOWING_WATER,		PAINT_BLUE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/flowing_water.png"),			NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_KNIGHT_SPAWN,		PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/knight_spawn.png"),			NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_LEAF_BLOCK,			PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/leaf_block.png"),			NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_POWER_SWITCH_ON,		PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/power_switch_on.png"),		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_POWER_SWITCH_OFF,	PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/power_switch_off.png"),		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_POWERED_DOOR_ON,		PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/powered_door_on.png"),		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_BLUE_SLIME_SPAWN,	PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/blue_slime_spawn.png"),		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_YELLOW_SLIME_SPAWN,	PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/yellow_slime_spawn.png"),	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_PURPLE_SLIME_SPAWN,	PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/purple_slime_spawn.png"),	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_GREEN_EASEL,			PAINT_GREEN,	0, 0, AEGfxTextureLoad("./Assets/Tiles/green_easel.png"),			NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_YELLOW_EASEL,		PAINT_YELLOW,	0, 0, AEGfxTextureLoad("./Assets/Tiles/yellow_easel.png"),			NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_BLUE_EASEL,			PAINT_BLUE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/blue_easel.png"),			NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_ORANGE_EASEL,		PAINT_ORANGE,	0, 0, AEGfxTextureLoad("./Assets/Tiles/orange_easel.png"),			NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_PURPLE_EASEL,		PAINT_PURPLE,	0, 0, AEGfxTextureLoad("./Assets/Tiles/purple_easel.png"),			NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_CEILING_SPIKES,		PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/ceiling_spikes.png"),		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_SNOW_BLOCK,			PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/snow_block.png"),			NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_STALACTITE,			PAINT_NONE,		0, 1, AEGfxTextureLoad("./Assets/Tiles/stalactite.png"),			NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_UNGROWN_FLOWER,		PAINT_GREEN,	0, 0, AEGfxTextureLoad("./Assets/Tiles/ungrown_flowers.png"),		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_STALACTITE_TRIGGER,	PAINT_BLUE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/stalactite_trigger.png"),	TriggerDespawn, NULL, TriggerStalactiteSpawn, DropStalactite, NULL, NULL, DropStalactite, NULL);
	NewTile(allTiles, TILE_WALL_SPIKES_LEFT,	PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/wall_spikes_left.png"),		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_WALL_SPIKES_RIGHT,	PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/wall_spikes_right.png"),		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_VINE_PATH,			PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/vine_path.png"),				TriggerDespawn, NULL, TriggerVinePathSpawn, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_VINE_BLOCK,			PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/vine_block.png"),			NULL, NULL, ToVinePath, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_EYEBAT_SPAWN,		PAINT_NONE,		0, 0, AEGfxTextureLoad("./Assets/Tiles/eyebat_spawn.png"),			NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_CRATE,				PAINT_NONE,		0, 1, AEGfxTextureLoad("./Assets/Tiles/crate.png"),					NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_VINE_STEM_DOWN, PAINT_GREEN, 0, 1, AEGfxTextureLoad("./Assets/Tiles/vine_stem_down.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_VINE_STEM_LEFT, PAINT_GREEN, 0, 1, AEGfxTextureLoad("./Assets/Tiles/vine_stem_left.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_VINE_STEM_RIGHT, PAINT_GREEN, 0, 1, AEGfxTextureLoad("./Assets/Tiles/vine_stem_right.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_HEART_CONTAINER, PAINT_NONE, 0, 1, AEGfxTextureLoad("./Assets/Tiles/heart_container.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_SEMI_SOLID_PLATFORM, PAINT_NONE, 0, 1, AEGfxTextureLoad("./Assets/Tiles/semi_solid_platform.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	NewTile(allTiles, TILE_PAINT_CONTAINER, PAINT_NONE, 0, 1, AEGfxTextureLoad("./Assets/Tiles/paint_container.png"), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

// Draw a tile at x, y (translated from screen coords).
void DrawTile(Tile* tile, float x, float y, AEGfxVertexList* mesh, float alpha)
{
	// Exit function if no texture exists.
	if (tile->texture == NULL)
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
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}
// Draw a tileEntity at x, y (translated from screen coords).
void DrawTileEntity(TileEntity* tileEntity, AEGfxVertexList* mesh, float alpha)
{
	// Exit function if no texture exists.
	if (tileEntity == NULL ||
		tileEntity->tile == NULL ||
		tileEntity->tile->texture == NULL)
		return;

	// Set render mode.
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	// Set the position.
	AEGfxSetPosition(tileEntity->xPos, tileEntity->yPos);

	// Render the texture.
	AEGfxTextureSet(tileEntity->tile->texture, 0.0F, 0.0F);

	// Set transparency.
	AEGfxSetTransparency(alpha);

	// Draw the mesh.
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}
// Draw all the tiles in a level.
void DrawLevel(Level* level, AEGfxVertexList* mesh)
{
	// Iterate through y dimension.
	for (int y = 0; y < MAX_AREA_HEIGHT; ++y)
	{
		// Iterate through x dimension.
		for (int x = 0; x < MAX_AREA_WIDTH; ++x)
		{
			// Convert tile position to screen position.
			long screenPosX, screenPosY;
			TileToScreenPos((unsigned char)x, (unsigned char)y, &screenPosX, &screenPosY);

			// Set the tile based on the tiles in the tile list.
			DrawTile(level->tilemap->tiles[x][y], (float)screenPosX, (float)screenPosY, mesh, 1.0F);
		}
	}
}

void DrawTileEntities(GameplayInstance *game, AEGfxVertexList* mesh)
{
	// Iterate through all the tile entities.
	for (int i = 0; i < MAX_TILE_ENTITIES; ++i)
	{
		DrawTileEntity(&game->tileEntities[i], mesh, 1.0F);
	}
}

// Get tile tile at x, y in level level.
Tile* GetTile(const Level* level, unsigned char x, unsigned char y)
{
	// Return the tile at x, y in level level.
	return level->tilemap->tiles[x][y];
}

// Place tile tile at x, y in level level.
void PlaceTile(Tile* tile, Level* level, unsigned char x, unsigned char y)
{
	// Place a tile in the level tilemap.
	level->tilemap->tiles[x][y] = tile;
}
// Erase tile tile at x, y in level level.
void EraseTile(Level* level, unsigned char x, unsigned char y, Tile* emptyTile)
{
	// Set the tile to air.
	level->tilemap->tiles[x][y] = emptyTile;
}

// Saves a level level to the given filePath.
int SaveLevel(const Level* level)
{
	TCHAR buffer_w[MAX_PATH] = { 0 };

	if (!ChooseLevel(buffer_w, SAVE_MODE))
		return 0;

	char buffer[MAX_PATH] = { 0 };
	size_t size;
	wcstombs_s(&size, buffer, sizeof(buffer), buffer_w, sizeof(buffer));

	return SaveLevelAtPath(level, buffer);
}
int SaveLevelAtPath(const Level* level, const char *path)
{
	// Open a file at filePath.
	FILE* file_ptr;
	fopen_s(&file_ptr, path, "w");

	// Check if file was successfully opened.
	if (file_ptr != NULL)
	{
		// Save the width and height of the level (MAX 255x255).
		putc(level->tilemap->width, file_ptr);
		putc(level->tilemap->height, file_ptr);
		putc(level->levelNum, file_ptr);

		// Iterate through y dimension.
		for (int y = 0; y < MAX_AREA_HEIGHT; ++y)
		{
			// Iterate through x dimension.
			for (int x = 0; x < MAX_AREA_WIDTH; ++x)
			{
				// Save the tile type for each position on the plane.
				putc((int)level->tilemap->tiles[x][y]->id, file_ptr);
			}
		}

		// Close the file.
		fclose(file_ptr);

		return 0;
	}

	return -1;
}
// Loads a level from filePath.
int LoadLevel(Tile allTiles[], Level *level)
{
	TCHAR buffer_w[MAX_PATH] = { 0 };

	if (!ChooseLevel(buffer_w, LOAD_MODE))
		return 0;

	char buffer[MAX_PATH] = { 0 };
	size_t size;
	wcstombs_s(&size, buffer, sizeof(buffer), buffer_w, sizeof(buffer));

	return LoadLevelAtPath(allTiles, level, buffer);
}
int LoadLevelAtPath(Tile allTiles[], Level* level, const char *path)
{
	// Open a file at filePath.
	FILE* file_ptr;
	fopen_s(&file_ptr, path, "r");

	// Check if file was successfully opened.
	if (file_ptr != NULL)
	{
		// Load the level name.
		strcpy_s(level->levelName, sizeof(level->levelName), path);

		// Get information about the tiles.
		level->tilemap->width = (unsigned char)getc(file_ptr);
		level->tilemap->height = (unsigned char)getc(file_ptr);
		level->levelNum = (unsigned char)getc(file_ptr);

		// Iterate through y dimension.
		for (int y = 0; y < MAX_AREA_HEIGHT; ++y)
		{
			// Iterate through x dimension.
			for (int x = 0; x < MAX_AREA_WIDTH; ++x)
			{
				// Set the tile based on the tiles in the tile list.
				level->tilemap->tiles[x][y] = &allTiles[getc(file_ptr)];
			}
		}

		// Close the file.
		fclose(file_ptr);

		return 0;
	}

	return -1;
}
// Creates a new level at filePath.
int NewLevel(Level *level)
{
	TCHAR buffer_w[MAX_PATH] = { 0 };

	if (!ChooseLevel(buffer_w, NEW_MODE))
		return 0;

	char buffer[MAX_PATH] = { 0 };
	size_t size;
	wcstombs_s(&size, buffer, sizeof(buffer), buffer_w, sizeof(buffer));

	return SaveLevelAtPath(level, buffer);
}

void ConvertSize(const char* path)
{
	// Open a file at filePath.
	FILE* file_ptr;
	fopen_s(&file_ptr, path, "r");

	int width = 0;
	int height = 0;
	int num = 1;

	// Create array of ids.
	int ids[MAX_AREA_WIDTH][MAX_AREA_HEIGHT] = { 0 };

	// Check if file was successfully opened.
	if (file_ptr != NULL)
	{
		width = (unsigned char)getc(file_ptr);
		height = (unsigned char)getc(file_ptr);
		num = (unsigned char)getc(file_ptr);

		// Iterate through y dimension.
		for (int y = 0; y < 35; ++y)
		{
			// Iterate through x dimension.
			for (int x = 0; x < 59; ++x)
			{
				int newID = getc(file_ptr);

				if (x < MAX_AREA_WIDTH &&
					y < MAX_AREA_HEIGHT)
				{
					ids[x][y] = newID;
				}
			}
		}

		// Close the file.
		fclose(file_ptr);

		// Open a file at filePath.
		FILE* file_ptr_2;
		fopen_s(&file_ptr_2, path, "w");

		// Check if file was successfully opened.
		if (file_ptr_2 != NULL)
		{
			// Save the width and height of the level (MAX 255x255).
			putc(width, file_ptr_2);
			putc(height, file_ptr_2);
			putc(num, file_ptr_2);

			// Iterate through y dimension.
			for (int y = 0; y < MAX_AREA_HEIGHT; ++y)
			{
				// Iterate through x dimension.
				for (int x = 0; x < MAX_AREA_WIDTH; ++x)
				{
					// Save the tile type for each position on the plane.
					putc(ids[x][y], file_ptr_2);
				}
			}

			// Close the file.
			fclose(file_ptr_2);
		}
	}
}

// Initializes an empty level.
void InitLevel(Tilemap* levelTiles, Level* currentLevel, Tile allTiles[], int* worldNum, const char *name)
{
	// Current level being edited.
	levelTiles->width = MIN_AREA_WIDTH - 1;
	levelTiles->height = MIN_AREA_HEIGHT - 1;
	currentLevel->levelNum = *worldNum++;
	currentLevel->tilemap = levelTiles;
	strcpy_s(currentLevel->levelName, sizeof(currentLevel->levelName), name);

	// Set default level layout.
	// Iterate through y dimension.
	for (int y = 0; y < MAX_AREA_HEIGHT; ++y)
	{
		// Iterate through x dimension.
		for (int x = 0; x < MAX_AREA_WIDTH; ++x)
		{
			// Set to air tile.
			currentLevel->tilemap->tiles[x][y] = ((x == 0 || x == 1 ||
												 y == 0 ||
												 x == 18 || x == 19 ||
												 y == 10 || y == 11) && 
												 x < 20 && y < 12) ? &allTiles[TILE_GROUND] : &allTiles[TILE_AIR];
		}
	}
}

// Choose a level with the file picker dialogue.
BOOL ChooseLevel(TCHAR buffer[], SAVELOADMODE mode)
{
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = L"Level Files (.lvl)\0*.lvl\0\0";
	ofn.lpstrTitle = mode == SAVE_MODE ? L"Save As" : mode == NEW_MODE ? L"New" : L"Open";
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
		case NEW_MODE:
			return GetSaveFileName(&ofn);
			break;
		default:
			return 0;
			break;
	}
}

// Convert a tile position to a screen position.
void TileToScreenPos(unsigned char tilePosX, unsigned char tilePosY, long *screenPosX, long *screenPosY)
{
	// Define new vector with translated values.
	*screenPosX = (long)(tilePosX * TILE_SIZE) - 832;
	*screenPosY = ((long)(-tilePosY * TILE_SIZE)) + 512;
}
// Convert a screen position to a tile position.
void ScreenToTilePos(long screenPosX, long screenPosY, unsigned char* tilePosX, unsigned char* tilePosY)
{
	// Translate values to snap to a grid.
	*tilePosX = (unsigned char)(screenPosX / TILE_SIZE);
	*tilePosY = (unsigned char)(screenPosY / TILE_SIZE);
}

void InitPlaymodeLevel(GameplayInstance *instance)
{
	int entityCount = 0;

	// Iterate through all the tile entities.
	for (int i = 0; i < MAX_TILE_ENTITIES; ++i)
	{
		instance->tileEntities[i].tile = NULL;

		instance->tileEntities[i].xPos = 0.0F;
		instance->tileEntities[i].yPos = 0.0F;

		instance->tileEntities[i].originalXPos = 0;
		instance->tileEntities[i].originalYPos = 0;
	}

	// Iterate through y dimension.
	for (int y = 0; y < MAX_AREA_HEIGHT; ++y)
	{
		// Iterate through x dimension.
		for (int x = 0; x < MAX_AREA_WIDTH; ++x)
		{
			if (instance->level->tilemap->tiles[x][y]->isEntity)
			{
				// Convert tile position to screen position.
				long screenPosX, screenPosY;
				TileToScreenPos((unsigned char)x, (unsigned char)y, &screenPosX, &screenPosY);

				// Create a new tile entity to replace the old one.
				NewTileEntity(instance->tileEntities, instance->level->tilemap->tiles[x][y], (float)screenPosX, (float)screenPosY, (unsigned char)x, (unsigned char)y, entityCount++);
				instance->level->tilemap->tiles[x][y] = &instance->allTiles[TILE_AIR];
			}

			if (instance->level->tilemap->tiles[x][y]->onInit != NULL)
			{
				// Call the tile's init function.
				instance->level->tilemap->tiles[x][y]->onInit(instance, (unsigned char)x, (unsigned char)y);
			}
		}
	}
}
void UpdatePlaymodeLevel(GameplayInstance* instance)
{
	// Iterate through y dimension.
	for (int y = 0; y < MAX_AREA_HEIGHT; ++y)
	{
		// Iterate through x dimension.
		for (int x = 0; x < MAX_AREA_WIDTH; ++x)
		{
			if (instance->level->tilemap->tiles[x][y]->onUpdate != NULL)
			{
				// Call the tile's init function.
				instance->level->tilemap->tiles[x][y]->onUpdate(instance, (unsigned char)x, (unsigned char)y);
			}
		}
	}
}
void ShutdownPlaymodeLevel(GameplayInstance* instance)
{
	// Iterate through all the tile entities.
	for (int i = 0; i < MAX_TILE_ENTITIES; ++i)
	{
		if (instance->tileEntities[i].tile != NULL)
		{
			instance->level->tilemap->tiles[instance->tileEntities[i].originalXPos][instance->tileEntities[i].originalYPos] = &instance->allTiles[instance->tileEntities[i].tile->id];

			instance->tileEntities[i].tile = NULL;

			instance->tileEntities[i].xPos = 0.0F;
			instance->tileEntities[i].yPos = 0.0F;

			instance->tileEntities[i].originalXPos = 0;
			instance->tileEntities[i].originalYPos = 0;
		}
	}

	// Iterate through y dimension.
	for (int y = 0; y < MAX_AREA_HEIGHT; ++y)
	{
		// Iterate through x dimension.
		for (int x = 0; x < MAX_AREA_WIDTH; ++x)
		{
			if (instance->level->tilemap->tiles[x][y]->onShutdown != NULL)
			{
				// Call the tile's init function.
				instance->level->tilemap->tiles[x][y]->onShutdown(instance, (unsigned char)x, (unsigned char)y);
			}
		}
	}
}

void ToFilledBlock(GameplayInstance* game, unsigned char tileX, unsigned char tileY)
{
	game->level->tilemap->tiles[tileX][tileY] = &game->allTiles[TILE_FILLED_BLOCK];
}
void ToDottedBlock(GameplayInstance* game, unsigned char tileX, unsigned char tileY)
{
	game->level->tilemap->tiles[tileX][tileY] = &game->allTiles[TILE_DOTTED_BLOCK];
}
void ToGem(GameplayInstance* game, unsigned char tileX, unsigned char tileY)
{
	game->level->tilemap->tiles[tileX][tileY] = &game->allTiles[TILE_GEM];
}
void ToCheckpoint(GameplayInstance* game, unsigned char tileX, unsigned char tileY)
{
	game->level->tilemap->tiles[tileX][tileY] = &game->allTiles[TILE_CHECKPOINT];
}
void ToIce(GameplayInstance* game, unsigned char tileX, unsigned char tileY)
{
	game->level->tilemap->tiles[tileX][tileY] = &game->allTiles[TILE_ICE];
}
void ToSolidWater(GameplayInstance* game, unsigned char tileX, unsigned char tileY)
{
	game->level->tilemap->tiles[tileX][tileY] = &game->allTiles[TILE_SOLID_WATER];
}
void ToVinePath(GameplayInstance* game, unsigned char tileX, unsigned char tileY)
{
	game->level->tilemap->tiles[tileX][tileY] = &game->allTiles[TILE_VINE_PATH];
}
void TriggerDespawn(GameplayInstance* game, unsigned char tileX, unsigned char tileY)
{
	game->level->tilemap->tiles[tileX][tileY]->texture = NULL;
}
void TriggerStalactiteSpawn(GameplayInstance* game, unsigned char tileX, unsigned char tileY)
{
	game->level->tilemap->tiles[tileX][tileY]->texture = AEGfxTextureLoad("./Assets/Tiles/stalactite_trigger.png");
}
void TriggerVinePathSpawn(GameplayInstance* game, unsigned char tileX, unsigned char tileY)
{
	game->level->tilemap->tiles[tileX][tileY]->texture = AEGfxTextureLoad("./Assets/Tiles/vine_path.png");
}
void DropStalactite(GameplayInstance* game, unsigned char tileX, unsigned char tileY)
{
	for (int i = 0; i < MAX_TILE_ENTITIES; ++i)
	{
		if (game->tileEntities[i].originalXPos == tileX && game->tileEntities[i].originalYPos < tileY)
		{
			game->tileEntities[i].yPos -= 0.1F;
			break;
		}
	}
}
void GrowVine(GameplayInstance* game, unsigned char tileX, unsigned char tileY)
{
	for (int y = tileY - 1; y < tileY + 2; ++y)
	{
		for (int x = tileX - 1; x < tileX + 2; ++x)
		{
			if (game->level->tilemap->tiles[x][y]->id == TILE_VINE_PATH)
			{
				game->level->tilemap->tiles[x][y] = &game->allTiles[TILE_VINE_BLOCK];
				GrowVine(game, (unsigned char)x, (unsigned char)y);
				break;
			}
		}
	}
}
