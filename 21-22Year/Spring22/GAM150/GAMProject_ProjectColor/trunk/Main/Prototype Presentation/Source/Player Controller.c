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

#include "stdafx.h"
#include "Vector2D.h"
#include "AEEngine.h"
#include "GameObject.h"
#include "Brush.h"
#include "Player Movement.h"


void PlayerUpdate(GameObjectPtr playerGameObject, GameObjectPtr brushGameObject, float objX, float objY, AABB* allTiles, int tileCountX, int tileCountY)
{
	BrushUpdate(brushGameObject, objX, objY);
	PlayerMovementUpdate(playerGameObject, allTiles, tileCountX, tileCountY);
}
