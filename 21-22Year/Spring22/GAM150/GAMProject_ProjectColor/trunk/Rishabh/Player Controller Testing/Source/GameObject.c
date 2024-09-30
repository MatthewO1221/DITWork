#include "stdafx.h"
#include "GameObject.h"
#include "Physics.h"
#include "Transform.h"


typedef struct GameObject GameObject, * GameObjectPtr;
typedef struct Physics* PhysicsPtr;
typedef struct Sprite* SpritePtr;
typedef struct Transform* TransformPtr;
typedef FILE* Stream;


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
	//SpritePtr sprite;

	// Pointer to an attached transform component.
	TransformPtr transform;

} GameObject;


GameObjectPtr MakeGameObject(void)
{
	GameObjectPtr newGameObject = calloc(1, sizeof(struct GameObject));
	if (newGameObject)
	{
		return newGameObject;
	}

	return 0;
}

void FreeGameObject(GameObjectPtr* gameObject)
{
	if (gameObject)
	{
		free(GetPhysicsGameObject(*gameObject));
		free(GetTransformGameObject(*gameObject));
		free(gameObject);

		gameObject = NULL;
	}
}

void AddPhysicsGameObject(GameObjectPtr gameObject, PhysicsPtr physics)
{
	if (gameObject && physics)
	{
		gameObject->physics = physics;
	}
}

void AddTransformGameObject(GameObjectPtr gameObject, TransformPtr transform)
{
	if (gameObject && transform)
	{
		gameObject->transform = transform;
	}
}

void SetNameGameObject(GameObjectPtr gameObject, const char* name)
{
	if (gameObject)
	{
		*gameObject->name = *name;
	}
}

const char* GetNameGameObject(const GameObject* gameObject)
{
	if (gameObject)
	{
		return gameObject->name;
	}

	return 0;
}

PhysicsPtr GetPhysicsGameObject(const GameObjectPtr gameObject)
{
	if (gameObject)
	{
		return gameObject->physics;
	}

	return 0;
}

TransformPtr GetTransformGameObject(const GameObjectPtr gameObject)
{
	if (gameObject)
	{
		return gameObject->transform;
	}

	return 0;
}

void UpdateGameObject(GameObjectPtr gameObject, float dt)
{
	if (gameObject && gameObject->physics && gameObject->transform)
	{
		UpdatePhysics(GetPhysicsGameObject(gameObject), GetTransformGameObject(gameObject), dt);
	}
}