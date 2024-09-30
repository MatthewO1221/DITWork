//------------------------------------------------------------------------------
//
// File Name:	SpriteSource.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 0
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SpriteSource.h"

typedef struct SpriteSource
{
  // The dimensions of the sprite sheet.
  unsigned	numRows, numCols;

  // Pointer to a texture created using the Alpha Engine.
  AEGfxTexturePtr	pTexture;
} SpriteSource;

// Dynamically allocate a new sprite source object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Params:
//	 numCols = The number of columns in the sprite sheet.
//	 numRows = The number of rows in the sprite sheet.
//	 pTexture = A pointer to a texture that has been loaded by the Alpha Engine.
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
SpriteSourcePtr SpriteSourceCreate(int numCols, int numRows, AEGfxTexturePtr pTexture)
{
  SpriteSourcePtr newSpriteSource = calloc(1, sizeof(SpriteSource));

  if (newSpriteSource)
  {
    newSpriteSource->numCols = numCols;
    newSpriteSource->numRows = numRows;
    newSpriteSource->pTexture = pTexture;

    return newSpriteSource;
  }
  else
    return NULL;
}

// Free the memory associated with a sprite source object.
// (Also, set the sprite source pointer to NULL.)
// Params:
//	 spriteSource = Pointer to the sprite source pointer.
void SpriteSourceFree(SpriteSourcePtr* spriteSource)
{
  if (spriteSource == NULL)
    return;

  free(*spriteSource);
  *spriteSource = NULL;
}

// Returns a pointer to the texture, for the purposes of rendering a sprite.
// Params:
//	 spriteSource = Pointer to the sprite source object.
// Returns:
//	 If the sprite source pointer is valid,
//		then return a pointer to its texture,
//		else return NULL.
AEGfxTexturePtr SpriteSourceGetTexture(SpriteSourcePtr spriteSource)
{
  if (spriteSource == NULL)
    return NULL;

  return spriteSource->pTexture;
}

// Returns the maximum number of frames possible, given the dimensions of the sprite sheet.
// (Hint: Frame count = numCols * numRows.)
// Params:
//	 spriteSource = Pointer to the sprite source object.
// Returns:
//	 If the sprite source pointer is valid,
//		then return the calculated frame count,
//		else return 0.
unsigned int SpriteSourceGetFrameCount(SpriteSourcePtr spriteSource)
{
  if (spriteSource == NULL)
    return 0;

  return spriteSource->numCols * spriteSource->numRows;
}

// Returns the UV coordinates of the specified frame in a sprite sheet.
// (Hint: Refer to the Week 2 lecture slides for the correct calculations.)
// Params:
//	 spriteSource = Pointer to the sprite source object.
//	 frameIndex = A frame index within a sprite sheet.
//   u = Pointer to a f32 to contain the U coordinate.
//   v = Pointer to a f32 to contain the V coordinate.
void SpriteSourceGetUV(SpriteSourcePtr spriteSource, unsigned int frameIndex, float* u, float* v)
{
  if (spriteSource == NULL)
    return;

  *u = 1.f / spriteSource->numCols * (frameIndex % spriteSource->numCols);
  *v = 1.f / spriteSource->numRows * (frameIndex / spriteSource->numCols);
}
