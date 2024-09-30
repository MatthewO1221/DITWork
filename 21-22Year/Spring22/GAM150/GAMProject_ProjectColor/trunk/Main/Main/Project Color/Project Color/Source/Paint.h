#include "AEEngine.h"

#define PIXEL_SIZE 4
#define PAINT_COUNT 4096
#define PAINT_COLOR_NUM 6

enum PaintColor
{
	PAINT_NONE = -1, PAINT_RED, PAINT_BLUE, PAINT_GREEN, PAINT_YELLOW, PAINT_ORANGE, PAINT_PURPLE
};

// Paint pixel definition. Stores data about a drop of paint.
typedef struct PaintPixel
{
	// Color of paint.
	char paintColor;

	// Tile position on tilemap that paint is on.
	unsigned char tileX, tileY;

	// Pixel pixel on tile that paint is on.
	unsigned char pixelX, pixelY;

	// How long the paint will last for.
	unsigned char lifeLeft;
} PaintPixel;

// Init paint system.
void InitPaintSystem(void);

// Updates paint info.
void UpdatePaintSystem(void);

// Shutdown paint system.
void ShutdownPaintSystem(void);

// Applies some paint in a formation to a location.
void ApplyPaintToLocation(unsigned int, unsigned int);

// Applies paint over an entire tile.
void ApplyPaintToTile(unsigned char, unsigned char);

// Applies paint to a single pixel.
void ApplyPaintToPixel(int, int, int, int);

// Removes paint from the entire level.
void RemoveAllPaint(void);

// Removes paint from a single pixel.
void RemovePaint(struct PaintPixel*);

// Recharges the paint meter.
void Recharge(void);

// Unlocks a new paint
void UnlockPaint(enum PaintColor);

// Handles mouse wheel.
LRESULT GetMouseWheelPaint(HWND, UINT, WPARAM, LPARAM);

// Swaps current paint in direction.
enum PaintColor SwapPaint(int);

// Draws all paint in a level.
void DrawAllPaint(void);

// Draws a single paint pixel.
void DrawPaint(struct PaintPixel*);

// Convert a screen position to a pixel position.
void ScreenToPixelPos(long, long, unsigned short*, unsigned short*);

// Returns whether or not (x, y) already has paint applied.
unsigned char IsPaintAt(enum PaintColor, unsigned char, unsigned char, unsigned short, unsigned short);

// Pattern for red paint.
unsigned char RedPattern(unsigned char, unsigned char);

// Pattern for blue paint.
unsigned char BluePattern(unsigned char, unsigned char);

// Pattern for green paint.
unsigned char GreenPattern(unsigned char, unsigned char);

// Pattern for yellow paint.
unsigned char YellowPattern(unsigned char, unsigned char);

// Pattern for orange paint.
unsigned char OrangePattern(unsigned char, unsigned char);

// Pattern for purple paint.
unsigned char PurplePattern(unsigned char, unsigned char);

// Increase the max paint count.
void IncreaseMaxPaint(float);

// Sets the brush's size.
void SetBrushSize(unsigned char);
