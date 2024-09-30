//------------------------------------------------------------------------------
//
// File Name:	DisplayHelper.h
// Author(s):	Emily Comes (emily.comes)
// Project:		"Project Color"
// Course:		GAM-150-B Spring 2022
//
// Copyright (C) 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Public Function Declarations:
//------------------------------------------------------------------------------

// returns the width (in pixels) of the window viewport
unsigned long GetWindowClientWidth();

// returns the height (in pixels) of the window viewport
unsigned long GetWindowClientHeight();

// returns an integer scale value representing how many times the larger the screen is compared to the nes's screen resolution
char GetWindowClientScaleNES();

// resizes the viewport window to the specified size
void ResizeClientWindow(unsigned long newWidth, unsigned long newHeight);
