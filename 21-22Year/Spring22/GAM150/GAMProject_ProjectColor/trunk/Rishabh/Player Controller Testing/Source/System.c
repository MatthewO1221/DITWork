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

//------------------------------------------------------------------------------
// Libraries
//------------------------------------------------------------------------------

#ifdef _DEBUG
#pragma comment (lib, "Alpha_Engine_D.lib")
#else
#pragma comment (lib, "Alpha_Engine.lib")
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
	// Initialize the system 
	AESysInitInfo sysInitInfo;
	sysInitInfo.mAppInstance = systemInfo.mAppInstance;
	sysInitInfo.mShow = systemInfo.mCreateWindow;
	sysInitInfo.mWinWidth = 800;
	sysInitInfo.mWinHeight = 600;
	sysInitInfo.mCreateConsole = 1;
	sysInitInfo.mMaxFrameRate = 60;
	sysInitInfo.mpWinCallBack = NULL;//MyWinCallBack;
	sysInitInfo.mClassStyle = CS_HREDRAW | CS_VREDRAW;
	sysInitInfo.mWindowStyle = WS_OVERLAPPEDWINDOW;//WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;;		
	sysInitInfo.mCreateWindow = 1;
	sysInitInfo.mWindowHandle = NULL;
	AESysInit(&sysInitInfo);

	// Change the window title.
	AESysSetWindowTitle("CS230 Project 2 - Graphics and Physics");

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
	if (AEInputCheckTriggered(VK_ESCAPE) || !AESysDoesWindowExist())
	{
		GameStateManagerSetNextState(GsQuit);
	}
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

