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

float rotateBrush(float objXPos, float objYPos);

void swipeBrush(float* angle);

void BrushUpdate(GameObjectPtr gameObject, float objXPos, float objYPos);