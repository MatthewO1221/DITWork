//------------------------------------------------------------------------------
//
// File Name:	GameStateDemo.h
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

// Load the resources associated with the Demo game state.
void GameStateDemoLoad();

// Initialize the memory associated with the Demo game state.
void GameStateDemoInit();

// Update the Demo game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateDemoUpdate(float dt);

// Shutdown any memory associated with the Demo game state.
void GameStateDemoShutdown();

// Unload the resources associated with the Demo game state.
void GameStateDemoUnload();

/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

