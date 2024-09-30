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


typedef struct 
{
	float mX;
	float mY;
}TextureOffset;

// ---------------------------------------------------------------------------
// Static function protoypes

// ---------------------------------------------------------------------------
// main


int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
	// Variable declaration	
	AEGfxVertexList*	pMesh1;				// Pointer to Mesh (Model)
	AEGfxTexture *pTex1;					// Pointer to Texture (Image)
	TextureOffset	textureOffsets[8];		// Array of texture offsets, using for the sprite animations
	unsigned int currentTextureOffset = 0;

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
		-150.0f, -75.0f, 0xFFFF0000, 0.0f, 0.25f, 
		150.0f, -75.0f, 0xFFFF0000, 0.5f, 0.25f,
		-150.0f, 75.0f, 0xFFFF0000, 0.0f, 0.0f);

	AEGfxTriAdd(
		150.0f, -75.0f, 0xFFFF0000, 0.5f, 0.25f,
		150.0f,  75.0f, 0xFFFF0000, 0.5f, 0.0f,
		-150.0f,  75.0f, 0xFFFF0000, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	pMesh1 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh1, "Failed to create mesh 1!!");

	// Texture 1: From file
	pTex1 = AEGfxTextureLoad("runningcat.png");
	AE_ASSERT_MESG(pTex1, "Failed to create texture1!!");


	// Setting the texture offsets
	textureOffsets[0].mX	= 0.0f;			textureOffsets[0].mY	= 0.0f;
	textureOffsets[1].mX	= 0.5f;			textureOffsets[1].mY	= 0.0f;
	textureOffsets[2].mX	= 0.0f;			textureOffsets[2].mY	= 0.25f;
	textureOffsets[3].mX	= 0.5f;			textureOffsets[3].mY	= 0.25f;
	textureOffsets[4].mX	= 0.0f;			textureOffsets[4].mY	= 0.5f;
	textureOffsets[5].mX	= 0.5f;			textureOffsets[5].mY	= 0.5f;
	textureOffsets[6].mX	= 0.0f;			textureOffsets[6].mY	= 0.75f;
	textureOffsets[7].mX	= 0.5f;			textureOffsets[7].mY	= 0.75f;
	
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// Game Loop
	while(gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();

		if(AEInputCheckTriggered(VK_SPACE))
		{
			++currentTextureOffset;

			if(currentTextureOffset >= 8)
				currentTextureOffset = 0;
		}

		// Drawing object 1
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		// Set poisition for object 1
		AEGfxSetPosition(0.0f, 0.0f);
		// No texture for object 1
		AEGfxTextureSet(pTex1, textureOffsets[currentTextureOffset].mX, textureOffsets[currentTextureOffset].mY);
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
	
	// Freeing the texture
	AEGfxTextureUnload(pTex1);

	// free the system
	AESysExit();

	return 1;
}

// ---------------------------------------------------------------------------

