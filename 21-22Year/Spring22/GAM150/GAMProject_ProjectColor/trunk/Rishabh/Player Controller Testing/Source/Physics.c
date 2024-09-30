#include "stdafx.h"
#include "Physics.h"
#include "Vector2D.h"
#include "Transform.h"



typedef struct Physics* PhysicsPtr;
typedef struct Transform* TransformPtr;


typedef struct Physics
{
	// Previous position.  May be used for resolving collisions.
	Vector2D	translation;

	// Acceleration = inverseMass * (sum of forces)
	Vector2D	acceleration;

	// Velocity may be stored as a direction vector and speed scalar, instead.
	Vector2D	velocity;

	// Used when calculating acceleration due to forces.
	// Used when resolving collision between two dynamic objects.
	//float		inverseMass;

} Physics;


PhysicsPtr MakePhysics(void)
{
	PhysicsPtr newPhysics = calloc(1, sizeof(struct Physics));

	if (newPhysics)
	{
		return newPhysics;
	}

	return 0;
}

void FreePhysics(PhysicsPtr* physics)
{
	free(physics);
	physics = NULL;
}

Vector2D* GetAccelerationPhysics(const PhysicsPtr physics)
{
	if (physics)
	{
		return &physics->acceleration;
	}

	return 0;
}

Vector2D* GetVelocityPhysics(const PhysicsPtr physics)
{
	if (physics)
	{
		return &physics->velocity;
	}

	return 0;
}

Vector2D* GetTranslationPhysics(const PhysicsPtr physics)
{
	if (physics)
	{
		return &physics->translation;
	}

	return 0;
}

void SetAccelerationPhysics(PhysicsPtr physics, const Vector2D* acceleration)
{
	if (physics && acceleration)
	{
		physics->acceleration = *acceleration;
	}
}

void SetVelocityPhysics(PhysicsPtr physics, const Vector2D* velocity)
{
	if (physics && velocity)
	{
		physics->velocity = *velocity;
	}
}

void UpdatePhysics(PhysicsPtr physics, TransformPtr transform, float dt)
{
	if (physics && transform)
	{
		physics->translation = *GetTranslationTransform(transform);
		Vector2D velocity;
		Vector2DScaleAdd(&velocity, &physics->acceleration, &physics->velocity, dt);
		Vector2D translation;
		Vector2DScaleAdd(&translation, &physics->velocity, &physics->translation, dt);
		physics->translation = translation;
		SetTranslationTransform(transform, &physics->translation);

	}
}
