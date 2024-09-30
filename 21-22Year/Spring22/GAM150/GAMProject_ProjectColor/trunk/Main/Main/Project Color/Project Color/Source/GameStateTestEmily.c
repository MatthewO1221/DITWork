//------------------------------------------------------------------------------
//
// File Name:	GameStateTestEmily.c
// Author(s):	Emily Comes (emily.comes)
// Project:		"Project Color"
// Course:		CS230S22
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
#include "GameStateTestEmily.h"
#include "Hud.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------
static const char maxHealth = 10;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------s
static void HealthBarTestInput(void);

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static char health = 10;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void GameStateTestEmilyLoad()
{
  LoadStatusBar();
}

void GameStateTestEmilyInit()
{
  //AESysSetWindowTitle("HUD Test");
  AEGfxSetCamPosition(0.f, 0.f);

  InitStatusBar();

  AEGfxSetBackgroundColor(.25f, .25f, .25f);
  AEGfxSetBlendMode(AE_GFX_BM_BLEND);
  AEGfxSetTextureMode(AE_GFX_TM_PRECISE);
}

void GameStateTestEmilyUpdate(float dt)
{
  ToggleGameState();
  HealthBarTestInput();
  UpdateAndDrawStatusBar(health, maxHealth, 0, 0, 1, 1, dt);
}

void GameStateTestEmilyShutdown()
{
  ShutdownStatusBar();
}

void GameStateTestEmilyUnload()
{
  UnloadStatusBar();
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void HealthBarTestInput()
{
  if (AEInputCheckTriggered(VK_LEFT) || AEInputCheckTriggered(VK_RIGHT))
  {
    if (AEInputCheckTriggered(VK_LEFT) && !AEInputCheckTriggered(VK_RIGHT)) // if left was pressed and right wasn't pressed
      --health;
    else if (!AEInputCheckTriggered(VK_LEFT) && AEInputCheckTriggered(VK_RIGHT)) // if left wasn't pressed and right was pressed
      ++health;

    // clamp health value to range of 0 - 10
    if (health < 0)
      health = 0;
    else if (health > maxHealth)
      health = maxHealth;
  }
}
