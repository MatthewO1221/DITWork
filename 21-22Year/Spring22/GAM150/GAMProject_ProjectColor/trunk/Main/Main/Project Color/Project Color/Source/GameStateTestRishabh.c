//------------------------------------------------------------------------------
//
// File Name:	GameStateTestRishabh.c
// Author(s):	Rishabh Anand
// Project:		Project Color
// Course:		GAM150B
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "GameStateSwitcher.h"
#include "GameStateTestRishabh.h"
#include "Mesh.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "GameObject.h"
#include "Physics.h"
#include "Vector2D.h"
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

void GameStateTestRishabhLoad()
{
	pMesh = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "Mesh1x1");
	pTexture = AEGfxTextureLoad("./Assets/player.png");
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
void GameStateTestRishabhInit()
{
	// Change the window title.
	//AESysSetWindowTitle("Project Color - Test Rishabh");

	if (pMesh && pTexture && pSpriteSource)
	{
		planetGameObject = GameObjectCreate();

	}
	else
	{
		AESysPrintf("Something was not created\n");
	}

	if (planetGameObject)
	{
		TransformPtr pTransform = TransformCreate();
		GameObjectAddTransform(planetGameObject, pTransform);
		PhysicsPtr pPhysics = PhysicsCreate();
		GameObjectAddPhysics(planetGameObject, pPhysics);
		SpritePtr pSprite = SpriteCreate();
		GameObjectAddSprite(planetGameObject, pSprite);

		Vector2D spawnPos = { 0, -150 };
		TransformSetTranslation(pTransform, &spawnPos);
		TransformSetRotation(pTransform, 0);
		Vector2D scale = { 100, 139 };
		TransformSetScale(pTransform, &scale);

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



	pGameObjectSpaceship2 = GameObjectCreate();

	if (pGameObjectSpaceship2)
	{
		TransformPtr sTransform = TransformCreate();
		GameObjectAddTransform(pGameObjectSpaceship2, sTransform);
		PhysicsPtr sPhysics = PhysicsCreate();
		GameObjectAddPhysics(pGameObjectSpaceship2, sPhysics);
		SpritePtr sSprite = SpriteCreate();
		GameObjectAddSprite(pGameObjectSpaceship2, sSprite);

		Vector2D spawnPos = { 0, 0 };
		TransformSetTranslation(sTransform, &spawnPos);
		TransformSetRotation(sTransform, 0);
		Vector2D scale = { 100, 80 };
		TransformSetScale(sTransform, &scale);

		SpriteSetMesh(GameObjectGetSprite(pGameObjectSpaceship2), pMeshSpaceship);
	}
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

// Update the ...
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateTestRishabhUpdate(float dt)
{

	UNREFERENCED_PARAMETER(dt);

	PlayerUpdate(0.0f, 0.0f, NULL, dt);

	GameObjectUpdate(planetGameObject, dt);
	GameObjectUpdate(pGameObjectSpaceship2, dt);

	GameObjectDraw(planetGameObject);
	GameObjectDraw(pGameObjectSpaceship2);

	// Let the user toggle between gamestates.
	ToggleGameState();
}

// Shutdown the ...
void GameStateTestRishabhShutdown()
{
	GameObjectFree(&planetGameObject);
}

void GameStateTestRishabhUnload()
{
	free(pSpriteSource);
	AEGfxMeshFree(pMesh);
	AEGfxTextureUnload(pTexture);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

