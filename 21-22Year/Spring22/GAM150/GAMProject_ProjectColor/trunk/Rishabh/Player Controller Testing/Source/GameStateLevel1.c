//------------------------------------------------------------------------------
//
// File Name:	Stub.c
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
#include "GameStateLevel1.h"
#include "Transform.h"
#include "Player Movement.h"
#include "Player Controller.h"
#include "Brush.h"

//------------------------------------------------------------------------------
// Private Constants:

static const float groundHeight = -150.0f;
static const float moveVelocity = 500.0f;
static const float jumpVelocity = 1000.0f;
static const Vector2D gravityNormal = { 0.0f, -1500.0f };
static const Vector2D gravityNone = { 0.0f, 0.0f };

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:

static int numLives1 = 0;
static char* livesFilename = "Data/Level1_Lives.txt";
static char* planetFilename = "Assets/PlanetTexture.png";
static char* planetJumpFilename = "Data/PlanetJump.txt";
char* spaceShipHoming2 = "Data/SpaceshipHoming.txt";
static Stream lives1;
static AEGfxVertexList* pMesh;
static AEGfxVertexList* pMeshSpaceship;
static AEGfxTexture* pTexture;
static SpriteSourcePtr pSpriteSource;
static GameObjectPtr planetGameObject;
static GameObjectPtr pGameObjectSpaceship2;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//----------------------------------------------------w--------------------------

//------------------------------------------------------------------------------
// Public Functions:
//-----------------------------------------------------------------------------
/*
void GameStateLevel1MovementController(GameObjectPtr gameObject)
{
	PhysicsPtr pPhysics = GameObjectGetPhysics(gameObject);
	TransformPtr pTransform = GameObjectGetTransform(gameObject);

	if (pPhysics && pTransform)
	{
		Vector2D velocity = *(PhysicsGetVelocity(pPhysics));
		Vector2D translation = *(TransformGetTranslation(pTransform));

		if (AEInputCheckCurr(VK_LEFT))
		{
			velocity.x = -moveVelocity;
			PhysicsSetVelocity(pPhysics, &velocity);
		}
		else if (AEInputCheckCurr(VK_RIGHT))
		{
			velocity.x = moveVelocity;
			PhysicsSetVelocity(pPhysics, &velocity);
		}
		else
		{
			velocity.x = 0;
			PhysicsSetVelocity(pPhysics, &velocity);
		}

		if (AEInputCheckTriggered(VK_UP))
		{
			velocity.y = jumpVelocity;
			PhysicsSetVelocity(pPhysics, &velocity);
			PhysicsSetAcceleration(pPhysics, &gravityNormal);
		}

		if (translation.y < groundHeight)
		{
			translation.y = groundHeight;
			velocity.y = 0;
			TransformSetTranslation(pTransform, &translation);
			PhysicsSetVelocity(pPhysics, &velocity);
			PhysicsSetAcceleration(pPhysics, &gravityNone);
			numLives1--;
		}

		if (numLives1 <= 0)
		{
			GameStateManagerSetNextState(GsLevel2);
		}
	}
}
*/

void GameStateLevel1Load()
{
	AESysPrintf("GSL1RUN\n");

	lives1 = StreamOpen(livesFilename);
	if (lives1 != NULL)
	{
		AESysPrintf("Lives Read Succesfully\n");
		numLives1 = StreamReadInt(lives1);
		AESysPrintf("Lives: %i\n", numLives1);
		StreamClose(&lives1);
	}

	pMesh = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "Mesh1x1");
	pTexture = AEGfxTextureLoad(planetFilename);
	pSpriteSource = SpriteSourceCreate(1, 1, pTexture);



	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);
	pMeshSpaceship = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMeshSpaceship, "Failed to create spaceship mesh!");
}

// Initialize the ...
void GameStateLevel1Init()
{
	if (pMesh && pTexture && pSpriteSource)
	{
		planetGameObject = GameObjectFactoryBuild(planetJumpFilename);
	}
	else
	{
		AESysPrintf("Something was not created\n");
	}

	if (planetGameObject)
	{
		SpriteSetMesh(GameObjectGetSprite(planetGameObject), pMesh);
		SpriteSetSpriteSource(GameObjectGetSprite(planetGameObject), pSpriteSource);
		SpriteSetFrame(GameObjectGetSprite(planetGameObject), 0);
		SpriteSetAlpha(GameObjectGetSprite(planetGameObject), 1.0f);

		//GameObjectAddSprite(planetGameObject, GameObjectGetSprite(planetGameObject));
	}
	else
	{
		AESysPrintf("planetGameObject Failed to be created\n");
	}

	AEGfxSetBackgroundColor(1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);



	pGameObjectSpaceship2 = GameObjectFactoryBuild(spaceShipHoming2);

	if (pGameObjectSpaceship2)
	{
		SpriteSetMesh(GameObjectGetSprite(pGameObjectSpaceship2), pMeshSpaceship);
	}
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

// Update the ...
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateLevel1Update(float dt)
{

	UNREFERENCED_PARAMETER(dt);
	
	PlayerMovementUpdate(planetGameObject);
	BrushUpdate(pGameObjectSpaceship2, 0.0f, 0.0f);
	
	GameObjectUpdate(planetGameObject, dt);
	GameObjectUpdate(pGameObjectSpaceship2, dt);

	GameObjectDraw(planetGameObject);
	GameObjectDraw(pGameObjectSpaceship2);

	if (AEInputCheckTriggered('1'))
	{
		GameStateManagerSetNextState(GsRestart);
	}
	else if (AEInputCheckTriggered('2'))
	{
		GameStateManagerSetNextState(GsLevel2);
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
void GameStateLevel1Shutdown()
{
	GameObjectFree(&planetGameObject);
}

void GameStateLevel1Unload()
{
	free(pSpriteSource);
	AEGfxMeshFree(pMesh);
	AEGfxTextureUnload(pTexture);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

