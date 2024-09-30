//------------------------------------------------------------------------------
//
// File Name:	System.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 2
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "System.h"
#include "GameStateManager.h"
#include <Windows.h>
#include "ConfigHelper.h"

//------------------------------------------------------------------------------
// Libraries
//------------------------------------------------------------------------------

#ifdef _DEBUG
#pragma comment (lib, "Alpha_Engine_D.lib")
#pragma comment (lib, "FMODL.dll")
#else // NDEBUG
#pragma comment (lib, "Alpha_Engine.lib")
#pragma comment (lib, "FMOD.dll")
#endif

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct SystemInfo
{
  HINSTANCE mAppInstance;
  bool mCreateWindow;
} SystemInfo;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static SystemInfo systemInfo;
static char SCREEN_SCALE = 4;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Store the Windows settings passed in WinMain.
void SystemStoreWindowSettings(HINSTANCE instanceH, int show)
{
  systemInfo.mAppInstance = instanceH;
  systemInfo.mCreateWindow = show;
}

// Initialize the System (Windows, Event Handlers).
void SystemInit()
{
  // read screen scale from config
  SCREEN_SCALE = ReadConfigByte("display", "scale", 1);

  // clamp the screen scale
  if (SCREEN_SCALE < 1)
  {
    SCREEN_SCALE = 1;
    WriteConfigByte("display", "scale", 1);
  }
  else if (SCREEN_SCALE > 8)
  {
    SCREEN_SCALE = 8;
    WriteConfigByte("display", "scale", 8);
  }

  // Initialize the system
  AESysInitInfo sysInitInfo;
  sysInitInfo.mAppInstance = systemInfo.mAppInstance;
  sysInitInfo.mShow = systemInfo.mCreateWindow;

  sysInitInfo.mWinWidth = 1536; // screen should be a multiple of NES's native resolution
  sysInitInfo.mWinHeight = 896;

  // only
#ifdef _DEBUG
  sysInitInfo.mCreateConsole = 1;
#else // NDEBUG
  sysInitInfo.mCreateConsole = 0;
#endif
  sysInitInfo.mMaxFrameRate = 60; // 4x the standard of 60fps
  sysInitInfo.mpWinCallBack = NULL;//MyWinCallBack;
  sysInitInfo.mClassStyle = CS_HREDRAW | CS_VREDRAW;
  sysInitInfo.mWindowStyle = WS_OVERLAPPEDWINDOW;//WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;;
  sysInitInfo.mCreateWindow = 1;
  sysInitInfo.mWindowHandle = NULL;
  AESysInit(&sysInitInfo);

  // Change the window title.
  AESysSetWindowTitle("Palette Knight");

  // reset the system modules
  AESysReset();
}

// Update the System (Windows, Event Handlers).
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void SystemUpdate(float dt)
{
  /* Tell the compiler that the 'dt' variable is unused. */
  UNREFERENCED_PARAMETER(dt);

  // Informing the system about the loop's start
  AESysFrameStart();

  // Update the Windows message queue.
  AESysUpdate();

  // Handling Input
  AEInputUpdate();

  // check if forcing the application to quit
  if (!AESysDoesWindowExist())
    GameStateManagerSetNextState(GsQuit);
}

// Complete the draw process for the current game loop.
void SystemDraw()
{
  // Informing the system about the loop's end
  AESysFrameEnd();
}

// Shutdown the System (Windows, Event Handlers).
void SystemShutdown()
{
  // Shutdown the Alpha Engine.
  AESysExit();
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
