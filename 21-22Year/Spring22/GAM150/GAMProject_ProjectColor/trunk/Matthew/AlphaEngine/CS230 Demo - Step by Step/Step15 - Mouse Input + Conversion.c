
// ---------------------------------------------------------------------------
// Project Name		:	Simple Demo
// File Name		:	Main.cpp
// Author			:	Antoine Abi Chacra
// Creation Date	:	2012/01/11
// Purpose			:	main entry point2 for the test program
// History			:
// - 2012/01/11		:	- initial implementation
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"

// ---------------------------------------------------------------------------

// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals

int gGameRunning = 1;

// ---------------------------------------------------------------------------
// Static function protoypes

// ---------------------------------------------------------------------------
// main


int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
	// Variable declaration	
	AEGfxVertexList*	pMesh1;				// Pointer to Mesh (Model)
	float camX, camY;						// Used to temporary store the camera position
	signed long mouseX, mouseY;				// Will store the mouse position (In the SCREEN coordinates system)
	float mouseInWorldX, mouseInWorldY;		// Will store the mouse position (In the WORLD coordinates system)
	
	// Initialize the system 
	AESysInitInfo sysInitInfo;
	sysInitInfo.mCreateWindow		= 1;
	sysInitInfo.mAppInstance		= instanceH;
	sysInitInfo.mShow				= show;
	sysInitInfo.mWinWidth			= 800; 
	sysInitInfo.mWinHeight			= 600;
	sysInitInfo.mCreateConsole		= 1;
	sysInitInfo.mMaxFrameRate		= 60;
	sysInitInfo.mpWinCallBack		= NULL;//MyWinCallBack;
	sysInitInfo.mClassStyle			= CS_HREDRAW | CS_VREDRAW;											
	sysInitInfo.mWindowStyle		= WS_OVERLAPPEDWINDOW;//WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;;	
	sysInitInfo.mWindowHandle		= NULL;
	sysInitInfo.mHandleWindowMessages	= 1;
	AESysInit (&sysInitInfo);

	// reset the system modules
	AESysReset();

	
	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// 1 triangle at a time
	// X, Y, Color, texU, texV
	AEGfxTriAdd(
		-25.5f, -25.5f, 0xFFFF0000, 0.0f, 0.0f, 
		25.5f,  0.0f, 0xFFFF0000, 0.0f, 0.0f,
		-25.5f,  25.5f, 0xFFFF0000, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1

	pMesh1 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh1, "Failed to create mesh 1!!");

	
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	// Game Loop
	while(gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();

		// Move the camera
		AEGfxGetCamPosition(&camX, &camY);
		if (AEInputCheckCurr('W'))
			AEGfxSetCamPosition(camX, camY + 2);
		else
		if (AEInputCheckCurr('S'))
			AEGfxSetCamPosition(camX, camY - 2);

		AEGfxGetCamPosition(&camX, &camY);
		if (AEInputCheckCurr('A'))
			AEGfxSetCamPosition(camX - 2, camY);
		else
		if (AEInputCheckCurr('D'))
			AEGfxSetCamPosition(camX + 2, camY);


		AEInputGetCursorPosition(&mouseX, &mouseY);
		AEGfxConvertScreenCoordinatesToWorld(mouseX, mouseY, &mouseInWorldX, &mouseInWorldY);


		// Drawing object 1
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		// Set position for object 1
		AEGfxSetPosition(mouseInWorldX, mouseInWorldY);
		// No texture for object 1
		AEGfxTextureSet(NULL, 0, 0);
		// Drawing the mesh (list of triangles)
		AEGfxSetTransparency(1.0f);
		AEGfxMeshDraw(pMesh1, AE_GFX_MDM_TRIANGLES);

		// Drawing object 1, again, at (0;0)
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		// Set position for object 1
		AEGfxSetPosition(0.0f, 0.0f);
		// No texture for object 1
		AEGfxTextureSet(NULL, 0, 0);
		// Drawing the mesh (list of triangles)
		AEGfxSetTransparency(1.0f);
		AEGfxMeshDraw(pMesh1, AE_GFX_MDM_TRIANGLES);


		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}

	// Freeing the objects and textures
	AEGfxMeshFree(pMesh1);

	// free the system
	AESysExit();

	return 1;
}

// ---------------------------------------------------------------------------

