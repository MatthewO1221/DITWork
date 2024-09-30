//------------------------------------------------------------------------------
//
// File Name:	Player Data.h
// Author(s):	Rishabh Anand
// Project:		Project 2
// Course:		CS230S20
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

typedef struct collisionData* AABB;
typedef struct Sprite* SpritePtr;
typedef struct SpriteSource* SpriteSourcePtr;
struct Vector2D;

void PlayerMovementUpdate(GameObjectPtr gameObject, AABB* levelTiles);

bool IsInWater(void);
void SetInWater(bool);

void SetGravity(struct Vector2D*);
void OverrideVelocity(bool);
