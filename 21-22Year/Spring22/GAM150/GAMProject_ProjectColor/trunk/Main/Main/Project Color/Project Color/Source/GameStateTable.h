//------------------------------------------------------------------------------
//
// File Name:	GameStateTable.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 0
// Course:		CS230S22
//
// Copyright � 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

  typedef enum
  {
    /* Special Game States */
    GsQuit = -3,
    GsRestart = -2,
    GsInvalid = -1,

    /* Normal Game States: */
    GsTitle,
    GsMain,
    GsCredits,
    GsEditor,
    GsTest,
    GsTestDerek,
    GsTestMatthew,
    GsTestBrody,
    GsTestRishabh,
    GsTestEmily,
    GsSplash,
    GsMenuTest,

    /* Bookkeeping States: */
    GsNum,
    GsLast = GsNum - 1,

    /* Specify the initial game state */
    GsInitial = GsSplash,
  } GameStates;

  //------------------------------------------------------------------------------
  // Public Structures:
  //------------------------------------------------------------------------------

  //------------------------------------------------------------------------------
  // Public Variables:
  //------------------------------------------------------------------------------

  //------------------------------------------------------------------------------
  // Public Function Declarations:
  //------------------------------------------------------------------------------

  // Gets the friendly name of a game state.
  const char* GetGameStateName(GameStates gameState);

  // Determine if the game state is valid.
  bool GameStateIsValid(GameStates gameState);

  // Determine if the game state is a "special" game state.
  bool GameStateIsSpecial(GameStates gameState);

  // Execute the Load function for the current game state.
  void GameStateExecuteLoad(GameStates gameState);

  // Execute the Init function for the current game state.
  void GameStateExecuteInit(GameStates gameState);

  // Execute the Update function for the current game state.
  void GameStateExecuteUpdate(GameStates gameState, float dt);

  // Execute the Shutdown function for the current game state.
  void GameStateExecuteShutdown(GameStates gameState);

  // Execute the Unload function for the current game state.
  void GameStateExecuteUnload(GameStates gameState);

  //------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
