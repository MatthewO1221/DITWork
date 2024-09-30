#include "AEEngine.h"

#define PIXEL_SIZE 4
#define PAINT_COUNT 8192

struct GameplayInstance;
enum PaintColor
{
	PAINT_NONE = -1, PAINT_RED, PAINT_GREEN, PAINT_ORANGE, PAINT_BLUE, PAINT_YELLOW, PAINT_PURPLE
};

typedef struct PaintPixel
{
	// Color of paint.
	// Needs to be converted to enum PaintColor.
	// -1 = no paint (default).
	char paintColor;

	// Tile position on tilemap that paint is on.
	unsigned char tileX, tileY;

	// Pixel pixel on tile that paint is on.
	unsigned char pixelX, pixelY;

	// How long the paint will last for.
	unsigned char lifeLeft;
} PaintPixel;

void InitPaint(PaintPixel*);
void ShutdownPaint(PaintPixel*);
void ApplyPaintToLocation(PaintPixel[], struct GameplayInstance*, enum PaintColor, unsigned short, unsigned short, unsigned char, int*);
void ApplyPaintToTile(PaintPixel[], struct GameplayInstance*, enum PaintColor, unsigned char, unsigned char, int*);
void ApplyPaintToPixel(PaintPixel[], struct GameplayInstance*, enum PaintColor, unsigned short, unsigned short, int*);
void RemovePaintFromTile(PaintPixel[], struct GameplayInstance*, unsigned char, unsigned char);
void RemovePaintFromPixel(PaintPixel*);
void UpdatePaint(PaintPixel[], struct GameplayInstance*);
void DrawAllPaint(PaintPixel[], AEGfxVertexList*, AEGfxTexture*);
void DrawPaintPixel(PaintPixel*, AEGfxVertexList*, AEGfxTexture*);
void ScreenToPixelPos(long, long, unsigned short*, unsigned short*);
void PolarToCartesian(float, float, float*, float*);
unsigned char IsPaintAt(PaintPixel[], enum PaintColor, unsigned short, unsigned short);
