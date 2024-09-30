//------------------------------------------------------------------------------
//
// File Name:	GameStateStub.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 0
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateStub.h"
#include "Trace.h"

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
void GameStateStubLoad()
{
}

// Initialize the memory associated with the Stub game state.
void GameStateStubInit()
{
}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateStubUpdate(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);

	GameStateManagerSetNextState(GsQuit);
}

// Free any memory associated with the Stub game state.
void GameStateStubShutdown()
{
	// Free all objects.
}

// Unload the resources associated with the Stub game state.
void GameStateStubUnload()
{
	// Free all sprite sources.

	// Unload all textures.

	// Free all meshes.
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

