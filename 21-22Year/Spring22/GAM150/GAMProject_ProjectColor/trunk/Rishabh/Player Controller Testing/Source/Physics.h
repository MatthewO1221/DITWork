#include "targetver.h" /* required by Windows */
#include <AEEngine.h>
#include <AEFrameRateController.h>


//Transform struct for use in all objects that move or require physics
typedef struct Physics* PhysicsPtr;
typedef struct Transform* TransformPtr;
typedef struct Vector2D Vector2D;
//Declaration of physics functions

PhysicsPtr MakePhysics(void);

void FreePhysics(PhysicsPtr* physics);

Vector2D* GetAccelerationPhysics(const PhysicsPtr physics);

Vector2D* GetVelocityPhysics(const PhysicsPtr physics);

Vector2D* GetTranslationPhysics(const PhysicsPtr physics);

void SetAccelerationPhysics(PhysicsPtr physics, const Vector2D* acceleration);

void SetVelocityPhysics(PhysicsPtr physics, const Vector2D* velocity);

void UpdatePhysics(PhysicsPtr physics, TransformPtr transform, float dt);


