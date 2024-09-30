//------------------------------------------------------------------------------
//
// File Name:	Mesh.c
// Author(s):	Rishabh Anand
// Project:		Project 2
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AeEngine.h"
#include "Mesh.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
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
	AEGfxVertexList* pMesh;
	
	AEGfxMeshStart();

	AEGfxTriAdd(
		-xHalfSize, -yHalfSize, 0xFFFFFFF, 0.0f, vSize,
		xHalfSize, -yHalfSize, 0xFFFFFFFF, uSize, vSize,
		-xHalfSize, yHalfSize, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		xHalfSize, -yHalfSize, 0xFFFFFFFF, uSize, vSize,
		xHalfSize, yHalfSize, 0xFFFFFFFF, uSize, 0.0f,
		-xHalfSize, yHalfSize, 0xFFFFFFFF, 0.0f, 0.0f);

	pMesh = AEGfxMeshEnd();

	if (pMesh)
	{
		return pMesh;
	}
	else
	{
		AESysPrintf("Mesh %s failed to load\n", name);
		return NULL;
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

