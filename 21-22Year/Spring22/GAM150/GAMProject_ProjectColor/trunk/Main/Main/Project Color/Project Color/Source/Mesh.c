//------------------------------------------------------------------------------
//
// File Name:	Mesh.c
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

#include "stdafx.h"
#include "Mesh.h"
#include "Matrix2D.h"
#include "SpriteSource.h"
#include "AEEngine.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a quadrilateral mesh using the Alpha Engine.
// Params:
//	 xHalfSize = The X half-size of the mesh.
//	 yHalfSize = The Y half-size of the mesh.
//   uSize = The U size of the mesh, relative to texture coordinates (0.0 .. 1.0).
//   vSize = The V size of the mesh, relative to texture coordinates (0.0 .. 1.0).
//	 name = A name for the mesh.  Currently used only for error messaging.
// Returns:
//	 If the mesh was created successfully,
//	   then return a pointer to the created mesh,
//	   else return NULL.
AEGfxVertexList* MeshCreateQuad(float xHalfSize, float yHalfSize, float uSize, float vSize, const char* name)
{
  AEGfxMeshStart();
  AEGfxTriAdd(
    xHalfSize * -1.f, yHalfSize * -1.f, 0xFFFFFFFF, 0.f, vSize,
    xHalfSize, yHalfSize * -1.f, 0xFFFFFFFF, uSize, vSize,
    xHalfSize * -1.f, yHalfSize, 0xFFFFFFFF, 0.f, 0.f);
  AEGfxTriAdd(
    xHalfSize, yHalfSize * -1.f, 0xFFFFFFFF, uSize, vSize,
    xHalfSize, yHalfSize, 0xFFFFFFFF, uSize, 0.f,
    xHalfSize * -1.f, yHalfSize, 0xFFFFFFFF, 0.f, 0.f);
  AEGfxVertexList* nuMsh = AEGfxMeshEnd();
  AE_ASSERT_MESG(nuMsh, "Failed to create mesh %s!", name);
  if (nuMsh != NULL)
    return nuMsh;
  return NULL;
}

// Render a mesh (textured or untextured) using the Alpha Engine.
// Params:
//	 mesh = Pointer to the mesh to be rendered.
//	 spriteSource = Pointer to sprite source used for rendering (this may be NULL).
//	 transform = The transformation matrix used to scale, rotate, and translate the sprite.
//	 frameIndex = The index into a sprite sheet; used for calculating UV offsets.
void MeshRender(AEGfxVertexList* mesh, SpriteSourcePtr spriteSource, Matrix2D* transform, unsigned frameIndex)
{
  if (mesh == NULL || transform == NULL || frameIndex < 0)
    return;
  if (spriteSource != NULL)  // the sprite has an actual graphic
  {
    float u, v;
    SpriteSourceGetUV(spriteSource, frameIndex, &u, &v);
    AEGfxTextureSet(SpriteSourceGetTexture(spriteSource), u, v);
  }
  AEGfxSetTransform(transform->m);
  AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}
