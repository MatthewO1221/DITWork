//------------------------------------------------------------------------------
//
// File Name:	GameStateLevel2.c
// Author(s):	Rishabh Anand
// Project:		Project 0
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Stub.h"
#include "GameStateManager.h"
#include "GameStateTable.h"
#include "AEEngine.h"
#include "Stream.h"
#include "Trace.h"
#include "Mesh.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Physics.h"
#include "Vector2D.h"
#include "GameStateLevel2.h"
#include "Transform.h"

//------------------------------------------------------------------------------
// Private Constants:

static const float spaceshipSpeed = 500.0f;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//static int numLives = 0;
//static int numHealth = 0;
static AEGfxVertexList* pMeshSpaceship;
static GameObjectPtr pGameObjectSpaceship;
char* livesFilename = "Data/Level2_Lives.txt";
char* spaceShipHoming = "Data/SpaceshipHoming.txt";
//char* healthFilename = "Data/Level2_Health.txt";
Stream lives2;
//Stream health2;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void GameStateLevel2MovementController(GameObjectPtr gameObject)
{
	PhysicsPtr pPhysics = GetPhysicsGameObject(gameObject);
	TransformPtr pTransform = GetTransformGameObject(gameObject);
	Vector2D currentTranslation = *(GetTranslationTransform(pTransform));
	Vector2D mousePosition;
	Vector2D directionVector;
	float angle;

	if (pPhysics && pTransform)
	{
		long cursorX;
		long cursorY;
		float worldX;
		float worldY;
		AEInputGetCursorPosition(&cursorX, &cursorY);
		AEGfxConvertScreenCoordinatesToWorld((float)cursorX, (float)cursorY, &worldX, &worldY);

		Vector2DSet(&mousePosition, worldX, worldY);
		Vector2DSub(&directionVector, &mousePosition, &currentTranslation);
		Vector2DNormalize(&directionVector, &directionVector);
		Vector2DScale(&directionVector, &directionVector, spaceshipSpeed);

		angle = Vector2DToAngleRad(&directionVector);

		SetRotationTransform(pTransform, angle);
		SetVelocityPhysics(pPhysics, &directionVector);
	}
}

void GameStateLevel2Load()
{
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);
	pMeshSpaceship = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMeshSpaceship, "Failed to create spaceship mesh!");
}

// Initialize the ...
void GameStateLevel2Init()
{
	pGameObjectSpaceship = GameObjectFactoryBuild(spaceShipHoming);

	if (pGameObjectSpaceship)
	{
		SpriteSetMesh(GameObjectGetSprite(pGameObjectSpaceship), pMeshSpaceship);
	}

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

// Update the ...
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateLevel2Update(float dt)
{
	if (AEInputCheckTriggered('Z'))
	{
		SpriteSetAlpha(GameObjectGetSprite(pGameObjectSpaceship), 0.5f);
	}
	else if (AEInputCheckTriggered('X'))
	{
		SpriteSetAlpha(GameObjectGetSprite(pGameObjectSpaceship), 1.0f);
	}

	GameStateLevel2MovementController(pGameObjectSpaceship);
	GameObjectUpdate(pGameObjectSpaceship, dt);
	GameObjectDraw(pGameObjectSpaceship);

	if (AEInputCheckTriggered('1'))
	{
		GameStateManagerSetNextState(GsLevel1);
	}
	else if (AEInputCheckTriggered('2'))
	{
		GameStateManagerSetNextState(GsRestart);
	}
	else if (AEInputCheckTriggered('9'))
	{
		GameStateManagerSetNextState(GsSandbox);
	}
	else if (AEInputCheckTriggered('0'))
	{
		GameStateManagerSetNextState(GsDemo);
	}
}

// Shutdown the ...
void GameStateLevel2Shutdown()
{
	GameObjectFree(&pGameObjectSpaceship);
}

void GameStateLevel2Unload()
{
	AEGfxMeshFree(pMeshSpaceship);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

