//------------------------------------------------------------------------------
//
// File Name:	GameStateStub.c
// Author(s):	Derek Herrera-Sturm
// Project:		Project 0
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "GameStateEditor.h"
#include "GameStateSwitcher.h"
#include "GameStateManager.h"
#include "World.h"
#include "Paint.h"
#include "Level.h"
#include "Tile.h"
#include "GameplayInstance.h"
#include "Serialization.h"
#include "MathHelper.h"
#include "Debug.h"
#include "Mesh.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static EDITMODE editMode = EDITMODE_INVALID;
static int isSaved = 1;

static float camX = 0, camY = 0;
static float worldCamX = 0, worldCamY = 0;
static long mouseX = 0, mouseY = 0;

static Tile* selectedTile = NULL;
static int isChangingBorderX = 0, isChangingBorderY = 0;
static AEGfxVertexList *xBorderMesh, *yBorderMesh;
static AEGfxTexture* borderTexture;

static AEGfxVertexList* sideBarMesh;
static float scrollAmount = 0;

static Room* areaDragging = NULL;
static int xOffset = 0, yOffset = 0;
static float doubleClickTimer = 0.0F;

static Room areaList[MAX_ROOM_COUNT] = { 0 };

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

void WorldEditMode(void);
void LevelEditMode(void);
void EditWorld(unsigned int, unsigned int);
void EditLevel(unsigned char, unsigned char);
void WorldSaving(void);
void LevelSaving(void);
LRESULT GetMouseWheelTile(HWND, UINT, WPARAM, LPARAM);
Tile* SwapTile(int);
void ResizeBorderX(unsigned char, unsigned char);
void ResizeBorderY(unsigned char, unsigned char);
void DrawBorder(float, float, AEGfxTexture*, AEGfxVertexList*);
void SwitchEditMode(EDITMODE);
void NewLevel(const char*, unsigned int, unsigned int, int, int);
LRESULT CheckForSaving(HWND, UINT, WPARAM, LPARAM);
void InitAreaList(void);
void DisplayAreaList(float, float);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Stub game state.
void GameStateEditorLoad()
{
	// Create a border mesh x.
	AEGfxMeshStart();
	AEGfxTriAdd(
		-TILE_SIZE * GetCurrentTilemap()->width - TILE_SIZE * 2.0F, -TILE_SIZE * 0.5F, 0xFFFF0000, 0.0f, 1.0f,
		TILE_SIZE * GetCurrentTilemap()->width + TILE_SIZE * 2.0F, -TILE_SIZE * 0.5F, 0xFFFF0000, 1.0f, 1.0f,
		-TILE_SIZE * GetCurrentTilemap()->width - TILE_SIZE * 2.0F, TILE_SIZE * 0.5F, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		TILE_SIZE * GetCurrentTilemap()->width + TILE_SIZE * 2.0F, -TILE_SIZE * 0.5F, 0xFFFF0000, 1.0f, 1.0f,
		TILE_SIZE * GetCurrentTilemap()->width + TILE_SIZE * 2.0F, TILE_SIZE * 0.5F, 0xFFFF0000, 1.0f, 0.0f,
		-TILE_SIZE * GetCurrentTilemap()->width - TILE_SIZE * 2.0F, TILE_SIZE * 0.5F, 0xFFFF0000, 0.0f, 0.0f);
	xBorderMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(xBorderMesh, "Failed to create X Border Mesh!");

	// Create border mesh y.
	AEGfxMeshStart();
	AEGfxTriAdd(
		-TILE_SIZE * 0.5F, -TILE_SIZE * GetCurrentTilemap()->height - TILE_SIZE * 2.0F, 0xFFFF0000, 0.0f, 1.0f,
		TILE_SIZE * 0.5F, -TILE_SIZE * GetCurrentTilemap()->height - TILE_SIZE * 2.0F, 0xFFFF0000, 1.0f, 1.0f,
		-TILE_SIZE * 0.5F, TILE_SIZE * GetCurrentTilemap()->height + TILE_SIZE * 2.0F, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		TILE_SIZE * 0.5F, -TILE_SIZE * GetCurrentTilemap()->height - TILE_SIZE * 2.0F, 0xFFFF0000, 1.0f, 1.0f,
		TILE_SIZE * 0.5F, TILE_SIZE * GetCurrentTilemap()->height + TILE_SIZE * 2.0F, 0xFFFF0000, 1.0f, 0.0f,
		-TILE_SIZE * 0.5F, TILE_SIZE * GetCurrentTilemap()->height + TILE_SIZE * 2.0F, 0xFFFF0000, 0.0f, 0.0f);
	yBorderMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(yBorderMesh, "Failed to create Y Border Mesh!");

	// Create sidebar mesh.
	sideBarMesh = MeshCreateQuad(0.5F, 0.5F, 1.0F, 1.0F, "Sidebar Mesh");

	// Create a border texture.
	unsigned char borderColor[4] = { 0, 0, 0, 255 };
	borderTexture = AEGfxTextureLoadFromMemory(borderColor, 1, 1);

	// Initialize all systems.
	InitTileSystem();
	InitWorldSystem();
}

// Initialize the memory associated with the Stub game state.
void GameStateEditorInit()
{
	// Change the window title.
	//AESysSetWindowTitle("Project Color - Editor");

	// Setup tile callback event for changing tiles.
	AESysAddCustomCallBack(WM_MOUSEWHEEL, GetMouseWheelTile);

	// Setup saving callback event for saving without quitting.
	///AESysAddCustomCallBack(WM_CLOSE, CheckForSaving);

	// Load all areas into the sidebar.
	InitAreaList();

	// Set camera to be focused on first level.
	if (GetWorld()->rooms[0].filePath[0] == '.')
	{
		worldCamX = GetWorld()->rooms[0].x * (float)TILE_SIZE_SMALL + (GetWorld()->rooms[0].width * (float)TILE_SIZE_SMALL / 2);
		worldCamY = GetWorld()->rooms[0].y * (float)TILE_SIZE_SMALL + (GetWorld()->rooms[0].height * (float)TILE_SIZE_SMALL / 2);
	}

	// Set ground as default selected tile.
	selectedTile = GetTileAt(TILE_GROUND);

	// Switch the mode to world edit mode if it's uninitialized.
	if (editMode != EDITMODE_LEVEL)
	{
		SwitchEditMode(EDITMODE_WORLD);
	}
}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateEditorUpdate(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	// Set the camera and mouse position variables.
	AEInputGetCursorPosition(&mouseX, &mouseY);

	// Create buffer for window title.
	char buffer[MAX_PATH];

	// Set window title buffer and apply update function for proper mode.
	switch (editMode)
	{
		case EDITMODE_WORLD:
			sprintf_s(buffer, sizeof(buffer), "Project Color - World Editor: %s%s", isSaved ? "" : "*", GetWorld()->filePath);
			WorldEditMode();
			break;

		case EDITMODE_LEVEL:
			sprintf_s(buffer, sizeof(buffer), "Project Color - Level Editor: %s%s", isSaved ? "" : "*", GetCurrentLevel()->filePath);
			LevelEditMode();
			break;
	}

	// Set window title.
	//AESysSetWindowTitle(buffer);

	// Set the camera's position.
	AEGfxSetCamPosition(camX, camY);

	// Let the user toggle between gamestates.
	ToggleGameState();
}

// Free any memory associated with the Stub game state.
void GameStateEditorShutdown()
{
	AESysRemoveCustomCallBack(WM_MOUSEWHEEL, GetMouseWheelTile);
	//AESysRemoveCustomCallBack(WM_CLOSE, CheckForSaving);
}

// Unload the resources associated with the Stub game state.
void GameStateEditorUnload()
{
	// Shutdown all systems.
	ShutdownTileSystem();
	ShutdownWorldSystem();

	// Free mesh information.
	AEGfxMeshFree(xBorderMesh);
	AEGfxMeshFree(yBorderMesh);
	AEGfxMeshFree(sideBarMesh);

	// Free texture information.
	AEGfxTextureUnload(borderTexture);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Handle world edit mode code.
void WorldEditMode(void)
{
	// Small tiles for world map.
	unsigned int tileX = 0, tileY = 0;
	ScreenToSmallTilePos(mouseX + (long)camX + (int)(TILE_SIZE_SMALL * 0.5F) - 768,
						-mouseY + (long)camY + (int)(TILE_SIZE_SMALL * 0.5F) + 448,
						 &tileX,
						 &tileY);

	// Handle world editing.
	EditWorld(tileX, tileY);

	// Draw level.
	DrawWorld(GetWorld(), 0, 0);

	// Handle sidebar for selecting areas.
	DisplayAreaList((float)mouseX, (float)mouseY);
}

// Handle level edit mode code.
void LevelEditMode(void)
{
	char tileX = (char)((mouseX - 32 + camX) / TILE_SIZE) + 1;
	char tileY = (char)((mouseY - 32 - camY) / TILE_SIZE) + 1 + (GetCurrentTilemap()->height - 13);

	// Handle level editing.
	EditLevel((unsigned char)tileX, (unsigned char)tileY);

	// Let the user quit level edit mode.
	if (AEInputCheckTriggered(VK_ESCAPE))
	{
		// Check to see if the level is saved.
		if (!isSaved)
		{
			// Prompt the user to save, and switch input accordingly.
			switch (MessageBox(NULL, L"Your level has unsaved changes. Would you like to save before quitting?", L"Unsaved Changes", MB_YESNOCANCEL))
			{
				case IDYES:
					// Apply correct textures for area.
					FromLevel(&GetWorld()->rooms[GetCurrentLevel()->levelNum], GetCurrentTilemap());

					// Save the level.
					if (SaveLevelAtPath(GetCurrentLevel(), GetCurrentTilemap(), GetCurrentLevel()->filePath) != PICKERRESULT_SUCCESS)
						break;

				case IDNO:
					// Go back to world edit mode.
					SwitchEditMode(EDITMODE_WORLD);
					break;
			}
		}
		else
		{
			// Apply correct textures for area.
			FromLevel(&GetWorld()->rooms[GetCurrentLevel()->levelNum], GetCurrentTilemap());

			// Go back to world edit mode.
			SwitchEditMode(EDITMODE_WORLD);
		}
	}

	// Let the user enter play mode.
	if (AEInputCheckTriggered(VK_SPACE))
	{
		// Save the level.
		SaveLevelAtPath(GetCurrentLevel(), GetCurrentTilemap(), GetCurrentLevel()->filePath);

		// Load the level in Derek's test scene.
		GameStateManagerSetNextState(GsTestDerek);
	}

	// Draw level.
	DrawLevel(GetCurrentTilemap());

	// Draw preview for currently selected tile.
	if (tileX >= 0 && tileX <= GetCurrentTilemap()->width &&
		tileY >= 0 && tileY <= GetCurrentTilemap()->height)
	{
		DrawTile(selectedTile, tileX * (float)TILE_SIZE - 768, 448 - tileY * (float)TILE_SIZE + (GetCurrentTilemap()->height - 13) * TILE_SIZE, 0.5F);
	}

	// Determine border positions.
	long minX = 0, minY = 0, midX = 0, midY = 0, maxX = 0, maxY = 0;
	TileToScreenPos(0, 0, &minX, &minY);
	TileToScreenPos(GetCurrentTilemap()->width / 2, GetCurrentTilemap()->height / 2, &midX, &midY);
	TileToScreenPos(GetCurrentTilemap()->width, GetCurrentTilemap()->height, &maxX, &maxY);

	// Draw borders.
	DrawBorder((float)midX, (float)minY + TILE_SIZE, borderTexture, xBorderMesh); // Top.
	DrawBorder((float)minX - TILE_SIZE, (float)midY, borderTexture, yBorderMesh); // Left.
	DrawBorder((float)midX, (float)maxY - TILE_SIZE, borderTexture, xBorderMesh); // Bottom.
	DrawBorder((float)maxX + TILE_SIZE, (float)midY, borderTexture, yBorderMesh); // Right.
}

// Handle world editing using the mouse and keyboard.
void EditWorld(unsigned int tileX, unsigned int tileY)
{
	// Handle mouse inputs for dragging levels and opening levels.
	if (AEInputCheckTriggered(VK_LBUTTON))
	{
		// Set double click timer.
		doubleClickTimer += 0.1F;

		// Variable for if there's an area being clicked on.
		int openLevel = 0;

		// Iterate through the areas in the world to find the one that was clicked on.
		for (int i = 0; i < GetWorld()->roomCount; ++i)
		{
			// Check if this was the area that was clicked on.
			if (tileX >= GetWorld()->rooms[i].x &&
				tileX <= GetWorld()->rooms[i].x + GetWorld()->rooms[i].width &&
				tileY >= GetWorld()->rooms[i].y &&
				tileY <= GetWorld()->rooms[i].y + GetWorld()->rooms[i].height)
			{
				// Set the area that's being dragged and adjust offset for mouse position.
				areaDragging = &GetWorld()->rooms[i];
				xOffset = (int)tileX - areaDragging->x;
				yOffset = (int)tileY - areaDragging->y;

				// Set variable for if a new level needs to be made.
				openLevel = 1;
				break;
			}
		}

		// Check for double clicks.
		if (doubleClickTimer > 0.11F)
		{
			// Check if you need to open or make a new level.
			if (openLevel)
			{
				// Load the level.
				switch (LoadLevelAtPath(GetCurrentLevel(), GetCurrentTilemap(), areaDragging->filePath))
				{
					case PICKERRESULT_FAIL:
						MessageBox(NULL, L"Level could not be opened.", L"Open Level Failed", MB_OK);
						break;

					case PICKERRESULT_SUCCESS:
						SwitchEditMode(EDITMODE_LEVEL);
						break;

					case PICKERRESULT_CANCEL:
						break;
				}
			}
			else
			{
				char levelName[MAX_PATH] = { 0 };

				// Create a new level.
				switch (NewLevelWithPicker(GetCurrentLevel(), GetCurrentTilemap(), levelName))
				{
					case PICKERRESULT_FAIL:
						MessageBox(NULL, L"Level could not be created.", L"New Level Failed", MB_OK);
						break;

					case PICKERRESULT_SUCCESS:
						NewLevel(levelName, tileX, tileY, MIN_LEVEL_WIDTH, MIN_LEVEL_HEIGHT);
						SwitchEditMode(EDITMODE_LEVEL);
						break;

					case PICKERRESULT_CANCEL:
						break;
				}
			}
		}	
	}
	else if (AEInputCheckReleased(VK_LBUTTON))
	{
		// Deselect area.
		areaDragging = NULL;
	}
	else if (AEInputCheckTriggered(VK_RBUTTON))
	{
		// Iterate through the areas in the world to find the one that was clicked on.
		for (int i = 0; i < GetWorld()->roomCount; ++i)
		{
			// Check if this was the area that was clicked on.
			if (tileX >= GetWorld()->rooms[i].x &&
				tileX <= GetWorld()->rooms[i].x + GetWorld()->rooms[i].width &&
				tileY >= GetWorld()->rooms[i].y &&
				tileY <= GetWorld()->rooms[i].y + GetWorld()->rooms[i].height)
			{
				// Set values to default.
				strcpy_s(GetWorld()->rooms[i].filePath, sizeof(GetWorld()->rooms[i].filePath), "");
				GetWorld()->rooms[i].x = 0;
				GetWorld()->rooms[i].y = 0;
				GetWorld()->rooms[i].width = 0;
				GetWorld()->rooms[i].height = 0;

				// Iterate through each of the tiles.
				for (int y = 0; y < MAX_LEVEL_HEIGHT; ++y)
				{
					for (int x = 0; x < MAX_LEVEL_WIDTH; ++x)
					{
						GetWorld()->rooms[i].tileID[x][y] = 0;
					}
				}

				// Decrement area count.
				--GetWorld()->roomCount;
				break;
			}
		}
	}

	// Check if user is dragging an area around.
	if (areaDragging != NULL)
	{
		// Move the area towards the mouse.
		areaDragging->x = (unsigned int)AEClamp(((float)tileX - xOffset), 0, (float)MAXUINT32);
		areaDragging->y = (unsigned int)AEClamp(((float)tileY - yOffset), 0, (float)MAXUINT32);
		isSaved = 0;
	}

	// Check for camera moving with mouse.
	if (AEInputCheckCurr(VK_MBUTTON))
	{
		// Get mouse movement.
		long deltaX = 0, deltaY = 0;
		AEInputGetCursorPositionDelta(&deltaX, &deltaY);

		// Set the new camera position.
		camX = AEClamp(camX - deltaX, 0, (float)MAXUINT32);
		camY = AEClamp(camY + deltaY, 0, (float)MAXUINT32);
	}

	// Cap double click timer and reduce by deltaTime.
	doubleClickTimer = AEClamp(doubleClickTimer - (float)AEFrameRateControllerGetFrameTime() * 0.25F, 0.0F, 0.12F);

	// Handle keyboard inputs for moving the camera.
	if (AEInputCheckCurr(VK_UP))
	{
		// Set the new camera Y position.
		camY = AEClamp(camY + 1000 * (float)AEFrameRateControllerGetFrameTime(), 0, (float)MAXUINT32);
	}
	if (AEInputCheckCurr(VK_DOWN))
	{
		// Set the new camera Y position.
		camY = AEClamp(camY - 1000 * (float)AEFrameRateControllerGetFrameTime(), 0, (float)MAXUINT32);
	}
	if (AEInputCheckCurr(VK_LEFT))
	{
		// Set the new camera X position.
		camX = AEClamp(camX - 1000 * (float)AEFrameRateControllerGetFrameTime(), 0, (float)MAXUINT32);
	}
	if (AEInputCheckCurr(VK_RIGHT))
	{
		// Set the new camera X position.
		camX = AEClamp(camX + 1000 * (float)AEFrameRateControllerGetFrameTime(), 0, (float)MAXUINT32);
	}

	// Handle saving.
	WorldSaving();
}

// Handle level editing using the mouse and keyboard.
void EditLevel(unsigned char tileX, unsigned char tileY)
{
	// Handle mouse inputs for placing tiles, moving the camera, and removing tiles.
	if (AEInputCheckCurr(VK_LBUTTON) && !isChangingBorderX && !isChangingBorderY)
	{
		// Check to make sure tile isn't already placed.
		if (GetTile(GetCurrentTilemap(), tileX, tileY) != NULL &&
			GetTile(GetCurrentTilemap(), tileX, tileY)->id != selectedTile->id)
		{
			// Place tile.
			PlaceTile(GetCurrentTilemap(), selectedTile, tileX, tileY);
			isSaved = 0;
		}
	}
	else if (AEInputCheckCurr(VK_MBUTTON))
	{
		// Get mouse movement.
		long deltaX = 0, deltaY = 0;
		AEInputGetCursorPositionDelta(&deltaX, &deltaY);

		// Set the new camera position.
		camX = AEClamp(camX - deltaX, -TILE_SIZE * 1.5F, GetCurrentTilemap()->width * TILE_SIZE - 1472.0F + TILE_SIZE * 0.5F);
		camY = AEClamp(camY + deltaY, -TILE_SIZE * 0.5F, GetCurrentTilemap()->height * TILE_SIZE - 768.0F + TILE_SIZE * 0.5F);
	}
	else if (AEInputCheckCurr(VK_RBUTTON))
	{
		// Check to make sure tile isn't already placed.
		if (GetTile(GetCurrentTilemap(), tileX, tileY) != NULL &&
			GetTile(GetCurrentTilemap(), tileX, tileY)->id != 0)
		{
			// Erase tile.
			EraseTile(GetCurrentTilemap(), tileX, tileY);
			isSaved = 0;
		}
	}

	// Handle mouse inputs for dragging border positions.
	if (AEInputCheckTriggered(VK_LBUTTON))
	{
		// Let user change X border.
		if ((int)tileX + 1 == 0 ||
			(int)tileX - 1 == GetCurrentTilemap()->width)
		{
			isChangingBorderX = 1;
		}

		// Let user change Y border.
		if ((int)tileY + 1 == 0 ||
			(int)tileY - 1 == GetCurrentTilemap()->height)
		{
			isChangingBorderY = 1;
		}
	}
	else if (AEInputCheckCurr(VK_LBUTTON))
	{
		// Check if the X border is moving.
		if (isChangingBorderX)
		{
			unsigned char newWidth = tileX - 1;
			ResizeBorderX(newWidth, GetCurrentTilemap()->height);
			GetWorld()->rooms[GetCurrentLevel()->levelNum].width = newWidth;
		}
		
		// Check if the Y border is moving.
		if (isChangingBorderY)
		{
			unsigned char newHeight = tileY - 1;
			ResizeBorderY(GetCurrentTilemap()->width, newHeight);
			GetWorld()->rooms[GetCurrentLevel()->levelNum].height = newHeight;
		}
	}
	else if (AEInputCheckReleased(VK_LBUTTON))
	{
		// Let go of both borders.
		isChangingBorderX = 0;
		isChangingBorderY = 0;
	}

	// Handle keyboard inputs for moving the camera.
	if (AEInputCheckReleased(VK_UP))
	{
		if (AEInputCheckCurr(VK_RIGHT))
		{
			if (GetCurrentRoom()->topRight == NULL)
				return;

			SetCurrentRoom(GetCurrentRoom()->topRight);
			LoadLevelAtPath(GetCurrentLevel(), GetCurrentTilemap(), GetCurrentRoom()->filePath);
		}
		else if (AEInputCheckCurr(VK_LEFT))
		{
			if (GetCurrentRoom()->topLeft == NULL)
				return;

			SetCurrentRoom(GetCurrentRoom()->topLeft);
			LoadLevelAtPath(GetCurrentLevel(), GetCurrentTilemap(), GetCurrentRoom()->filePath);
		}
		else
		{
			if (GetCurrentRoom()->top == NULL)
				return;

			SetCurrentRoom(GetCurrentRoom()->top);
			LoadLevelAtPath(GetCurrentLevel(), GetCurrentTilemap(), GetCurrentRoom()->filePath);
		}
	}
	if (AEInputCheckReleased(VK_DOWN))
	{
		if (AEInputCheckCurr(VK_RIGHT))
		{
			if (GetCurrentRoom()->bottomRight == NULL)
				return;

			SetCurrentRoom(GetCurrentRoom()->bottomRight);
			LoadLevelAtPath(GetCurrentLevel(), GetCurrentTilemap(), GetCurrentRoom()->filePath);
		}
		else if (AEInputCheckCurr(VK_LEFT))
		{
			if (GetCurrentRoom()->bottomLeft == NULL)
				return;

			SetCurrentRoom(GetCurrentRoom()->bottomLeft);
			LoadLevelAtPath(GetCurrentLevel(), GetCurrentTilemap(), GetCurrentRoom()->filePath);
		}
		else
		{
			if (GetCurrentRoom()->bottom == NULL)
				return;

			SetCurrentRoom(GetCurrentRoom()->bottom);
			LoadLevelAtPath(GetCurrentLevel(), GetCurrentTilemap(), GetCurrentRoom()->filePath);
		}
	}
	if (AEInputCheckReleased(VK_LEFT))
	{
		if (AEInputCheckCurr(VK_UP))
		{
			if (GetCurrentRoom()->topLeft == NULL)
				return;

			SetCurrentRoom(GetCurrentRoom()->topLeft);
			LoadLevelAtPath(GetCurrentLevel(), GetCurrentTilemap(), GetCurrentRoom()->filePath);
		}
		else if (AEInputCheckCurr(VK_DOWN))
		{
			if (GetCurrentRoom()->bottomLeft == NULL)
				return;

			SetCurrentRoom(GetCurrentRoom()->bottomLeft);
			LoadLevelAtPath(GetCurrentLevel(), GetCurrentTilemap(), GetCurrentRoom()->filePath);
		}
		else
		{
			if (GetCurrentRoom()->left == NULL)
				return;

			SetCurrentRoom(GetCurrentRoom()->left);
			LoadLevelAtPath(GetCurrentLevel(), GetCurrentTilemap(), GetCurrentRoom()->filePath);
		}
	}
	if (AEInputCheckReleased(VK_RIGHT))
	{
		if (AEInputCheckCurr(VK_UP))
		{
			if (GetCurrentRoom()->topRight == NULL)
				return;

			SetCurrentRoom(GetCurrentRoom()->topRight);
			LoadLevelAtPath(GetCurrentLevel(), GetCurrentTilemap(), GetCurrentRoom()->filePath);
		}
		else if (AEInputCheckCurr(VK_DOWN))
		{
			if (GetCurrentRoom()->bottomRight == NULL)
				return;

			SetCurrentRoom(GetCurrentRoom()->bottomRight);
			LoadLevelAtPath(GetCurrentLevel(), GetCurrentTilemap(), GetCurrentRoom()->filePath);
		}
		else
		{
			if (GetCurrentRoom()->right == NULL)
				return;

			SetCurrentRoom(GetCurrentRoom()->right);
			LoadLevelAtPath(GetCurrentLevel(), GetCurrentTilemap(), GetCurrentRoom()->filePath);
		}
	}

	// Handle saving.
	LevelSaving();
}

// Handles world saving logic.
void WorldSaving(void)
{
	if (AEInputCheckCurr(VK_CONTROL))
	{
		// New world.
		if (AEInputCheckTriggered('N'))
		{
			char worldName[MAX_PATH] = { 0 };

			// Create a new world.
			switch (NewWorldWithPicker(GetWorld(), worldName))
			{
				case PICKERRESULT_FAIL:
					MessageBox(NULL, L"World could not be created.", L"New World Failed", MB_OK);
					break;

				case PICKERRESULT_SUCCESS:
					// Make a new world.
					strcpy_s(GetWorld()->filePath, sizeof(GetWorld()->filePath), worldName);
					GetWorld()->roomCount = 0;

					// Save the world.
					isSaved = 1;
					break;

				case PICKERRESULT_CANCEL:
					break;
			}
		}
		// Save world.
		else if (AEInputCheckTriggered('S'))
		{
			// Save as.
			if (AEInputCheckCurr(VK_SHIFT))
			{
				// Save the world as.
				switch (SaveWorldWithPicker(GetWorld()))
				{
					case PICKERRESULT_FAIL:
						MessageBox(NULL, L"World could not be saved.", L"Save World Failed", MB_OK);
						break;

					case PICKERRESULT_SUCCESS:
						// Save the world.
						isSaved = 1;
						break;

					case PICKERRESULT_CANCEL:
						break;
				}
			}
			// Save.
			else
			{
				// Save the world.
				switch (SaveWorldAtPath(GetWorld(), GetWorld()->filePath))
				{
					case PICKERRESULT_FAIL:
						MessageBox(NULL, L"World could not be saved.", L"Save World Failed", MB_OK);
						break;

					case PICKERRESULT_SUCCESS:
						// Save the world.
						isSaved = 1;
						break;

					case PICKERRESULT_CANCEL:
						break;
				}
			}
		}
		// Load world.
		else if (AEInputCheckTriggered('O'))
		{
			// Load a world.
			switch (LoadWorldWithPicker(GetWorld()))
			{
				case PICKERRESULT_FAIL:
					MessageBox(NULL, L"World could not be opened.", L"Open World Failed", MB_OK);
					break;

				case PICKERRESULT_SUCCESS:
					SwitchEditMode(EDITMODE_WORLD);
					isSaved = 1;
					break;

				case PICKERRESULT_CANCEL:
					break;
			}
		}
	}
}

// Handles level saving logic.
void LevelSaving(void)
{
	if (AEInputCheckCurr(VK_CONTROL))
	{
		// New world.
		if (AEInputCheckTriggered('N'))
		{
			char levelName[MAX_PATH] = { 0 };

			// Save a level as.
			switch (NewLevelWithPicker(GetCurrentLevel(), GetCurrentTilemap(), levelName))
			{
				case PICKERRESULT_FAIL:
					MessageBox(NULL, L"Level could not be created.", L"New Level Failed", MB_OK);
					break;

				case PICKERRESULT_SUCCESS:
					// Make a new level.
					NewLevel(levelName, 1000, 1000, MIN_LEVEL_WIDTH, MIN_LEVEL_HEIGHT);
					isSaved = 1;
					break;

				case PICKERRESULT_CANCEL:
					break;
			}
		}
		// Save world.
		else if (AEInputCheckTriggered('S'))
		{
			// Save as.
			if (AEInputCheckCurr(VK_SHIFT))
			{
				// Save a level as.
				switch (SaveLevelWithPicker(GetCurrentLevel(), GetCurrentTilemap()))
				{
					case PICKERRESULT_FAIL:
						MessageBox(NULL, L"Level could not be saved.", L"Save Level Failed", MB_OK);
						break;

					case PICKERRESULT_SUCCESS:
						isSaved = 1;
						break;

					case PICKERRESULT_CANCEL:
						break;
				}
			}
			// Save.
			else
			{
				// Save a level.
				switch (SaveLevelAtPath(GetCurrentLevel(), GetCurrentTilemap(), GetCurrentLevel()->filePath))
				{
					case PICKERRESULT_FAIL:
						MessageBox(NULL, L"Level could not be saved.", L"Save Level Failed", MB_OK);
						break;

					case PICKERRESULT_SUCCESS:
						isSaved = 1;
						break;

					case PICKERRESULT_CANCEL:
						break;
				}
			}
		}
		// Load world.
		else if (AEInputCheckTriggered('O'))
		{
			// Load a level.
			switch (LoadLevelWithPicker(GetCurrentLevel(), GetCurrentTilemap()))
			{
				case PICKERRESULT_FAIL:
					MessageBox(NULL, L"Level could not be opened.", L"Open Level Failed", MB_OK);
					break;

				case PICKERRESULT_SUCCESS:
					isSaved = 1;
					break;

				case PICKERRESULT_CANCEL:
					break;
			}
		}
	}
}

// Handles mouse wheel.
LRESULT GetMouseWheelTile(HWND hWin, UINT msg, WPARAM wp, LPARAM lp)
{
	UNREFERENCED_PARAMETER(hWin);
	UNREFERENCED_PARAMETER(msg);
	UNREFERENCED_PARAMETER(lp);

	// Switch behavior based on whether you're scrolling up or down.
	switch (wp)
	{
			// Scroll up.
		case 0x00780000:
			SwapTile(1);
			break;
			// Scroll down.
		case 0xFF880000:
			SwapTile(-1);
			break;
	}

	// I guess it does need to return something?
	LONG_PTR l = 0;

	// Function doesn't need to return anything.
	return l;
}

// Swaps current tile in direction.
Tile* SwapTile(int direction)
{
	if (editMode == EDITMODE_WORLD)
	{
		scrollAmount -= direction * 100;

		return selectedTile;
	}
	else
	{
		// Increment tile in direction.
		Tile* newTile = GetTileAt((selectedTile->id + direction) % TILE_COUNT);

		// Set new tile.
		selectedTile = newTile;

		return selectedTile;
	}
}

// Let the user resize the x border.
void ResizeBorderX(unsigned char newWidth, unsigned char newHeight)
{
	AEGfxMeshStart();
	AEGfxTriAdd(
		-TILE_SIZE * GetCurrentTilemap()->width * 0.5F - TILE_SIZE * 2, -TILE_SIZE * 0.5F, 0xFFFF0000, 0.0f, 1.0f,
		TILE_SIZE * GetCurrentTilemap()->width * 0.5F + TILE_SIZE * 2, -TILE_SIZE * 0.5F, 0xFFFF0000, 1.0f, 1.0f,
		-TILE_SIZE * GetCurrentTilemap()->width * 0.5F - TILE_SIZE * 2, TILE_SIZE * 0.5F, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		TILE_SIZE * GetCurrentTilemap()->width * 0.5F + TILE_SIZE * 2, -TILE_SIZE * 0.5F, 0xFFFF0000, 1.0f, 1.0f,
		TILE_SIZE * GetCurrentTilemap()->width * 0.5F + TILE_SIZE * 2, TILE_SIZE * 0.5F, 0xFFFF0000, 1.0f, 0.0f,
		-TILE_SIZE * GetCurrentTilemap()->width * 0.5F - TILE_SIZE * 2, TILE_SIZE * 0.5F, 0xFFFF0000, 0.0f, 0.0f);
	xBorderMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(xBorderMesh, "Failed to create X Border Mesh!");

	GetCurrentTilemap()->width = newWidth;
	GetCurrentTilemap()->height = newHeight;
}

// Let the user resize the y border.
void ResizeBorderY(unsigned char newWidth, unsigned char newHeight)
{
	AEGfxMeshStart();
	AEGfxTriAdd(
		-TILE_SIZE * 0.5F, -TILE_SIZE * GetCurrentTilemap()->height * 0.5F - TILE_SIZE * 2, 0xFFFF0000, 0.0f, 1.0f,
		TILE_SIZE * 0.5F, -TILE_SIZE * GetCurrentTilemap()->height * 0.5F - TILE_SIZE * 2, 0xFFFF0000, 1.0f, 1.0f,
		-TILE_SIZE * 0.5F, TILE_SIZE * GetCurrentTilemap()->height * 0.5F + TILE_SIZE * 2, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		TILE_SIZE * 0.5F, -TILE_SIZE * GetCurrentTilemap()->height * 0.5F - TILE_SIZE * 2, 0xFFFF0000, 1.0f, 1.0f,
		TILE_SIZE * 0.5F, TILE_SIZE * GetCurrentTilemap()->height * 0.5F + TILE_SIZE * 2, 0xFFFF0000, 1.0f, 0.0f,
		-TILE_SIZE * 0.5F, TILE_SIZE * GetCurrentTilemap()->height * 0.5F + TILE_SIZE * 2, 0xFFFF0000, 0.0f, 0.0f);
	yBorderMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(yBorderMesh, "Failed to create Y Border Mesh!");

	GetCurrentTilemap()->width = newWidth;
	GetCurrentTilemap()->height = newHeight;
}

// Render the border.
void DrawBorder(float x, float y, AEGfxTexture* borderTexture, AEGfxVertexList* borderMesh)
{
	// Set render mode.
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	// Set the position.
	AEGfxSetPosition(x, y);

	// Render the texture.
	AEGfxTextureSet(borderTexture, 0.0F, 0.0F);

	// Set transparency.
	AEGfxSetTransparency(1.0F);

	// Draw the mesh.
	AEGfxMeshDraw(borderMesh, AE_GFX_MDM_TRIANGLES);
}

// Switch between edit modes.
void SwitchEditMode(EDITMODE newEditMode)
{
	// Change the edit mode.
	editMode = newEditMode;
	
	// Apply neccessary changes based on level being opened, world, etc.
	switch (editMode)
	{
		case EDITMODE_WORLD:
			// Set the background color to black.
			AEGfxSetBackgroundColor(0.4F, 0.4F, 0.4F);

			// Reset camera position.
			camX = worldCamX;
			camY = worldCamY;

			// Let go of level pieces.
			areaDragging = NULL;
			break;

		case EDITMODE_LEVEL:
			// Set the background color to light blue.
			AEGfxSetBackgroundColor(0.7F, 0.7F, 1.0F);

			// Set the old cam positions so we know where to go back to.
			worldCamX = camX;
			worldCamY = camY;

			// Reset the camera position.
			camX = -64;
			camY = 96;

			// Reset border sizes.
			ResizeBorderX(GetCurrentTilemap()->width, GetCurrentTilemap()->height);
			ResizeBorderY(GetCurrentTilemap()->width, GetCurrentTilemap()->height);
			break;
	}
}

// Create a new level.
void NewLevel(const char* name, unsigned int x, unsigned int y, int width, int height)
{
	// Set default values for level.
	strcpy_s(GetCurrentLevel()->filePath, sizeof(GetCurrentLevel()->filePath), name);
	GetCurrentLevel()->levelNum = GetWorld()->roomCount;

	// Set default values for tilemap.
	GetCurrentTilemap()->width = width - 1;
	GetCurrentTilemap()->height = height - 1;
	for (int y = 0; y < MAX_LEVEL_HEIGHT; ++y)
	{
		for (int x = 0; x < MAX_LEVEL_WIDTH; ++x)
		{
			GetCurrentTilemap()->tiles[x][y] = GetTileAt(TILE_AIR);
		}
	}

	// Set default values for area.
	strcpy_s(GetWorld()->rooms[GetWorld()->roomCount].filePath, sizeof(GetWorld()->rooms[GetWorld()->roomCount].filePath), GetCurrentLevel()->filePath);
	GetWorld()->rooms[GetWorld()->roomCount].x = x;
	GetWorld()->rooms[GetWorld()->roomCount].y = y;
	GetWorld()->rooms[GetWorld()->roomCount].width = width;
	GetWorld()->rooms[GetWorld()->roomCount].height = height;

	// Increase world level count to match new level count.
	GetWorld()->roomCount++;

	// Save the world.
	SaveWorldAtPath(GetWorld(), GetWorld()->filePath);

	// Save the level.
	isSaved = 1;
}

// Handles mouse wheel.
LRESULT CheckForSaving(HWND hWin, UINT msg, WPARAM wp, LPARAM lp)
{
	UNREFERENCED_PARAMETER(hWin);
	UNREFERENCED_PARAMETER(msg);
	UNREFERENCED_PARAMETER(wp);
	UNREFERENCED_PARAMETER(lp);

	// I guess it does need to return something?
	LONG_PTR l = 0;

	/*
	// Prompt the user to save, and switch input accordingly.
	if (!isSaved)
	{
		switch (editMode)
		{
			case EDITMODE_WORLD:
				switch (MessageBox(NULL, L"Your world has unsaved changes. Would you like to save before quitting?", L"Unsaved Changes",	MB_YESNOCANCEL))
				{
					case IDNO:
						break;

					case IDYES:
						// Save the world.
						if (SaveWorldAtPath(GetWorld(), GetWorld()->filePath) == PICKERRESULT_SUCCESS)
							break;

					case IDCANCEL:
						GameStateManagerSetNextState(GsEditor);
						break;
				}
				break;

			case EDITMODE_LEVEL:
				switch (MessageBox(NULL, L"Your world has unsaved changes. Would you like to save before quitting?", L"Unsaved Changes",	MB_YESNOCANCEL))
				{
					case IDNO:
						break;

					case IDYES:
						// Save the level.
						if (SaveLevelAtPath(GetCurrentLevel(), GetCurrentLevel()->filePath) == PICKERRESULT_SUCCESS)
							break;

					case IDCANCEL:
						GameStateManagerSetNextState(GsEditor);
						break;
				}
				break;
		}
	}
	*/

	// Function doesn't need to return anything.
	return l;
}

// Gets the area list populated.
void InitAreaList(void)
{
	// Create temp level and tilemap values.
	Tilemap tilemap = { 0 };
	Level level = { 0 };

	// Iterate through each level in a file.
	int roomIndex = 0;
	for (char c = 'A'; c < 'H'; ++c)
	{
		for (int i = 1; i < 30; ++i)
		{
			// Create buffer for filename.
			char buffer[MAX_PATH];
			sprintf_s(buffer, sizeof(buffer), "./Levels/%c%d.lvl", c, i);

			// Check if level exists.
			if (LoadLevelAtPath(&level, &tilemap, buffer) == PICKERRESULT_SUCCESS)
			{
				// Set filename for level.
				strcpy_s(areaList[roomIndex].filePath, sizeof(areaList[roomIndex].filePath), buffer);

				// Iterate through tilemap.
				FromLevel(&areaList[roomIndex], &tilemap);

				// Increment room index variable.
				++roomIndex;
			}
		}
	}
}

// Draws the list of areas for the world.
void DisplayAreaList(float x, float y)
{
	// Draw the background.
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetFullTransform(camX + 576, camY, 0, 384, 896);
	AEGfxTextureSet(borderTexture, 0.0F, 0.0F);
	AEGfxSetTransparency(0.5F);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxMeshDraw(sideBarMesh, AE_GFX_MDM_TRIANGLES);
	
	// Convert mouse coordinates to world coordinates.
	float tileX = 0, tileY = 0;
	AEGfxConvertScreenCoordinatesToWorld(x, y, &tileX, &tileY);

	// Draw each area in the list.
	float xPos = camX + 440;
	float yPos = camY + 380 + scrollAmount;
	for (int i = 0; i < sizeof(areaList) / sizeof(areaList[0]); ++i)
	{
		// Draw the area in the appropriate position.
		DrawArea(&areaList[i], camX + 440, yPos);

		// Handle mouse inputs for dragging levels and opening levels.
		if (AEInputCheckTriggered(VK_LBUTTON) &&
			tileX >= xPos - TILE_SIZE_SMALL / 2 &&
			tileX <= xPos + areaList[i].width * TILE_SIZE_SMALL &&
			tileY <= yPos + TILE_SIZE_SMALL * 2 &&
			tileY >= yPos - 2 - areaList[i].height * TILE_SIZE_SMALL)
		{
			// Add area to world based on one selected.
			GetWorld()->rooms[GetWorld()->roomCount] = areaList[i];
			areaDragging = &GetWorld()->rooms[GetWorld()->roomCount];

			// Offset area being dragged.
			//xOffset = (int)tileX - areaDragging->x;
			//yOffset = (int)tileY - areaDragging->y;

			// Increment the number of areas in the world.
			++GetWorld()->roomCount;
		}

		// Make next area below.
		yPos -= (areaList[i].height + 5) * TILE_SIZE_SMALL;
	}
}
