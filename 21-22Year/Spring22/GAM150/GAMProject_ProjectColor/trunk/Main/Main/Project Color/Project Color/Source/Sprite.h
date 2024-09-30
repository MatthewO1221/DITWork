//------------------------------------------------------------------------------
//
// File Name:	Sprite.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 3
// Course:		CS230S22
//
// Copyright (C) 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

#include "Vector2D.h"

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct AEGfxVertexList AEGfxVertexList;
typedef struct SpriteSource* SpriteSourcePtr;
typedef struct Sprite* SpritePtr;
typedef struct Transform* TransformPtr;

// typedef FILE* Stream;

//------------------------------------------------------------------------------
// Public Function Declarations:
//------------------------------------------------------------------------------

// Dynamically allocate a new sprite object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (NOTE: Make sure to initialize the 'alpha' member variable to 1.0f.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
SpritePtr SpriteCreate(void);

// Free the memory associated with a sprite object.
// (Also, set the sprite pointer to NULL.)
// Params:
//	 sprite = Pointer to the sprite pointer.
void SpriteFree(SpritePtr* sprite);

// Read the properties of a Sprite component from a file.
// [NOTE: Read the frameIndex value using StreamReadInt.]
// [NOTE: Read the alpha value using StreamReadFloat.]
// Params:
//	 sprite = Pointer to the Sprite component.
//	 stream = Pointer to the data stream used for reading.
//
//void SpriteRead(SpritePtr sprite, Stream stream);

// Draw a sprite (Sprite can be textured or untextured).
// Params:
//	 sprite = Pointer to the sprite object.
//   transform = Pointer to the sprite object's transform.
void SpriteDraw(const SpritePtr sprite, TransformPtr transform);

// Read the properties of a Sprite component from a file.
// [NOTE: Read the frameIndex value using StreamReadInt.]
// [NOTE: Read the alpha value using StreamReadFloat.]
// Params:
//	 sprite = Pointer to the Sprite component.
//	 stream = Pointer to the data stream used for reading.
//
//void SpriteRead(SpritePtr sprite, Stream stream);

// Draw a sprite (Sprite can be textured or untextured).
// Params:
//	 sprite = Pointer to the sprite object.
//   transform = Pointer to the sprite object's transform.
void SpriteDrawOffset(const SpritePtr sprite, TransformPtr transform, Vector2D* offset);

// Get a sprite's alpha value.
// Params:
//	 sprite = Pointer to the sprite object.
// Returns:
//	 If the pointer is valid,
//		then return the sprite's alpha value (a value between 0.0f and 1.0f),
//		else return 0.0f.
float SpriteGetAlpha(SpritePtr sprite);

// Set a sprite's alpha value.
// (NOTE: Make sure to clamp the resulting alpha value between 0.0f and 1.0f, as the
//	input value might be outside of this range.)
// (HINT: See AEClamp in AEMath.h.)
// Params:
//	 sprite = Pointer to the sprite object.
//   newAlpha = The sprite's new alpha value.
void SpriteSetAlpha(SpritePtr sprite, float newAlpha);

// Set the sprite's current frame.
// (NOTE: You must verify that the frameIndex parameter is within the
//	range [0 to frame count] before changing the sprite's frame index!)
// Params:
//	 sprite = Pointer to the sprite object.
//   frameIndex = New frame index for the sprite (0 .. frame count).
// ADDITIONAL REQUIREMENTS:
// - This function must make the following function call:
//     TraceMessage("SpriteSetFrame: frame index = %d", frameIndex);
void SpriteSetFrame(SpritePtr sprite, unsigned frameIndex);

// Set the sprite's mesh.
// (NOTE: This mesh may be textured or untextured.)
// (NOTE: This mesh may contain any number of triangles.)
// Params:
//	 sprite = Pointer to the sprite object.
//   mesh = Pointer to a mesh created using the Alpha Engine.
void SpriteSetMesh(SpritePtr sprite, AEGfxVertexList* mesh);

// Get the sprite's mesh.
// Params:
//	 sprite = Pointer to the sprite object.
// Returns:
//	 If the pointer is valid,
//		then return the sprite's mesh,
//		else return NULL.
AEGfxVertexList* SpriteGetMesh(SpritePtr sprite);

// Set a new SpriteSource for the specified sprite.
// (NOTE: The spriteSource parameter may be NULL.  This will remove an existing
//	texture from a sprite and cause the sprite to be displayed as a colored mesh.)
// Params:
//	 sprite = Pointer to the sprite to be modified.
//	 spriteSource = A new sprite source for the sprite (this pointer may be NULL).
void SpriteSetSpriteSource(SpritePtr sprite, SpriteSourcePtr spriteSource);

// Assign a text string to a sprite object.  This will allow a sequence of
//	 characters to be displayed as text.
// (NOTE: The text parameter may be NULL.  This will remove an existing text
//	 string from a sprite and cause the sprite to be displayed normally.)
// Params:
//	 sprite = Pointer to the sprite to be modified.
//	 text = Pointer to a zero-terminated array of characters.
void SpriteSetText(SpritePtr sprite, const char* text);

// Get the sprite's sprite source.
// Params:
//	 sprite = Pointer to the sprite object.
// Returns:
//	 If the pointer is valid,
//		then return the sprite's sprite source,
//		else return NULL.
SpriteSourcePtr SpriteGetSpriteSource(SpritePtr sprite);
