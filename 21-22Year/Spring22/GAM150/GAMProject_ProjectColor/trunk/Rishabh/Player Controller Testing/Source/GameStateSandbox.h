//------------------------------------------------------------------------------
//
// File Name:	GameStateSandbox.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 0
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Constants:
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


static void GameStateSandboxTraceFloat(const char* text, float f);

static void GameStateSandboxTraceVector(const char* text, const Vector2D* v);

// Load the resources associated with the Sandbox game state.
void GameStateSandboxLoad();

// Initialize the memory associated with the Sandbox game state.
void GameStateSandboxInit();

// Update the Sandbox game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateSandboxUpdate(float dt);

// Shutdown any memory associated with the Sandbox game state.
void GameStateSandboxShutdown();

// Unload the resources associated with the Sandbox game state.
void GameStateSandboxUnload();

/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

