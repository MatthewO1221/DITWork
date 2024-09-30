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

static const float groundHeight = -150.0f;
static const float moveVelocity = 500.0f;
static const float jumpVelocity = 1000.0f;
static const Vector2D gravityNormal = { 0.0f, -1500.0f };
static const Vector2D gravityNone = { 0.0f, 0.0f };

void PlayerMovementUpdate(GameObjectPtr gameObject)
{
	PhysicsPtr pPhysics = GetPhysicsGameObject(gameObject);
	TransformPtr pTransform = GetTransformGameObject(gameObject);

	if (pPhysics && pTransform)
	{
		Vector2D velocity = *(GetVelocityPhysics(pPhysics));
		Vector2D translation = *(GetTranslationTransform(pTransform));

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

		if (AEInputCheckTriggered(VK_UP))
		{
			velocity.y = jumpVelocity;
			SetVelocityPhysics(pPhysics, &velocity);
			SetAccelerationPhysics(pPhysics, &gravityNormal);
		}

		if (translation.y < groundHeight)
		{
			translation.y = groundHeight;
			velocity.y = 0;
			SetTranslationTransform(pTransform, &translation);
			SetVelocityPhysics(pPhysics, &velocity);
			SetAccelerationPhysics(pPhysics, &gravityNone);
		}

	}
}