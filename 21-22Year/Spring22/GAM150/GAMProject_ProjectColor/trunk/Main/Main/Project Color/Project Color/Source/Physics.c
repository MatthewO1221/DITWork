//------------------------------------------------------------------------------
//
// File Name:	Physics.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 0
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Physics.h"
#include "Transform.h"

typedef struct Physics
{
	// Previous position.  May be used for resolving collisions.
	Vector2D oldTranslation;

	// Acceleration = inverseMass * (sum of forces)
	Vector2D acceleration;

	// Velocity may be stored as a direction vector and speed scalar, instead.
	Vector2D velocity;

	// Used when calculating acceleration due to forces.
	// Used when resolving collision between two dynamic objects.
	float inverseMass;

} Physics;

// Dynamically allocate a new Physics component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
PhysicsPtr PhysicsCreate(void)
{
	PhysicsPtr newPhysics = calloc(1, sizeof(Physics));

	if (newPhysics)
	{
		return newPhysics;
	}
	else
	{
		return NULL;
	}
}

// Free the memory associated with a Physics component.
// (Also, set the physics pointer to NULL.)
// Params:
//	 physics = Pointer to the Physics component pointer.
void PhysicsFree(PhysicsPtr* physics)
{
	if (physics == NULL)
		return;

	free(*physics);
	*physics = NULL;
}

// Get the acceleration of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's acceleration structure,
//		else return a NULL pointer.
Vector2D* PhysicsGetAcceleration(const PhysicsPtr physics)
{
	if (physics == NULL)
		return NULL;

	return &physics->acceleration;
}

// Get the velocity of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's velocity structure,
//		else return a NULL pointer.
Vector2D* PhysicsGetVelocity(const PhysicsPtr physics)
{
	if (physics == NULL)
		return NULL;

	return &physics->velocity;
}

// Get the old translation (position) of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's oldTranslation structure,
//		else return a NULL pointer.
Vector2D* PhysicsGetOldTranslation(PhysicsPtr physics)
{
	if (physics == NULL)
		return NULL;

	return &physics->oldTranslation;
}

// Set the acceleration of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
//	 acceleration = Pointer to an acceleration vector.
void PhysicsSetAcceleration(PhysicsPtr physics, const Vector2D* acceleration)
{
	if (physics == NULL)
		return;

	physics->acceleration = *acceleration;
}

// Set the velocity of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
//	 velocity = Pointer to a velocity vector.
void PhysicsSetVelocity(PhysicsPtr physics, const Vector2D* velocity)
{
	if (physics == NULL)
		return;

	physics->velocity = *velocity;
}

// Update the state of a Physics component using the Semi-Implicit Euler method,
//	 as outlined in the "Dynamics" lecture slides and the project instructions.
// (NOTE: This function must verify that the physics and transform pointers are valid.)
// Params:
//	 physics = Pointer to the physics component.
//	 transform = Pointer to the associated transform component.
//	 dt = Change in time (in seconds) since the last game loop.
void PhysicsUpdate(PhysicsPtr physics, TransformPtr transform, float dt)
{
	physics->oldTranslation = *TransformGetTranslation(transform);

	Vector2DScaleAdd(&physics->velocity, PhysicsGetAcceleration(physics), PhysicsGetVelocity(physics), dt);
	Vector2DScaleAdd(&physics->oldTranslation, PhysicsGetVelocity(physics), TransformGetTranslation(transform), dt);

	TransformSetTranslation(transform, &physics->oldTranslation);

}
