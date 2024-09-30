// ---------------------------------------------------------------------------
// Project Name		:	Simple Demo
// File Name		:	Main.cpp
// Author			:	Antoine Abi Chacra
// Creation Date	:	2012/01/11
// Purpose			:	Demonstrates Alpha Engine features.
// History			:
// - 2012/01/11		:	- initial implementation
// - 2019/01/04		:	- Fixed AE_ASSERT/AE_WARNING issues.
// - 2019/01/06		:	- Added missing call to AESysUpdate().
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// includes

#include "stdafx.h"		// Should be included first in every .c file!

#include "AEEngine.h"

#include "EnemyAI.h"
// ---------------------------------------------------------------------------

// Libraries
#ifdef _DEBUG
#pragma comment (lib, "Alpha_Engine_D.lib")
#else
#pragma comment (lib, "Alpha_Engine.lib")
#endif

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
	UNREFERENCED_PARAMETER(command_line);
	UNREFERENCED_PARAMETER(prevInstanceH);

	// Variable declaration	
	unsigned char colors[16];				// 2x2 image
	float obj1X = 0.0f, obj1Y = 0.0f;
	float obj1texX = 0, obj1texY = 0;		// Texture variables for object 2's texture
	AEGfxVertexList* pMesh1;				// Pointer to Mesh (Model)
	AEGfxVertexList* pMesh2;				// Pointer to Mesh (Model)
	AEGfxVertexList* pMeshLine;			// Pointer to Mesh (Model)
	AEGfxTexture* pTex1;					// Pointer to Texture (Image)
	AEGfxTexture* pTex2;					// Pointer to Texture (Image)
	float camX, camY;						// Used to temporary store the camera position
	float alpha = 1.0f;
	TextureOffset	textureOffsets[8];		// Array of texture offsets, using for the sprite animations
	unsigned int currentTextureOffset = 0;
	AEGfxVertexList* pMesh3;				// Pointer to Mesh (Model)
	AEGfxTexture* pTex3;					// Pointer to Texture (Image)
	signed long mouseX, mouseY;				// Will store the mouse position (In the SCREEN coordinates system)
	float mouseInWorldX, mouseInWorldY;		// Will store the mouse position (In the WORLD coordinates system)
	int frameCount = 0;
	float color = 0.0f;
	int direction = 0;



	// Initialize the system 
	AESysInitInfo sysInitInfo;
	sysInitInfo.mAppInstance = instanceH;
	sysInitInfo.mShow = show;
	sysInitInfo.mWinWidth = 800;
	sysInitInfo.mWinHeight = 600;
	sysInitInfo.mCreateConsole = 1;
	sysInitInfo.mMaxFrameRate = 60;
	sysInitInfo.mpWinCallBack = NULL;//MyWinCallBack;
	sysInitInfo.mClassStyle = CS_HREDRAW | CS_VREDRAW;
	sysInitInfo.mWindowStyle = WS_OVERLAPPEDWINDOW;//WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;;		
	sysInitInfo.mCreateWindow = 1;
	sysInitInfo.mWindowHandle = NULL;
	AESysInit(&sysInitInfo);

	// reset the system modules
	AESysReset();


	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// 1 triangle at a time
	// X, Y, Color, texU, texV
	AEGfxTriAdd(
		-25.5f, -25.5f, 0xFFFF0000, 0.0f, 0.0f,
		25.5f, 0.0f, 0xFFFF0000, 0.0f, 0.0f,
		-25.5f, 25.5f, 0xFFFF0000, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1

	pMesh1 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh1, "Failed to create mesh 1!!");

	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// This shape has 2 triangles
	AEGfxTriAdd(
		-30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
		30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
		-30.0f, 30.0f, 0x00F00FFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
		30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);

	pMesh2 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh2, "Failed to create mesh 2!!");

	// Informing the library that we're about to start adding vertices
	AEGfxMeshStart();

	// This shape has 5 vertices

	AEGfxVertexAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f); //Point
	AEGfxVertexAdd(50.0f, -50.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxVertexAdd(100.0f, -50.0f, 0xFFFFFFFF, 0.0f, 0.0f); //Point
	AEGfxVertexAdd(50.0f, -100.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxVertexAdd(60.0f, -150.0f, 0xFFFFFFFF, 0.0f, 0.0f); //Point
	AEGfxVertexAdd(0.0f, -100.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxVertexAdd(-60.0f, -150.0f, 0xFFFFFFFF, 0.0f, 0.0f); //Point
	AEGfxVertexAdd(-50.0f, -100.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxVertexAdd(-100.0f, -50.0f, 0xFFFFFFFF, 0.0f, 0.0f); //Point
	AEGfxVertexAdd(-50.0f, -50.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxVertexAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f); //Point


	pMeshLine = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMeshLine, "Failed to create mesh 2!!");

	// Texture 1: From file
	pTex1 = AEGfxTextureLoad("Assets/PlanetTexture.png");
	AE_ASSERT_MESG(pTex1, "Failed to create texture1!!");


	// Texture 2: From memory
	// RGBA format
	colors[0] = 255;	colors[1] = 0;		colors[2] = 0;		colors[3] = 255;
	colors[4] = 0;		colors[5] = 255;	colors[6] = 0;		colors[7] = 255;
	colors[8] = 0;		colors[9] = 0;		colors[10] = 255;	colors[11] = 255;
	colors[12] = 255;	colors[13] = 255;	colors[14] = 255;	colors[15] = 255;

	pTex2 = AEGfxTextureLoadFromMemory(colors, 2, 2);
	// This step is optional, it creates a file from the texture argument
	AE_ASSERT_MESG(pTex2, "Failed to create texture2!!");



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
		150.0f, 75.0f, 0xFFFF0000, 0.5f, 0.0f,
		-150.0f, 75.0f, 0xFFFF0000, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1
	pMesh3 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh3, "Failed to create mesh 1!!");

	// Texture 1: From file
	pTex3 = AEGfxTextureLoad("CS230 Demo - Step by Step/runningcat.png");
	AE_ASSERT_MESG(pTex3, "Failed to create texture1!!");


	// Setting the texture offsets
	textureOffsets[0].mX = 0.0f;			textureOffsets[0].mY = 0.0f;
	textureOffsets[1].mX = 0.5f;			textureOffsets[1].mY = 0.0f;
	textureOffsets[2].mX = 0.0f;			textureOffsets[2].mY = 0.25f;
	textureOffsets[3].mX = 0.5f;			textureOffsets[3].mY = 0.25f;
	textureOffsets[4].mX = 0.0f;			textureOffsets[4].mY = 0.5f;
	textureOffsets[5].mX = 0.5f;			textureOffsets[5].mY = 0.5f;
	textureOffsets[6].mX = 0.0f;			textureOffsets[6].mY = 0.75f;
	textureOffsets[7].mX = 0.5f;			textureOffsets[7].mY = 0.75f;


	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);


	
	//float time = 0.0f;
	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		AESysUpdate();

		// Handling Input
		AEInputUpdate();

		
		//time = GravityTime();
		//Gravity(&triangle, time);

		if (AEInputCheckCurr(VK_UP))
		{

		}
		else if (AEInputCheckCurr(VK_DOWN))
		{

		}

		if (AEInputCheckCurr(VK_LEFT))
		{

		}
		else if (AEInputCheckCurr(VK_RIGHT))
		{

		}

		// Alpha value
		if (AEInputCheckCurr('Z'))
			alpha -= 0.01f;
		else
			if (AEInputCheckCurr('X'))
				alpha += 0.01f;

		alpha = AEClamp(alpha, 0.0f, 1.0f);

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

		// Texture offset
		if (AEInputCheckCurr('L'))
			obj1texX += 0.01f;
		else
			if (AEInputCheckCurr('J'))
				obj1texX -= 0.01f;

		if (AEInputCheckCurr('I'))
			obj1texY += 0.01f;
		else
			if (AEInputCheckCurr('K'))
				obj1texY -= 0.01f;

		frameCount++;
		if (frameCount % 3 == 0)
		{
			++currentTextureOffset;

			if (currentTextureOffset >= 8)
				currentTextureOffset = 0;
		}



		AEInputGetCursorPosition(&mouseX, &mouseY);
		AEGfxConvertScreenCoordinatesToWorld((float)mouseX, (float)mouseY, &mouseInWorldX, &mouseInWorldY);


		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetPosition(0.0f, 0.0f);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMeshLine, AE_GFX_MDM_LINES_STRIP);


		// Drawing object 1
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		// Set poisition for object 1
		AEGfxSetPosition(obj1X, obj1Y);
		// No texture for object 1
		AEGfxTextureSet(NULL, 0, 0);
		// Drawing the mesh (list of triangles)
		AEGfxSetTransparency(alpha);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMesh1, AE_GFX_MDM_TRIANGLES);

		if (frameCount % 5 == 0)
		{
			if (direction == 0)
			{
				color += 1.0f;

				if (color >= 100.0f)
				{
					direction = 1;
				}
			}

			if (direction == 1)
			{
				color -= 1.0f;

				if (color <= 0.0f)
				{
					direction = 0;
				}
			}

		}

		// Drawing object 2
		// Set position for object 2
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(100.0f, -60.0f);
		// Set texture for object 2
		AEGfxTextureSet(pTex1, obj1texX, obj1texY);
		AEGfxSetTransparency(1.0f);
		// Drawing the mesh (list of triangles)
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0f, 0.25f);
		AEGfxMeshDraw(pMesh2, AE_GFX_MDM_TRIANGLES);


		// Drawing object 2 again
		// Set poisition for object 2
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(-100.0f, -60.0f);
		// Set texture for object 2
		AEGfxTextureSet(pTex2, 0.0f, 0.0f);
		AEGfxSetTransparency(1.0f);
		// Drawing the mesh (list of triangles)
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMesh2, AE_GFX_MDM_TRIANGLES);


		// Drawing object 2 again and again
		// Set poisition for object 2
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(100.0f, 100.0f);
		// Set texture for object 2
		AEGfxTextureSet(pTex1, obj1texX, obj1texY);
		AEGfxSetTransparency(1.0f);
		AEGfxSetBlendColor(0.5f, 0.0f, 0.0, 0.75f);
		// Drawing the mesh (list of triangles)
		AEGfxMeshDraw(pMesh2, AE_GFX_MDM_TRIANGLES);


		// Drawing object 1
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		// Set poisition for object 1
		AEGfxSetPosition(mouseInWorldX, mouseInWorldY);
		// No texture for object 1
		AEGfxTextureSet(pTex3, textureOffsets[currentTextureOffset].mX, textureOffsets[currentTextureOffset].mY);
		// Drawing the mesh (list of triangles)
		AEGfxSetTransparency(1.0f);
		AEGfxMeshDraw(pMesh3, AE_GFX_MDM_TRIANGLES);



		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}

	// Freeing the objects and textures
	AEGfxMeshFree(pMesh1);
	AEGfxMeshFree(pMesh2);
	AEGfxMeshFree(pMeshLine);

	AEGfxTextureUnload(pTex1);
	AEGfxTextureUnload(pTex2);

	// Freeing the objects and textures
	AEGfxMeshFree(pMesh3);

	// Freeing the texture
	AEGfxTextureUnload(pTex3);

	// free the system
	AESysExit();

	return 1;
}
// ---------------------------------------------------------------------------

