//------------------------------------------------------------------------------
//
// File Name:	DisplayHelper.c
// Author(s):	Emily Comes (emily.comes)
// Project:		"Project Color"
// Course:		GAM-150-B Spring 2022
//
// Copyright (C) 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "AEEngine.h"
#include "DisplayHelper.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// returns the width (in pixels) of the window viewport
unsigned long GetWindowClientWidth()
{
  unsigned long windowWidth = 0;
  RECT* clientRect = (RECT*)calloc(1, sizeof(RECT));
  if (errno == ENOMEM || clientRect == NULL) // memory allocation failed
    clientRect = NULL;
  else
  {
    GetClientRect(AESysGetWindowHandle(), clientRect);
    windowWidth = clientRect->right - clientRect->left;
  }
  if (clientRect != NULL)
    free(clientRect);
  clientRect = NULL;
  return windowWidth;
}

// returns the height (in pixels) of the window viewport
unsigned long GetWindowClientHeight()
{
  unsigned long windowHeight = 0;
  RECT* clientRect = (RECT*)calloc(1, sizeof(RECT));
  if (errno == ENOMEM || clientRect == NULL) // memory allocation failed
    clientRect = NULL;
  else
  {
    GetClientRect(AESysGetWindowHandle(), clientRect);
    windowHeight = clientRect->bottom - clientRect->top;
  }
  if (clientRect != NULL)
    free(clientRect);
  clientRect = NULL;
  return windowHeight;
}

// returns an integer scale value representing how many times the larger the screen is compared to the nes's screen resolution
char GetWindowClientScaleNES()
{
  const char xScale = (char)(GetWindowClientWidth() / 256),
    /*    */ yScale = (char)(GetWindowClientHeight() / 240);
  if (xScale == yScale)
    return xScale;
  return 0;
}

// resizes the viewport window to the specified size
void ResizeClientWindow(unsigned long newWidth, unsigned long newHeight)
{
  RECT* rect = (RECT*)calloc(1, sizeof(RECT));
  if (errno == ENOMEM || rect == NULL) // skip doing stuff if memory allocation failed
    rect = NULL;
  else
  {
    // set up client rect / "true screen resolution"
    rect->left = rect->top = 0;
    rect->right = newWidth;
    rect->bottom = newHeight;
    AdjustWindowRect(rect, WS_OVERLAPPEDWINDOW, FALSE); // adjust window rect to include borders or something idk i hate the windows api
    SetWindowPos(AESysGetWindowHandle(), 0, 0, 0, rect->right - rect->left, rect->bottom - rect->top, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
    free(rect); // we don't need the rect anymore
    rect = NULL;
  }
}
