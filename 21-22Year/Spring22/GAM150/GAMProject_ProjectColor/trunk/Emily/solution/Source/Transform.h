
#include "targetver.h" /* required by Windows */
#include <AEEngine.h>


//Transform struct for use in all objects that move or require physics
typedef struct Transform* TransformPtr;
typedef struct Vector2D Vector2D;
//Declaration of physics functions

TransformPtr MakeTransform(void);

void FreeTransform(TransformPtr* transform);

Vector2D* GetTranslationTransform(const TransformPtr transform);

float GetRotationTransform(const TransformPtr transform);

Vector2D* GetScaleTransform(const TransformPtr transform);

void SetTranslationTransform(TransformPtr transform, const Vector2D* translation);

void SetRotationTransform(TransformPtr transform, float rotation);

void SetScaleTransform(TransformPtr transform, const Vector2D* scale);


