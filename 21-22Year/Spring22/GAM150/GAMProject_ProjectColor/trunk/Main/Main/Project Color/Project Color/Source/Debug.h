#include "AEEngine.h"

#define DEBUG_MODE 0

typedef struct collisionData* AABB;

// Define bounding box sprite.
void SetBoundingBox(void);

// Free bounding box sprite.
void FreeBoundingBox(void);

// Return bounding box sprite.
AEGfxTexture* GetBoundingBox(void);

// Draws an AABB for a tile.
void DrawBoundingBox(AABB);
