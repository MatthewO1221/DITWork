#include "stdafx.h"		// Should be included first in every .c file!
#include "Camera.h"
#include "Vector2D.h"
#include "Tile.h"
#include "Level.h"
#include "MathHelper.h"
#include "Mesh.h"

AEGfxTexture* cameraTexture;
AEGfxVertexList* cameraMesh;

static Vector2D oldCamPos;
static Vector2D targetCamPos;

static int isClamped;

// Initializes memory used for the camera.
void InitCamera(void)
{
	cameraTexture = AEGfxTextureLoad("./Assets/Tiles/camera_lock.png");
	cameraMesh = MeshCreateQuad(0.5F, 0.5F, 1.0F, 1.0F, "Camera Mesh");
}

// Updates the camera's position.
void UpdateCamera(void)
{
	// Move the camera.
	FollowTarget(&targetCamPos, isClamped);
}

// Shutsdown and frees memory uesd by the camera.
void ShutdownCamera(void)
{
	AEGfxTextureUnload(cameraTexture);
	AEGfxMeshFree(cameraMesh);
}

// Renders the camera target position.
void DrawCamera(void)
{
	// Set render mode.
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	// Set the position.
	AEGfxSetFullTransform(targetCamPos.x, targetCamPos.y, 0, TILE_SIZE, TILE_SIZE);

	// Render the texture.
	AEGfxTextureSet(cameraTexture, 0.0F, 0.0F);

	// Set transparency.
	AEGfxSetTransparency(0.75F);

	// Draw the mesh.
	AEGfxMeshDraw(cameraMesh, AE_GFX_MDM_TRIANGLES);
}

// Update camera position.
void FollowTarget(const Vector2D* pos, int isClamped)
{
	float dt = (float)AEFrameRateControllerGetFrameTime();

	// Get max values for level based on tilemap size.
	float maxX = (GetCurrentTilemap()->width - MIN_LEVEL_WIDTH) * (float)TILE_SIZE + 32;
	float maxY = (GetCurrentTilemap()->height - MIN_LEVEL_HEIGHT) * (float)TILE_SIZE + 32;

	// Get the camera's current position (for lerping).
	float oldCamX = 0, oldCamY = 0;
	AEGfxGetCamPosition(&oldCamX, &oldCamY);

	// Change logic to either clamp or not clamp the camera.
	float camX = 0, camY = 0;
	if (isClamped)
	{
		// Lerp the camera's position with clamping.
		camX = Lerp(oldCamX, AEClamp(pos->x, -32, maxX), dt * CAMERA_SPEED);
		camY = Lerp(oldCamY, AEClamp(pos->y, 32.01F, maxY + 64.01F), dt * CAMERA_SPEED);

		// Apply camera position with clamping.
		AEGfxSetCamPosition(AEClamp(camX, -32, maxX), AEClamp(camY, 32.01F, maxY + 64.01F));
	}
	else
	{
		// Lerp the camera's position without clamping.
		camX = Lerp(oldCamX, pos->x, dt * CAMERA_SPEED);
		camY = Lerp(oldCamY, pos->y, dt * CAMERA_SPEED);

		// Apply camera position without clamping.
		AEGfxSetCamPosition(camX, camY);
	}
}

// Returns the camera's position.
Vector2D GetCamPos(void)
{
	Vector2D camPos = { 0, 0 };
	AEGfxGetCamPosition(&camPos.x, &camPos.y);

	return camPos;
}

// Sets the camera's position.
void SetCamPos(const Vector2D* pos, int isClamped)
{
	// Get max values for level based on tilemap size.
	float maxX = (GetCurrentTilemap()->width - MIN_LEVEL_WIDTH) * (float)TILE_SIZE + 32;
	float maxY = (GetCurrentTilemap()->height - MIN_LEVEL_HEIGHT) * (float)TILE_SIZE + 32;

	// Change logic to either clamp or not clamp the camera.
	float camX = 0, camY = 0;
	if (isClamped)
	{
		// Lerp the camera's position with clamping.
		camX = Lerp(0, AEClamp(pos->x, -32, maxX), 1.0F);
		camY = Lerp(0, AEClamp(pos->y, 32, maxY + 64), 1.0F);

		// Apply camera position with clamping.
		AEGfxSetCamPosition(AEClamp(camX, -32, maxX), AEClamp(camY, 32, maxY + 64));
	}
	else
	{
		// Lerp the camera's position without clamping.
		camX = Lerp(0, pos->x, 1.0F);
		camY = Lerp(0, pos->y, 1.0F);

		// Apply camera position without clamping.
		AEGfxSetCamPosition(camX, camY);
	}
}

// Sets the X position of the camera.
void SetCamX(float x)
{
	AEGfxSetCamPosition(x, GetCamPos().y);
}

// Sets the Y position of the camera.
void SetCamY(float y)
{
	AEGfxSetCamPosition(GetCamPos().x, y);
}

// Returns the camera's old position.
struct Vector2D GetOldCamPos(void)
{
	return oldCamPos;
}

// Sets the camera's old position.
void SetOldCamPos(const Vector2D* position)
{
	oldCamPos = *position;
}

// Sets the old X position of the camera.
void SetOldCamX(float x)
{
	oldCamPos.x = x;
}

// Sets the old Y position of the camera.
void SetOldCamY(float y)
{
	oldCamPos.y = y;
}

// Returns the camera's target position.
struct Vector2D GetTargetCamPos(void)
{
	return targetCamPos;
}

// Sets the camera's target position.
void SetTargetCamPos(const Vector2D* position)
{
	targetCamPos = *position;
}

// Sets the target X position of the camera.
void SetTargetCamX(float x)
{
	targetCamPos.x = x;
}

// Sets the target Y position of the camera.
void SetTargetCamY(float y)
{
	targetCamPos.y = y;
}

// Sets the camera to be clamped.
void ClampCamera(int clamp)
{
	isClamped = clamp;
}
