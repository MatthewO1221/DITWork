//------------------------------------------------------------------------------
//
// File Name:	Sprite.c
// Author(s):	Emily Comes (emily.comes)
// Project:		Project 3
// Course:		CS230S22
//
// Copyright (C) 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

// #include "stdafx.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "Mesh.h"

//#include "Stream.h"
//#include "Trace.h"
#include "Transform.h"
#include "Vector2D.h"
#include "AEEngine.h"
#include "Matrix2D.h"

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

typedef struct Sprite
{
  // The frame currently being displayed (for sprite sheets).
  unsigned frameIndex;

  // The alpha transparency to use when drawing the sprite.
  float alpha;

  // The sprite source used when drawing (NULL = simple colored mesh).
  SpriteSourcePtr spriteSource;

  // The mesh used to draw the sprite.
  AEGfxVertexList* mesh;

  // NUL-terminated string used to display sprite text.
  const char* text;
} Sprite;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new sprite object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (NOTE: Make sure to initialize the 'alpha' member variable to 1.0f.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
SpritePtr SpriteCreate(void)
{
  SpritePtr nuSpr = (SpritePtr)calloc(1, sizeof(Sprite));
  if (errno == ENOMEM || nuSpr == NULL) // memory allocation failed
    return NULL;
  nuSpr->frameIndex = 0;
  nuSpr->alpha = 1.f;
  nuSpr->spriteSource = NULL;
  nuSpr->mesh = NULL;
  return nuSpr;
}

// Free the memory associated with a sprite object.
// (Also, set the sprite pointer to NULL.)
// Params:
//	 sprite = Pointer to the sprite pointer.
void SpriteFree(SpritePtr* sprite)
{
  if (sprite != NULL && *sprite != NULL)
  {
    free(*sprite);
    *sprite = NULL;
  }
}

// Read the properties of a Sprite component from a file.
// [NOTE: Read the frameIndex value using StreamReadInt.]
// [NOTE: Read the alpha value using StreamReadFloat.]
// Params:
//	 sprite = Pointer to the Sprite component.
//	 stream = Pointer to the data stream used for reading.

//void SpriteRead(SpritePtr sprite, Stream stream)
//{
//  sprite->frameIndex = StreamReadInt(stream);
//  sprite->alpha = StreamReadFloat(stream);
//}

// Draw a sprite (Sprite can be textured or untextured).
// Params:
//	 sprite = Pointer to the sprite object.
//   transform = Pointer to the sprite object's transform.
void SpriteDraw(const SpritePtr sprite, TransformPtr transform)
{
  if (sprite == NULL || transform == NULL || sprite->mesh == NULL || sprite->alpha <= 0.f)
    return;
  if (sprite->spriteSource != NULL)
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
  else
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
  AEGfxSetTransparency(sprite->alpha);
  AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);
  if (sprite->text == NULL)
    MeshRender(sprite->mesh, sprite->spriteSource, TransformGetMatrix(transform), sprite->frameIndex);
  else
  {
    Matrix2D tfMtxLocal = *TransformGetMatrix(transform),
      offsetMtx;
    Matrix2DTranslate(&offsetMtx, TransformGetScale(transform)->x, 0.f);
    const char* walkChr = sprite->text;
    while (*walkChr != '\0')
    {
      MeshRender(sprite->mesh, sprite->spriteSource, &tfMtxLocal, *walkChr - ' ');
      ++walkChr;
      Matrix2DConcat(&tfMtxLocal, &offsetMtx, &tfMtxLocal);
    }
  }
}

// Draw a sprite (Sprite can be textured or untextured).
// Params:
//	 sprite = Pointer to the sprite object.
//   transform = Pointer to the sprite object's transform.
void SpriteDrawOffset(const SpritePtr sprite, TransformPtr transform, Vector2D* offset)
{
	Vector2D newPos = *TransformGetTranslation(transform);
	Vector2DAdd(&newPos, &newPos, offset);
	TransformSetTranslation(transform, &newPos);

	if (sprite == NULL || transform == NULL || sprite->mesh == NULL || sprite->alpha <= 0.f)
		return;
	if (sprite->spriteSource != NULL)
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	else
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransparency(sprite->alpha);
	AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);
	if (sprite->text == NULL)
		MeshRender(sprite->mesh, sprite->spriteSource, TransformGetMatrix(transform), sprite->frameIndex);
	else
	{
		Matrix2D tfMtxLocal = *TransformGetMatrix(transform),
			offsetMtx;
		Matrix2DTranslate(&offsetMtx, TransformGetScale(transform)->x, 0.f);
		const char* walkChr = sprite->text;
		while (*walkChr != '\0')
		{
			MeshRender(sprite->mesh, sprite->spriteSource, &tfMtxLocal, *walkChr - ' ');
			++walkChr;
			Matrix2DConcat(&tfMtxLocal, &offsetMtx, &tfMtxLocal);
		}
	}
}

// Get a sprite's alpha value.
// Params:
//	 sprite = Pointer to the sprite object.
// Returns:
//	 If the pointer is valid,
//		then return the sprite's alpha value (a value between 0.0f and 1.0f),
//		else return 0.0f.
float SpriteGetAlpha(SpritePtr sprite)
{
  if (sprite != NULL)
    return sprite->alpha;
  return 0.f;
}

// Set a sprite's alpha value.
// (NOTE: Make sure to clamp the resulting alpha value between 0.0f and 1.0f, as the
//	input value might be outside of this range.)
// (HINT: See AEClamp in AEMath.h.)
// Params:
//	 sprite = Pointer to the sprite object.
//   newAlpha = The sprite's new alpha value.
void SpriteSetAlpha(SpritePtr sprite, float newAlpha)
{
  if (sprite != NULL)
    sprite->alpha = AEClamp(newAlpha, 0.f, 1.f);
}

// Set the sprite's current frame.
// (NOTE: You must verify that the frameIndex parameter is within the
//	range [0 to frame count] before changing the sprite's frame index!)
// Params:
//	 sprite = Pointer to the sprite object.
//   frameIndex = New frame index for the sprite (0 .. frame count).
// ADDITIONAL REQUIREMENTS:
// - This function must make the following function call:
//     TraceMessage("SpriteSetFrame: frame index = %d", frameIndex);
void SpriteSetFrame(SpritePtr sprite, unsigned frameIndex)
{
  if (sprite != NULL && frameIndex <= SpriteSourceGetFrameCount(sprite->spriteSource))
  {
    //TraceMessage("SpriteSetFrame: frame index = %d", frameIndex);
    sprite->frameIndex = frameIndex;
  }
}

// Set the sprite's mesh.
// (NOTE: This mesh may be textured or untextured.)
// (NOTE: This mesh may contain any number of triangles.)
// Params:
//	 sprite = Pointer to the sprite object.
//   mesh = Pointer to a mesh created using the Alpha Engine.
void SpriteSetMesh(SpritePtr sprite, AEGfxVertexList* mesh)
{
  if (sprite != NULL && mesh != NULL)
    sprite->mesh = mesh;
}

// Get the sprite's mesh.
// Params:
//	 sprite = Pointer to the sprite object.
// Returns:
//	 If the pointer is valid,
//		then return the sprite's mesh,
//		else return NULL.
AEGfxVertexList* SpriteGetMesh(SpritePtr sprite)
{
  if (sprite == NULL || sprite->mesh == NULL)
    return NULL;
  return sprite->mesh;
}

// Set a new SpriteSource for the specified sprite.
// (NOTE: The spriteSource parameter may be NULL.  This will remove an existing
//	texture from a sprite and cause the sprite to be displayed as a colored mesh.)
// Params:
//	 sprite = Pointer to the sprite to be modified.
//	 spriteSource = A new sprite source for the sprite (this pointer may be NULL).
void SpriteSetSpriteSource(SpritePtr sprite, SpriteSourcePtr spriteSource)
{
  if (sprite != NULL && spriteSource != NULL)
    sprite->spriteSource = spriteSource;
}

// Assign a text string to a sprite object.  This will allow a sequence of
//	 characters to be displayed as text.
// (NOTE: The text parameter may be NULL.  This will remove an existing text
//	 string from a sprite and cause the sprite to be displayed normally.)
// Params:
//	 sprite = Pointer to the sprite to be modified.
//	 text = Pointer to a zero-terminated array of characters.
void SpriteSetText(SpritePtr sprite, const char* text)
{
  sprite->text = text;
}

// Get the sprite's sprite source.
// Params:
//	 sprite = Pointer to the sprite object.
// Returns:
//	 If the pointer is valid,
//		then return the sprite's sprite source,
//		else return NULL.
SpriteSourcePtr SpriteGetSpriteSource(SpritePtr sprite)
{
  if (sprite == NULL || sprite->spriteSource == NULL)
    return NULL;

  return sprite->spriteSource;
}
