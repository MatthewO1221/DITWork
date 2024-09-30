
#include "targetver.h" /* required by Windows */
#include <AEEngine.h>


typedef struct GameObject GameObject, * GameObjectPtr;
typedef struct Physics* PhysicsPtr;
typedef struct Sprite* SpritePtr;
typedef struct Transform* TransformPtr;

GameObjectPtr MakeGameObject(void);

void FreeGameObject(GameObjectPtr* gameObject);

void AddPhysicsGameObject(GameObjectPtr gameObject, PhysicsPtr physics);

void AddTransformGameObject(GameObjectPtr gameObject, TransformPtr transform);

void SetNameGameObject(GameObjectPtr gameObject, const char* name);

const char* GetNameGameObject(const GameObject* gameObject);

PhysicsPtr GetPhysicsGameObject(const GameObjectPtr gameObject);

TransformPtr GetTransformGameObject(const GameObjectPtr gameObject);

void UpdateGameObject(GameObjectPtr gameObject, float dt);
