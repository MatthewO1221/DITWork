#include "AEEngine.h"
#include "Vector2D.h"

typedef struct collisionData* AABB;
typedef struct GameObject *GameObjectPtr;

// Gameplay instance definition. Stores info about current GetGame()->
typedef struct GameplayInstance
{
	// List of all entities.
	GameObjectPtr gameObjects[64];
	Vector2D gameObjectOffsets[64];
	int gameObjectCount;

	// Info for each tile.
	AABB tileCollisions[72][42];
	char tileColors[72][42];
	char tileNormals[72][42]; // 0 = top, 1 = bottom, 2 = left, 3 = right.
} GameplayInstance;

// Adds a gameobject to the list.
void InitGameplayInstance(void);

// Adds a gameobject to the list.
void AddGameObject(GameObjectPtr);

// Removes a gameobject from the list.
void RemoveGameObject(GameObjectPtr);

// Draws a gameobject from the list.
void DrawGameObjects(void);

// Returns gameplay instance.
GameplayInstance* GetGame(void);
