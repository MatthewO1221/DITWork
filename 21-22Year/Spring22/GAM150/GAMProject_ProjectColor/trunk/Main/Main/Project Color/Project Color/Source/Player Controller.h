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

typedef struct Player *PlayerPtr;
typedef struct Enemy* EnemyPtr; //Declaration of enemy struct + pointer
typedef struct GameObject *GameObjectPtr;
typedef struct collisionData *AABB;
struct Vector2D;

enum PlayerMesh
{
    ONE_BY_ONE, FOUR_BY_ONE, SIX_BY_ONE, SIXTEEN_BY_ONE, ONE_BY_ONE_REVERSE, FOUR_BY_ONE_REVERSE, SIX_BY_ONE_REVERSE, SIXTEEN_BY_ONE_REVERSE
};

enum PlayerState
{
    IDLE, WALK, RUN, JUMP, FALL, WALL_SLIDE, WALL_JUMP, SWING, CHARGE, HURT, DEATH
};

void LoadPlayer(void);

void InitPlayer(void);

void PlayerUpdate(float objX, float objY, AABB* levelTiles, float dt);

void ShutdownPlayer(void);

void UnloadPlayer(void);

void DamageEnemy(PlayerPtr player, EnemyPtr enemy);

void PlayerAnimate(enum PlayerMesh, enum PlayerState, int, float, bool);

void PlayerHurt(struct Vector2D*);

PlayerPtr GetPlayer(void);

GameObjectPtr GetBrush(void);

void CheckForPlayerOutOfBounds(void);
