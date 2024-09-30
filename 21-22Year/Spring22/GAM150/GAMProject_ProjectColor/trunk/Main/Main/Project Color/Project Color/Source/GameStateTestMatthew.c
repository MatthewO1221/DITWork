//------------------------------------------------------------------------------
//
// File Name:	GameStateTestMatthew.c
// Author(s):	Matthew Oliver
// Project:		Project Color
// Course:		GAM150B
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "GameStateTestDerek.h"
#include <math.h>
#include "GameStateSwitcher.h"
#include "GameStateManager.h"
#include "World.h"
#include "Paint.h"
#include "Level.h"
#include "Tile.h"
#include "GameplayInstance.h"
#include "Player Controller.h"
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
AEGfxTexture* backgroundTexture;
AEGfxVertexList* backgroundMesh;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


void SpawnFleye(void);

// Load the resources associated with the Stub game state.
void GameStateTestMatthewLoad()
{
	// Load debug bounding box sprite.
#if DEBUG_MODE
	SetBoundingBox();
#endif

	// Create background stuff.
	backgroundTexture = AEGfxTextureLoad("./Assets/background.png");
	backgroundMesh = MeshCreateQuad(1, 1, 1.0F, 1.0F, "Background");

	// Initialize everything.
	InitTileSystem();
	InitWorldSystem();
	InitPaintSystem();

	// Load player and paintbrush.
	LoadPlayer();
	LoadBrush();
}

// Initialize the memory associated with the Stub game state.
void GameStateTestMatthewInit()
{
	// Change the window title.
	//AESysSetWindowTitle("Project Color - Test Matthew");

	// Set the background color to black.
	AEGfxSetBackgroundColor(0, 0, 0);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// Load level (for testing).
	if (GetCurrentLevel()->filePath[0] != '.')
	{
		LoadLevelAtPath(GetCurrentLevel(), GetCurrentTilemap(), "./Levels/A1.lvl");
	}

	// Init gameplay instance.
	InitGameplayInstance();

	// Init player and paintbrush.
	InitPlayer();

	// Init level.
	InitLevel(GetCurrentTilemap());

	// Init camera.
	InitCamera();
}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateTestMatthewUpdate(float dt)
{
	// Set the camera to be targeted at the player's position.
	SetTargetCamPos(TransformGetTranslation(GameObjectGetTransform(GetPlayer()->parent)));

	// Draw background.
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetFullTransform(GetCamPos().x - 32, GetCamPos().y, 0, AEGfxGetWinMaxX() + 64, AEGfxGetWinMaxY());
	AEGfxTextureSet(backgroundTexture, 0.0F, 0.0F);
	AEGfxSetTransparency(1.0F);
	AEGfxMeshDraw(backgroundMesh, AE_GFX_MDM_TRIANGLES);

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
	GameObjectUpdate(GetPlayer()->parent, dt);
	GameObjectDraw(GetPlayer()->parent);

	// Update and draw paintbrush if it's been unlocked yet.
	if (GetPlayer()->hasBrush)
	{
		GameObjectUpdate(GetBrush(), dt);
		GameObjectDraw(GetBrush());
	}

	// Update and draw all enemies in the level.
	for (int i = 0; i < MaxSpawnedEnemies; ++i)
	{
		if (GetEnemyIndex(i) != NULL)
		{
			GameObjectUpdate(GetEnemyIndex(i)->gameObject, dt);
		}
	}
	UpdateAllEnemies(GetPlayer(), *GetGame()->tileCollisions, dt);
	DrawAllEnemies();

	// Update all enemy gameobjects.
	

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

	if (AEInputCheckTriggered(VK_SPACE))
	{
		SpawnFleye();
	}
	// Let the user toggle between gamestates.
	ToggleGameState();
}

// Free any memory associated with the Stub game state.
void GameStateTestMatthewShutdown()
{
	// Free memory with level.
	ShutdownLevel(GetCurrentTilemap());

	// Free memory with camera.
	ShutdownCamera();

	// Free memory with player.
	ShutdownPlayer();
}

// Unload the resources associated with the Stub game state.
void GameStateTestMatthewUnload()
{
	// Unload assets with player and paintbrush.
	UnloadPlayer();
	UnloadBrush();

	// Shutdown all systems.
	ShutdownPaintSystem();
	ShutdownWorldSystem();
	ShutdownTileSystem();

	// Free background stuff.
	AEGfxTextureUnload(backgroundTexture);
	AEGfxMeshFree(backgroundMesh);

	// Free debug bounding box sprite.
#if DEBUG_MODE
	FreeBoundingBox();
#endif
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Initialize player info.

// Initialize brush info.

void SpawnFleye()
{
	Vector2D spawnPos = { 100,100 };
	SpawnEnemy(Slime, Green, spawnPos, "TestFleye");
}