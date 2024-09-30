//------------------------------------------------------------------------------
//
// File Name:	Player Movement.c
// Author(s):	Rishabh Anand
// Project:		Project 2
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
#include "Sprite.h"
#include "tilemap.h"

static const float moveVelocity = 500.0f;
static const float jumpVelocity = 750.0f;
static const Vector2D gravityNormal = { 0.0f, -1500.0f };
static const Vector2D gravityNone = { 0.0f, 0.0f };

void PlayerMovementUpdate(GameObjectPtr gameObject, AABB* allTiles, int tileCountX, int tileCountY)
{
	PhysicsPtr pPhysics = GetPhysicsGameObject(gameObject);
	TransformPtr pTransform = GetTransformGameObject(gameObject);

	if (pPhysics && pTransform)
	{
		Vector2D velocity = *(GetVelocityPhysics(pPhysics));
		Vector2D translation = *(GetTranslationTransform(pTransform));

		AABB bounds = GetColliderGameObject(gameObject);

		bounds->left = translation.x;
		bounds->right = bounds->left + GetScaleTransform(pTransform)->x * 64;
		bounds->bottom = translation.y - 160;
		bounds->top = bounds->bottom + GetScaleTransform(pTransform)->y * 64;

		if (AEInputCheckCurr(VK_LEFT))
		{
			velocity.x = -moveVelocity;
			SetVelocityPhysics(pPhysics, &velocity);
		}
		else if (AEInputCheckCurr(VK_RIGHT))
		{
			velocity.x = moveVelocity;
			SetVelocityPhysics(pPhysics, &velocity);
		}
		else
		{
			velocity.x = 0;
			SetVelocityPhysics(pPhysics, &velocity);
		}

		SetAccelerationPhysics(pPhysics, &gravityNormal);
		int hasCollided = false;

		for (int y = 0; y < TILEMAP_HEIGHT; ++y)
		{
			for (int x = 0; x < TILEMAP_WIDTH; ++x)
			{
				if (!CheckAABBCollision(bounds, allTiles[y * TILEMAP_WIDTH + x]))
				{
					// intentionally blank, no collision
				}
				else
				{
					hasCollided = true;

					{
						translation.y = allTiles[y * TILEMAP_WIDTH + x]->top + 160;
						velocity.y = 0;
						SetTranslationTransform(pTransform, &translation);
						SetVelocityPhysics(pPhysics, &velocity);
						SetAccelerationPhysics(pPhysics, &gravityNone);
					}
				}
			}
		}

		if (hasCollided && AEInputCheckTriggered(VK_UP))
		{
			velocity.y = jumpVelocity;
			SetVelocityPhysics(pPhysics, &velocity);

			bounds->bottom += 10;
			bounds->top += 10;
		}
		if (!hasCollided)
		{
			PhysicsPtr physics = GetPhysicsGameObject(gameObject);
			Vector2D newVel = { GetVelocityPhysics(physics)->x, GetVelocityPhysics(physics)->y - 20};

			SetVelocityPhysics(GetPhysicsGameObject(gameObject), &newVel);
		}
	}
}