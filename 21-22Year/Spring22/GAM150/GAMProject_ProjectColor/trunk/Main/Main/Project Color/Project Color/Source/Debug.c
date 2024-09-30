#include "stdafx.h"		// Should be included first in every .c file!
#include "Debug.h"
#include "GameplayInstance.h"
#include "Tile.h"
#include "Mesh.h"
#include "AABB.h"

static AEGfxTexture* boundingBoxTexture;
static AEGfxVertexList* boundingBoxMesh;

static int isDebugging = 0;

// Define bounding box sprite.
void SetBoundingBox(void)
{
	// Load texture.
	boundingBoxTexture = AEGfxTextureLoad("./Assets/bounding_box.png");

	// Create mesh.
	boundingBoxMesh = MeshCreateQuad(0.5F, 0.5F, 1.0F, 1.0F, "Bounding Box");
}

// Free bounding box sprite.
void FreeBoundingBox(void)
{
	// Free texture.
	AEGfxTextureUnload(boundingBoxTexture);

	// Free mesh.
	AEGfxMeshFree(boundingBoxMesh);
}

// Return bounding box sprite.
AEGfxTexture* GetBoundingBox(void)
{
	return boundingBoxTexture;
}

// Draws an AABB for a tile.
void DrawBoundingBox(AABB collider)
{
	if (collider == NULL)
		return;

	// Set render mode.
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	// Set the position.
	AEGfxSetFullTransform(collider->WorldPos.x + collider->xOff,
						  collider->WorldPos.y + collider->yOff,
						   0,
						  collider->width,
						  collider->height);


	// Render the texture.
	AEGfxTextureSet(boundingBoxTexture, 0.0F, 0.0F);

	// Set transparency.
	AEGfxSetTransparency(0.5F);

	// Draw the mesh.
	AEGfxMeshDraw(boundingBoxMesh, AE_GFX_MDM_TRIANGLES);
}
