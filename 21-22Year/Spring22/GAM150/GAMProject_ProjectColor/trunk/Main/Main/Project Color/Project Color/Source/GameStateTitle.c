//------------------------------------------------------------------------------
//
// File Name:	GameStateStub.c
// Author(s):	Derek Herrera-Sturm
// Project:		Project 0
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "GameStateTitle.h"
#include "GameStateSwitcher.h"
#include "Button.h"
#include "Audio.h"
#include "Mesh.h"
#include "Sprite.h"
#include "SpriteSource.h"
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
AEGfxVertexList* logoMesh;
AEGfxTexture* logoText;

SpritePtr logosprite;
GameObjectPtr logo;
SpriteSourcePtr sourceptr;

int selectedButt = -1;
AEGfxTexture* backgroundTexture;
AEGfxVertexList* backgroundMesh;
//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Stub game state.
void GameStateTitleLoad()
{
	LoadSound("./Assets/SFX/button_down.mp3", sfx);
	LoadSound("./Assets/SFX/button_up.mp3", sfx);

	logoText = AEGfxTextureLoad("./Assets/Tiles/logo.png");
	logoMesh = MeshCreateQuad(1, 1, 1.0, 1.0, "logo");

	backgroundTexture = AEGfxTextureLoad("./Assets/background.png");
	backgroundMesh = MeshCreateQuad(1, 1, 1.0F, 1.0F, "Background");

}

// Initialize the memory associated with the Stub game state.
void GameStateTitleInit()
{
	// Change the window title.
	//AEGfxSetBackgroundColor(58.0f, 182.0f, 189.0f);
	
	

	Vector2D pos = { 0, 30 };

	if (DoesFileExist("player_data.svd") == 0)
	{
		ButtonCreate(pos, BUTTON_WIDTH, BUTTON_HEIGHT, "Play");
	}

	pos.y -= 80;
	ButtonCreate(pos, BUTTON_WIDTH, BUTTON_HEIGHT, "New Game");

	pos.y -= 80;
	ButtonCreate(pos, BUTTON_WIDTH, BUTTON_HEIGHT, "Settings");

	pos.y -= 80;
	ButtonCreate(pos, BUTTON_WIDTH, BUTTON_HEIGHT, "Credits");

	pos.y -= 80;
	ButtonCreate(pos, BUTTON_WIDTH, BUTTON_HEIGHT, "Quit");

}

// Update the Stub game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateTitleUpdate(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetFullTransform(0,0, 0, AEGfxGetWinMaxX() + 64, AEGfxGetWinMaxY());
	AEGfxTextureSet(backgroundTexture, 0.0F, 0.0F);
	AEGfxSetTransparency(1.0F);
	AEGfxMeshDraw(backgroundMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetFullTransform(0, 200, 0, 64 * 3, 64 * 3);
	AEGfxTextureSet(logoText, 0.0F, 0.0F);
	AEGfxSetTransparency(1.0F);
	AEGfxMeshDraw(logoMesh, AE_GFX_MDM_TRIANGLES);


	// Let the user toggle between gamestates.
	ToggleGameState();

	/*
	if (AEInputCheckTriggered(VK_UP) || AEInputCheckTriggered(VK_DOWN)) {
		//down
		if (AEInputCheckTriggered(VK_DOWN) && !AEInputCheckTriggered(VK_UP)) {
			++selectedButt;
			if (selectedButt != ButtonsInUse());
				getButt(selectedButt)->onDeselect(getButt(selectedButt + 1));
		}
		//up
		else {
			--selectedButt;
			if (selectedButt != 0)
				getButt(selectedButt)->onDeselect(getButt(selectedButt - 1));
		}

	//		if (selectedButt <= ButtonsInUse())

	}
	*/
	
	if (selectedButt != -1) {
		if (AEInputCheckTriggered(VK_RETURN))
			getButt(selectedButt)->onTriggered(getButt(selectedButt));
	}


	//		++selectedButt;
	//		if (selectedButt != ButtonsInUse());
	//			getButt(selectedButt)->onDeselect(getButt(selectedButt + 1));
	//	}
	//	//up
	//	else {
	//		--selectedButt;
	//		if (selectedButt != 0)
	//			getButt(selectedButt)->onDeselect(getButt(selectedButt - 1));
	//	}

	//	selectedButt = (int)AEClamp((f32)selectedButt, 0, (f32)ButtonsInUse());
	//	getButt(selectedButt)->onSelect(getButt(selectedButt));

	//}
	//
	//if (selectedButt != -1) {
	//	if (AEInputCheckTriggered(VK_RETURN))
	//		getButt(selectedButt)->onTriggered(getButt(selectedButt));

	//	if (AEInputCheckCurr(VK_RETURN))	
	//		getButt(selectedButt)->onDown(getButt(selectedButt));

	//	if (AEInputCheckReleased(VK_RETURN)) {
	//		getButt(selectedButt)->onRelease(getButt(selectedButt));
	//		getButt(selectedButt)->onDeselect(getButt(selectedButt));
	//	}
	//}

	for (int i = 0; i < MAX_BUTTONS_ONSCREEN; ++i) {
		if (getButt(i) != NULL)
			ButtonUpdate(getButt(i));
	}
}

// Free any memory associated with the Stub game state.
void GameStateTitleShutdown()
{
	for (int i = 0; i < MAX_BUTTONS_ONSCREEN; ++i) {
	if (getButt(i) != NULL)
		ButtonDestroy(getButt(i));
	}

	AEGfxTextureUnload(backgroundTexture);
	AEGfxMeshFree(backgroundMesh);

	AEGfxTextureUnload(logoText);
	AEGfxMeshFree(logoMesh);
}

// Unload the resources associated with the Stub game state.
void GameStateTitleUnload()
{

}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

