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
#include "GameObject.h"
#pragma once

enum BrushMesh
{
    ONE_BY_ONE_BRUSH, FOUR_BY_ONE_BRUSH, ONE_BY_ONE_BRUSH_REVERSE, FOUR_BY_ONE_BRUSH_REVERSE,
};

enum BrushState
{
    BACK_BRUSH, IDLE_BRUSH, SWING_BRUSH, CHARGE_BRUSH
};

typedef struct Enemy* EnemyPtr; //Declaration of enemy struct + pointer

void LoadBrush(void);

void UnloadBrush(void);

EnemyPtr BrushCollidingWithEnemy(GameObjectPtr brushGameObject);

bool BrushCollidingWithEnemyOnDownswing(void);

void moveBrush(GameObjectPtr brushGameObject, GameObjectPtr playerGameObject, Vector2D brushTranslationOffset);

float rotateBrush(GameObjectPtr brushGameObject, GameObjectPtr playerGameObject);

void swipeBrush(GameObjectPtr brushGameObject, GameObjectPtr playerGameObject, float* tempAngle);

void BrushUpdate(GameObjectPtr brushGameObject, GameObjectPtr playerGameObject);

void BrushAnimate(enum BrushMesh, enum BrushState, int, float, bool);

void UnlockBrush(void);

bool HasBrush(void);
