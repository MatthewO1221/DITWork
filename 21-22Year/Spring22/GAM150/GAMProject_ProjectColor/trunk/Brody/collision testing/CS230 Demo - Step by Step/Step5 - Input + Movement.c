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
	float obj1X = 0.0f, obj1Y = 0.0f;		// Position variables for object 1
	AEGfxVertexList*	pMesh1;				// Pointer to Mesh (Model)


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

		
		// Object Control
		if (AEInputCheckCurr(VK_UP))
			obj1Y += 3.0f;
		else
		if (AEInputCheckCurr(VK_DOWN))
			obj1Y -= 3.0f;

		if (AEInputCheckCurr(VK_LEFT))
			obj1X -= 3.0f;
		else
		if (AEInputCheckCurr(VK_RIGHT))
			obj1X += 3.0f;


		// Drawing object 1
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		// Set position for object 1
		AEGfxSetPosition(obj1X, obj1Y);
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

