// ---------------------------------------------------------------------------
// Project Name		:	Simple Demo
// File Name		:	Main.cpp
// Author			:	Antoine Abi Chacra
// Creation Date	:	2012/01/11
// Purpose			:	Demonstrates Alpha Engine features.
// History			:
// - 2012/01/11		:	- initial implementation
// - 2019/01/04		:	- Fixed AE_ASSERT/AE_WARNING issues.
// - 2019/01/06		:	- Added missing call to AESysUpdate().
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Includes.

#include "stdafx.h"		// Should be included first in every .c file!
#include "AEEngine.h"
#include "tilemap.h"
#include "world.h"
#include "paint.h"
#include "EnemyAI.h"
#include "Player Controller.h"
#include "Player Movement.h"
#include "GameObject.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "AABB.h"
#include <stdio.h>

// ---------------------------------------------------------------------------

// Libraries.
#ifdef _DEBUG
#pragma comment (lib, "Alpha_Engine_D.lib")
#else
#pragma comment (lib, "Alpha_Engine.lib")
#endif

// Defines.
typedef int EDITMODE;
#define LEVEL_EDIT_MODE 1
#define WORLD_EDIT_MODE 2

// ---------------------------------------------------------------------------
// Global variables.

int isGameRunning = 1;

// ---------------------------------------------------------------------------
// Function protoypes.

void UIInputs(Tile**, GameplayInstance*, Level*, World*, long, long, unsigned char, unsigned char, unsigned int, unsigned int, float*, float*, unsigned char, unsigned char*, unsigned char*);
void DrawBorder(float, float, AEGfxTexture**, AEGfxVertexList**);
void ResizeBorderX(AEGfxVertexList**, Level*, unsigned char, unsigned char);
void ResizeBorderY(AEGfxVertexList**, Level*, unsigned char, unsigned char);
void DrawBoundingBoxes(AABB*, int, int, AEGfxVertexList*, AEGfxTexture*);

// ---------------------------------------------------------------------------
// Function definitions.

int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
	UNREFERENCED_PARAMETER(prevInstanceH);
	UNREFERENCED_PARAMETER(command_line);

	// Initialize the system.
	AESysInitInfo systemInfo =
	{
		.mCreateWindow = 1,
		.mAppInstance = instanceH,
		.mShow = show,
		.mWinWidth = 1408,
		.mWinHeight = 896,
		.mCreateConsole = 0,
		.mMaxFrameRate = 60,
		.mpWinCallBack = NULL,
		.mClassStyle = CS_HREDRAW | CS_VREDRAW,
		.mWindowStyle = WS_OVERLAPPEDWINDOW,
		.mWindowHandle = NULL,
		.mHandleWindowMessages = 1
	};
	AESysInit(&systemInfo);
	AESysReset();

	// Initialize level stuff.
	GameplayInstance game;
	Level currentLevel;
	Tilemap levelTiles;
	World currentWorld;

	game.level = &currentLevel;
	DefineTiles(game.allTiles);
	InitLevel(&levelTiles, &currentLevel, game.allTiles, &currentWorld.areaCount, "");

	// Set the current tile to ground.
	Tile* selectedTile = &(game.allTiles[TILE_GROUND]);

	for (int i = 0; i < MAX_AREA_COUNT; ++i)
	{
		currentWorld.areas[i].areaName[0] = '\0';
		currentWorld.areas[i].areaName[1] = '\0';
		currentWorld.areas[i].areaName[2] = '\0';
		currentWorld.areas[i].areaName[3] = '\0';
		currentWorld.areas[i].areaName[4] = '\0';
		currentWorld.areas[i].areaName[5] = '\0';
		currentWorld.areas[i].areaName[6] = '\0';
		currentWorld.areas[i].areaName[7] = '\0';

		currentWorld.areas[i].x = 0;
		currentWorld.areas[i].y = 0;

		currentWorld.areas[i].width = 0;
		currentWorld.areas[i].height = 0;

		for (int y = 0; y < 99; ++y)
		{
			for (int x = 0; x < 59; ++x)
			{
				currentWorld.areas[i].tileID[x][y] = TILE_GROUND;
			}
		}
	}

	currentWorld.areaCount = 0;

	// Create a pixel (paint) mesh.
	AEGfxVertexList* pixelMesh;
	AEGfxMeshStart();
	AEGfxTriAdd(
		-TILE_SIZE * 0.5F / PIXELS_PER_TILE, -TILE_SIZE * 0.5F / PIXELS_PER_TILE, 0xFFFF0000, 0.0f, 1.0f,
		TILE_SIZE * 0.5F / PIXELS_PER_TILE, -TILE_SIZE * 0.5F / PIXELS_PER_TILE, 0xFFFF0000, 0.0833f, 1.0f,
		-TILE_SIZE * 0.5F / PIXELS_PER_TILE, TILE_SIZE * 0.5F / PIXELS_PER_TILE, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		TILE_SIZE * 0.5F / PIXELS_PER_TILE, -TILE_SIZE * 0.5F / PIXELS_PER_TILE, 0xFFFF0000, 0.0833f, 1.0f,
		TILE_SIZE * 0.5F / PIXELS_PER_TILE, TILE_SIZE * 0.5F / PIXELS_PER_TILE, 0xFFFF0000, 0.0833f, 0.0f,
		-TILE_SIZE * 0.5F / PIXELS_PER_TILE, TILE_SIZE * 0.5F / PIXELS_PER_TILE, 0xFFFF0000, 0.0f, 0.0f);
	pixelMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pixelMesh, "Failed to create pixel mesh!");

	// Create a tile mesh.
	AEGfxVertexList* tileMesh;
	AEGfxMeshStart();
	AEGfxTriAdd(
		-TILE_SIZE * 0.5F, -TILE_SIZE * 0.5F, 0xFFFF0000, 0.0f, 1.0f,
		TILE_SIZE * 0.5F, -TILE_SIZE * 0.5F, 0xFFFF0000, 1.0f, 1.0f,
		-TILE_SIZE * 0.5F, TILE_SIZE * 0.5F, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		TILE_SIZE * 0.5F, -TILE_SIZE * 0.5F, 0xFFFF0000, 1.0f, 1.0f,
		TILE_SIZE * 0.5F, TILE_SIZE * 0.5F, 0xFFFF0000, 1.0f, 0.0f,
		-TILE_SIZE * 0.5F, TILE_SIZE * 0.5F, 0xFFFF0000, 0.0f, 0.0f);
	tileMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(tileMesh, "Failed to create tile mesh!");

	// Create a world tile mesh.
	AEGfxVertexList* worldTileMesh;
	AEGfxMeshStart();
	AEGfxTriAdd(
		-TILE_SIZE_WORLD * 0.5F, -TILE_SIZE_WORLD * 0.5F, 0xFFFF0000, 0.0f, 1.0f,
		TILE_SIZE_WORLD * 0.5F, -TILE_SIZE_WORLD * 0.5F, 0xFFFF0000, 1.0f, 1.0f,
		-TILE_SIZE_WORLD * 0.5F, TILE_SIZE_WORLD * 0.5F, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		TILE_SIZE_WORLD * 0.5F, -TILE_SIZE_WORLD * 0.5F, 0xFFFF0000, 1.0f, 1.0f,
		TILE_SIZE_WORLD * 0.5F, TILE_SIZE_WORLD * 0.5F, 0xFFFF0000, 1.0f, 0.0f,
		-TILE_SIZE_WORLD * 0.5F, TILE_SIZE_WORLD * 0.5F, 0xFFFF0000, 0.0f, 0.0f);
	worldTileMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(worldTileMesh, "Failed to create world tile mesh!");

	// Create a border mesh x.
	AEGfxVertexList* borderMeshX;
	AEGfxMeshStart();
	AEGfxTriAdd(
		-TILE_SIZE * currentLevel.tilemap->width - TILE_SIZE * 2.0F, -TILE_SIZE * 0.5F, 0xFFFF0000, 0.0f, 1.0f,
		TILE_SIZE * currentLevel.tilemap->width + TILE_SIZE * 2.0F, -TILE_SIZE * 0.5F, 0xFFFF0000, 1.0f, 1.0f,
		-TILE_SIZE * currentLevel.tilemap->width - TILE_SIZE * 2.0F, TILE_SIZE * 0.5F, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		TILE_SIZE * currentLevel.tilemap->width + TILE_SIZE * 2.0F, -TILE_SIZE * 0.5F, 0xFFFF0000, 1.0f, 1.0f,
		TILE_SIZE * currentLevel.tilemap->width + TILE_SIZE * 2.0F, TILE_SIZE * 0.5F, 0xFFFF0000, 1.0f, 0.0f,
		-TILE_SIZE * currentLevel.tilemap->width - TILE_SIZE * 2.0F, TILE_SIZE * 0.5F, 0xFFFF0000, 0.0f, 0.0f);
	borderMeshX = AEGfxMeshEnd();
	AE_ASSERT_MESG(borderMeshX, "Failed to create border mesh X!");

	// Create border mesh y.
	AEGfxVertexList* borderMeshY;
	AEGfxMeshStart();
	AEGfxTriAdd(
		-TILE_SIZE * 0.5F, -TILE_SIZE * currentLevel.tilemap->height - TILE_SIZE * 2.0F, 0xFFFF0000, 0.0f, 1.0f,
		TILE_SIZE * 0.5F, -TILE_SIZE * currentLevel.tilemap->height - TILE_SIZE * 2.0F, 0xFFFF0000, 1.0f, 1.0f,
		-TILE_SIZE * 0.5F, TILE_SIZE * currentLevel.tilemap->height + TILE_SIZE * 2.0F, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		TILE_SIZE * 0.5F, -TILE_SIZE * currentLevel.tilemap->height - TILE_SIZE * 2.0F, 0xFFFF0000, 1.0f, 1.0f,
		TILE_SIZE * 0.5F, TILE_SIZE * currentLevel.tilemap->height + TILE_SIZE * 2.0F, 0xFFFF0000, 1.0f, 0.0f,
		-TILE_SIZE * 0.5F, TILE_SIZE * currentLevel.tilemap->height + TILE_SIZE * 2.0F, 0xFFFF0000, 0.0f, 0.0f);
	borderMeshY = AEGfxMeshEnd();
	AE_ASSERT_MESG(borderMeshY, "Failed to create border mesh Y!");

	// Create a border texture.
	unsigned char borderColor[4] = { 0, 0, 0, 255 };
	AEGfxTexture* borderTexture = AEGfxTextureLoadFromMemory(borderColor, 1, 1);

	// Create paint textures.
	unsigned char paintColors[48] = { 153, 50, 0, 255,		// Red 1.
									  240, 128, 106, 255,   // Red 2.
									  74, 209, 126, 255,	// Green 1.
									  190, 246, 211, 255,   // Green 2.
									  216, 152, 44, 255,	// Orange 1.
									  248, 223, 177, 255,	// Orange 2.
									  142, 137, 255, 255,   // Blue 1.
									  217, 217, 255, 255,   // Blue 2.
									  237, 234, 164, 255,	// Yellow 1.
									  255, 255, 255, 255,	// Yellow 2.
									  147, 27, 174, 255,	// Purple 1.
									  224, 111, 255, 255	// Purple 2.
	};
	AEGfxTexture* paintTexture = AEGfxTextureLoadFromMemory(paintColors, 12, 1);

	// Set the background color to light blue.
	AEGfxSetBackgroundColor(0.6F, 0.6F, 1.0F);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// Init paint.
	int paintOffset = 0;
	PaintPixel* allPaint = (PaintPixel*)calloc(PAINT_COUNT, sizeof(PaintPixel));
	InitPaint(allPaint);

	// Bool for controlling border resizing.
	unsigned char isDraggingBorderX = 0;
	unsigned char isDraggingBorderY = 0;

	// Define how the game is being played.
	int isInEditMode = 1;

	// See if the user is in World Edit mode or not.
	unsigned char worldEditMode = 1;
	AreaBox* areaToDrag = NULL;
	int xOffset = 0;
	int yOffset = 0;

	// Enum for current paint color.
	enum PaintColor currentColor = PAINT_RED;

	// Multiplier for making brush swing.
	float brushSizeMultiplier = 1;

	unsigned char isUnsaved = 0;

	AEVec2 mouseMov;
	
	float doubleClickTimer = 0.0F;

	GameObjectPtr player = MakeGameObject();

	AEGfxTexture* playerText = AEGfxTextureLoad("./Assets/player.png");

	SpriteSourcePtr pSpriteSrc = SpriteSourceCreate(1, 1, playerText);
	SpritePtr pSprite = SpriteCreate();
	SpriteSetMesh(pSprite, tileMesh);
	SpriteSetSpriteSource(pSprite, pSpriteSrc);
	AddSpriteGameObject(player, pSprite);
	PhysicsPtr pPhysics = MakePhysics();
	AddPhysicsGameObject(player, pPhysics);
	TransformPtr pTransform = MakeTransform();
	Vector2D scale = { 1.5F, 2.0865F };
	Vector2D pos = { 0, 0 };
	SetScaleTransform(pTransform, &scale);
	SetTranslationTransform(pTransform, &pos);
	AddTransformGameObject(player, pTransform);
	AABB pColl = FromPoints(0, scale.x, 0, scale.y);
	AddColliderGameObject(player, pColl);

	GameObjectPtr brush = MakeGameObject();

	SpriteSourcePtr bSpriteSrc = SpriteSourceCreate(1, 1, playerText);
	SpritePtr bSprite = SpriteCreate();
	SpriteSetMesh(bSprite, tileMesh);
	SpriteSetSpriteSource(bSprite, bSpriteSrc);
	AddSpriteGameObject(brush, bSprite);
	TransformPtr bTransform = MakeTransform();
	AddTransformGameObject(brush, bTransform);

	float* playCamX = &(GetTranslationTransform(pTransform)->x);
	float* playCamY = &(GetTranslationTransform(pTransform)->y);

	// Load all the levels in world edit mode.
	if (worldEditMode)
	{
		Level level = { 0 };
		Tilemap tilemap = { 0 };
		level.tilemap = &tilemap;

		int currX = 0;
		int currY = 0;
		int newY = 0;

		for (char c = 'A'; c < 'H'; ++c)
		{
			currX = 0;
			newY = 0;

			for (int i = 1; i < 21; ++i)
			{
				char buffer[MAX_PATH];
				sprintf_s(buffer, sizeof(buffer), "./Levels/%c%d.lvl", c, i);

				if (LoadLevelAtPath(game.allTiles, &level, buffer) == 0)
				{
					currentWorld.areas[currentWorld.areaCount] = FromLevel(&level);
					currentWorld.areas[currentWorld.areaCount].x = 1000 + currX;
					currentWorld.areas[currentWorld.areaCount].y = 1000 + currY;

					currX += currentWorld.areas[currentWorld.areaCount].width + 2;
					newY = currentWorld.areas[currentWorld.areaCount].height + 2 > newY ? currentWorld.areas[currentWorld.areaCount].height + 2 : newY;
					++currentWorld.areaCount;
				}
			}

			currY += newY;
		}
	}

	AEGfxSetCamPosition(currentWorld.areas[0].x * 8.068F, currentWorld.areas[0].y * 8.045F);

	// Load the main world by default.
	// If one doesn't exist, make one.
	if (LoadWorldAtPath(&currentWorld, "./Levels/main.wld") != 0)
	{
		SaveWorldAtPath(&currentWorld, "./Levels/main.wld");
	}

	// Run the game loop until the game stops running.
	while (isGameRunning)
	{
		// Inform the system about the loop's start and update user input.
		AESysFrameStart();
		AESysUpdate();
		AEInputUpdate();

		// Get camera position.
		float camX, camY;
		AEGfxGetCamPosition(&camX, &camY);

		// Get mouse info.
		long mouseX, mouseY;
		long mouseDeltaX, mouseDeltaY;
		AEInputGetCursorPosition(&mouseX, &mouseY);
		AEInputGetCursorPositionDelta(&mouseDeltaX, &mouseDeltaY);
		float mouseSpeed = sqrtf((float)(mouseDeltaX * mouseDeltaX + mouseDeltaY * mouseDeltaY));

		// Get mouse position.
		unsigned short pixelX, pixelY;
		ScreenToPixelPos(mouseX + (long)camX + (int)(TILE_SIZE * 0.5F), mouseY - (long)camY + (int)(TILE_SIZE * 0.5F), &pixelX, &pixelY);

		// Get tile position.
		unsigned char tileX, tileY;
		ScreenToTilePos(mouseX + (long)camX + (int)(TILE_SIZE * 0.5F), mouseY - (long)camY + (int)(TILE_SIZE * 0.5F), &tileX, &tileY);

		// Get tile position.
		unsigned int smallTileX, smallTileY;
		ScreenToSmallTilePos(mouseX + (long)camX + (int)(TILE_SIZE_WORLD * 0.5F) - 704, -mouseY + (long)camY + (int)(TILE_SIZE_WORLD * 0.5F) + 448, &smallTileX, &smallTileY);

		// Toggle between edit and play mode.
		if (AEInputCheckTriggered(VK_SPACE))
		{
			isInEditMode = !isInEditMode;

			if (isInEditMode)
			{
				ShutdownPlaymodeLevel(&game);
			}
			else
			{
				InitPlaymodeLevel(&game);
			}
		}

		char buffer[64];
		if (worldEditMode)
		{
			sprintf_s(buffer, sizeof(buffer), "%s%s - World Editor", isUnsaved ? "*" : "", currentWorld.fileName);
		}
		else
		{
			sprintf_s(buffer, sizeof(buffer), "%s%s (%dx%d) - Level Editor", isUnsaved ? "*" : "", currentLevel.levelName, currentLevel.tilemap->width + 1, currentLevel.tilemap->height + 1);
		}
		AESysSetWindowTitle(buffer);

		// Toggle between world and level edit mode.
		if (AEInputCheckTriggered(VK_ESCAPE) && !worldEditMode)
		{
			if (isUnsaved)
			{
				switch (MessageBox(NULL, L"Your work has unsaved changes. Save changes before closing?", L"Unsaved Changes", MB_YESNOCANCEL))
				{
					case IDYES:
						SaveLevelAtPath(&currentLevel, currentLevel.levelName);

					case IDNO:
						worldEditMode = 1;
						isUnsaved = 0;

						camX = currentWorld.areas[currentLevel.levelNum].x * 8.068F;
						camY = currentWorld.areas[currentLevel.levelNum].y * 8.045F;
						break;
				}
			}
			else
			{
				worldEditMode = 1;
			}
		}

		if (!isInEditMode)
		{
			UpdateGameObject(player, (float)AEFrameRateControllerGetFrameTime());
			UpdateGameObject(brush, (float)AEFrameRateControllerGetFrameTime());

			UpdatePlaymodeLevel(&game);
			PlayerUpdate(player, brush, 0.0F, 0.0F, GetTileColliders(&game), currentLevel.tilemap->width, currentLevel.tilemap->height);

			GameObjectDraw(player);
			GameObjectDraw(brush);

			AEGfxSetCamPosition(*playCamX, *playCamY);
		}

		if (!worldEditMode)
		{
			// Let the user drag to resize the border.
			if (AEInputCheckTriggered(VK_LBUTTON))
			{
				// Let user change X border.
				if ((int)tileX + 1 == 0 ||
					(int)tileX - 1 == currentLevel.tilemap->width)
				{
					isDraggingBorderX = 1;
				}

				// Let user change Y border.
				if ((int)tileY + 1 == 0 ||
					(int)tileY - 1 == currentLevel.tilemap->height)
				{
					isDraggingBorderY = 1;
				}
			}
			else if (AEInputCheckCurr(VK_LBUTTON))
			{
				if (isDraggingBorderX)
				{
					unsigned char newWidth = tileX - 1;
					ResizeBorderX(&borderMeshX, &currentLevel, newWidth, currentLevel.tilemap->height);
					currentWorld.areas[currentLevel.levelNum].width = newWidth;
				}
				if (isDraggingBorderY)
				{
					unsigned char newHeight = tileY - 1;
					ResizeBorderY(&borderMeshY, &currentLevel, currentLevel.tilemap->width, newHeight);
					currentWorld.areas[currentLevel.levelNum].height = newHeight;
				}

			}
			else if (AEInputCheckReleased(VK_LBUTTON))
			{
				// Let go of both borders.
				isDraggingBorderX = 0;
				isDraggingBorderY = 0;
			}
		}
		
		if (isInEditMode)
		{
			// Parse user inputs with given position variables.
			UIInputs(&selectedTile, &game, &currentLevel, &currentWorld, mouseX, mouseY, tileX, tileY, smallTileX, smallTileY, &camX, &camY, isDraggingBorderX || isDraggingBorderY, &worldEditMode, &isUnsaved);
		}
		else
		{
			if (AEInputCheckTriggered(VK_LEFT))
			{
				currentColor = (currentColor - 1) % 6;
			}
			if (AEInputCheckTriggered(VK_RIGHT))
			{
				currentColor = (currentColor + 1) % 6;
			}

			// See if the user is holding LMB.
			if (AEInputCheckCurr(VK_LBUTTON))
			{
				brushSizeMultiplier = 6 + fabsf(mouseSpeed) * 0.0375F;

				mouseMov.x = (float)mouseDeltaX;
				mouseMov.y = (float)mouseDeltaY;
				AEVec2Normalize(&mouseMov, &mouseMov);

				for (float x = 0, y = 0; fabsf(x) < abs(mouseDeltaX) && fabsf(y) < abs(mouseDeltaY); x += mouseMov.x, y += mouseMov.y)
				{
					ApplyPaintToLocation(allPaint, &game, currentColor, pixelX + (unsigned short)(x * 0.2F), pixelY - (unsigned short)(y * 0.2F), (unsigned char)brushSizeMultiplier, &paintOffset);
				}

				// If they are, apply paint to the world.
				ApplyPaintToLocation(allPaint, &game, currentColor, pixelX, pixelY, (unsigned char)brushSizeMultiplier, &paintOffset);
			}
		}
		
		if (!worldEditMode)
		{
			// Draw the level.
			DrawLevel(&currentLevel, tileMesh);

			if (!isInEditMode)
			{
				DrawBoundingBoxes(GetTileColliders(&game), currentLevel.tilemap->width, currentLevel.tilemap->height, tileMesh, game.allTiles[TILE_FILLED_BLOCK].texture);

				if (AEFrameRateControllerGetFrameCount() % 1 == 0)
				{
					UpdatePaint(allPaint, &game);
					
					// Temporary just to show that things work.
					for (int i = 0; i < MaxSpawnedEnemies; ++i)
					{
						//Existing(&enemyList[i], player);
						DrawEnemy(&enemyList[i], tileMesh, game.allTiles[TILE_GREEN_SLIME_SPAWN].texture);
					}
				}

				DrawAllPaint(allPaint, pixelMesh, paintTexture);
				DrawTileEntities(&game, tileMesh);
			}

			// Determine border positions.
			long borderXMin, borderXMax, borderYMin, borderYMax;
			TileToScreenPos(0, 0, &borderXMin, &borderYMin);
			TileToScreenPos(currentLevel.tilemap->width, currentLevel.tilemap->height, &borderXMax, &borderYMax);

			if (isInEditMode)
			{
				// Draw the borders.
				DrawBorder((float)borderXMin - TILE_SIZE, (float)borderYMin + TILE_SIZE, &borderTexture, &borderMeshX);
				DrawBorder((float)borderXMax + TILE_SIZE, (float)borderYMin + TILE_SIZE, &borderTexture, &borderMeshY);
				DrawBorder((float)borderXMin - TILE_SIZE, (float)borderYMax - TILE_SIZE, &borderTexture, &borderMeshY);
				DrawBorder((float)borderXMax + TILE_SIZE, (float)borderYMax - TILE_SIZE, &borderTexture, &borderMeshX);
			}

			// Check to see if tile is within level bounds.
			if (isInEditMode &&
				!worldEditMode &&
				tileX <= currentLevel.tilemap->width &&
				tileY <= currentLevel.tilemap->height)
			{
				// Draw preview for currently selected tile.
				long screenPosX, screenPosY;

				TileToScreenPos((unsigned char)tileX, (unsigned char)tileY, &screenPosX, &screenPosY);
				DrawTile(selectedTile, (float)screenPosX, (float)screenPosY, tileMesh, 0.5F);
			}
		}
		else
		{
			DrawAreas(&currentWorld, worldTileMesh, game.allTiles);

			if (AEInputCheckTriggered(VK_LBUTTON))
			{
				doubleClickTimer += 0.1F;

				for (int i = 0; i < currentWorld.areaCount; ++i)
				{
					if (smallTileX > currentWorld.areas[i].x &&
						smallTileX < currentWorld.areas[i].x + (unsigned int)currentWorld.areas[i].width &&
						smallTileY > currentWorld.areas[i].y &&
						smallTileY < currentWorld.areas[i].y + (unsigned int)currentWorld.areas[i].height)
					{
						areaToDrag = &currentWorld.areas[i];
						xOffset = (int)smallTileX - currentWorld.areas[i].x;
						yOffset = (int)smallTileY - currentWorld.areas[i].y;

						if (doubleClickTimer > 0.11F)
						{
							LoadLevelAtPath(game.allTiles, &currentLevel, currentWorld.areas[i].areaName);
							worldEditMode = 0;

							camX = -96;
							camY = 96;
							AEGfxSetCamPosition(camX, camY);
						}
					}
				}
			}
			if (areaToDrag != NULL)
			{
				areaToDrag->x = (unsigned int)AEClamp(((float)smallTileX - xOffset), 0, (float)MAXUINT32);
				areaToDrag->y = (unsigned int)AEClamp(((float)smallTileY - yOffset), 0, (float)MAXUINT32);

				isUnsaved = 1;
			}
		}
		if (AEInputCheckReleased(VK_LBUTTON))
		{
			areaToDrag = NULL;
		}

		doubleClickTimer = AEClamp(doubleClickTimer - (float)AEFrameRateControllerGetFrameTime() * 0.25F, 0.0F, 0.12F);

		// Inform the system about the loop's end.
		AESysFrameEnd();

		// Check if forcing the application to quit.
		if (!AESysDoesWindowExist())
		{
			// End the loop.
			isGameRunning = 0;
		}
	}

	FreeGameObject(&player);
	FreeGameObject(&brush);

	// Free all memory associated to paint system.
	ShutdownPaint(allPaint);

	// Free the system.
	AESysExit();

	// Return success to the system.
	return EXIT_SUCCESS;
}

// Parse user inputs for the UI.
void UIInputs(Tile** selectedTile, GameplayInstance* game, Level* currentLevel, World* currentWorld, long mouseX, long mouseY, unsigned char tileX, unsigned char tileY, unsigned int smallX, unsigned int smallY, float* camX, float* camY, unsigned char isDraggingBorder, unsigned char *isInWorldEditMode, unsigned char* isUnsaved)
{
	UNREFERENCED_PARAMETER(mouseX);
	UNREFERENCED_PARAMETER(mouseY);

	// Let the user scroll through the list of items.
	if (AEInputCheckTriggered(VK_LEFT))
	{
		if ((*selectedTile)->id - 1 > 0) {
			*selectedTile = &game->allTiles[(*selectedTile)->id - 1];
		}
	}
	if (AEInputCheckTriggered(VK_RIGHT))
	{
		if ((*selectedTile)->id + 1 <= TILE_COUNT) {
			*selectedTile = &game->allTiles[(*selectedTile)->id + 1];
		}
	}

	// Draw.
	if (!isDraggingBorder)
	{
		if (AEInputCheckCurr(VK_LBUTTON) &&
			!*isInWorldEditMode &&
			tileX <= currentLevel->tilemap->width &&
			tileY <= currentLevel->tilemap->height)
		{
			// Check to make sure tile isn't already placed.
			if (GetTile(currentLevel, tileX, tileY)->id != (*selectedTile)->id)
			{
				// Place tile.
				PlaceTile(*selectedTile, currentLevel, tileX, tileY);
				currentWorld->areas[currentLevel->levelNum].tileID[tileX][tileY] = (*selectedTile)->id;
				*isUnsaved = 1;
			}
		}
		// Move the camera.
		else if (AEInputCheckCurr(VK_MBUTTON))
		{
			// Get mouse movement.
			long deltaX, deltaY;
			AEInputGetCursorPositionDelta(&deltaX, &deltaY);
			
			// Move camera by mouse movement.
			if (!*isInWorldEditMode)
			{
				AEGfxSetCamPosition(AEClamp(*camX - deltaX, -TILE_SIZE * 1.5F, currentLevel->tilemap->width * TILE_SIZE - 1408.0F + TILE_SIZE * 1.5F),
									AEClamp(*camY + deltaY, -currentLevel->tilemap->height * TILE_SIZE + 896.0F - TILE_SIZE * 1.5F, TILE_SIZE * 1.5F));
			}
			else
			{
				AEGfxSetCamPosition(AEClamp(*camX - deltaX, 0, (float)MAXUINT32), AEClamp(*camY + deltaY, 0, (float)MAXUINT32));
			}
		}
		// Erase.
		else if (AEInputCheckCurr(VK_RBUTTON) &&
			tileX <= currentLevel->tilemap->width &&
			tileY <= currentLevel->tilemap->height)
		{
			// Check to make sure tile isn't already placed.
			if (GetTile(currentLevel, tileX, tileY)->id != 0)
			{
				// Place tile.
				EraseTile(currentLevel, tileX, tileY, &game->allTiles[TILE_AIR]);
				currentWorld->areas[currentLevel->levelNum].tileID[tileX][tileY] = TILE_AIR;
				*isUnsaved = 1;
			}
		}
	}

	// Check for saving.
	if (AEInputCheckCurr(VK_CONTROL))
	{
		if (!*isInWorldEditMode)
		{
			if (AEInputCheckTriggered('S'))
			{
				if (AEInputCheckCurr(VK_SHIFT))
				{
					SaveLevel(currentLevel);
					SaveWorldAtPath(currentWorld, currentWorld->fileName);
				}
				else
				{
					SaveLevelAtPath(currentLevel, currentLevel->levelName);
					SaveWorldAtPath(currentWorld, currentWorld->fileName);
				}
				*isUnsaved = 0;
			}
		}
		else
		{
			if (AEInputCheckTriggered('S'))
			{
				if (AEInputCheckCurr(VK_SHIFT))
				{
					SaveWorld(currentWorld);
				}
				else
				{
					SaveWorldAtPath(currentWorld, currentWorld->fileName);
				}
				*isUnsaved = 0;
			}
			else if (AEInputCheckTriggered('O'))
			{
				LoadWorld(currentWorld);
				*isUnsaved = 0;
			}
		}

		if (AEInputCheckTriggered('N'))
		{
			InitLevel(currentLevel->tilemap, currentLevel, game->allTiles, &currentWorld->areaCount, currentLevel->levelName);
			NewLevel(currentLevel);
			
			currentWorld->areas[currentWorld->areaCount].width = 19;
			currentWorld->areas[currentWorld->areaCount].height = 11;
			currentWorld->areas[currentWorld->areaCount].x = smallX;
			currentWorld->areas[currentWorld->areaCount].y = smallY;

			++currentWorld->areaCount;

			for (int y = 0; y < 99; ++y)
			{
				for (int x = 0; x < 59; ++x)
				{
					currentWorld->areas[currentLevel->levelNum].tileID[x][y] = currentLevel->tilemap->tiles[x][y]->id;
				}
			}

			SaveWorldAtPath(currentWorld, currentWorld->fileName);
			*isUnsaved = 0;
			*isInWorldEditMode = 0;

			*camX = -96;
			*camY = 96;
			AEGfxSetCamPosition(*camX, *camY);
		}
	}
}

// Render the border.
void DrawBorder(float x, float y, AEGfxTexture **borderTexture, AEGfxVertexList **borderMesh)
{
	// Set render mode.
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	// Set the position.
	AEGfxSetPosition(x, y);

	// Render the texture.
	AEGfxTextureSet(*borderTexture, 0.0F, 0.0F);

	// Set transparency.
	AEGfxSetTransparency(1.0F);

	// Draw the mesh.
	AEGfxMeshDraw(*borderMesh, AE_GFX_MDM_TRIANGLES);
}

// Let the user resize the x border.
void ResizeBorderX(AEGfxVertexList **borderMeshX, Level *currentLevel, unsigned char newWidth, unsigned char newHeight)
{
	AEGfxMeshStart();
	AEGfxTriAdd(
		-TILE_SIZE * currentLevel->tilemap->width - TILE_SIZE * 2.0F, -TILE_SIZE * 0.5F, 0xFFFF0000, 0.0f, 1.0f,
		TILE_SIZE * currentLevel->tilemap->width + TILE_SIZE * 2.0F, -TILE_SIZE * 0.5F, 0xFFFF0000, 1.0f, 1.0f,
		-TILE_SIZE * currentLevel->tilemap->width - TILE_SIZE * 2.0F, TILE_SIZE * 0.5F, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		TILE_SIZE * currentLevel->tilemap->width + TILE_SIZE * 2.0F, -TILE_SIZE * 0.5F, 0xFFFF0000, 1.0f, 1.0f,
		TILE_SIZE * currentLevel->tilemap->width + TILE_SIZE * 2.0F, TILE_SIZE * 0.5F, 0xFFFF0000, 1.0f, 0.0f,
		-TILE_SIZE * currentLevel->tilemap->width - TILE_SIZE * 2.0F, TILE_SIZE * 0.5F, 0xFFFF0000, 0.0f, 0.0f);
	*borderMeshX = AEGfxMeshEnd();
	AE_ASSERT_MESG(*borderMeshX, "Failed to create border mesh!");
	
	currentLevel->tilemap->width = newWidth;
	currentLevel->tilemap->height = newHeight;
}

// Let the user resize the y border.
void ResizeBorderY(AEGfxVertexList** borderMeshY, Level* currentLevel, unsigned char newWidth, unsigned char newHeight)
{
	AEGfxMeshStart();
	AEGfxTriAdd(
		-TILE_SIZE * 0.5F, -TILE_SIZE * currentLevel->tilemap->height - TILE_SIZE * 2.0F, 0xFFFF0000, 0.0f, 1.0f,
		TILE_SIZE * 0.5F, -TILE_SIZE * currentLevel->tilemap->height - TILE_SIZE * 2.0F, 0xFFFF0000, 1.0f, 1.0f,
		-TILE_SIZE * 0.5F, TILE_SIZE * currentLevel->tilemap->height + TILE_SIZE * 2.0F, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		TILE_SIZE * 0.5F, -TILE_SIZE * currentLevel->tilemap->height - TILE_SIZE * 2.0F, 0xFFFF0000, 1.0f, 1.0f,
		TILE_SIZE * 0.5F, TILE_SIZE * currentLevel->tilemap->height + TILE_SIZE * 2.0F, 0xFFFF0000, 1.0f, 0.0f,
		-TILE_SIZE * 0.5F, TILE_SIZE * currentLevel->tilemap->height + TILE_SIZE * 2.0F, 0xFFFF0000, 0.0f, 0.0f);
	*borderMeshY = AEGfxMeshEnd();
	AE_ASSERT_MESG(*borderMeshY, "Failed to create border mesh!");

	currentLevel->tilemap->width = newWidth;
	currentLevel->tilemap->height = newHeight;
}

void DrawBoundingBoxes(AABB* aabbs, int countX, int countY, AEGfxVertexList* mesh, AEGfxTexture* texture)
{
	for (int y = 0; y < TILEMAP_HEIGHT; ++y)
	{
		for (int x = 0; x < TILEMAP_WIDTH; ++x)
		{
			int index = y * TILEMAP_WIDTH + x;

			// Exit function if no texture exists.
			if (aabbs[index] == NULL)
				continue;

			// Set render mode.
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

			// Set the position.
			AEGfxSetFullTransform(
				aabbs[index]->left,
				aabbs[index]->bottom,
				0,
				(aabbs[index]->right - aabbs[index]->left) / TILE_SIZE,
				(aabbs[index]->top - aabbs[index]->bottom) / TILE_SIZE);

			// Render the texture.
			AEGfxTextureSet(texture, 0.0F, 0.0F);

			// Set transparency.
			AEGfxSetTransparency(0.5F);

			// Draw the mesh.
			AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
		}
	}
}

// ---------------------------------------------------------------------------
