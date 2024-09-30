#include "stdafx.h"
#include "AEEngine.h"
#include "Vector2D.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Transform.h"
#include "DisplayHelper.h"
#include "ConfigHelper.h"
#include "Camera.h"
#include "Hud.h"
#include "Player.h"
#include "Player Controller.h"

static char SCREEN_SCALE = 1;
static AEGfxVertexList* mesh16x6fontSheet, * mesh1x1;
static AEGfxTexture* fontTexture;
static SpriteSourcePtr fontSprSrc;
static GameObjectPtr textObj, healthBarObj;
static char* textBuffer1 = NULL;
static char charIndex_textBuffer1 = 0;
static char health = 10;

static GameObjectPtr gemObj;

static GameObjectPtr textObjPaint, paintBarObj;
static char* textBuffer2 = NULL;

static GameObjectPtr saved;

void LoadStatusBar()
{
  // it seems as though using higher-precision division causes parts of adjacent sprites in the sheet to be rendered when they shouldn't be
  mesh16x6fontSheet = MeshCreateQuad(.5f, .5f, 1.f / 16.f, 1.f / 6.f, "mesh16x6fontSheet");
  mesh1x1 = MeshCreateQuad(.5f, .5f, 1, 1, "mesh1x1");
  fontTexture = AEGfxTextureLoad("./Assets/font.png");
  fontSprSrc = SpriteSourceCreate(16, 6, fontTexture);
}

void InitStatusBar()
{
  SCREEN_SCALE = ReadConfigByte("display", "scale", 1);
  AEGfxSetWorldOriginMode(AE_WOM_TOP_LEFT);
  charIndex_textBuffer1 = 0;
  textBuffer1 = (char*)calloc(32, sizeof(char));
  textBuffer2 = (char*)calloc(32, sizeof(char));
  if (errno == ENOMEM || textBuffer1 == NULL) // memory allocation failed
    textBuffer1 = NULL;

  if (errno == ENOMEM || textBuffer2 == NULL) // memory allocation failed
      textBuffer2 = NULL;

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
    SpriteSetMesh(textSpr, mesh16x6fontSheet);
    SpriteSetSpriteSource(textSpr, fontSprSrc);

    // set sprite text of text game object's sprite component
    sprintf_s(textBuffer1, 32, "Health: %2d", health);
    SpriteSetText(textSpr, textBuffer1);
    GameObjectAddSprite(textObj, textSpr);
  }

  healthBarObj = GameObjectCreate();
  if (healthBarObj != NULL && textBuffer1 != NULL)
  {
    GameObjectSetName(healthBarObj, "healthBar");

    // set transform of text game object
    TransformPtr nuTF = TransformCreate();
    Vector2D pos;
    pos.x = (12.f + (health - 1) * 4.f) * SCREEN_SCALE;
    pos.y = (12.f + 8.f) * SCREEN_SCALE; // I do not know why it's 12 pixels offset from the corner when it should be 8
    TransformSetTranslation(nuTF, &pos);
    TransformSetRotation(nuTF, 0);
    Vector2D scale;
    scale.y = 8.f * SCREEN_SCALE;
    scale.x = 8.f * health * SCREEN_SCALE;
    TransformSetScale(nuTF, &scale);
    GameObjectAddTransform(healthBarObj, nuTF);

    // set sprite component of text game object
    SpritePtr spr = SpriteCreate();
    SpriteSetAlpha(spr, 1);
    SpriteSetMesh(spr, mesh1x1);
    GameObjectAddSprite(healthBarObj, spr);
  }

  gemObj = GameObjectCreate();
  if (gemObj != NULL && textBuffer1 != NULL)
  {
      GameObjectSetName(gemObj, "gems");

      // set transform of text game object
      TransformPtr nuTF = TransformCreate();
      Vector2D pos;
      pos.x = (12.f + (health - 1) * 4.f) * SCREEN_SCALE;
      pos.y = (12.f + 8.f) * SCREEN_SCALE; // I do not know why it's 12 pixels offset from the corner when it should be 8
      TransformSetTranslation(nuTF, &pos);
      TransformSetRotation(nuTF, 0);
      Vector2D textScale;
      textScale.x = textScale.y = 8.f * SCREEN_SCALE;
      textScale.y *= -1;
      TransformSetScale(nuTF, &textScale);
      GameObjectAddTransform(gemObj, nuTF);

      // set sprite component of text game object
      SpritePtr textSpr = SpriteCreate();
      SpriteSetAlpha(textSpr, 1);
      SpriteSetMesh(textSpr, mesh16x6fontSheet);
      SpriteSetSpriteSource(textSpr, fontSprSrc);

      // set sprite text of text game object's sprite component
      sprintf_s(textBuffer1, 32, "Health: %2d", health);
      SpriteSetText(textSpr, textBuffer1);
      GameObjectAddSprite(gemObj, textSpr);
  }

  textObjPaint = GameObjectCreate();
  if (textObjPaint != NULL && textBuffer2 != NULL)
  {
      GameObjectSetName(textObj, "text2");

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
      GameObjectAddTransform(textObjPaint, nuTF);

      // set sprite component of text game object
      SpritePtr textSpr = SpriteCreate();
      SpriteSetAlpha(textSpr, 1);
      SpriteSetMesh(textSpr, mesh16x6fontSheet);
      SpriteSetSpriteSource(textSpr, fontSprSrc);

      // set sprite text of text game object's sprite component
      sprintf_s(textBuffer2, 32, "Health: %2d", health);
      SpriteSetText(textSpr, textBuffer1);
      GameObjectAddSprite(textObjPaint, textSpr);
  }

  saved = GameObjectCreate();
  if (saved != NULL && textBuffer2 != NULL)
  {
      GameObjectSetName(textObj, "text2");

      // set transform of text game object
      TransformPtr nuTF = TransformCreate();
      Vector2D textPos;
      textPos.x = textPos.y = 12.f * SCREEN_SCALE; // I do not know why it's 12 pixels offset from the corner when it should be 8
      TransformSetTranslation(nuTF, &textPos);
      TransformSetRotation(nuTF, 0);
      Vector2D textScale;
      textScale.x = textScale.y = 8.f * SCREEN_SCALE;
      TransformSetScale(nuTF, &textScale);
      GameObjectAddTransform(saved, nuTF);

      // set sprite component of text game object
      SpritePtr textSpr = SpriteCreate();
      SpriteSetAlpha(textSpr, 1);
      SpriteSetMesh(textSpr, mesh16x6fontSheet);
      SpriteSetSpriteSource(textSpr, fontSprSrc);

      // set sprite text of text game object's sprite component
      SpriteSetText(textSpr, "Saved!");
      GameObjectAddSprite(saved, textSpr);
  }

  paintBarObj = GameObjectCreate();
  if (paintBarObj != NULL && textBuffer2 != NULL)
  {
      GameObjectSetName(healthBarObj, "paintBar");

      // set transform of text game object
      TransformPtr nuTF = TransformCreate();
      Vector2D pos;
      pos.x = (12.f + (health - 1) * 4.f) * SCREEN_SCALE;
      pos.y = (12.f + 8.f) * SCREEN_SCALE; // I do not know why it's 12 pixels offset from the corner when it should be 8
      TransformSetTranslation(nuTF, &pos);
      TransformSetRotation(nuTF, 0);
      Vector2D scale;
      scale.y = 8.f * SCREEN_SCALE;
      scale.x = 8.f * health * SCREEN_SCALE;
      TransformSetScale(nuTF, &scale);
      GameObjectAddTransform(paintBarObj, nuTF);

      // set sprite component of text game object
      SpritePtr spr = SpriteCreate();
      SpriteSetAlpha(spr, 1);
      SpriteSetMesh(spr, mesh1x1);
      GameObjectAddSprite(paintBarObj, spr);
  }

}

void UpdateAndDrawStatusBar(char health, char maxHealth, int gems, int paintColor, float paintAmount, float maxPaint, float dt)
{
    // Health meter.
    {
        // update string showing actual value
        sprintf_s(textBuffer1, 32, "Health:%2d", health);

        Vector2D scale;
        scale.y = 8.f * SCREEN_SCALE;
        scale.x = 8.f * health * SCREEN_SCALE;
        TransformSetScale(GameObjectGetTransform(healthBarObj), &scale);

        // adjust status bar size + position
        Vector2D pos;
        pos.x = GetCamPos().x + 80 + (scale.x * 0.5F);
        pos.y = GetCamPos().y + 96; // I do not know why it's 12 pixels offset from the corner when it should be 8
        TransformSetTranslation(GameObjectGetTransform(healthBarObj), &pos);

        // adjust status bar size + position
        Vector2D pos2;
        pos2.x = GetCamPos().x + 96;
        pos2.y = GetCamPos().y + 64; // I do not know why it's 12 pixels offset from the corner when it should be 8
        TransformSetTranslation(GameObjectGetTransform(textObj), &pos2);

        // finally call gameobjectupdate + gameobjectdraw
        AEGfxSetWorldOriginMode(AE_WOM_TOP_LEFT);

        GameObjectUpdate(textObj, dt);
        GameObjectDraw(textObj);

        GameObjectUpdate(healthBarObj, dt);
        GameObjectDraw(healthBarObj);
    }

    if (GetPlayer()->hasBrush)
    {
        // Paint meter.
        {
            // update string showing actual value
            sprintf_s(textBuffer1, 32, "Color:%4s", paintColor == -1 ? "None" : "Red");

            Vector2D scale;
            scale.y = 8.f * SCREEN_SCALE;
            scale.x = 8.f * (paintAmount / maxPaint) * SCREEN_SCALE * 10;
            TransformSetScale(GameObjectGetTransform(paintBarObj), &scale);

            // adjust status bar size + position
            Vector2D pos;
            pos.x = GetCamPos().x + 432 + (scale.x * 0.5F);
            pos.y = GetCamPos().y + 96; // I do not know why it's 12 pixels offset from the corner when it should be 8
            TransformSetTranslation(GameObjectGetTransform(paintBarObj), &pos);

            // adjust status bar size + position
            Vector2D pos2;
            pos2.x = GetCamPos().x + 448;
            pos2.y = GetCamPos().y + 64; // I do not know why it's 12 pixels offset from the corner when it should be 8
            TransformSetTranslation(GameObjectGetTransform(textObjPaint), &pos2);

            // finally call gameobjectupdate + gameobjectdraw
            AEGfxSetWorldOriginMode(AE_WOM_TOP_LEFT);

            GameObjectUpdate(textObjPaint, dt);
            GameObjectDraw(textObjPaint);

            GameObjectUpdate(paintBarObj, dt);
            GameObjectDraw(paintBarObj);
        }
    }
    
    // Gems.
    {
        // update string showing actual value for gems.
        sprintf_s(textBuffer1, 32, "Gems: %3d", gems);

        // adjust status bar position
        Vector2D pos3;
        pos3.x = AEGfxGetWinMaxX() + 424;
        pos3.y = GetCamPos().y + 64; // I do not know why it's 12 pixels offset from the corner when it should be 8
        TransformSetTranslation(GameObjectGetTransform(gemObj), &pos3);

        GameObjectUpdate(gemObj, dt);
        GameObjectDraw(gemObj);

        AEGfxSetWorldOriginMode(AE_WOM_NORMAL);
    }
}

void DisplaySaveMessage(float alpha)
{
    // adjust status bar size + position
    Vector2D pos;
    pos.x = GetCamPos().x + 524;
    pos.y = GetCamPos().y - 400; // I do not know why it's 12 pixels offset from the corner when it should be 8
    TransformSetTranslation(GameObjectGetTransform(saved), &pos);

    SpritePtr sprite = GameObjectGetSprite(saved);
    SpriteSetAlpha(sprite, alpha);
    GameObjectDraw(saved);
}

void ShutdownStatusBar()
{
  if (textBuffer1 != NULL)
    free(textBuffer1);
  textBuffer1 = NULL;
  GameObjectFree(&textObj);
  GameObjectFree(&healthBarObj);
}

void UnloadStatusBar()
{
  SpriteSourceFree(&fontSprSrc);
  AEGfxTextureUnload(fontTexture);
  AEGfxMeshFree(mesh16x6fontSheet);
  AEGfxMeshFree(mesh1x1);
}
