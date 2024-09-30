#include "stdafx.h"		// Should be included first in every .c file!
#include "Paint.h"
#include "Tile.h"
#include "Level.h"
#include "GameplayInstance.h"
#include "Mesh.h"
#include "MathHelper.h"
#include "Player Controller.h"
#include "Player.h"
#include "Audio.h"

static AEGfxVertexList* paintMesh;
static AEGfxTexture* paintTexture;

static PaintPixel* allPaint;

static int paintOffset;
static unsigned char brushSizeMultiplier;

// Init paint system.
void InitPaintSystem(void)
{
	// Setup paint callback event for changing paints.
	AESysAddCustomCallBack(WM_MOUSEWHEEL, GetMouseWheelPaint);

	// Create a paint mesh.
	AEGfxMeshStart();
	AEGfxTriAdd(
		-TILE_SIZE * 0.5F / PIXELS_PER_TILE, -TILE_SIZE * 0.5F / PIXELS_PER_TILE, 0xFFFF0000, 0.0f, 1.0f,
		TILE_SIZE * 0.5F / PIXELS_PER_TILE, -TILE_SIZE * 0.5F / PIXELS_PER_TILE, 0xFFFF0000, 0.0833f, 1.0f,
		-TILE_SIZE * 0.5F / PIXELS_PER_TILE, TILE_SIZE * 0.5F / PIXELS_PER_TILE, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		TILE_SIZE * 0.5F / PIXELS_PER_TILE, -TILE_SIZE * 0.5F / PIXELS_PER_TILE, 0xFFFF0000, 0.0833f, 1.0f,
		TILE_SIZE * 0.5F / PIXELS_PER_TILE, TILE_SIZE * 0.5F / PIXELS_PER_TILE, 0xFFFF0000, 0.0833f, 0.0f,
		-TILE_SIZE * 0.5F / PIXELS_PER_TILE, TILE_SIZE * 0.5F / PIXELS_PER_TILE, 0xFFFF0000, 0.0f, 0.0f);
	paintMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(paintMesh, "Failed to create Paint Mesh!");

	// Create paint colors.
	unsigned char paintColors[] = {
		167, 0, 0, 255,		// Red 1.
		255, 95, 95, 255,	// Red 2.
		83, 83, 255, 255,   // Blue 1.
		171, 171, 255, 255, // Blue 2.
		83, 175, 71, 255,	// Green 1.
		119, 255, 111, 255,	// Green 2.
		215, 187, 67, 255,	// Yellow 1.
		248, 223, 177, 255,	// Yellow 2.
		235, 111, 15, 255,	// Orange 1.
		255, 143, 59, 255,	// Orange 2.
		207, 0, 207, 255,	// Purple 1.
		255, 123, 255, 255	// Purple 2.
	};
	paintTexture = AEGfxTextureLoadFromMemory(paintColors, 12, 1);

	// Set default values.
	allPaint = calloc(PAINT_COUNT, sizeof(PaintPixel));

	paintOffset = 0;
	brushSizeMultiplier = 7;

	// Init new paint.
	if (allPaint)
	{
		for (int i = 0; i < PAINT_COUNT; ++i)
		{
			allPaint[i].paintColor = -1;
			allPaint[i].lifeLeft = 255;
		}
	}

	LoadSound("./Assets/SFX/paint_splash.mp3", sfx);
	
}

// Updates paint info.
void UpdatePaintSystem(void)
{
	// Allow user to switch paints by using the number row.
	if (AEInputCheckTriggered('1') &&
		GetPlayer()->unlockedColors & (1 << (PAINT_RED + 1)))
	{
		GetPlayer()->currColor = PAINT_RED;
	}
	else if (AEInputCheckTriggered('2') &&
		GetPlayer()->unlockedColors & (1 << (PAINT_BLUE + 1)))
	{
		GetPlayer()->currColor = PAINT_BLUE;
	}
	else if (AEInputCheckTriggered('3') &&
		GetPlayer()->unlockedColors & (1 << (PAINT_GREEN + 1)))
	{
		GetPlayer()->currColor = PAINT_GREEN;
	}
	else if (AEInputCheckTriggered('4') &&
		GetPlayer()->unlockedColors & (1 << (PAINT_YELLOW + 1)))
	{
		GetPlayer()->currColor = PAINT_YELLOW;
	}
	else if (AEInputCheckTriggered('5') &&
		GetPlayer()->unlockedColors & (1 << (PAINT_ORANGE + 1)))
	{
		GetPlayer()->currColor = PAINT_ORANGE;
	}
	else if (AEInputCheckTriggered('6') &&
		GetPlayer()->unlockedColors & (1 << (PAINT_PURPLE + 1)))
	{
		GetPlayer()->currColor = PAINT_PURPLE;
	}

	if (AEFrameRateControllerGetFrameCount() % 2 == 0)
	{
		for (int i = 0; i < PAINT_COUNT; ++i)
		{
			if (--(allPaint[i].lifeLeft) == 0)
			{
				RemovePaint(&allPaint[i]);
			}
		}
	}
}

// Shutdown paint system.
void ShutdownPaintSystem(void)
{
	// Free mesh.
	AEGfxMeshFree(paintMesh);

	// Unload texture.
	AEGfxTextureUnload(paintTexture);

	// Free memory for paint.
	free(allPaint);

	AESysRemoveCustomCallBack(WM_MOUSEWHEEL, GetMouseWheelPaint);
}

// Applies some paint in a formation to a location.
void ApplyPaintToLocation(unsigned int x, unsigned int y)
{
	if (GetPlayer()->currColor == PAINT_NONE ||
		GetPlayer()->paint <= 0)
		return;

	// Decrement paint.
	GetPlayer()->paint -= (float)AEFrameRateControllerGetFrameTime();
	GetPlayer()->paint = AEClamp(GetPlayer()->paint, 0, GetPlayer()->maxPaint);

	// Find area of brush and apply paint.
	int area = (int)(PI * brushSizeMultiplier * brushSizeMultiplier);
	for (float theta = 0; theta < area; ++theta)
	{
		float xAdd = 0, yAdd = 0;
		PolarToCartesian(theta, theta / 16.0F, &xAdd, &yAdd);

		int newX = (int)x + (int)xAdd;
		int newY = (int)y + (int)yAdd;

		int tileX = newX / 16;
		int tileY = newY / 16;

		ApplyPaintToPixel(newX, newY, tileX, tileY);
	}
}

// Applies paint over an entire tile.
void ApplyPaintToTile(unsigned char tileX, unsigned char tileY)
{
	// Exit if tile is air.
	if (GetCurrentTilemap()->tiles[tileX][tileY]->id == TILE_AIR)
		return;

	// Cover whole tile.
	for (unsigned char y = 0; y < PIXELS_PER_TILE; ++y)
	{
		for (unsigned char x = 0; x < PIXELS_PER_TILE; ++x)
		{
			// Color the pixel on the tile.
			ApplyPaintToPixel((unsigned int)tileX * PIXELS_PER_TILE + x, (unsigned int)tileY * PIXELS_PER_TILE + y, tileX, tileY);
		}
	}
}

// Applies paint to a single pixel.
void ApplyPaintToPixel(int x, int y, int tileX, int tileY)
{
	int newTileY = GetCurrentTilemap()->height - tileY + 1;

	if (tileX >= MAX_LEVEL_WIDTH ||
		newTileY >= MAX_LEVEL_HEIGHT ||
		tileX < 0 ||
		newTileY < 0 ||
		!GetCurrentTilemap()->tiles[tileX][newTileY]->canBePainted ||
		IsPaintAt(GetPlayer()->currColor, tileX, tileY, x % 16, y % 16))
		return;

	// Loop around at end of array.
	if (paintOffset + 1 == PAINT_COUNT)
	{
		paintOffset = 0;
		return;
	}

	// Iterate through paint.
	PaintPixel* paintToUse = &allPaint[paintOffset++];

	// Apply translated paint color.
	paintToUse->paintColor = (unsigned char)GetPlayer()->currColor;

	// Set position values.
	paintToUse->tileX = tileX;
	paintToUse->tileY = tileY;
	paintToUse->pixelX = (unsigned char)(x % 16);
	paintToUse->pixelY = (unsigned char)(y % 16);

	// Set the tile color.
	GetGame()->tileColors[tileX][newTileY] = GetPlayer()->currColor;

	// Set lifetime.
	paintToUse->lifeLeft = GetCurrentTilemap()->tiles[tileX][newTileY]->id == TILE_AIR ? 31 : 255;

	// Apply paint effect.
	if (GetCurrentTilemap()->tiles[tileX][newTileY]->paintColor == GetPlayer()->currColor &&
		GetCurrentTilemap()->tiles[tileX][newTileY]->onApplyPaint != NULL)
	{
		GetCurrentTilemap()->tiles[tileX][newTileY]->onApplyPaint(tileX, newTileY);
	}
}

// Removes paint from the entire level.
void RemoveAllPaint(void)
{
	// Cover whole tile.
	for (int i = 0; i < PAINT_COUNT; ++i)
	{
		RemovePaint(&allPaint[i]);
	}
}

// Removes paint from a single pixel.
void RemovePaint(PaintPixel* paint)
{
	int newTileY = GetCurrentTilemap()->height - paint->tileY + 1;
	GetGame()->tileColors[paint->tileX][newTileY] = PAINT_NONE;

	if (GetCurrentTilemap()->tiles[paint->tileX][newTileY]->onRemovePaint != NULL)
	{
		GetCurrentTilemap()->tiles[paint->tileX][newTileY]->onRemovePaint(paint->tileX, newTileY);
	}

	// Apply translated paint color.
	paint->paintColor = PAINT_NONE;

	// Set position values.
	paint->tileX = 0;
	paint->tileY = 0;
	paint->pixelX = 0;
	paint->pixelY = 0;

	// Set lifetime.
	paint->lifeLeft = 255;
}

// Recharges the paint meter.
void Recharge(void)
{
	// Increment paint.
	GetPlayer()->paint += (float)AEFrameRateControllerGetFrameTime() * 2.0F;
	GetPlayer()->paint = AEClamp(GetPlayer()->paint, 0, GetPlayer()->maxPaint);
	paintOffset = 0;
	
	for (int i = 0; i < PAINT_COUNT; ++i)
	{
		if (allPaint[i].lifeLeft != 0)
		{
			RemovePaint(&allPaint[i]);
			//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
		}
	}
}

// Unlocks a new paint
void UnlockPaint(enum PaintColor paint)
{
	// Sets bit for new paint.
	GetPlayer()->unlockedColors |= (paint + 1);

	// Automatically equip new paint.
	GetPlayer()->currColor = paint;

	PlaySFX("./Assets/SFX/paint_splash.mp3");
}

// Handles mouse wheel.
LRESULT GetMouseWheelPaint(HWND hWin, UINT msg, WPARAM wp, LPARAM lp)
{
	UNREFERENCED_PARAMETER(hWin);
	UNREFERENCED_PARAMETER(msg);
	UNREFERENCED_PARAMETER(lp);

	// Switch behavior based on whether you're scrolling up or down.
	switch (wp)
	{
		// Scroll up.
		case 0x00780000:
			SwapPaint(1);
			break;

		// Scroll down.
		case 0xFF880000:
			SwapPaint(-1);
			break;
	}

	// I guess it does need to return something?
	LONG_PTR l = 0;

	// Function doesn't need to return anything.
	return l;
}

// Swaps current paint in direction.
enum PaintColor SwapPaint(int direction)
{
	if (GetPlayer()->unlockedColors == 0 || 
		GetPlayer()->unlockedColors == 1)
		return PAINT_NONE;

	// Increment paint in direction.
	enum PaintColor newPaint = (GetPlayer()->currColor + direction) % PAINT_COLOR_NUM;

	// Set new paint.
	GetPlayer()->currColor = newPaint;
	if (!(GetPlayer()->unlockedColors & (1 << (newPaint + 1))))
	{
		// If player doesn't have paint, keep going until a paint they have that's unlocked is found.
		return SwapPaint(direction);
	}

	AESysPrintf("Current Paint: %d\n", GetPlayer()->currColor);
	return GetPlayer()->currColor;
}

// Draws all paint in a level.
void DrawAllPaint(void)
{
	// Iterate through all the paint in the level.
	for (int i = 0; i < PAINT_COUNT; ++i)
	{
		// Draw the paint at its appropriate location.
		DrawPaint(&allPaint[i]);
	}
}

// Draws a single paint pixel.
void DrawPaint(PaintPixel* paint)
{
	if (paint->paintColor == PAINT_NONE)
		return;

	// Set render mode.
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	Vector2D drawPos =
	{
		.x = (paint->tileX * TILE_SIZE + paint->pixelX * PIXEL_SIZE) - 768.0F,
		.y = (paint->tileY * TILE_SIZE + paint->pixelY * PIXEL_SIZE) - 448.0F
	};
	
	// Set the position.
	AEGfxSetPosition(drawPos.x - 32, drawPos.y - 32);

	// Determine paint pattern.
	int edgeOffset = 0;
	switch (paint->paintColor)
	{
		case PAINT_RED:
			edgeOffset = RedPattern(paint->pixelX, paint->pixelY);
			break;

		case PAINT_BLUE:
			edgeOffset = BluePattern(paint->pixelX, paint->pixelY);
			break;

		case PAINT_ORANGE:
			edgeOffset = OrangePattern(paint->pixelX, paint->pixelY);
			break;

		case PAINT_GREEN:
			edgeOffset = GreenPattern(paint->pixelX, paint->pixelY);
			break;

		case PAINT_PURPLE:
			edgeOffset = PurplePattern(paint->pixelX, paint->pixelY);
			break;

		case PAINT_YELLOW:
			edgeOffset = YellowPattern(paint->pixelX, paint->pixelY);
			break;
	}

	// Render the texture.
	float textureOffsetX = paint->paintColor * 0.1666F;
	float textureOffsetY = 1.0F;
	AEGfxTextureSet(paintTexture, textureOffsetX + edgeOffset * 0.08333F, textureOffsetY);

	// Set transparency.
	AEGfxSetTransparency((float)paint->lifeLeft / 255);

	// Draw the mesh.
	AEGfxMeshDraw(paintMesh, AE_GFX_MDM_TRIANGLES);
}

// Convert a screen position to a pixel position.
void ScreenToPixelPos(long screenPosX, long screenPosY, unsigned short* pixelPosX, unsigned short* pixelPosY)
{
	// Translate values to snap to a grid.
	*pixelPosX = (unsigned char)(screenPosX / PIXEL_SIZE);
	*pixelPosY = (unsigned char)(-screenPosY / PIXEL_SIZE);
}

// Returns whether or not (x, y) already has paint applied.
unsigned char IsPaintAt(enum PaintColor color, unsigned char tileX, unsigned char tileY, unsigned short pixelX, unsigned short pixelY)
{
	for (int i = 0; i < PAINT_COUNT; ++i)
	{
		if (allPaint[i].tileX == tileX &&
			allPaint[i].tileY == tileY &&
			allPaint[i].pixelX == pixelX &&
			allPaint[i].pixelY == pixelY &&
			allPaint[i].lifeLeft > 30 &&
			allPaint[i].paintColor == color)
			return 1;
	}

	return 0;
}

// Pattern for red paint.
unsigned char RedPattern(unsigned char x, unsigned char y)
{
	return x % 4 == y % 4;
}

// Pattern for blue paint.
unsigned char BluePattern(unsigned char x, unsigned char y)
{
	UNREFERENCED_PARAMETER(x);

	return y % 4 == 0;
}

// Pattern for green paint.
unsigned char GreenPattern(unsigned char x, unsigned char y)
{
	return x % 2 == 1 && y % 2 == 1;
}

// Pattern for yellow paint.
unsigned char YellowPattern(unsigned char x, unsigned char y)
{
	UNREFERENCED_PARAMETER(y);

	return x % 4 == 0;
}

// Pattern for orange paint.
unsigned char OrangePattern(unsigned char x, unsigned char y)
{
	return x % 4 == (16 - y) % 4;
}

// Pattern for purple paint.
unsigned char PurplePattern(unsigned char x, unsigned char y)
{
	return x % 4 == 0 || y % 4 == 0;
}

// Increase the max paint count.
void IncreaseMaxPaint(float amount)
{
	GetPlayer()->maxPaint += amount;
	GetPlayer()->paint = GetPlayer()->maxPaint;
	PlaySFX("./Assets/SFX/paint_splash.mp3");
}

// Sets the brush's size.
void SetBrushSize(unsigned char size)
{
	brushSizeMultiplier = size;
}
