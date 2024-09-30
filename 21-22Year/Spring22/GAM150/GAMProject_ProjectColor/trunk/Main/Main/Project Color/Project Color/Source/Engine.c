//------------------------------------------------------------------------------
//
// File Name:	Engine.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 0
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Engine.h"
#include "GameStateManager.h"
#include "System.h"

//------------------------------------------------------------------------------
// Private Consts:
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

// Initialize the game engine.
void EngineInit()
{
	//--------------------------------------------------------------------------
	// NOTE: Certain modules need to be initialized first.
	//--------------------------------------------------------------------------

	// Initialize the System (Windows, Event Handlers).
	SystemInit();

	//--------------------------------------------------------------------------
	// NOTE: Other modules can be initialized later and in any order.
	//--------------------------------------------------------------------------

	// Initialize the game state manager.
	GameStateManagerInit();
}

// Update the game engine.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void EngineUpdate(float dt)
{
	// Update the System (Windows, Event Handlers).
	SystemUpdate(dt);

	// Update the game state manager.
	GameStateManagerUpdate(dt);

	// Complete the draw process for the current game loop.
	SystemDraw();
}

// Shutdown the game engine.
void EngineShutdown()
{
	//--------------------------------------------------------------------------
	// NOTE: Some modules can be shutdown in any order.
	//--------------------------------------------------------------------------

	// Shutdown the game state manager.
	GameStateManagerShutdown();

	//--------------------------------------------------------------------------
	// NOTE: Certain modules need to be shutdown last and in reverse order.
	//--------------------------------------------------------------------------

	// Shutdown the System (Windows, Event Handlers).
	SystemShutdown();
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

