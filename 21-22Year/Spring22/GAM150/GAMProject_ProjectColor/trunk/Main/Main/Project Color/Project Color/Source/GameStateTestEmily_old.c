//------------------------------------------------------------------------------
//
// File Name:	GameStateTestEmily.c
// Author(s):	Emily Comes (emily.comes)
// Project:		"Project Color"
// Course:		CS230S22
//
// Copyright (C) 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "GameStateSwitcher.h"
#include "GameStateManager.h"
#include "GameStateTestEmily.h"
#include "Vector2D.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Physics.h"
#include "Transform.h"
#include "DisplayHelper.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
void KeyboardInput(char* textBuffer, char* charIndex);

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static char SCREEN_SCALE = 1;
static AEGfxVertexList* mesh16x6fontSheet;
static AEGfxTexture* fontTexture;
static SpriteSourcePtr fontSprSrc;
static GameObjectPtr textObj;
static char* textBuffer1 = NULL;
static char charIndex_textBuffer1 = 0;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the game state.
void GameStateTestEmilyLoad()
{
  // it seems as though using higher-precision division causes parts of adjacent sprites in the sheet to be rendered when they shouldn't be
  //mesh16x6fontSheet = MeshCreateQuad(.5f, .5f, 1.f / 16.f, 1.f / 6.f, "mesh16x6fontSheet");
  mesh16x6fontSheet = MeshCreateQuad(.5f, .5f, .0625f, .1666f, "mesh16x6fontSheet");
  fontTexture = AEGfxTextureLoad("./Assets/font.png");
  fontSprSrc = SpriteSourceCreate(16, 6, fontTexture);
}

// Initialize the memory associated with the game state.
void GameStateTestEmilyInit()
{
  AESysSetWindowTitle("Emily Test");
  SCREEN_SCALE = GetWindowClientScaleNES();
  AEGfxSetWorldOriginMode(AE_WOM_TOP_LEFT);
  AEGfxSetCamPosition(0.f, 0.f);
  charIndex_textBuffer1 = 0;
  textBuffer1 = (char*)calloc(128, sizeof(char));
  if (errno == ENOMEM || textBuffer1 == NULL) // memory allocation failed
    textBuffer1 = NULL;

  textObj = GameObjectCreate();
  if (textObj != NULL && textBuffer1 != NULL)
  {
    GameObjectSetName(textObj, "text");

    // set transform of text game object
    TransformPtr nuTF = TransformCreate();
    Vector2D textPos;
    textPos.x = textPos.y = 12.f * SCREEN_SCALE; // I do not know why it's 12 pixels offset from the corner when it should be 8
    TransformSetTranslation(nuTF, &textPos);
    TransformSetRotation(nuTF, 0);
    Vector2D textScale;
    textScale.x = textScale.y = 8.f * SCREEN_SCALE;
    textScale.y *= -1;
    TransformSetScale(nuTF, &textScale);
    GameObjectAddTransform(textObj, nuTF);

    // set sprite component of text game object
    SpritePtr textSpr = SpriteCreate();
    SpriteSetAlpha(textSpr, 1);
    GameObjectAddSprite(textObj, textSpr);
    SpriteSetMesh(textSpr, mesh16x6fontSheet);
    SpriteSetSpriteSource(textSpr, fontSprSrc);

    // set sprite text of text game object's sprite component
    //sprintf_s(textBuffer1, 128, "%d", SCREEN_SCALE);
    sprintf_s(textBuffer1, 128, "menutest");
    charIndex_textBuffer1 = 8;
    SpriteSetText(textSpr, textBuffer1);
  }

  AEGfxSetBackgroundColor(.25f, .25f, .25f);
  AEGfxSetBlendMode(AE_GFX_BM_BLEND);
  AEGfxSetTextureMode(AE_GFX_TM_PRECISE);
}

// Update the game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateTestEmilyUpdate(float dt)
{
  ToggleGameState();
  KeyboardInput(textBuffer1, &charIndex_textBuffer1);
  GameObjectUpdate(textObj, dt);
  GameObjectDraw(textObj);
}

// Free any memory associated with the game state.
void GameStateTestEmilyShutdown()
{
  if (textBuffer1 != NULL)
    free(textBuffer1);
  textBuffer1 = NULL;

  // Free all objects.
  GameObjectFree(&textObj);
}

// Unload the resources associated with the game state.
void GameStateTestEmilyUnload()
{
  // Free all sprite sources.
  SpriteSourceFree(&fontSprSrc);

  // Unload all textures.
  AEGfxTextureUnload(fontTexture);

  // Free all meshes.
  AEGfxMeshFree(mesh16x6fontSheet);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// This function is a big fucking mess, although its literally just for testing the text system and input system because input was being fucky at one point
void KeyboardInput(char* textBuffer, char* charIndex)
{
  if (textBuffer == NULL || charIndex == NULL)
    return;
  if (*charIndex <= 128)
  {
    if (AEInputCheckCurr(VK_SHIFT) || AEInputCheckCurr(VK_LSHIFT) || AEInputCheckCurr(VK_RSHIFT))
    {
      if (AEInputCheckTriggered(VK_OEM_3))
        sprintf_s(textBuffer + (*charIndex)++, 2, "~");
      if (AEInputCheckTriggered('1'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "!");
      if (AEInputCheckTriggered('2'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "@");
      if (AEInputCheckTriggered('3'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "#");
      if (AEInputCheckTriggered('4'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "$");
      if (AEInputCheckTriggered('5'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "%%");
      if (AEInputCheckTriggered('6'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "^");
      if (AEInputCheckTriggered('7'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "&");
      if (AEInputCheckTriggered('8'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "*");
      if (AEInputCheckTriggered('9'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "(");
      if (AEInputCheckTriggered('0'))
        sprintf_s(textBuffer + (*charIndex)++, 2, ")");
      if (AEInputCheckTriggered(VK_OEM_MINUS))
        sprintf_s(textBuffer + (*charIndex)++, 2, "_");
      if (AEInputCheckTriggered(VK_OEM_PLUS))
        sprintf_s(textBuffer + (*charIndex)++, 2, "=");
      if (AEInputCheckTriggered(VK_OEM_4))
        sprintf_s(textBuffer + (*charIndex)++, 2, "{");
      if (AEInputCheckTriggered(VK_OEM_6))
        sprintf_s(textBuffer + (*charIndex)++, 2, "}");
      if (AEInputCheckTriggered(VK_OEM_5))
        sprintf_s(textBuffer + (*charIndex)++, 2, "|");
      if (AEInputCheckTriggered(VK_OEM_1))
        sprintf_s(textBuffer + (*charIndex)++, 2, ":");
      if (AEInputCheckTriggered(VK_OEM_7))
        sprintf_s(textBuffer + (*charIndex)++, 2, "\"");
      if (AEInputCheckTriggered(VK_OEM_COMMA))
        sprintf_s(textBuffer + (*charIndex)++, 2, "<");
      if (AEInputCheckTriggered(VK_OEM_PERIOD))
        sprintf_s(textBuffer + (*charIndex)++, 2, ">");
      if (AEInputCheckTriggered(VK_OEM_2))
        sprintf_s(textBuffer + (*charIndex)++, 2, "?");
      if (AEInputCheckTriggered('Q'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "Q");
      if (AEInputCheckTriggered('W'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "W");
      if (AEInputCheckTriggered('E'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "E");
      if (AEInputCheckTriggered('R'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "R");
      if (AEInputCheckTriggered('T'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "T");
      if (AEInputCheckTriggered('Y'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "Y");
      if (AEInputCheckTriggered('U'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "U");
      if (AEInputCheckTriggered('I'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "I");
      if (AEInputCheckTriggered('O'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "O");
      if (AEInputCheckTriggered('P'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "P");
      if (AEInputCheckTriggered('A'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "A");
      if (AEInputCheckTriggered('S'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "S");
      if (AEInputCheckTriggered('D'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "D");
      if (AEInputCheckTriggered('F'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "F");
      if (AEInputCheckTriggered('G'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "G");
      if (AEInputCheckTriggered('H'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "H");
      if (AEInputCheckTriggered('J'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "J");
      if (AEInputCheckTriggered('K'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "K");
      if (AEInputCheckTriggered('L'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "L");
      if (AEInputCheckTriggered('Z'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "Z");
      if (AEInputCheckTriggered('X'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "X");
      if (AEInputCheckTriggered('C'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "C");
      if (AEInputCheckTriggered('V'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "V");
      if (AEInputCheckTriggered('B'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "B");
      if (AEInputCheckTriggered('N'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "N");
      if (AEInputCheckTriggered('M'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "M");
    }
    else
    {
      if (AEInputCheckTriggered(VK_OEM_3))
        sprintf_s(textBuffer + (*charIndex)++, 2, "`");
      if (AEInputCheckTriggered('1'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "1");
      if (AEInputCheckTriggered('2'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "2");
      if (AEInputCheckTriggered('3'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "3");
      if (AEInputCheckTriggered('4'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "4");
      if (AEInputCheckTriggered('5'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "5");
      if (AEInputCheckTriggered('6'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "6");
      if (AEInputCheckTriggered('7'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "7");
      if (AEInputCheckTriggered('8'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "8");
      if (AEInputCheckTriggered('9'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "9");
      if (AEInputCheckTriggered('0'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "0");
      if (AEInputCheckTriggered(VK_OEM_MINUS))
        sprintf_s(textBuffer + (*charIndex)++, 2, "-");
      if (AEInputCheckTriggered(VK_OEM_PLUS))
        sprintf_s(textBuffer + (*charIndex)++, 2, "=");
      if (AEInputCheckTriggered(VK_OEM_4))
        sprintf_s(textBuffer + (*charIndex)++, 2, "[");
      if (AEInputCheckTriggered(VK_OEM_6))
        sprintf_s(textBuffer + (*charIndex)++, 2, "]");
      if (AEInputCheckTriggered(VK_OEM_5))
        sprintf_s(textBuffer + (*charIndex)++, 2, "\\");
      if (AEInputCheckTriggered(VK_OEM_1))
        sprintf_s(textBuffer + (*charIndex)++, 2, ";");
      if (AEInputCheckTriggered(VK_OEM_7))
        sprintf_s(textBuffer + (*charIndex)++, 2, "\'");
      if (AEInputCheckTriggered(VK_OEM_COMMA))
        sprintf_s(textBuffer + (*charIndex)++, 2, ",");
      if (AEInputCheckTriggered(VK_OEM_PERIOD))
        sprintf_s(textBuffer + (*charIndex)++, 2, ".");
      if (AEInputCheckTriggered(VK_OEM_2))
        sprintf_s(textBuffer + (*charIndex)++, 2, "/");
      if (AEInputCheckTriggered('Q'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "q");
      if (AEInputCheckTriggered('W'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "w");
      if (AEInputCheckTriggered('E'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "e");
      if (AEInputCheckTriggered('R'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "r");
      if (AEInputCheckTriggered('T'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "t");
      if (AEInputCheckTriggered('Y'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "y");
      if (AEInputCheckTriggered('U'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "u");
      if (AEInputCheckTriggered('I'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "i");
      if (AEInputCheckTriggered('O'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "o");
      if (AEInputCheckTriggered('P'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "p");
      if (AEInputCheckTriggered('A'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "a");
      if (AEInputCheckTriggered('S'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "s");
      if (AEInputCheckTriggered('D'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "d");
      if (AEInputCheckTriggered('F'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "f");
      if (AEInputCheckTriggered('G'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "g");
      if (AEInputCheckTriggered('H'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "h");
      if (AEInputCheckTriggered('J'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "j");
      if (AEInputCheckTriggered('K'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "k");
      if (AEInputCheckTriggered('L'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "l");
      if (AEInputCheckTriggered('Z'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "z");
      if (AEInputCheckTriggered('X'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "x");
      if (AEInputCheckTriggered('C'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "c");
      if (AEInputCheckTriggered('V'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "v");
      if (AEInputCheckTriggered('B'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "b");
      if (AEInputCheckTriggered('N'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "n");
      if (AEInputCheckTriggered('M'))
        sprintf_s(textBuffer + (*charIndex)++, 2, "m");
    }
    if (AEInputCheckTriggered(' '))
      sprintf_s(textBuffer + (*charIndex)++, 2, " ");
  }
  if (AEInputCheckTriggered(VK_BACK))
    if (*charIndex != 0)
      sprintf_s(textBuffer + --(*charIndex), 2, "\0");
  SpriteSetText(GameObjectGetSprite(textObj), textBuffer);

  if (AEInputCheckTriggered(VK_RETURN))
  {
    if (!strncmp(textBuffer, "menutest", 128))
    {
      GameStateManagerSetNextState(GsMenuTest);
    }
  }
}
