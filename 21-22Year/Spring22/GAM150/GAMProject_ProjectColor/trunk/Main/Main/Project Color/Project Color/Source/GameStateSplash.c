//------------------------------------------------------------------------------
//
// File Name:	GameStateSplash.c
// Author(s):	Emily Comes (emily.comes)
// Project:		"Project Color"
// Course:		GAM-150-B Spring 2022
//
// Copyright (C) 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "GameStateSwitcher.h"
#include "GameStateManager.h"
#include "GameStateSplash.h"
#include "Vector2D.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Transform.h"
#include "DisplayHelper.h"
#include "ConfigHelper.h"

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static char SCREEN_SCALE = 1;
static AEGfxVertexList* mesh1x1;
static AEGfxVertexList* logoMesh;
static AEGfxTexture* digiLogoTexture, * fmodLogoTexture;
static SpriteSourcePtr digiLogoSprSrc, fmodLogoSprSrc;
static GameObjectPtr logoObj;
static float splashAlpha = 0.f;
static float stateTime = 0.f;
static char alreadySplashed = 0;
static char skipSplash = 0;
static char doneWithDigiLogo = 0;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load external resources.
void GameStateSplashLoad()
{
  mesh1x1 = MeshCreateQuad(.5f, .5f, 1.f, 1.f, "mesh1x1");
  logoMesh = mesh1x1;
  digiLogoTexture = AEGfxTextureLoad("./Assets/dp.png");
  digiLogoSprSrc = SpriteSourceCreate(1, 1, digiLogoTexture);
  fmodLogoTexture = AEGfxTextureLoad("./Assets/fmod.png");
  fmodLogoSprSrc = SpriteSourceCreate(1, 1, fmodLogoTexture);
}

// Initialize the memory.
void GameStateSplashInit()
{
  alreadySplashed = ReadConfigByte("game", "alreadysplashed", 0);
  skipSplash = 0;
  doneWithDigiLogo = 0;
  AEGfxSetWorldOriginMode(AE_WOM_NORMAL);
  AEGfxSetCamPosition(0.f, 0.f);
  AEGfxSetBackgroundColor(0.f, 0.f, 0.f);
  AEGfxSetBlendMode(AE_GFX_BM_BLEND);
  AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);

  //AESysSetWindowTitle("Splash");
  SCREEN_SCALE = ReadConfigByte("display", "scale", 1);

  stateTime = 0.f;
  splashAlpha = 0.f;

  logoObj = GameObjectCreate();
  if (logoObj != NULL)
  {
    GameObjectSetName(logoObj, "logo");

    TransformPtr logoTF = TransformCreate();
    Vector2D logoScale;
    logoScale.x = 728 * (30 / 91.f) * SCREEN_SCALE;
    logoScale.y = 192 * (30 / 91.f) * SCREEN_SCALE;
    if (SCREEN_SCALE > 2)
    {
      logoScale.x = 728 * (30 / 91.f) * 3;
      logoScale.y = 192 * (30 / 91.f) * 3;
    }
    TransformSetScale(logoTF, &logoScale);
    GameObjectAddTransform(logoObj, logoTF);

    SpritePtr logoSpr = SpriteCreate();
    SpriteSetAlpha(logoSpr, splashAlpha);
    SpriteSetMesh(logoSpr, logoMesh);
    SpriteSetSpriteSource(logoSpr, digiLogoSprSrc);
    GameObjectAddSprite(logoObj, logoSpr);
  }
}

// Update the game state.
void GameStateSplashUpdate(float dt)
{
  stateTime += dt;

  if (alreadySplashed && (AEInputCheckTriggered(VK_SPACE) || AEInputCheckTriggered(VK_RETURN) || AEInputCheckTriggered(VK_ESCAPE) || AEInputCheckTriggered(VK_LBUTTON)))
    skipSplash = 1;

  if (1.f < stateTime && stateTime <= 2.f && !skipSplash)
  {
    splashAlpha += dt;
    SpriteSetAlpha(GameObjectGetSprite(logoObj), splashAlpha);
  }
  else if (5.f < stateTime && stateTime <= 6.f || skipSplash)
  {
    splashAlpha -= dt;
    SpriteSetAlpha(GameObjectGetSprite(logoObj), splashAlpha);
  }

  GameObjectUpdate(logoObj, dt);
  GameObjectDraw(logoObj);

  if (7.f < stateTime || (skipSplash && splashAlpha <= 0.f))
  {
    if (!doneWithDigiLogo)
    {
      stateTime = 0.f;
      splashAlpha = 0.f;
      skipSplash = 0;
      doneWithDigiLogo = 1;
      SpriteSetSpriteSource(GameObjectGetSprite(logoObj), fmodLogoSprSrc);
    }
    else
      GameStateManagerSetNextState(GsTitle);
  }
}

// Free memory allocated when the state was initialized.
void GameStateSplashShutdown()
{
  GameObjectFree(&logoObj);
  if (!alreadySplashed)
    WriteConfigByte("game", "alreadysplashed", 1);
}

// Unload external resources.
void GameStateSplashUnload()
{
  SpriteSourceFree(&digiLogoSprSrc);
  SpriteSourceFree(&fmodLogoSprSrc);
  AEGfxTextureUnload(digiLogoTexture);
  AEGfxTextureUnload(fmodLogoTexture);
  AEGfxMeshFree(mesh1x1);
}
