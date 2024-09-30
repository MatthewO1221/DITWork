//------------------------------------------------------------------------------
//
// File Name:	GameStateTestBrody.c
// Author(s):	Brody Endstrasser
// Project:		Project Color
// Course:		GAM150B
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include <math.h>
#include "GameStateSwitcher.h"
#include "GameStateManager.h"
#include "Paint.h"
#include "Player.h"
#include "Player Controller.h"
#include "Player Movement.h"
#include "Brush.h"
#include "Tile.h"
#include "GameplayInstance.h"
#include "World.h"
#include "Level.h"
#include "Serialization.h"
#include "Button.h"
#include "Vector2D.h"
#include "GameObject.h"
#include "Transform.h"
#include "Animation.h"
#include "Physics.h"
#include "SpriteSource.h"
#include "Sprite.h"
#include "Mesh.h"
#include "Serialization.h"
#include "AABB.h"
#include "Debug.h"
#include "MathHelper.h"
#include "EnemyAI.h"
#include "Camera.h"
#include "Player.h"
#include "Player Movement.h"
#include "Brush.h"
#include "GameStateTestDerek.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

#define CLOUD_COUNT 10

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

AEGfxTexture* backgroundTexture;
AEGfxVertexList* backgroundMesh;

AEGfxTexture* cloudTextures[3];
AEGfxVertexList* cloudMeshes[3];

Vector2D cloudPositions[CLOUD_COUNT];
float cloudSpeeds[CLOUD_COUNT];
float cloudParallaxes[CLOUD_COUNT];
AEGfxTexture** cloudTexturesA[CLOUD_COUNT];
AEGfxVertexList** cloudMeshesA[CLOUD_COUNT];
float cloudAlphas[CLOUD_COUNT];

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

void ResetCloud(int);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


// Load the resources associated with the Stub game state.
void GameStateTestBrodyLoad()
{
	// Initialize everything.
	InitTileSystem();
	InitWorldSystem();
	InitPaintSystem();

	// Load player and paintbrush.
	LoadPlayer();
	LoadBrush();
}

// Initialize the memory associated with the Stub game state.
void GameStateTestBrodyInit()
{

	// Change the window title.
	//AESysSetWindowTitle("Project Color - Test Brody");

	// Load level (for testing).
	if (GetCurrentLevel()->filePath[0] != '.')
	{
		LoadLevelAtPath(GetCurrentLevel(), GetCurrentTilemap(), "./Levels/A1.lvl");
	}

	/*Vector2D pos = { 1, 1 };
	ButtonCreate(pos, BUTTON_WIDTH, BUTTON_HEIGHT, "Play");

	Vector2DScale(&pos, &pos, 80);
	ButtonCreate(pos, BUTTON_WIDTH, BUTTON_HEIGHT, "Quit");*/
	// Set the background color to black.
	AEGfxSetBackgroundColor(0, 0, 0);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// Init gameplay instance.
	InitGameplayInstance();

	// Init player and paintbrush.
	InitPlayer();

}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateTestBrodyUpdate(float dt)
{
	/*UNREFERENCED_PARAMETER(dt);

	AEGfxSetBackgroundColor(0.7F, 0.7F, 1.0F);

	// Update and draw paint.
	UpdatePaintSystem();
	DrawAllPaint();

	// Update player information from user input.
	PlayerUpdate(0.0F, 0.0F, *GetGame()->tileCollisions);

	TransformPtr t = GameObjectGetTransform(GetPlayer()->parent);
	Vector2D pos = { 0, 0 };
	TransformSetTranslation(t, &pos);

	// Update and draw player.
	GameObjectUpdate(GetPlayer()->parent, !IsInWater() ? dt : dt * 0.5F);
	GameObjectDraw(GetPlayer()->parent);

	GameObjectUpdate(GetBrush(), dt);
	GameObjectDraw(GetBrush());

	// Let the user toggle between gamestates.
	ToggleGameState();

	for (int i = 0; i < MAX_BUTTONS_ONSCREEN; ++i){
		if (getButt(i) != NULL)
			ButtonUpdate(getButt(i));
	}*/
	// Set the camera to be targeted at the player's position.
	SetTargetCamPos(TransformGetTranslation(GameObjectGetTransform(GetPlayer()->parent)));

	// Draw background.
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetFullTransform(GetCamPos().x - 32, GetCamPos().y, 0, AEGfxGetWinMaxX() + 64, AEGfxGetWinMaxY());
	AEGfxTextureSet(backgroundTexture, 0.0F, 0.0F);
	AEGfxSetTransparency(1.0F);
	AEGfxMeshDraw(backgroundMesh, AE_GFX_MDM_TRIANGLES);

	// Draw clouds
	for (int i = 0; i < sizeof(cloudPositions) / sizeof(cloudPositions[0]); ++i)
	{
		cloudPositions[i].x -= dt * cloudSpeeds[i];

		// Reset cloud.
		if (cloudPositions[i].x < -768)
		{
			ResetCloud(i);
		}

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(GetCamPos().x * cloudParallaxes[i] + cloudPositions[i].x, GetCamPos().y * 0.5F + cloudPositions[i].y);
		AEGfxTextureSet(*cloudTexturesA[i], 0.0F, 0.0F);
		AEGfxSetTransparency(cloudAlphas[i]);
		AEGfxMeshDraw(*cloudMeshesA[i], AE_GFX_MDM_TRIANGLES);
	}

	// Draw extra ground on the top.
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(GetTileAt(TILE_GROUND_6)->texture, 0.0F, 0.0F);
	AEGfxSetTransparency(1.0F);

	//AEGfxSetFullTransform(GetCamPos().x - 32, 544, 0, AEGfxGetWinMaxX() + 64, 64);
	//AEGfxMeshDraw(backgroundMesh, AE_GFX_MDM_TRIANGLES);

	// On the bottom.
	AEGfxSetFullTransform(GetCamPos().x - 32, -480, 0, AEGfxGetWinMaxX() + 64, 64);
	AEGfxMeshDraw(backgroundMesh, AE_GFX_MDM_TRIANGLES);

	// On the left.
	AEGfxSetFullTransform(-896, GetCamPos().y - 32, 0, 96, AEGfxGetWinMaxY());
	AEGfxMeshDraw(backgroundMesh, AE_GFX_MDM_TRIANGLES);

	// On the right.
	//AEGfxSetFullTransform(832, GetCamPos().y - 32, 0, 96, AEGfxGetWinMaxY());
	//AEGfxMeshDraw(backgroundMesh, AE_GFX_MDM_TRIANGLES);

	// Update and draw the current level.
	UpdateLevel(GetCurrentTilemap());
	DrawLevel(GetCurrentTilemap());

	// Draw all gameobjects in the level.
	DrawGameObjects();

	// Update and draw paint.
	UpdatePaintSystem();
	DrawAllPaint();

	// Update player information from user input.
	PlayerUpdate(0.0F, 0.0F, *GetGame()->tileCollisions, dt);

	// Update and draw player.
	GameObjectUpdate(GetPlayer()->parent, !IsInWater() ? dt : dt * 0.5F);
	GameObjectDraw(GetPlayer()->parent);

	// Update and draw paintbrush if it's been unlocked.
	if (GetPlayer()->hasBrush)
	{
		GameObjectUpdate(GetBrush(), dt);
		GameObjectDraw(GetBrush());
	}

	// Update and draw all enemies in the level.
	UpdateAllEnemies(GetPlayer(), *GetGame()->tileCollisions, dt);
	DrawAllEnemies();

	// Update all enemy gameobjects.
	for (int i = 0; i < MaxSpawnedEnemies; ++i)
	{
		if (GetEnemyIndex(i) != NULL)
		{
			GameObjectUpdate(GetEnemyIndex(i)->gameObject, dt);
		}
	}

	// Update and draw the camera.
	ClampCamera(true);
	UpdateCamera();

#if DEBUG_MODE
	DrawCamera();
#endif

	// Apply room-switching logic if the player leaves the level.
	CheckForPlayerOutOfBounds();

	// Let the user exit play mode.
	if (AEInputCheckTriggered(VK_ESCAPE))
	{
		// Load the level in Derek's test scene.
		GameStateManagerSetNextState(GsEditor);
	}

	// Let the user toggle between gamestates.
	ToggleGameState();
}

// Free any memory associated with the Stub game state.
void GameStateTestBrodyShutdown()
{
	// Free memory with level.
	ShutdownLevel(GetCurrentTilemap());

	// Free memory with player.
	ShutdownPlayer();

	/*for (int i = 0; i < MAX_BUTTONS_ONSCREEN; ++i) {
		if (getButt(i) != NULL)
			ButtonDestroy(getButt(i));
	}*/

	// Free memory with level.
	ShutdownLevel(GetCurrentTilemap());

	// Free memory with camera.
	ShutdownCamera();

	// Free memory with player.
	ShutdownPlayer();
}

// Unload the resources associated with the Stub game state.
void GameStateTestBrodyUnload()
{
	// Unload assets with player and paintbrush.
	UnloadPlayer();
	UnloadBrush();

	// Shutdown all systems.
	ShutdownPaintSystem();
	ShutdownWorldSystem();
	ShutdownTileSystem();
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

