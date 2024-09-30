//------------------------------------------------------------------------------
//
// File Name:	Player Movement.c
// Author(s):	Rishabh Anand
// Project:		Project Color
// Course:		CS230S20
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Vector2D.h"
#include "AEEngine.h"
#include "GameObject.h"
#include "Transform.h"
#include "Physics.h"
#include "AABB.h"
#include "Level.h"
#include "Sprite.h"
#include "Debug.h"
#include "Animation.h"
#include "Mesh.h"
#include "SpriteSource.h"
#include "Player Movement.h"
#include "Player Controller.h"
#include "Player.h"
#include "GameplayInstance.h"
#include "Tile.h"
#include "Brush.h"
#include "Paint.h"
#include "Audio.h"
#include "PauseScreen.h"

static int animDirection = 1;

static const float moveVelocity = 400.0f;						//max velocity when walking
static const float wallJumpVelocity = 750.0f;
static float acceleration = 20.0f;						//for accelerating player
static float decceleration = 40.0f;						//drag value
static const float lowSpeedCap = 50.0f;							//prevents jitter at low speed in move functons
static const float sprintVelocity = 600.0f;						//max velocity when sprinting
static const float jumpVelocity = 750.0f;						//vertical velocity at start of jump
static Vector2D gravityDefault = { 0.0f, -1500.0f };
static const Vector2D gravityNone = { 0.0f, 0.0f };
static const Vector2D wallFriction = { 0.0f, -750.0f };
static Vector2D gravityNormal = { 0.0f, -1500.0f };

float timeSinceGround = 0.0f;
float timeSinceWallL = 0.0f;
float timeSinceWallR = 0.0f;

bool isInWater = false;

bool isJumping = false;
bool isColliding = false;
bool hittingCeiling = false;
bool isTakingDamage = false;
bool jumpSound = false;

bool isOveridingVelocity = false;

int collisionDirection = 0;

int f = 0;

void MoveLeft(Vector2D* velocity)
{
	float maxVelocity = 0.0f;									//velocity cap

	bool direction = (int)gravityNormal.x == 0;

	switch (direction)
	{
		case true:
			if (collisionDirection == -1)
			{
				maxVelocity = 0.0f;
			}
			else if (AEInputCheckCurr('A') || AEInputCheckCurr(VK_LEFT))
			{
				animDirection = -1;
				velocity->x -= acceleration;

				if (AEInputCheckCurr(VK_LSHIFT))						//sprint check
				{
					maxVelocity = -sprintVelocity;

					if (velocity->y == 0)
					{
						PlayerAnimate(SIX_BY_ONE_REVERSE, RUN, 6, 0.05F, true);
					}
				}
				else													//set normal velocity
				{
					maxVelocity = -moveVelocity;

					if (velocity->y == 0)
					{
						PlayerAnimate(FOUR_BY_ONE_REVERSE, WALK, 4, 0.1F, true);
					}
				}

			}
			else if (velocity->x > -lowSpeedCap && velocity->x <= 0.0f)		//prevent jittering at low speeds
			{
				maxVelocity = 0.0f;
			}
			else if (velocity->x < 0.0f)								//drag when left isnt pressed
			{
				maxVelocity = velocity->x + decceleration;
			}



			if (maxVelocity >= velocity->x)								//cap velocity based on sprinting
			{
				velocity->x = maxVelocity;
			}
			break;

		case false:
			if (collisionDirection == -1)
			{
				maxVelocity = 0.0f;
			}
			else if (AEInputCheckCurr('A') || AEInputCheckCurr(VK_LEFT))
			{
				animDirection = -1;
				velocity->y -= acceleration;

				if (AEInputCheckCurr(VK_LSHIFT))						//sprint check
				{
					maxVelocity = -sprintVelocity;

					if (velocity->x == 0)
					{
						PlayerAnimate(SIX_BY_ONE_REVERSE, RUN, 6, 0.05F, true);
					}
				}
				else													//set normal velocity
				{
					maxVelocity = -moveVelocity;

					if (velocity->x == 0)
					{
						PlayerAnimate(FOUR_BY_ONE_REVERSE, WALK, 4, 0.1F, true);
					}
				}

			}
			else if (velocity->y > -lowSpeedCap && velocity->y <= 0.0f)		//prevent jittering at low speeds
			{
				maxVelocity = 0.0f;
			}
			else if (velocity->y < 0.0f)								//drag when left isnt pressed
			{
				maxVelocity = velocity->y + decceleration;
			}



			if (maxVelocity >= velocity->y)								//cap velocity based on sprinting
			{
				velocity->y = maxVelocity;
			}
			break;
	}
}



void MoveRight(Vector2D* velocity)
{
	float maxVelocity = 0;										//velocity cap

	bool direction = (int)gravityNormal.x == 0;

	switch (direction)
	{
	case true:
		if (collisionDirection == 1)
		{
			maxVelocity = 0.0f;
		}
		else if (AEInputCheckCurr('D') || AEInputCheckCurr(VK_RIGHT))
		{
			animDirection = 1;
			velocity->x += acceleration;

			if (AEInputCheckCurr(VK_LSHIFT))						//sprint check
			{
				maxVelocity = sprintVelocity;

				if (velocity->y == 0)
				{
					PlayerAnimate(SIX_BY_ONE, RUN, 6, 0.05F, true);
				}
			}
			else													//set normal velocity
			{
				maxVelocity = moveVelocity;

				if (velocity->y == 0)
				{
					PlayerAnimate(FOUR_BY_ONE, WALK, 4, 0.1F, true);
				}
			}
		}
		else if (velocity->x < lowSpeedCap && velocity->x >= 0.0f)		//prevent jittering at low speeds
		{
			maxVelocity = 0.0f;
		}
		else if (velocity->x > 0.0f)								//drag when right isn't pressed
		{
			maxVelocity = velocity->x - decceleration;
		}



		if (maxVelocity <= velocity->x)								//cap velocity based on sprinting
		{
			velocity->x = maxVelocity;
		}
		break;

	case false:
		if (collisionDirection == 1)
		{
			maxVelocity = 0.0f;
		}
		else if (AEInputCheckCurr('D') || AEInputCheckCurr(VK_RIGHT))
		{
			animDirection = 1;
			velocity->y += acceleration;

			if (AEInputCheckCurr(VK_LSHIFT))						//sprint check
			{
				maxVelocity = sprintVelocity;

				if (velocity->x == 0)
				{
					PlayerAnimate(SIX_BY_ONE, RUN, 6, 0.05F, true);
				}
			}
			else													//set normal velocity
			{
				maxVelocity = moveVelocity;

				if (velocity->x == 0)
				{
					PlayerAnimate(FOUR_BY_ONE, WALK, 4, 0.1F, true);
				}
			}
		}
		else if (velocity->y < lowSpeedCap && velocity->y >= 0.0f)		//prevent jittering at low speeds
		{
			maxVelocity = 0.0f;
		}
		else if (velocity->y > 0.0f)								//drag when right isn't pressed
		{
			maxVelocity = velocity->y - decceleration;
		}



		if (maxVelocity <= velocity->y)								//cap velocity based on sprinting
		{
			velocity->y = maxVelocity;
		}
		break;
	}
}






void Jump(Vector2D* velocity)
{
	bool direction = (int)gravityNormal.x == 0;

	if ((AEInputCheckTriggered('W') || AEInputCheckTriggered(VK_SPACE) || AEInputCheckTriggered(VK_UP)) && !isJumping)		//normal jump functionality
	{
		timeSinceGround = 1.0F;
		isColliding = false;
		//isJumping = true;

		switch (direction)
		{
			case true:
				velocity->y = gravityNormal.y * -0.5F;
				break;

			case false:
				velocity->x = gravityNormal.x * -0.5F;
				break;
		}

		PlayerAnimate(animDirection == 1 ? FOUR_BY_ONE : FOUR_BY_ONE_REVERSE, JUMP, 4, 0.1F, false);
		jumpSound = true;

		SetGravity(&gravityDefault);
		TransformSetRotation(GameObjectGetTransform(GetPlayer()->parent), 0);
	}

	if ((AEInputCheckReleased('W') || AEInputCheckReleased(VK_SPACE) || AEInputCheckTriggered(VK_UP)) && velocity->y > 0)		//reduces jump height based on time pressing space
	{
		if (velocity->y > 0)
		{
			velocity->y /= 2;
		}
		//isJumping = false;
	}
}





void WallJump(Vector2D* velocity, int direction)
{
	if ((AEInputCheckTriggered('W') || AEInputCheckTriggered(VK_SPACE) || AEInputCheckTriggered(VK_UP)) && isJumping)
	{
		velocity->y = jumpVelocity;
		velocity->x = wallJumpVelocity * direction;

		PlayerAnimate(animDirection == -1 ? FOUR_BY_ONE : FOUR_BY_ONE_REVERSE, WALL_JUMP, 4, 0.1F, false);
	}
}





void PlayerMovementUpdate(GameObjectPtr gameObject, AABB* levelTiles)
{
	if (GetPaused())
		return;

	PhysicsPtr pPhysics = GameObjectGetPhysics(gameObject);
	TransformPtr pTransform = GameObjectGetTransform(gameObject);
	AABB pCollider = GameObjectGetCollider(gameObject);


	if (isJumping)
	{
		acceleration = 10.0f;
		decceleration = 20.0f;
	}
	else
	{
		acceleration = 20.0f;
		decceleration = 40.0f;
	}

	float rotation = TransformGetRotation(pTransform);
	AABB* List = BoxCast(pCollider, (int)(rotation / (PI / 2) * 2));

	if (pPhysics && pTransform)
	{
		Vector2D velocity = *(PhysicsGetVelocity(pPhysics));
		Vector2D translation = *(TransformGetTranslation(pTransform));

		if (GetPlayer()->hasBrush)
		{
			if (AEInputCheckCurr(VK_LBUTTON))
			{
				PlayerAnimate(animDirection == 1 ? FOUR_BY_ONE : FOUR_BY_ONE_REVERSE, SWING, 4, 0.1F, false);
				if (f == 15) {
					if (GetPlayer()->paint <= 0)
						PlayRandSound('d');
					else
						PlayRandSound('w');
					f = 0;
				}
				else
					++f;
			}

			if (AEInputCheckCurr(VK_RBUTTON))
			{
				PlayerAnimate(animDirection == 1 ? FOUR_BY_ONE : FOUR_BY_ONE_REVERSE, CHARGE, 4, 0.1F, false);
				velocity.x = 0;
			}
		}

		if (!AEInputCheckCurr(VK_RBUTTON))
		{
			if (velocity.y < 0)
			{
				PlayerAnimate(animDirection == 1 ? FOUR_BY_ONE : FOUR_BY_ONE_REVERSE, FALL, 4, 0.1F, false);
			}

			if (velocity.x == 0 && velocity.y == 0)
			{
				PlayerAnimate(animDirection == 1 ? FOUR_BY_ONE : FOUR_BY_ONE_REVERSE, IDLE, 4, 0.1F, true);
			}
		}

		if (levelTiles)
		{
			int colCount = 0;
			enum collision_direction direct;

			collisionDirection = 0;
			isColliding = false;

			for (int i = 1; i < 5; ++i) {
				if (AABBReturnColNumberinRange(gameObject, List[i])) 
				{
					// Floor.
					if (i == 4)
					{
						isColliding = true;
						direct = i;
						++colCount;
						if (jumpSound) {
							PlaySFX("./Assets/SFX/player_land.mp3");
							jumpSound = 0;
						}
						

						//int tileY = (((int)List[i]->WorldPos.y + 448) / 64) - 1;
						//translation.y = (tileY * TILE_SIZE) - 448.0F + 160;
					}

					// Ceiling.
					if (i == 3)
					{
						hittingCeiling = true;
						++colCount;
						velocity.y = 0;
					}

					// Left wall.
					if (i == 1)
					{
						collisionDirection = -1;
						++colCount;
						timeSinceWallL = 0;

						if (velocity.y < 0)
						{
							PlayerAnimate(ONE_BY_ONE, WALL_SLIDE, 1, 1.0F, false);
						}

						//int tileX = (((int)List[i]->WorldPos.x + 768) / 64);
						//translation.x = (tileX * TILE_SIZE) - 768.0F + 64;
					}

					// Right wall.
					if (i == 2)
					{
						collisionDirection = 1;
						++colCount;
						timeSinceWallR = 0;

						if (velocity.y < 0)
						{
							PlayerAnimate(ONE_BY_ONE_REVERSE, WALL_SLIDE, 1, 1.0F, false);
						}

						//int tileX = (((int)List[i]->WorldPos.x + 768) / 64);
						//translation.x = (tileX * TILE_SIZE) - 768.0F;
					}
				}
			}
		}

		if (collisionDirection != 1)
		{
			timeSinceWallR += (float)AEFrameRateControllerGetFrameTime();
		}
		if (collisionDirection != -1)
		{
			timeSinceWallL += (float)AEFrameRateControllerGetFrameTime();
		}

		if (timeSinceWallL < 0.15F)
		{
			WallJump(&velocity, 1);
		}
		else if (timeSinceWallR < 0.15F)
		{
			WallJump(&velocity, -1);
		}

		GameObjectPtr collisionObject = ReturnGameObjectAABBCollision(pCollider);
		if (collisionObject)
		{
			if (strncmp(GameObjectGetName(collisionObject), "Paintbrush", _countof("Paintbrush")) == 0)
			{
				// Delete the paintbush.
				RemoveGameObject(collisionObject);
				GameObjectFree(&collisionObject);

				// Unlock the paintbrush.
				UnlockBrush();
			}
		}

		if (!isTakingDamage && !AEInputCheckCurr(VK_RBUTTON))
		{
			MoveLeft(&velocity);
			MoveRight(&velocity);
		}

#if DEBUG_MODE
		for (int i = 1; i < 5; ++i)
			DrawBoundingBox(List[i]);
#endif

		//Jump(&velocity);

		if (isColliding)
		{
			if (!isOveridingVelocity)
			{
				if (gravityNormal.x == 0 && velocity.y != 750.0f && velocity.y != -750.0f)
				{
					velocity.y = 0;
				}
				else 
				{
					velocity.x = 0;
				}
			}

			TransformSetTranslation(pTransform, &translation);
			PhysicsSetAcceleration(pPhysics, &gravityNone);
			isJumping = false;

			timeSinceGround = 0;
		}
		else
		{
			if ((collisionDirection == -1 || collisionDirection == 1) && velocity.y < 0)
			{
				PhysicsSetAcceleration(pPhysics, &wallFriction);
			}
			else
			{
				PhysicsSetAcceleration(pPhysics, &gravityNormal);
			}

			timeSinceGround += (float)AEFrameRateControllerGetFrameTime();
		}

		if (timeSinceGround > 0.15F && !isJumping)
		{
			isJumping = true;
		}

		if (!AEInputCheckCurr(VK_RBUTTON))
		{
			Jump(&velocity);
		}

		if (!isOveridingVelocity)
		{
			PhysicsSetVelocity(pPhysics, &velocity);
		}
		else
		{
			OverrideVelocity(false);
		}
	}

	FreeBoxCast(List);
}

bool IsInWater(void)
{
	return isInWater;
}

void SetInWater(bool water)
{
	isInWater = water;
}

void SetGravity(Vector2D* newGrav)
{
	gravityNormal = *newGrav;
}

void OverrideVelocity(bool override)
{
	isOveridingVelocity = override;
}
