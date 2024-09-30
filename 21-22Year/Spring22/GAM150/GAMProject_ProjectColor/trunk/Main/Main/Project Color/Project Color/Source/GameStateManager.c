//------------------------------------------------------------------------------
//
// File Name:	GameStateManager.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 0
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateTable.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct
{
	GameStates	current;	/* Index of the current game state. */
	GameStates	previous;	/* Index of the previous game state. */
	GameStates	next;		/* Index of the next game state. */

} GameState;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static GameState gameState;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static bool	GameStateIsChanging();

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the game state manager.
void GameStateManagerInit()
{
	// Set the previous and current game states to invalid.
	gameState.previous = GsInvalid;
	gameState.current = GsInvalid;

	// Set the initial game state to launch the GetGame()->
	gameState.next = GsInitial;
}

// Update the game state manager.
void GameStateManagerUpdate(float dt)
{
	// Check for a game state change.
	if (GameStateIsChanging())
	{
		// Shutdown the current game state.
		GameStateExecuteShutdown(gameState.current);

		if (GameStateManagerIsRestarting())
		{
			// Clear restart.
			gameState.next = gameState.current;
		}
		else
		{
			// Unload current game state.
			GameStateExecuteUnload(gameState.current);

			// Update the recorded states.
			gameState.previous = gameState.current;
			gameState.current = gameState.next;

			// Load new game state.
			GameStateExecuteLoad(gameState.current);
		}

		// Initialize the new game state.
		GameStateExecuteInit(gameState.current);
	}

	// Update the current game state.
	GameStateExecuteUpdate(gameState.current, dt);
}

// Shutdown the game state manager.
void GameStateManagerShutdown()
{
	
}

// Determine if the current game state is being restarted.
bool GameStateManagerIsRestarting()
{
	return gameState.next == GsRestart;
}

// Determine if the game is still running.
bool GameStateManagerIsRunning()
{
	return gameState.current != GsQuit;
}

// Set the next game state to run.
void GameStateManagerSetNextState(GameStates nextState)
{
	if (GameStateIsValid(nextState) || GameStateIsSpecial(nextState))
	{
		gameState.next = nextState;
	}
	else
	{
#ifdef _DEBUG
		/* In debug mode, print a message that an error has occurred. */
		printf("\nERROR: Invalid game state: %d\n", nextState);
#endif
	}
}

GameStates GetPrevGS()
{
	return gameState.previous;
}

GameStates GetCurrentGS()
{
	return gameState.current;
}

GameStates GetNextGS()
{
	return gameState.next;
}

/*------------------------------------------------------------------------------
// Private Functions:
//----------------------------------------------------------------------------*/

// Determine if a game state change has been requested.
static bool	GameStateIsChanging()
{
	return (gameState.current != gameState.next);
}
