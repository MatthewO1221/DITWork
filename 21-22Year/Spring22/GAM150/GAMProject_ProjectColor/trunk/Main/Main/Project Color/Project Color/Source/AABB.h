// ---------------------------------------------------------------------------
// Project Name		:	project
// File Name		:	AABB.h
// Author			:	Brody Endstrasser
// Creation Date	:	01-21-22
// Purpose			:	Defines functions used to calculate and implement collisions
// History			:
// - 2012/01/21		:	- initial implementation
// - 
// ---------------------------------------------------------------------------

#include "Vector2D.h"
#include "Transform.h"
#include "Physics.h"

#pragma once

typedef struct GameObject *GameObjectPtr;

#define BOX_SIZE 20

struct collisionData {

	//top left point!!
	Vector2D WorldPos;
	float width, height;
	float xOff, yOff;
	bool isTrigger;
	
	//float left;
	//float right;
	//float top;
	//float bottom;

};

typedef struct collisionData* AABB;

//the tileData struct is only for testing, do not use
struct tileData {
	float vx1, vy1;
	float vx2, vy2;
	float vx3, vy3;
	float vx4, vy4;
	float xpos, ypos;

	AEGfxVertexList* Mesh;
	AABB aabb;

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

/**
 * @brief Frees an AABB, and returns an AABB null pointer
 *format
 * @param pointer to aabb
 */
AABB FreeAABB(AABB aabb);

/**
 * @brief Creates an AABB collision box from mesh, and returns a pointer to AABB struct
 *
 * @param tile pointer to tile data
 */
//AABB CreateAABBFromTile(tileInst tile);

void updateAABBPosition(AABB aabb, PhysicsPtr trans);

/**
 * @brief Creates an AABB collision box from 4 sides, and returns a pointer to AABB struct
 *
 * @param left
 * @param right
 * @param top
 * @param bottom
 */
AABB FromPoints(Vector2D pos, float w, float h, float x_, float y_, bool);

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

/**
 * @brief returns X coordinate of the center of a bounding box
 *
 * @param col: AABB data
 */
int AABBX(AABB col);

/**
 * @brief returns Y coordinate of the center of a bounding box
 *
 * @param col: AABB data
 */
int AABBY(AABB col);

/**
 * @brief returns a float X distance between 2 bounding boxes
 *
 * @param a: first AABB
 * @param b: second AABB
 */
float AABBgetXDistance(AABB a, AABB b);

/**
 * @brief returns a float Y distance between 2 bounding boxes
 *
 * @param a: first AABB
 * @param b: second AABB
 */
float AABBgetYDistance(AABB a, AABB b);

/**
 * @brief returns an int number equal to the amount of tiles in gameplayInstance that
 * are colliding with AABB
 * 
 * @param col: AABB to determine colisions
 */
int AABBReturnColNumber(AABB col);

/**
 * @brief returns the collisions
 *
 * @param col: AABB to determine colisions
 */
GameObjectPtr ReturnGameObjectAABBCollision(AABB col);

/**
 * @brief same as AABBReturnColNumber but within a range of 3 tiles
 *
 * @param col: AABB to determine colisions
 */
int AABBReturnColNumberinRange(GameObjectPtr parent, AABB col);

/**
 * @brief returns a pointer to an array that contains pointers to each AABB colliding with col 
 *
 * @param col: AABB to determine colisions
 */
AABB** ReturnAABBCollisions(AABB col);

/**
 * @brief returns a pointer to an array that contains pointers to each AABB colliding with col
 * only scans through 3 adjcent tiles in every direction from the AABB
 *
 * @param col: AABB to determine colisions
 */
AABB ReturnAABBCollisionsInRange(AABB col, int* colCount);

/**
 * @brief creates 4 AABBs to the left, right, top, bottom of col and stores the 
 * aabbs in an array, returns the array. each aabb can be called with list[Top], list[Bottom], etc
 * col is stored in List[0] for good measure
 *
 * @param col: aabb to box cast
 */
AABB* BoxCast(AABB col, int);


/**
 * @brief frees the 4 box cast aabbs in the array, then frees the array
 *
 * @param ar:  array
 */
void FreeBoxCast(AABB* ar);


/**
 * @brief frees an array allocated with ReturnAABBCollisions() or BoxCast()
 *
 * @param ar: pointer array
 */
void FreeAABBarray(AABB** ar);


//AEFrameRateControllerGetFrameTime

