//------------------------------------------------------------------------------
//
// File Name:	Stub.c
// Author(s):	Rishabh Anand
// Project:		Project 0
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Player.h"
#include "Player Controller.h"
#include "Paint.h"
#include "Serialization.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void IncreaseMaxHealth(int amount)
{
	GetPlayer()->maxHealth += amount;
	GetPlayer()->health = GetPlayer()->maxHealth;
}

void LoadPlayerStats(void)
{
	if (LoadPlayerData("player_data.svd") == -1)
	{
		// Set default values.
		GetPlayer()->hasBrush = 0;

		GetPlayer()->maxHealth = 5;
		GetPlayer()->health = GetPlayer()->maxHealth;

		GetPlayer()->maxPaint = 4.0F;
		GetPlayer()->paint = GetPlayer()->maxPaint;
		GetPlayer()->currColor = PAINT_NONE;
		GetPlayer()->unlockedColors = 0;

		GetPlayer()->roomNum = 0;
		GetPlayer()->roomPosX = 0;
		GetPlayer()->roomPosY = -225;

		GetPlayer()->iFrames = false;
		GetPlayer()->iFrameTime = 1.0f;

		GetPlayer()->gemCount = 0;

		// Write new file.
		SavePlayerData("player_data.svd");
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

