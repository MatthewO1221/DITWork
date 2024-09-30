//------------------------------------------------------------------------------
//
// File Name:	GameStateTable.c
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

/* Include all game state headers here. */
#include "GameStateTitle.h"
#include "GameStateMain.h"
#include "GameStateCredits.h"
#include "GameStateEditor.h"
#include "GameStateTest.h"
#include "GameStateTestDerek.h"
#include "GameStateTestMatthew.h"
#include "GameStateTestBrody.h"
#include "GameStateTestRishabh.h"
#include "GameStateTestEmily.h"
#include "GameStateSplash.h"
#include "GameStateMenuTest.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef void(*GameStateVoidFunctionPtr)();
typedef void(*GameStateDtFunctionPtr)(float dt);

typedef struct
{
  const char* gameStateName;		// The name of the game state as a zero-terminated string.
  GameStateVoidFunctionPtr	gameStateLoad;		/* Pointer to a game state load function. */
  GameStateVoidFunctionPtr	gameStateInit;		/* Pointer to a game state initialization function. */
  GameStateDtFunctionPtr		gameStateUpdate;	/* Pointer to a game state update function. */
  GameStateVoidFunctionPtr	gameStateShutdown;	/* Pointer to a game state shutdown function. */
  GameStateVoidFunctionPtr	gameStateUnload;	/* Pointer to a game state unload function. */
} GameStateTableEntry;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static const GameStateTableEntry GameStateTab[GsNum] =
{
  // Demo Game State.
  { "Title",	GameStateTitleLoad, GameStateTitleInit, GameStateTitleUpdate, GameStateTitleShutdown, GameStateTitleUnload },
  { "Main",	GameStateMainLoad, GameStateMainInit, GameStateMainUpdate, GameStateMainShutdown, GameStateMainUnload },
  { "Credits",GameStateCreditsLoad, GameStateCreditsInit, GameStateCreditsUpdate, GameStateCreditsShutdown, GameStateCreditsUnload },
  { "Editor",	GameStateEditorLoad, GameStateEditorInit, GameStateEditorUpdate, GameStateEditorShutdown, GameStateEditorUnload },
  { "Test",	GameStateTestLoad, GameStateTestInit, GameStateTestUpdate, GameStateTestShutdown, GameStateTestUnload },
  { "Test Derek",	GameStateTestDerekLoad, GameStateTestDerekInit, GameStateTestDerekUpdate, GameStateTestDerekShutdown, GameStateTestDerekUnload },
  { "Test Matthew",	GameStateTestMatthewLoad, GameStateTestMatthewInit, GameStateTestMatthewUpdate, GameStateTestMatthewShutdown, GameStateTestMatthewUnload },
  { "Test Brody",	GameStateTestBrodyLoad, GameStateTestBrodyInit, GameStateTestBrodyUpdate, GameStateTestBrodyShutdown, GameStateTestBrodyUnload },
  { "Test Rishabh",	GameStateTestRishabhLoad, GameStateTestRishabhInit, GameStateTestRishabhUpdate, GameStateTestRishabhShutdown, GameStateTestRishabhUnload },
  { "Test Emily",	GameStateTestEmilyLoad, GameStateTestEmilyInit, GameStateTestEmilyUpdate, GameStateTestEmilyShutdown, GameStateTestEmilyUnload },
  { "Splash",	GameStateSplashLoad, GameStateSplashInit, GameStateSplashUpdate, GameStateSplashShutdown, GameStateSplashUnload },
  { "Menu Test",	GameStateMenuTestLoad, GameStateMenuTestInit, GameStateMenuTestUpdate, GameStateMenuTestShutdown, GameStateMenuTestUnload },
};

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Gets the friendly name of a game state.
const char* GetGameStateName(GameStates gameState)
{
  return GameStateTab[gameState].gameStateName;
}

// Determine if the game state is valid.
bool GameStateIsValid(GameStates gameState)
{
  return ((0 <= gameState) && (gameState < GsNum));
}

// Determine if the game state is a "special" game state.
bool GameStateIsSpecial(GameStates gameState)
{
  return ((gameState == GsRestart) || (gameState == GsQuit));
}

// Execute the Load function for the current game state.
void GameStateExecuteLoad(GameStates gameState)
{
  // First validate the game state and the function pointer.
  if (GameStateIsValid(gameState) &&
    ((*GameStateTab[gameState].gameStateLoad) != NULL))
  {
    // Execute the Init function.
    (*GameStateTab[gameState].gameStateLoad)();
  }
}

// Execute the Init function for the current game state.
void GameStateExecuteInit(GameStates gameState)
{
  // First validate the game state and the function pointer.
  if (GameStateIsValid(gameState) &&
    ((*GameStateTab[gameState].gameStateInit) != NULL))
  {
    // Execute the Init function.
    (*GameStateTab[gameState].gameStateInit)();
  }
}

// Execute the Update function for the current game state.
void GameStateExecuteUpdate(GameStates gameState, float dt)
{
  // First validate the game state and the function pointer.
  if (GameStateIsValid(gameState) &&
    ((*GameStateTab[gameState].gameStateUpdate) != NULL))
  {
    // Execute the Update function.
    (*GameStateTab[gameState].gameStateUpdate)(dt);
  }
}

// Execute the Shutdown function for the current game state.
void GameStateExecuteShutdown(GameStates gameState)
{
  // First validate the game state and the function pointer.
  if (GameStateIsValid(gameState) &&
    ((*GameStateTab[gameState].gameStateShutdown) != NULL))
  {
    // Execute the Shutdown function.
    (*GameStateTab[gameState].gameStateShutdown)();
  }
}

// Execute the Unload function for the current game state.
void GameStateExecuteUnload(GameStates gameState)
{
  // First validate the game state and the function pointer.
  if (GameStateIsValid(gameState) &&
    ((*GameStateTab[gameState].gameStateUnload) != NULL))
  {
    // Execute the Init function.
    (*GameStateTab[gameState].gameStateUnload)();
  }
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
