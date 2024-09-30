//------------------------------------------------------------------------------
//
// File Name:	Physics.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 2
// Course:		CS230S20
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
#include "Vector2D.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Physics * PhysicsPtr;
typedef struct Transform * TransformPtr;
typedef struct Vector2D Vector2D;
typedef FILE* Stream;



static const Vector2D Gravity = { 0.0f,-1500.0f };
static const Vector2D noGravity = { 0.0f,0.0f };
//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// An example of the structure to be defined in Physics.c.
#if 0
typedef struct Physics
{
	// Previous position.  May be used for resolving collisions.
	Vector2D	oldTranslation;

	// Acceleration = inverseMass * (sum of forces)
	Vector2D	acceleration;

	// Velocity may be stored as a direction vector and speed scalar, instead.
	Vector2D	velocity;

	// Used when calculating acceleration due to forces.
	// Used when resolving collision between two dynamic objects.
	//float		inverseMass;

} Physics;
#endif

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new Physics component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
PhysicsPtr PhysicsCreate(void);

// Free the memory associated with a Physics component.
// (Also, set the physics pointer to NULL.)
// Params:
//	 physics = Pointer to the Physics component pointer.
void PhysicsFree(PhysicsPtr * physics);

// Get the acceleration of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's acceleration structure,
//		else return a NULL pointer.
Vector2D * PhysicsGetAcceleration(const PhysicsPtr physics);

// Get the velocity of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's velocity structure,
//		else return a NULL pointer.
Vector2D * PhysicsGetVelocity(const PhysicsPtr physics);

// Get the old translation (position) of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
// Returns:
//	 If the physics pointer is valid,
//		then return a pointer to the component's oldTranslation structure,
//		else return a NULL pointer.
Vector2D * PhysicsGetOldTranslation(PhysicsPtr physics);

// Set the acceleration of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
//	 acceleration = Pointer to an acceleration vector.
void PhysicsSetAcceleration(PhysicsPtr physics, const Vector2D * acceleration);

// Set the velocity of a Physics component.
// Params:
//	 physics = Pointer to the Physics component.
//	 velocity = Pointer to a velocity vector.
void PhysicsSetVelocity(PhysicsPtr physics, const Vector2D * velocity);

// Update the state of a Physics component using the Semi-Implicit Euler method,
//	 as outlined in the "Dynamics" lecture slides and the project instructions.
// (NOTE: This function must verify that the physics and transform pointers are valid.)
// Params:
//	 physics = Pointer to the physics component.
//	 transform = Pointer to the associated transform component.
//	 dt = Change in time (in seconds) since the last game loop.
void PhysicsUpdate(PhysicsPtr physics, TransformPtr transform, float dt);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

