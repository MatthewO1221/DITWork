//------------------------------------------------------------------------------
//
// File Name:	GameStateMenuTest.c
// Author(s):	Emily Comes (emily.comes)
// Project:		"Project Color"
// Course:		GAM-150-B Spring 2022
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
#include "GameStateMenuTest.h"
#include "Vector2D.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Transform.h"
#include "DisplayHelper.h"
#include "ConfigHelper.h"
#include "GameStateTable.h"
#include "Audio.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

static const char MAX_TEXTOBJS = 8;
static const char MAX_GAMESTATES = 11;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void UpdateScaleStuff(void);

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static GameStates targetStateNum = GsTitle;
static char SCREEN_SCALE = 1;
static char selectedMenuItemIndex = 1;
static AEGfxVertexList* mesh16x6fontSheet;
static AEGfxTexture* fontTexture;
static SpriteSourcePtr fontSprSrc;
static GameObjectPtr* textObjs;
static char** textBuffers = NULL;

bool fromTitle;
bool leave = false;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the game state.
void GameStateMenuTestLoad()
{
  // it seems as though using higher-precision division causes edges of adjacent sprites in a sheet to be rendered when they shouldn't be
  //mesh16x6fontSheet = MeshCreateQuad(.5f, .5f, 1.f / 16.f, 1.f / 6.f, "mesh16x6fontSheet");
  mesh16x6fontSheet = MeshCreateQuad(.5f, .5f, .0625f, .1666f, "mesh16x6fontSheet");
  fontTexture = AEGfxTextureLoad("./Assets/font.png");
  fontSprSrc = SpriteSourceCreate(16, 6, fontTexture);

  LoadSound("./Assets/SFX/sample_beep.wav", sfx);
  leave = false;
}

// Initialize the memory associated with the game state.
void GameStateMenuTestInit()
{
  //AESysSetWindowTitle("Test Menu: IT'S A SECRET TO NOBODY.");
  SCREEN_SCALE = ReadConfigByte("display", "scale", 1);
  AEGfxSetWorldOriginMode(AE_WOM_TOP_LEFT);
  AEGfxSetCamPosition(0.f, 0.f);

  selectedMenuItemIndex = 1;
  targetStateNum = GsTitle;

  textObjs = (GameObjectPtr*)calloc(MAX_TEXTOBJS, sizeof(GameObjectPtr));
  if (errno == ENOMEM || textObjs == NULL) // if memory allocation failed
    textObjs = NULL;

  textBuffers = (char**)calloc(MAX_TEXTOBJS, sizeof(char*));
  if (errno == ENOMEM || textBuffers == NULL) // if memory allocation failed
    textBuffers = NULL;

  for (char i = 0; i < MAX_TEXTOBJS; ++i)
  {
    *(textBuffers + i) = (char*)calloc(64, sizeof(char));
    if (errno == ENOMEM || *(textBuffers + i) == NULL) // if memory allocation failed
      *(textBuffers + i) = NULL;
  }

  if (GetPrevGS() == GsTitle)
      fromTitle = true;
  else
      fromTitle = false;
     

  if (textBuffers != NULL)
  {
      float volume;
    //sprintf_s(*(textBuffers + 0), 64, "-=- DEVELOPER OPTIONS MENU -=-");
    sprintf_s(*(textBuffers + _title), 64, "-=- Palette Knight: SETTINGS -=-");
    sprintf_s(*(textBuffers + _winScale), 64, "     Window Scale: < %dx >", SCREEN_SCALE);
    sprintf_s(*(textBuffers + _resetConfig), 64, "     Reset Config");
    sprintf_s(*(textBuffers + _loadState), 64, "     Load State:   %s >", GetGameStateName(targetStateNum));

        FMOD_ChannelGroup_GetVolume(Master, &volume);
    sprintf_s(*(textBuffers + _volMasterAdjust), 64, "     Master Volume Level: < %.0f", volume * 10.0);

        FMOD_ChannelGroup_GetVolume(Music, &volume);
    sprintf_s(*(textBuffers + _volMusicAdjust), 64, "     Music Volume Level: < %.0f >", volume * 10.0);

        FMOD_ChannelGroup_GetVolume(SFX, &volume);
    sprintf_s(*(textBuffers + _volSFXAdjust), 64, "     Sound Effects Volume Level: < %.0f >", volume * 10.0);

    if (fromTitle)
        sprintf_s(*(textBuffers + _titleReturn), 64, "     Return to title screen");
    else
        sprintf_s(*(textBuffers + _titleReturn), 64, "     Return to game");
    *(*(textBuffers + selectedMenuItemIndex) + 3) = '>';
  }

  for (char i = 0; i < MAX_TEXTOBJS; ++i)
  {
    *(textObjs + i) = GameObjectCreate();
    if (*(textObjs + i) != NULL && textBuffers != NULL && *(textBuffers + i) != NULL)
    {
      char* gameObjectName = (char*)calloc(64, sizeof(char));
      if (errno == ENOMEM || gameObjectName == NULL) // if memory allocation failed
        gameObjectName = NULL;
      else
      {
        sprintf_s(gameObjectName, 64, "textObjs[%d]", i);
        GameObjectSetName(*(textObjs + i), gameObjectName);
      }
      if (gameObjectName != NULL)
        free(gameObjectName);
      gameObjectName = NULL;

      // set transform of text game object
      TransformPtr nuTF = TransformCreate();
      Vector2D textPos;
      textPos.x = textPos.y = 12.f * SCREEN_SCALE; // I do not know why it's 12 pixels offset from the corner when it should be 8
      textPos.y += 8 * i * SCREEN_SCALE;
      TransformSetTranslation(nuTF, &textPos);
      TransformSetRotation(nuTF, 0);
      Vector2D textScale;
      textScale.x = textScale.y = 8.f * SCREEN_SCALE;
      textScale.y *= -1;
      TransformSetScale(nuTF, &textScale);
      GameObjectAddTransform(*(textObjs + i), nuTF);

      // set sprite component of text game object
      SpritePtr textSpr = SpriteCreate();
      SpriteSetAlpha(textSpr, 1);
      SpriteSetMesh(textSpr, mesh16x6fontSheet);
      SpriteSetSpriteSource(textSpr, fontSprSrc);
      SpriteSetText(textSpr, *(textBuffers + i));
      GameObjectAddSprite(*(textObjs + i), textSpr);
    }
  }

  AEGfxSetBackgroundColor(0.f, 0.f, 0.f);
  AEGfxSetBlendMode(AE_GFX_BM_BLEND);
  AEGfxSetTextureMode(AE_GFX_TM_PRECISE);
}

// Update the game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateMenuTestUpdate(float dt)
{
  ToggleGameState();

  // update screen scale text
  switch (SCREEN_SCALE)
  {
  case 1:
    sprintf_s(*(textBuffers + _winScale), 64, "     Window Scale:   1x >");
    break;
  case 8:
    sprintf_s(*(textBuffers + _winScale), 64, "     Window Scale: < 8x");
    break;
  default:
    sprintf_s(*(textBuffers + _winScale), 64, "     Window Scale: < %dx >", SCREEN_SCALE);
    break;
  }

  // update selected game state text
  if (targetStateNum == 0)
    sprintf_s(*(textBuffers + _loadState), 64, "     Load State:   %s >", GetGameStateName(targetStateNum));
  else if (targetStateNum == MAX_GAMESTATES)
    sprintf_s(*(textBuffers + _loadState), 64, "     Load State: < %s", GetGameStateName(targetStateNum));
  else
    sprintf_s(*(textBuffers + _loadState), 64, "     Load State: < %s >", GetGameStateName(targetStateNum));

  float volume;
  //----------update master vol text ------------------
  FMOD_ChannelGroup_GetVolume(Master, &volume);
  
  if (volume == 0.0)
      sprintf_s(*(textBuffers + _volMasterAdjust), 64, "     Master Volume Level:   %.0f >", volume * 10.0);
  else if (volume == 1.0)
      sprintf_s(*(textBuffers + _volMasterAdjust), 64, "     Master Volume Level: < %.0f", volume * 10.0);
  else
      sprintf_s(*(textBuffers + _volMasterAdjust), 64, "     Master Volume Level: < %.0f >", volume * 10.0);
  
  //----------update Music vol text ------------------
  FMOD_ChannelGroup_GetVolume(Music, &volume);
  if (volume == 0.0)
      sprintf_s(*(textBuffers + _volMusicAdjust), 64, "     Music Volume Level:   %.0f >", volume * 10.0);
  else if (volume == 1.0)
      sprintf_s(*(textBuffers + _volMusicAdjust), 64, "     Music Volume Level: < %.0f", volume * 10.0);
  else
      sprintf_s(*(textBuffers + _volMusicAdjust), 64, "     Music Volume Level: < %.0f >", volume * 10.0);

  //----------update SFX vol text ------------------
  FMOD_ChannelGroup_GetVolume(SFX, &volume);
  if (volume == 0.0)
      sprintf_s(*(textBuffers + _volSFXAdjust), 64, "     Sound Effects Volume Level:   %.0f >", volume * 10.0);
  else if ( volume == 1.0)
      sprintf_s(*(textBuffers + _volSFXAdjust), 64, "     Sound Effects Volume Level: < %.0f", volume * 10.0);
  else
      sprintf_s(*(textBuffers + _volSFXAdjust), 64, "     Sound Effects Volume Level: < %.0f >", volume * 10.0);
  



  if (AEInputCheckTriggered(VK_LEFT) || AEInputCheckTriggered(VK_RIGHT))
  {
    switch (selectedMenuItemIndex)
    {
    case _winScale: // "Scale" menu option
      if (AEInputCheckTriggered(VK_LEFT) && !AEInputCheckTriggered(VK_RIGHT))
      {
        if (SCREEN_SCALE != 1)
          --SCREEN_SCALE;
      }
      else
        if (SCREEN_SCALE != 8)
          ++SCREEN_SCALE;
      UpdateScaleStuff();
      break;
    case _loadState: // "Load State" menu option
      if (AEInputCheckTriggered(VK_LEFT) && !AEInputCheckTriggered(VK_RIGHT))
      {
        if (targetStateNum != 0)
          --targetStateNum;
      }
      else
        if (targetStateNum != MAX_GAMESTATES)
          ++targetStateNum;
      break;
    case _volMasterAdjust:
        if (AEInputCheckTriggered(VK_LEFT) && !AEInputCheckTriggered(VK_RIGHT)) {
            DecreaseVol(master);
        }
        else
            IncreaseVol(master);

        PlaySFX("./Assets/SFX/sample_beep.wav");
        break;
    case _volMusicAdjust:
        if (AEInputCheckTriggered(VK_LEFT) && !AEInputCheckTriggered(VK_RIGHT)) {
            DecreaseVol(music);
        }
        else
            IncreaseVol(music);

        PlaySFX("./Assets/SFX/sample_beep.wav");
        break;
    case _volSFXAdjust:
        if (AEInputCheckTriggered(VK_LEFT) && !AEInputCheckTriggered(VK_RIGHT)) {
            DecreaseVol(sfx);
        }
        else
            IncreaseVol(sfx);

        PlaySFX("./Assets/SFX/sample_beep.wav");
        break;

    default:
      break;
    }
  }

  // clear marker on list item
  if (textBuffers != NULL)
    *(*(textBuffers + selectedMenuItemIndex) + 3) = ' ';

  if (AEInputCheckTriggered(VK_UP) || AEInputCheckTriggered(VK_DOWN))
    if (AEInputCheckTriggered(VK_UP) && !AEInputCheckTriggered(VK_DOWN)) // if up was pressed and down isn't pressed
    {
      --selectedMenuItemIndex; // decrease menu index, going up the menu
      if (selectedMenuItemIndex < 1) // check if we should wrap
        selectedMenuItemIndex = MAX_TEXTOBJS - 1;
    }
    else // if down was pressed and up isn't pressed
    {
      ++selectedMenuItemIndex; // increase menu index, going down the menu
      if (selectedMenuItemIndex > MAX_TEXTOBJS - 1) // check if we should wrap
        selectedMenuItemIndex = 1;
    }

  // put marker on new list item
  if (textBuffers != NULL)
    *(*(textBuffers + selectedMenuItemIndex) + 3) = '>';

  if (AEInputCheckTriggered(VK_RETURN))
  {
    switch (selectedMenuItemIndex)
    {
    case _resetConfig: // "Reset Config"
      ResetConfig();
      SCREEN_SCALE = ReadConfigByte("display", "scale", 1);
      UpdateScaleStuff();
      break;
    case _loadState: // "Load State"
      if (targetStateNum != GsMenuTest)
      {
        AEGfxSetWorldOriginMode(AE_WOM_NORMAL);
        GameStateManagerSetNextState(targetStateNum);
        leave = true;
      }
      break;
    case _titleReturn:
        AEGfxSetWorldOriginMode(AE_WOM_NORMAL);
        if (fromTitle)
            GameStateManagerSetNextState(GsTitle);
        else
            GameStateManagerSetNextState(GsMain);
        leave = true;
      break;
    default:
      break;
    }
  }

  if (!leave) {
    for (char i = 0; i < MAX_TEXTOBJS; ++i)
    {
        GameObjectUpdate(*(textObjs + i), dt);
        GameObjectDraw(*(textObjs + i));
    }
  }
  // draw the text objects

}

// Free any memory associated with the game state.
void GameStateMenuTestShutdown()
{
  // Free string buffers
  if (textBuffers != NULL)
  {
    for (char i = 0; i < MAX_TEXTOBJS; ++i)
    {
      if (*(textBuffers + i) != NULL)
        free(*(textBuffers + i));
      *(textBuffers + i) = NULL;
    }
    free(textBuffers);
    textBuffers = NULL;
  }

  // Free text objects
  if (textObjs != NULL)
  {
    for (char i = 0; i < MAX_TEXTOBJS; ++i)
    {
      if (*(textObjs + i) != NULL)
        GameObjectFree(textObjs + i);
      *(textObjs + i) = NULL;
    }
    free(textObjs);
    textObjs = NULL;
  }

  // write to config when exiting the game state
  WriteConfigByte("display", "scale", SCREEN_SCALE);
}

// Unload the resources associated with the game state.
void GameStateMenuTestUnload()
{
  SpriteSourceFree(&fontSprSrc);
  AEGfxTextureUnload(fontTexture);
  AEGfxMeshFree(mesh16x6fontSheet);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
static void UpdateScaleStuff(void)
{
  ResizeClientWindow(256 * SCREEN_SCALE, 240 * SCREEN_SCALE);
  for (char i = 0; i < MAX_TEXTOBJS; ++i)
  {
    if (*(textObjs + i) != NULL && *(textBuffers + i) != NULL)
    {
      // set transform of text game object
      const TransformPtr textObjTF = GameObjectGetTransform(*(textObjs + i));
      Vector2D textPos;
      textPos.x = textPos.y = 12.f * SCREEN_SCALE;
      textPos.y += 8 * i * SCREEN_SCALE;
      TransformSetTranslation(textObjTF, &textPos);
      Vector2D textScale;
      textScale.x = textScale.y = 8.f * SCREEN_SCALE;
      textScale.y *= -1; // because AE doing things from top-left requires graphics to be vertically flipped
      TransformSetScale(textObjTF, &textScale);
    }
  }
}
