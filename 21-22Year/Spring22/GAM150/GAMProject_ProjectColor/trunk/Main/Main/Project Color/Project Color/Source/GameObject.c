//------------------------------------------------------------------------------
//
// File Name:	GameObject.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 0
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObject.h"
#include "Physics.h"
#include "Sprite.h"
#include "Transform.h"
#include "Physics.h"
#include "Sprite.h"
#include "AABB.h"
#include "Animation.h"

typedef struct GameObject
{
	// The name of the game object.
	// A buffer is used to allow each game object to have a unique name.
	// The buffer is hardcoded to an arbitrary length that will be sufficient
	//	 for all CS230 assignments.  You may, instead, use dynamically-allocated
	//	 arrays for this purpose but the additional complexity is unnecessary
	//	 and it is your responsibility to ensure that the memory is successfully
	//	 allocated and deallocated in all possible situations.
	// [NOTE: When setting the name, use strcpy_s() to reduce the risk of
	//	 buffer overruns. Additionally, do NOT hardcode the number "32" when
	//	 calling this function!  Instead, use the _countof() macro to get the
	//	 size of the "name" array.]
	char name[32];

	// Pointer to an attached physics component.
	PhysicsPtr physics;

	// Pointer to an attached sprite component.
	SpritePtr sprite;

	// Pointer to an attached transform component.
	TransformPtr transform;

	// Pointer to an attached collider component.
	AABB collider;

	AnimationPtr animation;

} GameObject;

// Dynamically allocate a new game object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
GameObjectPtr GameObjectCreate(void)
{
	GameObjectPtr newGameObject = calloc(1, sizeof(GameObject));

	if (newGameObject)
	{
		return newGameObject;
	}
	else
	{
		return NULL;
	}
}

// Free the memory associated with a game object, including all components.
// (Also, set the game object pointer to NULL.)
// Params:
//	 gameObject = Pointer to the game object pointer.
void GameObjectFree(GameObjectPtr* gameObject)
{
	if (gameObject == NULL)
		return;

	PhysicsFree(&(*gameObject)->physics);
	SpriteFree(&(*gameObject)->sprite);
	TransformFree(&(*gameObject)->transform);
	AnimationFree(&(*gameObject)->animation);
	FreeAABB((*gameObject)->collider);

	free(*gameObject);
	*gameObject = NULL;
}

// Attach a physics component to a game object.
// Params:
//	 gameObject = Pointer to the game object.
//   physics = Pointer to the physics component to be attached.
void GameObjectAddPhysics(GameObjectPtr gameObject, PhysicsPtr physics)
{
	if (gameObject == NULL || physics == NULL)
		return;

	gameObject->physics = physics;
}

// Attach a sprite component to a game object.
// Params:
//	 gameObject = Pointer to the game object.
//   sprite = Pointer to the sprite component to be attached.
void GameObjectAddSprite(GameObjectPtr gameObject, SpritePtr sprite)
{
	if (gameObject == NULL || sprite == NULL)
		return;

	gameObject->sprite = sprite;
}

// Attach a transform component to a game object.
// Params:
//	 gameObject = Pointer to the game object.
//   transform = Pointer to the transform component to be attached.
void GameObjectAddTransform(GameObjectPtr gameObject, TransformPtr transform)
{
	if (gameObject == NULL || transform == NULL)
		return;

	gameObject->transform = transform;
}

// Attach an AABB component to a game object.
// Params:
//	 gameObject = Pointer to the game object.
//   collider = Pointer to the AABB to be attached.
void GameObjectAddCollider(GameObjectPtr gameObject, AABB collider)
{
	if (gameObject == NULL || collider == NULL)
		return;

	gameObject->collider = collider;
}

// Set the game object's name.
// [NOTE: Verify that both pointers are valid before setting the name.]
// [NOTE: When setting the name, use strcpy_s() to reduce the risk of
//	 buffer overruns. Additionally, do NOT hardcode the number "32" when
//	 calling this function!  Instead, use the _countof() macro to get the
//	 size of the "name" array.]
// Params:
//	 gameObject = Pointer to the game object.
//	 name = Pointer to the game object's new name.
void GameObjectSetName(GameObjectPtr gameObject, const char* name)
{
	if (gameObject == NULL || name == NULL)
		return;

	strcpy_s(gameObject->name, sizeof(gameObject->name), name);
}

// Get the game object's name.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the game object's name,
//		else return NULL.
const char* GameObjectGetName(const GameObject* gameObject)
{
	if (gameObject == NULL)
		return NULL;

	return gameObject->name;
}

// Get the physics component attached to a game object.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the attached physics component,
//		else return NULL.
PhysicsPtr GameObjectGetPhysics(const GameObjectPtr gameObject)
{
	if (gameObject == NULL)
		return NULL;

	return gameObject->physics;
}

// Get the sprite component attached to a game object.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the attached sprite component,
//		else return NULL.
SpritePtr GameObjectGetSprite(const GameObjectPtr gameObject)
{
	if (gameObject == NULL)
		return NULL;

	return gameObject->sprite;
}

// Get the transform component attached to a game object.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the attached transform component,
//		else return NULL.
TransformPtr GameObjectGetTransform(const GameObjectPtr gameObject)
{
	if (gameObject == NULL)
		return NULL;

	return gameObject->transform;
}

// Get the collider component attached to a game object.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the attached collider component,
//		else return NULL.
AABB GameObjectGetCollider(const GameObjectPtr gameObject)
{
	if (gameObject == NULL)
		return NULL;

	return gameObject->collider;
}

// Update any components attached to the game object.
// (Hint: You will need to call PhysicsUpdate().)
// (NOTE: You must first check for a valid pointer before calling this function.)
// Params:
//	 gameObject = Pointer to the game object.
//	 dt = Change in time (in seconds) since the last game loop.
void GameObjectUpdate(GameObjectPtr gameObject, float dt)
{
	if (gameObject == NULL || gameObject->physics == NULL || gameObject->transform == NULL)
		return;

	PhysicsUpdate(gameObject->physics, gameObject->transform, dt);
	AnimationUpdate(gameObject->animation, dt);

	updateAABBPosition(GameObjectGetCollider(gameObject), GameObjectGetPhysics(gameObject));
}

// Draw any visible components attached to the game object.
// (Hint: You will need to call SpriteDraw(), passing the sprite and transform components.)
// (NOTE: You must first check for valid pointers before calling this function.)
// Params:
//	 gameObject = Pointer to the game object.
void GameObjectDraw(GameObjectPtr gameObject)
{
	if (gameObject == NULL || gameObject->sprite == NULL || gameObject->transform == NULL)
		return;

	SpriteDraw(gameObject->sprite, gameObject->transform);
}

// Draw any visible components attached to the game object.
// (Hint: You will need to call SpriteDraw(), passing the sprite and transform components.)
// (NOTE: You must first check for valid pointers before calling this function.)
// Params:
//	 gameObject = Pointer to the game object.
void GameObjectDrawOffset(GameObjectPtr gameObject, Vector2D* offset)
{
	if (gameObject == NULL || gameObject->sprite == NULL || gameObject->transform == NULL)
		return;

	SpriteDrawOffset(gameObject->sprite, gameObject->transform, offset);
}

// Get the animation component attached to a game object.
// Params:
//	 gameObject = Pointer to the game object.
// Returns:
//	 If the game object pointer is valid,
//		then return a pointer to the attached animation component,
//		else return NULL.
AnimationPtr GameObjectGetAnimation(const GameObjectPtr gameObject)
{
	if (gameObject)
	{
		return gameObject->animation;
	}

	return 0;
}

// Attach an animation component to a game object.
// (NOTE: This function must set the animation component's parent pointer by
//	  calling the AnimationSetParent() function.)
// Params:
//	 gameObject = Pointer to the game object.
//   animation = Pointer to the animation component to be attached.
void GameObjectAddAnimation(GameObjectPtr gameObject, AnimationPtr animation)
{
	if (gameObject && animation)
	{
		AnimationSetParent(animation, gameObject);
		gameObject->animation = animation;
	}
}
