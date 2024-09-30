//------------------------------------------------------------------------------
//
// File Name:	System.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 0
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <Windows.h>

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Store the Windows settings passed in WinMain.
void SystemStoreWindowSettings(HINSTANCE instanceH, int show);

// Initialize the System (Windows, Event Handlers).
void SystemInit();

// Update the System (Windows, Event Handlers).
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void SystemUpdate(float dt);

// Complete the draw process for the current game loop.
void SystemDraw();

// Shutdown the System (Windows, Event Handlers).
void SystemShutdown();

/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

