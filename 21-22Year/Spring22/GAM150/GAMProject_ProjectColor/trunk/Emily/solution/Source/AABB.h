// ---------------------------------------------------------------------------
// Project Name		:	project
// File Name		:	AABB.h
// Author			:	Brody Endstrasser
// Creation Date	:	01-21-22
// Purpose			:	Demonstrates Alpha Engine features.
// History			:
// - 2012/01/21		:	- initial implementation
// - 
// ---------------------------------------------------------------------------

struct collisionData {

	float left;
	float right;
	float top;
	float bottom;

};

typedef struct collisionData* AABB;

struct tileData {
	float vx1, vy1;
	float vx2, vy2;
	float vx3, vy3;
	float vx4, vy4;
	float xpos, ypos;

	AEGfxVertexList* Mesh;
	AABB aabb;

	//pointer to player struct

};

typedef struct tileData* tileInst;

enum collision_direction{Not, Left, Right, Top, Bottom};
						//0    1     2      3     4

/**
 * @brief Creates a tile instance by adding two tris to mesh, as well as creating a tileInst struct
 *
 * @param Mesh: pointer to mesh data
 * @param width: width of tile
 * @param height: height of tile
 */
tileInst DefineSquareMesh(AEGfxVertexList* mesh, float width, float height);

/**
 * @brief Frees a tile inst
 *
 * @param Mesh pointer to tile inst
 */
void FreeTileInst(tileInst tile);

void FreeAABB(AABB aabb);

/**
 * @brief Creates an AABB collision box from mesh, and returns a pointer to AABB struct
 *
 * @param tile pointer to tile data
 */
AABB CreateAABBFromTile(tileInst tile);

/**
 * @brief Creates an AABB collision box from 4 sides, and returns a pointer to AABB struct
 *
 * @param left
 * @param right
 * @param top
 * @param bottom
 */
AABB FromPoints(float left, float right, float top, float bottom);

/**
 * @brief Takes a tileInst, updates its collision based on vertex & position data
 *
 * @param tile pointer to tile data
 */
void UpdateTileCollision(tileInst tile);

/**
 * @brief Checks if a point is inside an AABB
 *
 * @param col aabb data
 * @param _x: x position
 * @param _y: y position
 */
int AABBPointContained(AABB col, float _x, float _y);


/**
 * @brief Checks if 2 AABBs are colliding
 *
 * @param tile1: first AABB
 * @param tile2: second AABB
 */
int CheckAABBCollision(AABB t1, AABB t2);

/**
 * @brief if 2 AABBs are colliding, returns enum collision direction that indicates
 * in which direction t is colliding with prime
 *
 * @param prime: first AABB
 * @param t: second AABB
 */
enum collision_direction CheckCollisionDirect(AABB prime, AABB t);






//AEFrameRateControllerGetFrameTime

