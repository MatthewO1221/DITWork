//------------------------------------------------------------------------------
//
// File Name:	Main.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 0
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <windows.h>
#include "AEEngine.h"

#include "Engine.h"
#include "GameStateManager.h"
#include "System.h"

//------------------------------------------------------------------------------
// Main Function:
//------------------------------------------------------------------------------

int WINAPI WinMain(_In_ HINSTANCE instanceH, _In_opt_ HINSTANCE prevInstanceH, _In_ LPSTR command_line, _In_ int show)
{
	UNREFERENCED_PARAMETER(prevInstanceH);
	UNREFERENCED_PARAMETER(command_line);

	// Store the Windows settings passed in WinMain.
	SystemStoreWindowSettings(instanceH, show);

	// Initialize the game engine.
	EngineInit();

	// MAIN LOOP: Do not exit until instructed to do so by the game state manager (GSM).
	while (GameStateManagerIsRunning())
	{
		// Change in time (in seconds) since the last game loop.
		float dt = (float)AEFrameRateControllerGetFrameTime();

		// Clamping prevents excessive dt values due to long load times or pausing execution in a debugger.
		dt = AEClamp(dt, 0.0f, 0.1f);

		// Update the game engine.
		EngineUpdate(dt);
	}

	// Shutdown the game engine.
	EngineShutdown();

	return 0;
}