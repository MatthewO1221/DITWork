
#include "targetver.h" /* required by Windows */
#include <AEEngine.h>


typedef struct GameObject GameObject, * GameObjectPtr;
typedef struct Physics* PhysicsPtr;
typedef struct Sprite* SpritePtr;
typedef struct Transform* TransformPtr;
typedef struct collisionData* AABB;

GameObjectPtr MakeGameObject(void);

void FreeGameObject(GameObjectPtr* gameObject);

void AddPhysicsGameObject(GameObjectPtr gameObject, PhysicsPtr physics);

void AddTransformGameObject(GameObjectPtr gameObject, TransformPtr transform);

void AddSpriteGameObject(GameObjectPtr gameObject, SpritePtr sprite);

void AddColliderGameObject(GameObjectPtr gameObject, AABB collider);

void SetNameGameObject(GameObjectPtr gameObject, const char* name);

const char* GetNameGameObject(const GameObject* gameObject);

PhysicsPtr GetPhysicsGameObject(const GameObjectPtr gameObject);

TransformPtr GetTransformGameObject(const GameObjectPtr gameObject);

SpritePtr GetSpriteGameObject(const GameObjectPtr gameObject);

AABB GetColliderGameObject(const GameObjectPtr gameObject);

void UpdateGameObject(GameObjectPtr gameObject, float dt);

void GameObjectDraw(GameObjectPtr gameObject);
