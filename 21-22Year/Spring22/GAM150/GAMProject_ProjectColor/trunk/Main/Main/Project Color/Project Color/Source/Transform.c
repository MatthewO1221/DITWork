//------------------------------------------------------------------------------
//
// File Name:	Transform.c
// Author(s):	Emily Comes (emily.comes)
// Project:		Project 3
// Course:		CS230S22
//
// Copyright (C) 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Transform.h"
#include "Vector2D.h"

//#include "Stream.h"
#include "Matrix2D.h"

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

typedef struct Transform
{
  // The translation (or world position) of a game object.
  Vector2D	translation;

  // The rotation (or orientation) of a game object (in radians).
  float	rotation;

  // The scale (or size) of a game object.
  // (Hint: This should be initialized to (1, 1).)
  Vector2D	scale;

  // The transformation matrix resulting from concatenating the
  //	 matrix = Translation*Rotation*Scale matrices.
  Matrix2D	matrix;

  // True if the transformation matrix needs to be recalculated.
  // (Hint: this should be initialized to true.)
  bool	isDirty;
} Transform;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new transform object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: Failing to initialize the scale values to non-zero values will result in invisible sprites.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
TransformPtr TransformCreate(void)
{
  TransformPtr nuTF = (TransformPtr)calloc(1, sizeof(Transform));
  if (errno == ENOMEM || nuTF == NULL) // memory allocation failed
    return NULL;
  Vector2DZero(&(nuTF->translation));
  nuTF->rotation = 0.f;
  nuTF->scale.x = 1.f;
  nuTF->scale.y = 1.f;
  nuTF->isDirty = true;
  return nuTF;
}

// Free the memory associated with a transform object.
// (Also, set the transform pointer to NULL.)
// Params:
//	 transform = Pointer to the transform pointer.
void TransformFree(TransformPtr* transform)
{
  if (transform != NULL)
  {
    free(*transform);
    *transform = NULL;
  }
}

// Read the properties of a Transform component from a file.
// [NOTE: Read the translation value using StreamReadVector2D.]
// [NOTE: Read the rotation value using StreamReadFloat.]
// [NOTE: Read the scale value using StreamReadVector2D.]
// Params:
//	 physics = Pointer to the Transform component.
//	 stream = Pointer to the data stream used for reading.

//void TransformRead(TransformPtr transform, Stream stream)
//{
//  if (transform == NULL || stream == NULL)
//    return;
//  StreamReadVector2D(stream, &(transform->translation));
//  transform->rotation = StreamReadFloat(stream);
//  StreamReadVector2D(stream, &(transform->scale));
//  transform->isDirty = true;
//}

// Get the transform matrix, based upon translation, rotation and scale settings.
// (HINT: If the isDirty flag is true, then recalculate the transform matrix.)
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return a pointer to the component's matrix structure,
//		else return a NULL pointer.
Matrix2D* TransformGetMatrix(const TransformPtr transform)
{
  if (transform == NULL)
    return NULL;
  if (transform->isDirty)
  {
    Matrix2D scaleMtx, rotMtx, transMtx;
    Matrix2DIdentity(&scaleMtx);
    Matrix2DIdentity(&rotMtx);
    Matrix2DIdentity(&transMtx);
    Matrix2DScale(&scaleMtx, transform->scale.x, transform->scale.y);
    Matrix2DRotRad(&rotMtx, transform->rotation);
    Matrix2DTranslate(&transMtx, transform->translation.x, transform->translation.y);
    Matrix2D resMtx;
    Matrix2DConcat(&resMtx, &rotMtx, &scaleMtx);
    Matrix2DConcat(&(transform->matrix), &transMtx, &resMtx);
  }
  transform->isDirty = false;
  return &(transform->matrix);
}

// Get the translation of a transform component.
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return a pointer to the component's translation structure,
//		else return a NULL pointer.
const Vector2D* TransformGetTranslation(const TransformPtr transform)
{
  if (transform != NULL)
    return &(transform->translation);
  return NULL;
}

// Get the rotation value of a transform component.
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return the component's rotation value (in radians),
//		else return 0.0f.
float TransformGetRotation(const TransformPtr transform)
{
  if (transform != NULL)
    return transform->rotation;
  return 0.f;
}

// Get the scale of a transform component.
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return a pointer to the component's scale structure,
//		else return a NULL pointer.
const Vector2D* TransformGetScale(const TransformPtr transform)
{
  if (transform != NULL)
    return &(transform->scale);
  return NULL;
}

// Set the translation of a transform component.
// Params:
//	 transform = Pointer to the transform component.
//	 translation = Pointer to a translation vector.
void TransformSetTranslation(TransformPtr transform, const Vector2D* translation)
{
  if (transform == NULL || translation == NULL)
    return;
  transform->translation.x = translation->x;
  transform->translation.y = translation->y;
  transform->isDirty = true;
}

// Set the rotation of a transform component.
// Params:
//	 transform = Pointer to the transform component.
//	 rotation = The rotation value (in radians).
void TransformSetRotation(TransformPtr transform, float rotation)
{
  if (transform == NULL)
    return;
  transform->rotation = rotation;
  transform->isDirty = true;
}

// Set the scale of a transform component.
// Params:
//	 transform = Pointer to the transform component.
//	 translation = Pointer to a scale vector.
void TransformSetScale(TransformPtr transform, const Vector2D* scale)
{
  if (transform == NULL || scale == NULL)
    return;
  transform->scale.x = scale->x;
  transform->scale.y = scale->y;
  transform->isDirty = true;
}
