//------------------------------------------------------------------------------
//
// File Name:	Player Brush.c
// Author(s):	Rishabh Anand
// Project:		Project Color
// Course:		GAM150
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Vector2D.h"
#include "AEEngine.h"
#include "Brush.h"
#include "Paint.h"
#include "GameObject.h"
#include "Transform.h"
#include "AABB.h"
#include "SpriteSource.h"
#include "Sprite.h"
#include "Player.h"
#include "Player Controller.h"
#include "Animation.h"
#include "Mesh.h"
#include "EnemyAI.h"
#include "PauseScreen.h"

#define _USE_MATH_DEFINES
#include <math.h>

float angle;
float swipeStartAngle;
float swipeEndAngle;
float swipeLength = 70.0f;								// To be added to initial angle to find swipeStartAngle
float playerHeightOffset = 15.0f;						// Raise centerpoint of brush for rotating
float angleOffset = 0.0f;								// Offset so paintBrush points at mouse
float brushTransformScale = 120.0f;
float prevAngle = 0.0f;
float brushSize = 5.0f;

bool swipingBrush = false;
bool swipingDown = false;
bool swipeSide = false;									// true = right, false = left
bool swipeReady = true;
bool isCharging = false;

bool isHittingEnemy = false;

bool isHittingEnemyOnDownSwipe = false;

static AEGfxVertexList* meshes[4];
static AEGfxTexture* textures[4];
static SpriteSourcePtr spriteSources[4];

void LoadBrush(void)
{
	meshes[0] = MeshCreateQuad(0.5F, 0.5F, 1.0F, 1.0F, "One By One Brush");
	meshes[1] = MeshCreateQuad(0.5F, 0.5F, 1.0F / 4, 1.0F, "Four By One Brush");
	meshes[2] = MeshCreateQuad(-0.5F, 0.5F, 1.0F, 1.0F, "One By One Brush Reverse");
	meshes[3] = MeshCreateQuad(-0.5F, 0.5F, 1.0F / 4, 1.0F, "Four By One Brush Reverse");

	textures[0] = AEGfxTextureLoad("./Assets/Player/brush_back.png");
	textures[1] = AEGfxTextureLoad("./Assets/Player/brush_idle.png");
	textures[2] = AEGfxTextureLoad("./Assets/Player/brush_swing.png");
	textures[3] = AEGfxTextureLoad("./Assets/Player/brush_charge.png");

	spriteSources[0] = SpriteSourceCreate(1, 1, textures[0]);
	spriteSources[1] = SpriteSourceCreate(1, 1, textures[1]);
	spriteSources[2] = SpriteSourceCreate(4, 1, textures[2]);
	spriteSources[3] = SpriteSourceCreate(1, 1, textures[3]);
}

void UnloadBrush(void)
{
	// Free textures and sprite sources.
	for (int i = 0; i < 4; ++i)
	{
		// Free texture.
		AEGfxTextureUnload(textures[i]);

		// Free sprite source.
		SpriteSourceFree(&spriteSources[i]);
	}

	// Free meshes.
	for (int i = 0; i < 4; ++i)
	{
		// Free mesh.
		AEGfxMeshFree(meshes[i]);
	}
}

void moveBrush(GameObjectPtr brushGameObject, GameObjectPtr playerGameObject, Vector2D brushTranslationOffset)
{
	if (!GetPlayer()->hasBrush || isCharging)
		return;

	const TransformPtr playerTransform = GameObjectGetTransform(playerGameObject);
	TransformPtr brushTransform = GameObjectGetTransform(brushGameObject);

	Vector2D newBrushTranslation = *(TransformGetTranslation(playerTransform));
	Vector2DAdd(&newBrushTranslation, &newBrushTranslation, &brushTranslationOffset);

	newBrushTranslation.y += playerHeightOffset;


	TransformSetTranslation(brushTransform, &newBrushTranslation);

	updateAABBPosition(GameObjectGetCollider(brushGameObject), GameObjectGetPhysics(brushGameObject));

}



float rotateBrush(GameObjectPtr brushGameObject, GameObjectPtr playerGameObject)
{
	if (!GetPlayer()->hasBrush || isCharging)
		return 0;

	if (!swipingBrush)
	{
		TransformPtr playerTransform = GameObjectGetTransform(playerGameObject);
		TransformPtr brushTransform = GameObjectGetTransform(brushGameObject);

		Vector2D brushTranslationOffset;

		float returnAngle = 0.0f;
		float mouseAngle = 0.0f;
		float currentAngle = AERadToDeg(TransformGetRotation(brushTransform));

		signed long mouseX, mouseY;											// Will store the mouse position (In the SCREEN coordinates system)
		float mouseInWorldX, mouseInWorldY;									// Will store the mouse position (In the WORLD coordinates system)
		float relativeMouseX, relativeMouseY;								// Will store the mouse position (In the RELATIVE coordinates system)

		float brushXPos = (TransformGetTranslation(playerTransform))->x;	// Get Transform x position of player
		float brushYPos = (TransformGetTranslation(playerTransform))->y;	// Get Transform y position of player

		AEInputGetCursorPosition(&mouseX, &mouseY);																			// Store Cursor position in appropriate variables
		AEGfxConvertScreenCoordinatesToWorld((float)mouseX, (float)mouseY, &(float)mouseInWorldX, &(float)mouseInWorldY);	// Convert to world coordinates

		relativeMouseX = mouseInWorldX - brushXPos;							// Convert to relative x
		relativeMouseY = mouseInWorldY - brushYPos;							// Convert to relative y

		mouseAngle = AERadToDeg(atan2f(relativeMouseY, relativeMouseX)) - angleOffset;

		returnAngle = currentAngle;
		
		if (prevAngle <= 135.0f && prevAngle >= 130 && mouseAngle <= -220 && mouseAngle >= -225)
		{
			returnAngle = prevAngle - 360.0f;
		}

		returnAngle = mouseAngle;
		
		brushTranslationOffset.x = cosf(AEDegToRad(returnAngle + angleOffset)) * brushTransformScale;
		brushTranslationOffset.y = sinf(AEDegToRad(returnAngle + angleOffset)) * brushTransformScale;

		
		moveBrush(brushGameObject, playerGameObject, brushTranslationOffset);

		prevAngle = mouseAngle;

		return returnAngle;
	}
	else
	{
		return 0.0f;
	}
}



EnemyPtr BrushCollidingWithEnemy(GameObjectPtr brushGameObject)
{
	AABB brushCollider = GameObjectGetCollider(brushGameObject);

	for (int i = 0; i < MaxSpawnedEnemies; ++i)
	{
		EnemyPtr enemy = GetEnemyIndex(i);

		if (enemy)
		{
			AABB enemyCollider = GameObjectGetCollider(enemy->gameObject);

			if (enemy && CheckAABBCollision(brushCollider, enemyCollider))	//if brush is hitting enemy
			{
				AESysPrintf("BRUH\n");
				isHittingEnemy = true;
				return enemy;
			}
		}
	}

	return NULL;
}

bool BrushCollidingWithEnemyOnDownswing()
{
	if (isHittingEnemyOnDownSwipe)
	{
		isHittingEnemyOnDownSwipe = false;
		return true;
	}
	else
	{
		return false;
	}
}



void swipeBrush(GameObjectPtr brushGameObject, GameObjectPtr playerGameObject, float* tempAngle)
{
	isHittingEnemyOnDownSwipe = false;

	if (!GetPlayer()->hasBrush || isCharging)
		return;

	if (AEInputCheckReleased(VK_LBUTTON) && swipeReady)
	{
		swipeEndAngle = *tempAngle;

		if ((swipeEndAngle >= 45.0f && swipeEndAngle < 225.0f) || (swipeEndAngle >= -225.0f && swipeEndAngle < -135.0f))
		{
			swipeSide = false;
			swipeStartAngle = *tempAngle - 70.0f;
		}
		else
		{
			swipeSide = true;
			swipeStartAngle = *tempAngle + 70.0f;
		}

		swipingBrush = true;
		swipeReady = false;
	}

	if (swipingBrush)
	{
		BrushAnimate(FOUR_BY_ONE_BRUSH, SWING_BRUSH, 4, 0.1F, false);

		Vector2D brushTranslationOffset;
		brushTranslationOffset.x = cosf(AEDegToRad(*tempAngle + angleOffset)) * brushTransformScale;
		brushTranslationOffset.y = sinf(AEDegToRad(*tempAngle + angleOffset)) * brushTransformScale;

		if (swipeSide)
		{
			if (*tempAngle < swipeStartAngle && swipingDown == false)
			{
				*tempAngle += 4.0f;
			}
			else
			{
				swipingDown = true;
				*tempAngle -= 8.0f;
				
				if (*tempAngle <= swipeEndAngle)
				{
					EnemyPtr enemy = BrushCollidingWithEnemy(brushGameObject);

					if (enemy)
					{
						isHittingEnemyOnDownSwipe = true;
					}

					swipingBrush = false;
					swipingDown = false;
					swipeReady = true;
				}
			}
		}
		else if (!swipeSide)
		{
			if (*tempAngle > swipeStartAngle && swipingDown == false)
			{
				*tempAngle -= 4.0f;
			}
			else
			{
				swipingDown = true;
				*tempAngle += 8.0f;

				if (*tempAngle >= swipeEndAngle)
				{
					EnemyPtr enemy = BrushCollidingWithEnemy(brushGameObject);

					if (enemy)
					{
						isHittingEnemyOnDownSwipe = true;
					}

					swipingBrush = false;
					swipingDown = false;
					swipeReady = true;
				}
			}
		}

		moveBrush(brushGameObject, playerGameObject, brushTranslationOffset);
	}
}



void BrushUpdate(GameObjectPtr brushGameObject, GameObjectPtr playerGameObject)
{
	if (!GetPlayer()->hasBrush || GetPaused())
		return;

	TransformPtr brushRotation = GameObjectGetTransform(brushGameObject);
	Vector2D brushPosition = *TransformGetTranslation(brushRotation);

	if (!swipingBrush && !AEInputCheckReleased(VK_LBUTTON))
	{
		angle = rotateBrush(brushGameObject, playerGameObject);

		BrushAnimate(ONE_BY_ONE_BRUSH, IDLE_BRUSH, 1, 1.0F, true);
		brushSize = 5;
	}
	else
	{
		swipeBrush(brushGameObject, playerGameObject, &angle);
		brushSize += (float)AEFrameRateControllerGetFrameTime() * 7.5F;
	}

	TransformSetRotation(brushRotation, ((angle * (float)M_PI) / 180.0f));

	brushPosition.x += 32;
	brushPosition.y += 32;

	if ((AEInputCheckCurr(VK_LBUTTON) || swipingBrush) && AEFrameRateControllerGetFrameCount() % 2)
	{
		// Translate it to a pixel position.
		unsigned int pixelX = (unsigned int)((brushPosition.x + 768) / PIXEL_SIZE);
		unsigned int pixelY = (unsigned int)((brushPosition.y + 448) / PIXEL_SIZE);

		// Paint that part of the screen.
		SetBrushSize((unsigned char)brushSize);
		ApplyPaintToLocation(pixelX, pixelY);
	}

	// Let the user recharge the paintbrush.
	if (AEInputCheckCurr(VK_RBUTTON))
	{
		// Recharge paint.
		Recharge();
		isCharging = true;

		BrushAnimate(ONE_BY_ONE_BRUSH, CHARGE_BRUSH, 1, 1.0F, false);

		Vector2D playerPos = *TransformGetTranslation(GameObjectGetTransform(playerGameObject));
		playerPos.x += 38;
		playerPos.y += 48;
		TransformSetTranslation(brushRotation, &playerPos);
	}
	else
	{
		isCharging = false;
	}
}

void BrushAnimate(enum BrushMesh mesh, enum BrushState state, int frameCount, float frameDuration, bool loop)
{
	if (SpriteGetMesh(GameObjectGetSprite(GetBrush())) == meshes[mesh] ||
		SpriteGetSpriteSource(GameObjectGetSprite(GetBrush())) == spriteSources[state])
		return;

	SpriteSetMesh(GameObjectGetSprite(GetBrush()), meshes[mesh]);
	SpriteSetSpriteSource(GameObjectGetSprite(GetBrush()), spriteSources[state]);
	AnimationPlay(GameObjectGetAnimation(GetBrush()), frameCount, frameDuration, loop);
}

void UnlockBrush(void)
{
	if (GetPlayer()->hasBrush)
		return;

	GetPlayer()->hasBrush = true;
}
