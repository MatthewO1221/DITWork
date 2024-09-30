#include "stdafx.h"		// Should be included first in every .c file!
#include "GameplayInstance.h"
#include "GameObject.h"
#include "Debug.h"

static GameplayInstance game;

// Adds a gameobject to the list.
void InitGameplayInstance(void)
{
	game.gameObjectCount = 0;
}

// Adds a gameobject to the list.
void AddGameObject(GameObjectPtr gameObject)
{
	game.gameObjects[game.gameObjectCount++] = gameObject;
}

// Removes a gameobject from the list.
void RemoveGameObject(GameObjectPtr gameObject)
{
	for (int i = 0; i < game.gameObjectCount; ++i)
	{
		// Find the right gameobject.
		if (game.gameObjects[i] == gameObject)
		{
			game.gameObjects[i] = NULL;

			// Push everything down by one.
			for (int j = i; j < game.gameObjectCount ; ++j)
			{
				game.gameObjects[j] = game.gameObjects[j + 1];
			}

			--game.gameObjectCount;
			break;
		}
	}
}

// Draws a gameobject from the list.
void DrawGameObjects(void)
{
	for (int i = 0; i < game.gameObjectCount; ++i)
	{
		GameObjectDrawOffset(game.gameObjects[i], &game.gameObjectOffsets[i]);

#if DEBUG_MODE
		DrawBoundingBox(GameObjectGetCollider(game.gameObjects[i]));
#endif
	}
}

// Returns gameplay instance.
GameplayInstance* GetGame(void)
{
	return &game;
}
