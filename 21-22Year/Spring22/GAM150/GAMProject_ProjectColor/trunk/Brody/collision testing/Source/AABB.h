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

enum collision_direction{Left, Right, Top, Bottom};
						 //0     1     2      3

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

/**
 * @brief Creates an AABB collision box from mesh, and returns a pointer to AABB struct
 *
 * @param tile pointer to tile data
 */
AABB CreateAABBFromTile(tileInst tile);

/**
 * @brief Takes a tileInst, updates its collision based on vertex & position data
 *
 * @param tile pointer to tile data
 */
void UpdateCollisionPos(tileInst tile);

/**
 * @brief Checks if a point is inside an AABB
 *
 * @param col aabb data
 * @param _x: x position
 * @param _y: y position
 */
int AABBPointContained(tileInst tile, float _x, float _y);


/**
 * @brief Checks if secondary AABB is inside prime AABB
 *
 * @param tile1: primary tileInst
 * @param tile2: secondary tileInst
 */
int CheckAABBCollision(tileInst prime, tileInst second);




//AEFrameRateControllerGetFrameTime

