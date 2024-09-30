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
#include "Brush.h"
#include "GameObject.h"
#include "Transform.h"
#define _USE_MATH_DEFINES
#include <math.h>

float angle;
float swipeStartAngle;
float swipeEndAngle;
float swipeLength = 70.0f;								//to be added to initial angle to find swipeStartAngle
bool swipingBrush = false;
bool swipingDown = false;
bool swipeSide = false;									// true = right, false = left

float rotateBrush(float objXPos, float objYPos)
{
	if (!swipingBrush)
	{
		float tempAngle;
		signed long mouseX, mouseY;				// Will store the mouse position (In the SCREEN coordinates system)
		float mouseInWorldX, mouseInWorldY;		// Will store the mouse position (In the WORLD coordinates system)
		float relativeMouseX, relativeMouseY;

		AEInputGetCursorPosition(&mouseX, &mouseY);
		AEGfxConvertScreenCoordinatesToWorld((float)mouseX, (float)mouseY, &(float)mouseInWorldX, &(float)mouseInWorldY);
		relativeMouseX = mouseInWorldX - objXPos;
		relativeMouseY = mouseInWorldY - objYPos;

		tempAngle = AERadToDeg(atan2f(relativeMouseY, relativeMouseX));

		tempAngle = AERadToDeg(atan2f(relativeMouseY, relativeMouseX));

		if (tempAngle < 0.0f && tempAngle < -90.0f)
		{
			tempAngle = 180.0f;
		}
		else if (tempAngle < 0.0f && tempAngle > -90.0f)
		{
			tempAngle = 0.0f;
		}
		else
		{
			tempAngle = AEClamp(tempAngle, 0.0f, 180.0f);
		}

		return tempAngle;
	}
	else
	{
		return 0.0f;
	}
}



void swipeBrush(float* tempAngle)
{
	if (AEInputCheckTriggered(VK_LBUTTON))
	{
		swipeEndAngle = *tempAngle;

		if (*tempAngle >= 0.0f && *tempAngle < 90.0f)
		{
			swipeSide = true;
			swipeStartAngle = *tempAngle + 70.0f;
		}
		else
		{
			swipeSide = false;
			swipeStartAngle = *tempAngle - 70.0f;
		}

		swipingBrush = true;
	}

	if (swipingBrush)
	{
		if (swipeSide)
		{
			if (*tempAngle < swipeStartAngle && swipingDown == false)
			{
				*tempAngle += 4.0f;
			}
			else
			{
				swipingDown = true;
				*tempAngle -= 8.0f;
				
				if (*tempAngle <= swipeEndAngle)
				{
					swipingBrush = false;
					swipingDown = false;
				}
			}
		}
		else if (!swipeSide)
		{
			if (*tempAngle > swipeStartAngle && swipingDown == false)
			{
				*tempAngle -= 4.0f;
			}
			else
			{
				swipingDown = true;
				*tempAngle += 8.0f;

				if (*tempAngle >= swipeEndAngle)
				{
					swipingBrush = false;
					swipingDown = false;
				}
			}
		}
	}
}



void BrushUpdate(GameObjectPtr gameObject, float objXPos, float objYPos)
{
	TransformPtr pRotation = GetTransformGameObject(gameObject);

	if (!swipingBrush)
	{
		angle = rotateBrush(objXPos, objYPos);
	}
	swipeBrush(&angle);

	SetRotationTransform(pRotation, ((angle * (float)M_PI) / 180.0f));
} 