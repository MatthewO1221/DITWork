//------------------------------------------------------------------------------
//
// File Name:	GameStateSplash.h
// Author(s):	Emily Comes (emily.comes)
// Project:		"Project Color"
// Course:		GAM-150-B Spring 2022
//
// Copyright (C) 2022 DigiPen (USA) Corporation.
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

// Load the resources associated with the Stub game state.
  void GameStateSplashLoad();

  // Initialize the memory associated with the Stub game state.
  void GameStateSplashInit();

  // Update the Stub game state.
  // Params:
  //	 dt = Change in time (in seconds) since the last game loop.
  void GameStateSplashUpdate(float dt);

  // Shutdown any memory associated with the Stub game state.
  void GameStateSplashShutdown();

  // Unload the resources associated with the Stub game state.
  void GameStateSplashUnload();

  /*----------------------------------------------------------------------------*/

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif
