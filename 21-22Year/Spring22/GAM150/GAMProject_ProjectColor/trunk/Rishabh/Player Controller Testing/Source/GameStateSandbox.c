//------------------------------------------------------------------------------
//
// File Name:	GameStateSandbox.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 0
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateSandbox.h"
#include "Vector2D.h"
#include "Stream.h"
#include "Trace.h"
#include "AEEngine.h"

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
static char* filename = "Data/VectorTests.txt";
Stream vectors;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

static void GameStateSandboxTraceFloat(const char* text, float f)
{
	TraceMessage("Vector Test: %s = %f", text, f);
}

static void GameStateSandboxTraceVector(const char* text, const Vector2D* v)
{
	TraceMessage("Vector Test: %s = [%f, %f]", text, v->x, v->y);
}

// Load the resources associated with the Sandbox game state.
void GameStateSandboxLoad()
{

}

// Initialize the memory associated with the Sandbox game state.
void GameStateSandboxInit()
{

}

// Update the Sandbox game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateSandboxUpdate(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);


	if (AEInputCheckTriggered('1'))
	{
		GameStateManagerSetNextState(GsLevel1);
	}
	else if (AEInputCheckTriggered('2'))
	{
		GameStateManagerSetNextState(GsLevel2);
	}
	else if (AEInputCheckTriggered('9'))
	{
		GameStateManagerSetNextState(GsRestart);
	}
	else if (AEInputCheckTriggered('0'))
	{
		GameStateManagerSetNextState(GsDemo);
	}

	
	vectors = StreamOpen(filename);
	if (vectors != NULL)
	{
		Vector2D tester;
		Vector2D v1;
		Vector2D v2;
		float scale;
		float angle;
		float radians;
		Vector2DZero(&tester);
		GameStateSandboxTraceVector("Vector2DZero", &tester);

		Vector2DSet(&tester, 1.0f, 1.5f);
		GameStateSandboxTraceVector("Vector2DSet", &tester);

		Vector2DNeg(&tester, &tester);
		GameStateSandboxTraceVector("Vector2DNeg", &tester);

		Vector2DAdd(&tester, &tester, &tester);
		GameStateSandboxTraceVector("Vector2DAdd", &tester);

		Vector2DSub(&tester, &tester, &tester);
		GameStateSandboxTraceVector("Vector2DSub", &tester);

		StreamReadVector2D(vectors, &tester);
		GameStateSandboxTraceVector("v", &tester);

		Vector2DNormalize(&tester, &tester);
		GameStateSandboxTraceVector("Vector2DNormalize", &tester);
		
		scale = StreamReadFloat(vectors);
		GameStateSandboxTraceFloat("scale", scale);

		Vector2DScale(&tester, &tester, scale);
		GameStateSandboxTraceVector("Vector2DScale", &tester);

		Vector2DScaleAdd(&tester, &tester, &tester, scale);
		GameStateSandboxTraceVector("Vector2DScaleAdd", &tester);

		Vector2DScaleSub(&tester, &tester, &tester, scale);
		GameStateSandboxTraceVector("Vector2DScaleSub", &tester);

		GameStateSandboxTraceFloat("Vector2DLength", Vector2DLength(&tester));

		GameStateSandboxTraceFloat("Vector2DSquareLength", Vector2DSquareLength(&tester));
		


		StreamReadVector2D(vectors, &v1);
		GameStateSandboxTraceVector("p1", &v1);

		StreamReadVector2D(vectors, &v2);
		GameStateSandboxTraceVector("p2", &v2);

		GameStateSandboxTraceFloat("Vector2DDistance", Vector2DDistance(&v1, &v2));

		GameStateSandboxTraceFloat("Vector2DSquareDistance", Vector2DSquareDistance(&v1, &v2));
		
		GameStateSandboxTraceFloat("Vector2DDotProduct", Vector2DDotProduct(&v1, &v2));



		angle = StreamReadFloat(vectors);
		GameStateSandboxTraceFloat("deg", angle);

		Vector2DFromAngleDeg(&tester, angle);
		GameStateSandboxTraceVector("Vector2DFromAngleDeg", &tester);

		radians = StreamReadFloat(vectors);
		GameStateSandboxTraceFloat("rad", radians);

		Vector2DFromAngleRad(&tester, radians);
		GameStateSandboxTraceVector("Vector2DFromAngleRad", &tester);

		GameStateSandboxTraceFloat("Vector2DToAngleRad", Vector2DToAngleRad(&tester));


		fclose(vectors);
	}


	//GameStateManagerSetNextState(GsQuit);
	
}

// Free any memory associated with the Sandbox game state.
void GameStateSandboxShutdown()
{
	// Free all objects.
}

// Unload the resources associated with the Sandbox game state.
void GameStateSandboxUnload()
{
	// Free all sprite sources.

	// Unload all textures.

	// Free all meshes.
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

