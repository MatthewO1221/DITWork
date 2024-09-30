#include "stdafx.h"		// Should be included first in every .c file!
#include "paint.h"
#include "tilemap.h"

void InitPaint(PaintPixel* allPaint)
{
	PaintPixel newPaint =
	{
		.paintColor = -1,

		.pixelX = 0,
		.pixelY = 0,
		.tileX = 0,
		.tileY = 0,

		.lifeLeft = 255
	};

	for (int i = 0; i < PAINT_COUNT; ++i)
	{
		*(allPaint + i) = newPaint;
	}
}

void ShutdownPaint(PaintPixel* allPaint)
{
	// Free memory for paint.
	free(allPaint);
}

void ApplyPaintToLocation(PaintPixel allPaint[], GameplayInstance* game, enum PaintColor color, unsigned short x, unsigned short y, unsigned char radius, int *paintOffset)
{
	UNREFERENCED_PARAMETER(game);

	unsigned short area = (unsigned short)(PI * radius * radius);
	for (unsigned short theta = 0; theta < area; ++theta)
	{
		float xAdd;
		float yAdd;
		PolarToCartesian(theta, theta / 16.0F, &xAdd, &yAdd);

		ApplyPaintToPixel(allPaint, game, color, x + (unsigned short)xAdd, y + (unsigned short)yAdd, paintOffset);
	}
}
void ApplyPaintToTile(PaintPixel allPaint[], GameplayInstance* game, enum PaintColor color, unsigned char tileX, unsigned char tileY, int* paintOffset)
{
	// Exit if tile is air.
	if (game->level->tilemap->tiles[tileX][tileY]->id == 0)
		return;

	// Cover whole tile.
	for (unsigned char y = 0; y < PIXELS_PER_TILE; ++y)
	{
		for (unsigned char x = 0; x < PIXELS_PER_TILE; ++x)
		{
			// Color the pixel on the tile.
			ApplyPaintToPixel(allPaint, game, color, (unsigned short)tileX * PIXELS_PER_TILE + x, (unsigned short)tileY * PIXELS_PER_TILE + y, paintOffset);
		}
	}
}
void ApplyPaintToPixel(PaintPixel allPaint[], GameplayInstance* game, enum PaintColor color, unsigned short x, unsigned short y, int *paintOffset)
{
	// Get position of tile.
	unsigned char tileX = (unsigned char)(x / 16);
	unsigned char tileY = (unsigned char)(15 - y / 16);

	// Exit early if tile is air or pixel is already painted.
	if ((tileX >= 0 &&
		 tileX <= TILEMAP_WIDTH &&
		 tileY >= 0 &&
		 tileY <= TILEMAP_HEIGHT &&
		 game->level->tilemap->tiles[tileX][tileY]->id == 0) ||
		 IsPaintAt(allPaint, color, x, y))
		return;

	// Iterate through paint.
	PaintPixel* paintToUse = &allPaint[*paintOffset];

	// Loop around at end of array.
	if ((*paintOffset)++ == PAINT_COUNT - 1)
	{
		*paintOffset = 0;
	}

	// Apply translated paint color.
	paintToUse->paintColor = (unsigned char)color;

	// Set position values.
	paintToUse->tileX = tileX;
	paintToUse->tileY = tileY;
	paintToUse->pixelX = (unsigned char)(x % 16);
	paintToUse->pixelY = (unsigned char)(y % 16);

	// Set lifetime.
	paintToUse->lifeLeft = 255;

	if (game->level->tilemap->tiles[tileX][tileY]->paintColor == color &&
		game->level->tilemap->tiles[tileX][tileY]->onApplyPaint != NULL)
	{
		game->level->tilemap->tiles[tileX][tileY]->onApplyPaint(game, tileX, tileY);
	}
}

void RemovePaintFromTile(PaintPixel allPaint[], GameplayInstance* game, unsigned char tileX, unsigned char tileY)
{
	// Exit if tile is air.
	if (game->level->tilemap->tiles[tileX][tileY]->id == 0)
		return;

	// Cover whole tile.
	for (unsigned char y = 0; y < PIXELS_PER_TILE; ++y)
	{
		for (unsigned char x = 0; x < PIXELS_PER_TILE; ++x)
		{
			// Color the pixel on the tile.
			RemovePaintFromPixel(allPaint);
		}
	}
}
void RemovePaintFromPixel(PaintPixel* paint)
{
	// Apply translated paint color.
	paint->paintColor = -1;

	// Set position values.
	paint->tileX = 0;
	paint->tileY = 0;
	paint->pixelX = 0;
	paint->pixelY = 0;

	// Set lifetime.
	paint->lifeLeft = 255;
}

void UpdatePaint(PaintPixel allPaint[], GameplayInstance* game)
{
	for (int i = 0; i < PAINT_COUNT; ++i)
	{
		if (--((allPaint + i)->lifeLeft) <= 0)
		{
			if (game->level->tilemap->tiles[(allPaint + i)->tileX][(allPaint + i)->tileY]->onRemovePaint != NULL)
			{
				game->level->tilemap->tiles[(allPaint + i)->tileX][(allPaint + i)->tileY]->onRemovePaint(game, (allPaint + i)->tileX, (allPaint + i)->tileY);
			}

			(allPaint + i)->paintColor = -1;

			(allPaint + i)->pixelX = 0;
			(allPaint + i)->pixelY = 0;
			(allPaint + i)->tileX = 0;
			(allPaint + i)->tileY = 0;

			(allPaint + i)->lifeLeft = 255;
		}
	}
}

void DrawAllPaint(PaintPixel allPaint[], AEGfxVertexList* mesh, AEGfxTexture* texture)
{
	// Iterate through all the paint in the level.
	for (int i = 0; i < PAINT_COUNT; ++i)
	{
		// Draw the paint at its appropriate location.
		DrawPaintPixel(&allPaint[i], mesh, texture);
	}
}
void DrawPaintPixel(PaintPixel* paint, AEGfxVertexList* mesh, AEGfxTexture* texture)
{
	// Set render mode.
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	// Set the position.
	long xPos, yPos;
	TileToScreenPos(paint->tileX, paint->tileY, &xPos, &yPos);
	AEGfxSetPosition((float)(xPos + (paint->pixelX * PIXEL_SIZE) - TILE_SIZE * 0.5F) + 2, (float)(yPos + (paint->pixelY * PIXEL_SIZE) - TILE_SIZE * 0.5F) + 2);

	// Render the texture.
	float edgeOffset = (paint->pixelX % 2 == paint->pixelY % 2) * 0.08333F;
	float textureOffsetX = paint->paintColor * 0.1666F;
	float textureOffsetY = 1.0F;
	AEGfxTextureSet(texture, textureOffsetX + edgeOffset, textureOffsetY);

	// Set transparency.
	AEGfxSetTransparency((float)paint->lifeLeft / 380);

	// Draw the mesh.
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}

// Convert a screen position to a pixel position.
void ScreenToPixelPos(long screenPosX, long screenPosY, unsigned short* pixelPosX, unsigned short* pixelPosY)
{
	// Translate values to snap to a grid.
	*pixelPosX = (unsigned char)(screenPosX / PIXEL_SIZE);
	*pixelPosY = (unsigned char)(-screenPosY / PIXEL_SIZE);
}

// Convert a polar equation to a cartesian equation.
void PolarToCartesian(float theta, float r, float* x, float* y)
{
	*x = r * cosf(theta);
	*y = r * sinf(theta);
}

// Find a better way to do this.
unsigned char IsPaintAt(PaintPixel allPaint[], enum PaintColor color, unsigned short x, unsigned short y)
{
	UNREFERENCED_PARAMETER(allPaint);
	UNREFERENCED_PARAMETER(color);
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
	return 0;
}
