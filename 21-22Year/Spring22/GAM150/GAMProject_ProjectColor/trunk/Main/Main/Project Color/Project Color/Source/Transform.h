//------------------------------------------------------------------------------
//
// File Name:	Transform.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 3
// Course:		CS230S22
//
// Copyright (C) 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct Matrix2D Matrix2D;
typedef struct Transform* TransformPtr;
typedef struct Vector2D Vector2D;

//typedef FILE* Stream;

//------------------------------------------------------------------------------
// Public Function Declarations:
//------------------------------------------------------------------------------

// Dynamically allocate a new transform object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: Make sure to initialize all member variables properly.)
// (Hint: Failing to initialize the scale values to non-zero values will result in invisible sprites.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
TransformPtr TransformCreate(void);

// Free the memory associated with a transform object.
// (Also, set the transform pointer to NULL.)
// Params:
//	 transform = Pointer to the transform pointer.
void TransformFree(TransformPtr* transform);

// Read the properties of a Transform component from a file.
// [NOTE: Read the translation value using StreamReadVector2D.]
// [NOTE: Read the rotation value using StreamReadFloat.]
// [NOTE: Read the scale value using StreamReadVector2D.]
// Params:
//	 transform = Pointer to the Transform component.
//	 stream = Pointer to the data stream used for reading.

//void TransformRead(TransformPtr transform, Stream stream);

// Get the transform matrix, based upon translation, rotation and scale settings.
// (HINT: If the isDirty flag is true, then recalculate the transform matrix.)
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return a pointer to the component's matrix structure,
//		else return a NULL pointer.
Matrix2D* TransformGetMatrix(const TransformPtr transform);

// Get the translation of a transform component.
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return a pointer to the component's translation structure,
//		else return a NULL pointer.
const Vector2D* TransformGetTranslation(const TransformPtr transform);

// Get the rotation value of a transform component.
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return the component's rotation value (in radians),
//		else return 0.0f.
float TransformGetRotation(const TransformPtr transform);

// Get the scale of a transform component.
// Params:
//	 transform = Pointer to the transform object.
// Returns:
//	 If the transform pointer is valid,
//		then return a pointer to the component's scale structure,
//		else return a NULL pointer.
const Vector2D* TransformGetScale(const TransformPtr transform);

// Set the translation of a transform component.
// Params:
//	 transform = Pointer to the transform component.
//	 translation = Pointer to a translation vector.
void TransformSetTranslation(TransformPtr transform, const Vector2D* translation);

// Set the rotation of a transform component.
// Params:
//	 transform = Pointer to the transform component.
//	 rotation = The rotation value (in radians).
void TransformSetRotation(TransformPtr transform, float rotation);

// Set the scale of a transform component.
// Params:
//	 transform = Pointer to the transform component.
//	 translation = Pointer to a scale vector.
void TransformSetScale(TransformPtr transform, const Vector2D* scale);
