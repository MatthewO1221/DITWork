//------------------------------------------------------------------------------
//
// File Name:	GameStateManager.h
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

#include "GameStateTable.h"

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

// Initialize the game state manager.
void GameStateManagerInit();

// Update the game state manager.
void GameStateManagerUpdate(float dt);

// Shutdown the game state manager.
void GameStateManagerShutdown();

// Determine if the current game state is being restarted.
bool GameStateManagerIsRestarting();

// Determine if the game is still running.
bool GameStateManagerIsRunning();

// Set the next game state to run.
void GameStateManagerSetNextState(GameStates nextState);

GameStates GetPrevGS();
GameStates GetCurrentGS();
GameStates GetNextGS();

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

