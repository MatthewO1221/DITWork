//------------------------------------------------------------------------------
//
// File Name:	GameStateTestDerek.c
// Author(s):	Derek Herrera-Sturm
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
#include "Audio.h"
#include "Hud.h"

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
void GameStateTestDerekLoad()
{
  // Load debug bounding box sprite.
#if DEBUG_MODE
  SetBoundingBox();
#endif

  // Create background stuff.
  backgroundTexture = AEGfxTextureLoad("./Assets/background.png");
  backgroundMesh = MeshCreateQuad(1, 1, 1.0F, 1.0F, "Background");

  // Create cloud stuff.
  cloudTextures[0] = AEGfxTextureLoad("./Assets/cloud_1.png");
  cloudMeshes[0] = MeshCreateQuad(59 * (PIXEL_SIZE / 2), 21 * (PIXEL_SIZE / 2), 1.0F, 1.0F, "Cloud 1");
  cloudTextures[1] = AEGfxTextureLoad("./Assets/cloud_2.png");
  cloudMeshes[1] = MeshCreateQuad(87 * (PIXEL_SIZE / 2), 35 * (PIXEL_SIZE / 2), 1.0F, 1.0F, "Cloud 2");
  cloudTextures[2] = AEGfxTextureLoad("./Assets/cloud_3.png");
  cloudMeshes[2] = MeshCreateQuad(54 * (PIXEL_SIZE / 2), 22 * (PIXEL_SIZE / 2), 1.0F, 1.0F, "Cloud 3");

  for (int i = 0; i < sizeof(cloudPositions) / sizeof(cloudPositions[0]); ++i)
  {
      ResetCloud(i);
      cloudPositions[i].x = (AERandFloat() * 1536) - 768;
  }

  // Initialize everything.
  InitTileSystem();
  InitWorldSystem();
  InitPaintSystem();
  LoadStream("./Assets/Music/idk.wav");

  // Load player and paintbrush.
  LoadPlayer();
  LoadBrush();

  // Load HUD stuff.
  LoadStatusBar();
}

// Initialize the memory associated with the Stub game state.
void GameStateTestDerekInit()
{
  // Change the window title.
 // AESysSetWindowTitle("Project Color - Test Derek");

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

  // Init HUD.
  InitStatusBar();

  //play the overworld music
  PlayStream("./Assets/Music/idk.wav");

  // Set default values.
  GetPlayer()->hasBrush = 1;

  GetPlayer()->maxHealth = 3;
  GetPlayer()->health = GetPlayer()->maxHealth;

  GetPlayer()->maxPaint = 5000.0F;
  GetPlayer()->paint = GetPlayer()->maxPaint;
  GetPlayer()->currColor = PAINT_NONE;
  GetPlayer()->unlockedColors = 63;

  GetPlayer()->roomNum = 0;

  // Write new file.
  SavePlayerData("player_data.svd");
}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateTestDerekUpdate(float dt)
{
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

  // Update and draw status bar.
  UpdateAndDrawStatusBar((char)GetPlayer()->health, (char)GetPlayer()->maxHealth, GetPlayer()->gemCount, GetPlayer()->currColor, GetPlayer()->paint, GetPlayer()->maxPaint, dt);

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
void GameStateTestDerekShutdown()
{
	// Free memory with HUD.
	ShutdownStatusBar();

  // Free memory with level.
  ShutdownLevel(GetCurrentTilemap());

  // Free memory with camera.
  ShutdownCamera();

  // Free memory with player.
  ShutdownPlayer();
}

// Unload the resources associated with the Stub game state.
void GameStateTestDerekUnload()
{
	// Unload assets with HUD.
	UnloadStatusBar();

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

  // Free cloud stuff.
  for (int i = 0; i < 3; ++i)
  {
    AEGfxTextureUnload(cloudTextures[i]);
    AEGfxMeshFree(cloudMeshes[i]);
  }

  // Free debug bounding box sprite.
#if DEBUG_MODE
  FreeBoundingBox();
#endif
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

void ResetCloud(int i)
{
    cloudPositions[i].x = (float)(GetCurrentTilemap()->width * TILE_SIZE) - 768;
    cloudPositions[i].y = -50 + AERandFloat() * 600;

    cloudSpeeds[i] = 15 + AERandFloat() * 25;
    cloudParallaxes[i] = 0.25F + AERandFloat() * 0.5F;
    cloudAlphas[i] = 0.5F + AERandFloat() * 0.5F;

    int r = (int)(AERandFloat() * 3);
    cloudTexturesA[i] = &cloudTextures[r];
    cloudMeshesA[i] = &cloudMeshes[r];
}
