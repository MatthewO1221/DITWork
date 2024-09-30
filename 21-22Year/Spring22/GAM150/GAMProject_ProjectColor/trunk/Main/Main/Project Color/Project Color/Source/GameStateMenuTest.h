//------------------------------------------------------------------------------
//
// File Name:	GameStateStub.h
// Author(s):	Derek Herrera-Sturm
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
	enum buffer_index {
		_title,
		_winScale,
		_resetConfig,
		_loadState, //this one should be removed before release
		_volMasterAdjust,
		_volMusicAdjust,
		_volSFXAdjust,
		_titleReturn,
	};
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Stub game state.
  void GameStateMenuTestLoad();

  // Initialize the memory associated with the Stub game state.
  void GameStateMenuTestInit();

  // Update the Stub game state.
  // Params:
  //	 dt = Change in time (in seconds) since the last game loop.
  void GameStateMenuTestUpdate(float dt);

  // Shutdown any memory associated with the Stub game state.
  void GameStateMenuTestShutdown();

  // Unload the resources associated with the Stub game state.
  void GameStateMenuTestUnload();

  /*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
