
#pragma once

#include "stdafx.h"
#include "AEEngine.h"
#include "Vector2D.h"
#include "AABB.h"
#include "GameObject.h"
#include "Button.h"
#include "PauseScreen.h"
#include "Mesh.h"
#include "Camera.h"
#include "SpriteSource.h"
#include "Sprite.h"
#include "Audio.h"

static const char MAX_TEXTOBJS = 7;

bool paused_state = false;

AEGfxVertexList* pausemesh;

static AEGfxVertexList* mesh16x6fontSheet;
static AEGfxTexture* fontTexture;
static SpriteSourcePtr fontSprSrc;
static GameObjectPtr* textObjs;
static char** textBuffers = NULL;

void PauseInit()
{
	pausemesh = MeshCreateQuad(1, 1, 1.0, 1.0, "pausebg");
	AEGfxMeshStart();

	// Bottom left.
	AEGfxTriAdd(			//nicenicenice
		1 * -1.f, 1 * -1.f, 0xC8696969, 0.f, 1.0F,
		1, 1 * -1.f, 0xC8696969, 1.0F, 1.0F,
		1 * -1.f, 1, 0xC8696969, 0.f, 0.f);
	// Top right.
	AEGfxTriAdd(
		1, 1 * -1.f, 0xC8696969, 1.0F, 1.0F,
		1, 1, 0xC8696969, 1.0F, 0.f,
		1 * -1.f, 1, 0xC8696969, 0.f, 0.f);

	pausemesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(pausemesh, "Failed to create Pause Mesh!");

	LoadSound("./Assets/SFX/pause_menu.mp3", sfx);
	//PlaySFX("./Assets/SFX/pause_menu.mp3");


	Vector2D pos = { 0, 60 };

	ButtonCreate(pos, BUTTON_WIDTH, BUTTON_HEIGHT, "Play");

	pos.y -= 80;
	ButtonCreate(pos, BUTTON_WIDTH, BUTTON_HEIGHT, "Settings");

	pos.y -= 80;
	ButtonCreate(pos, BUTTON_WIDTH, BUTTON_HEIGHT, "Credits");

	pos.y -= 80;
	ButtonCreate(pos, BUTTON_WIDTH, BUTTON_HEIGHT, "Quit");

	mesh16x6fontSheet = MeshCreateQuad(.5f, .5f, .0625f, .1666f, "mesh16x6fontSheet");
	fontTexture = AEGfxTextureLoad("./Assets/font.png");
	fontSprSrc = SpriteSourceCreate(16, 6, fontTexture);

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
		
	if (textBuffers != NULL)
	{
		//sprintf_s(*(textBuffers + 0), 64, "-=- DEVELOPER OPTIONS MENU -=-");
		sprintf_s(*(textBuffers + 0), 64, "Controls:");
		sprintf_s(*(textBuffers + 1), 64, "WASD to move/ space to jump");
		sprintf_s(*(textBuffers + 2), 64, "Shift to run");
		sprintf_s(*(textBuffers + 3), 64, "click to swing brush");
		sprintf_s(*(textBuffers + 4), 64, "Hold to paint");
		sprintf_s(*(textBuffers + 5), 64, "right click to recharge brush");
		sprintf_s(*(textBuffers + 6), 64, "scroll wheel / num keys to swap paint");
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
			textPos.x = textPos.y = 175.f;
			textPos.y += 25 * i;
			TransformSetTranslation(nuTF, &textPos);
			TransformSetRotation(nuTF, 0);
			Vector2D textScale;
			textScale.x = textScale.y = 25.f;
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

}

void PauseUpdate()
{
	if (AEInputCheckTriggered(VK_ESCAPE)) {
		PauseToggle();
		
	}

	if (!paused_state)
		return;

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetFullTransform(0, 0, 0, AEGfxGetWinMaxX() + 64, AEGfxGetWinMaxY());
	AEGfxMeshDraw(pausemesh, AE_GFX_MDM_TRIANGLES);

	for (int i = 0; i < MAX_BUTTONS_ONSCREEN; ++i) {
		if (getButt(i) != NULL)
			ButtonUpdate(getButt(i));
	}

	AEGfxSetWorldOriginMode(AE_WOM_TOP_LEFT);
	for (char i = 0; i < MAX_TEXTOBJS; ++i)
	{
		GameObjectDraw(*(textObjs + i));
	}
	AEGfxSetWorldOriginMode(AE_WOM_NORMAL);

}

void PauseShutdown()
{
	AEGfxMeshFree(pausemesh);

	for (int i = 0; i < MAX_BUTTONS_ONSCREEN; ++i) {
		if (getButt(i) != NULL)
			ButtonDestroy(getButt(i));
	}

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

	SpriteSourceFree(&fontSprSrc);
	AEGfxTextureUnload(fontTexture);
	AEGfxMeshFree(mesh16x6fontSheet);
}

bool GetPaused()
{
	return paused_state;
}

void PauseToggle()
{
	paused_state = !paused_state;
	PlaySFX("./Assets/SFX/pause_menu.mp3");
}
