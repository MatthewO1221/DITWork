//------------------------------------------------------------------------------
//
// File Name:	Sprite.c
// Author(s):	Rishabh Anand
// Project:		Project 2
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AeEngine.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "Vector2D.h"
#include "Transform.h"


//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:

typedef struct Sprite
{
	// The frame currently being displayed (for sprite sheets).
	unsigned int frameIndex;

	// The alpha transparency to use when drawing the sprite.
	float alpha;

	// The sprite source used when drawing (NULL = simple colored mesh).
	SpriteSourcePtr spriteSource;

	// The mesh used to draw the sprite.
	AEGfxVertexList* mesh;

} Sprite;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

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
	SpritePtr sprite = calloc(1, sizeof(Sprite));

	if (sprite)
	{
		sprite->alpha = 1.0f;
		AESysPrintf("Sprite Created\n");
		return sprite;
	}
	else
	{
		return NULL;
	}
}

// Free the memory associated with a sprite object.
// (Also, set the sprite pointer to NULL.)
// Params:
//	 sprite = Pointer to the sprite pointer.
void SpriteFree(SpritePtr* sprite)
{
	if (*sprite)
	{
		free(*sprite);

		*sprite = NULL;
	}
}

// Draw a sprite (Sprite can be textured or untextured).
// Params:
//	 sprite = Pointer to the sprite object.
//   transform = Pointer to the sprite object's transform.
void SpriteDraw(const Sprite* sprite, TransformPtr transform)
{
	if (!sprite)
		return;

	if (sprite->spriteSource)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		float u;
		float v;
		SpriteSourceGetUV(sprite->spriteSource, sprite->frameIndex, &u, &v);
		AEGfxTextureSet(SpriteSourceGetTexture(sprite->spriteSource), u, v);
	}
	else
	{
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	}
	AEGfxSetFullTransform(GetTranslationTransform(transform)->x, GetTranslationTransform(transform)->y, AERadToDeg(GetRotationTransform(transform)), GetScaleTransform(transform)->x, GetScaleTransform(transform)->y);
	AEGfxSetTransparency(sprite->alpha);
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxMeshDraw(sprite->mesh, AE_GFX_MDM_TRIANGLES);
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
	if (sprite)
	{
		return sprite->alpha;
	}
	else
	{
		return 0.0f;
	}
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
	AEClamp(newAlpha, 0.0f, 1.0f);
	sprite->alpha = newAlpha;
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
void SpriteSetFrame(SpritePtr sprite, unsigned int frameIndex)
{
	if (frameIndex <= SpriteSourceGetFrameCount(sprite->spriteSource))
	{
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
	sprite->mesh = mesh;
}

// Set a new SpriteSource for the specified sprite.
// (NOTE: The spriteSource parameter may be NULL.  This will remove an existing
//	texture from a sprite and cause the sprite to be displayed as a colored mesh.)
// Params:
//	 sprite = Pointer to the sprite to be modified.
//	 spriteSource = A new sprite source for the sprite (this pointer may be NULL).
void SpriteSetSpriteSource(SpritePtr sprite, SpriteSourcePtr spriteSource)
{
	sprite->spriteSource = spriteSource;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

