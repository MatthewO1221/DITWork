#include "stdafx.h"
#include "Physics.h"
#include "Vector2D.h"
#include "Transform.h"



typedef struct Transform* TransformPtr;
typedef struct Vector2D Vector2D;


typedef struct Transform
{
	// The translation (or world position) of a game object.
	Vector2D	translation;

	// The rotation (or orientation) of a game object (in radians).
	float	rotation;

	// The scale (or size) of a game object.
	// (Hint: This should be initialized to (1, 1).)
	Vector2D	scale;

} Transform;


TransformPtr MakeTransform(void)
{
	TransformPtr newTransform = calloc(1, sizeof(struct Transform));

	if (newTransform)
	{
		return newTransform;
	}

	return 0;
}

void FreeTransform(TransformPtr* transform)
{
	free(transform);
	transform = NULL;
}

Vector2D* GetTranslationTransform(const TransformPtr transform)
{
	if (transform)
	{
		return &transform->translation;
	}

	return 0;
}

float GetRotationTransform(const TransformPtr transform)
{
	if (transform)
	{
		return transform->rotation;
	}

	return 0;
}

Vector2D* GetScaleTransform(const TransformPtr transform)
{
	if (transform)
	{
		return &transform->scale;
	}

	return 0;
}

void SetTranslationTransform(TransformPtr transform, const Vector2D* translation)
{
	if (transform && translation)
	{
		transform->translation = *translation;
	}
}

void SetRotationTransform(TransformPtr transform, const float rotation)
{
	if (transform)
	{
		transform->rotation = rotation;
	}
}

void SetScaleTransform(TransformPtr transform, const Vector2D* scale)
{
	if (transform && scale)
	{
		transform->scale = *scale;
	}
}
