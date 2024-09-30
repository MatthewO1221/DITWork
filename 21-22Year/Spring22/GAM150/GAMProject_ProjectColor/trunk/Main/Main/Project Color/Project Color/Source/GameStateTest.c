//------------------------------------------------------------------------------
//
// File Name:	GameStateStub.c
// Author(s):	Derek Herrera-Sturm
// Project:		Project 0
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "GameStateTest.h"
#include "GameStateSwitcher.h"

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
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Stub game state.
void GameStateTestLoad()
{

}

// Initialize the memory associated with the Stub game state.
void GameStateTestInit()
{
	// Change the window title.
	//AESysSetWindowTitle("Project Color - Test");
}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateTestUpdate(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	// Let the user toggle between gamestates.
	ToggleGameState();
}

// Free any memory associated with the Stub game state.
void GameStateTestShutdown()
{

}

// Unload the resources associated with the Stub game state.
void GameStateTestUnload()
{

}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

