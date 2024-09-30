// ---------------------------------------------------------------------------
// Project Name		:	Simple Demo
// File Name		:	Main.cpp
// Author			:	Brody Endstrasser
// Creation Date	:	01-13-22
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
#include "AABB.h"

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
	float obj1X = 0.0f, obj1Y = 0.0f;		// Position variables for object 1
	float obj1texX = 0, obj1texY = 0;		// Texture variables for object 2's texture
	AEGfxVertexList* pMesh1;				// Pointer to Mesh (Model)
	AEGfxVertexList* pMesh2;				// Pointer to Mesh (Model)
	AEGfxVertexList* pMesh3;				// Pointer to Mesh (Model)
	AEGfxVertexList* pMeshSquare;			// Pointer to Mesh (Model)
	AEGfxTexture* pTex1;					// Pointer to Texture (Image)
	AEGfxTexture* pTex2;					// Pointer to Texture 2 (Image)
	AEGfxTexture* pTex3;					// Pointer to Texture 3 (Image)
	float camX, camY;						// Used to temporary store the camera position
	float alpha = 1.0f;						// Max alpha value
	float playerAngle = 0.0f;				// player angle
	float gridtexX = 0, gridtexY = 0;

	signed long mouseX, mouseY;				// Will store the mouse position (In the SCREEN coordinates system)
	float mouseInWorldX, mouseInWorldY;		// Will store the mouse position (In the WORLD coordinates system)

	TextureOffset	textureOffsets[8];		// Array of texture offsets, using for the sprite animations
	unsigned int currentTextureOffset = 0;

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

	//--------------------------------------------------
	// setting the player character mesh
	//--------------------------------------------------
	// 0xAARRGGBB - alpha, red green blue
	AEGfxTriAdd(
		-10.5f, -10.5f, 0xFFFF0000, 0.0f, 0.0f,
		35.5f, 0.0f, 0x00FF0000, 0.0f, 0.0f,
		-10.5f, 10.5f, 0xFFFF0000, 0.0f, 0.0f);

	// Saving the mesh (list of triangles) in pMesh1

	pMesh1 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh1, "Failed to create mesh 1!!");

	
	//--------------------------------------------------
	//square shape
	//---------------------------------------------------
	
	// Informing the library that we're about to start adding triangles

	tileInst collisiontile;

	if (pMesh2) {
		 collisiontile = DefineSquareMesh(pMesh2, 60.0f, 60.0f);
		 CreateAABBFromTile(collisiontile);
	}
		

	//--------------------------------------------------
	//rectangle shape
	//---------------------------------------------------

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
	AE_ASSERT_MESG(pMesh1, "Failed to create mesh 3!!");

	//--------------------------------------------------
	// Setting the texture offsets
	//--------------------------------------------------

	textureOffsets[0].mX = 0.0f;			textureOffsets[0].mY = 0.0f;
	textureOffsets[1].mX = 0.5f;			textureOffsets[1].mY = 0.0f;
	textureOffsets[2].mX = 0.0f;			textureOffsets[2].mY = 0.25f;
	textureOffsets[3].mX = 0.5f;			textureOffsets[3].mY = 0.25f;
	textureOffsets[4].mX = 0.0f;			textureOffsets[4].mY = 0.5f;
	textureOffsets[5].mX = 0.5f;			textureOffsets[5].mY = 0.5f;
	textureOffsets[6].mX = 0.0f;			textureOffsets[6].mY = 0.75f;
	textureOffsets[7].mX = 0.5f;			textureOffsets[7].mY = 0.75f;

	
	//--------------------------------------------------
	//adding vertexes to a mesh
	//--------------------------------------------------
	
	AEGfxMeshStart();

	AEGfxVertexAdd(0.0f, 0.0f, 0xFF32a852, 0.0f, 0.0f);
	AEGfxVertexAdd(0.0f, 100.0f, 0xFF502bad, 0.0f, 0.0f);
	AEGfxVertexAdd(100.0f, 100.0f, 0xFF972bad, 0.0f, 0.0f);
	AEGfxVertexAdd(100.0f, 0.0f, 0xFFb01c44, 0.0f, 0.0f);


	pMeshSquare = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMeshSquare, "Failed to create mesh 2!!");


	//--------------------------------------------------
	// Texture 1: Planet
	//--------------------------------------------------
	pTex1 = AEGfxTextureLoad("PlanetTexture.png");
	AE_ASSERT_MESG(pTex1, "Failed to create texture1!!");

	//--------------------------------------------------
	// Texture 2: 4 colors
	//--------------------------------------------------
	// RGBA format
	colors[0] = 110;	colors[1] = 38;		colors[2] = 5;		colors[3] = 255;
	colors[4] = 100;	colors[5] = 176;	colors[6] = 0;		colors[7] = 255;
	colors[8] = 130;	colors[9] = 20;		colors[10] = 20;	colors[11] = 255;
	colors[12] = 100;	colors[13] = 100;	colors[14] = 255;	colors[15] = 255;

	pTex2 = AEGfxTextureLoadFromMemory(colors, 2, 2);
	// This step is optional, it creates a file from the texture argument
	AE_ASSERT_MESG(pTex2, "Failed to create texture2!!");

	//--------------------------------------------------
	// Texture 3: running cat
	//--------------------------------------------------
	// Texture 1: From file
	pTex3 = AEGfxTextureLoad("runningcat.png");
	AE_ASSERT_MESG(pTex1, "Failed to create texture3!!");


	//--------------------------------------------------
	// background color
	//--------------------------------------------------
	AEGfxSetBackgroundColor(0.5f, 0.4f, 0.3f);

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();
		AESysUpdate();

		// Handling Input
		AEInputUpdate();

		//--------------------------------------------------
		// Object Control
		//--------------------------------------------------
		if (AEInputCheckCurr('W'))
			obj1Y += 3.0f;
		else if (AEInputCheckCurr('S'))
			obj1Y -= 3.0f;

		if (AEInputCheckCurr('A'))
			obj1X -= 3.0f;
		else if (AEInputCheckCurr('D'))
			obj1X += 3.0f;

		if (AEInputCheckCurr('Q'))
			playerAngle -= 2.0f;
		else if (AEInputCheckCurr('E'))
			playerAngle += 2.0f;

		AEClamp(playerAngle, -360.0f, 360.0f);


		//--------------------------------------------------
		// settting alpha value
		//--------------------------------------------------
		if (AEInputCheckCurr('Z'))
			alpha -= 0.01f;
		else
			if (AEInputCheckCurr('X'))
				alpha += 0.01f;

		//checks if alpha is above or below the max values and clamps it to one. 
		//so the alpha value is between 0 and 1
		alpha = AEClamp(alpha, 0.0f, 1.0f);

		//--------------------------------------------------
		// Move the camera
		//--------------------------------------------------
		AEGfxGetCamPosition(&camX, &camY);
		if (AEInputCheckCurr(VK_UP))
			AEGfxSetCamPosition(camX, camY + 4);
		else
			if (AEInputCheckCurr(VK_DOWN))
				AEGfxSetCamPosition(camX, camY - 4);

		AEGfxGetCamPosition(&camX, &camY);
		if (AEInputCheckCurr(VK_LEFT))
			AEGfxSetCamPosition(camX - 4, camY);
		else
			if (AEInputCheckCurr(VK_RIGHT))
				AEGfxSetCamPosition(camX + 4, camY);

		AEInputGetCursorPosition(&mouseX, &mouseY);
		AEGfxConvertScreenCoordinatesToWorld((float)mouseX, (float)mouseY, &mouseInWorldX, &mouseInWorldY);


		//--------------------------------------------------
		// Texture offset input
		//--------------------------------------------------
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

		//--------------------------------------------------
		// advance texture offset
		//--------------------------------------------------
		if (AEInputCheckTriggered(VK_SPACE))
		{
			++currentTextureOffset;

			if (currentTextureOffset >= 8)
				currentTextureOffset = 0;
		}

		//--------------------------------------------------
		//draw mesh line
		//--------------------------------------------------
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetPosition(0.0f, 0.0f);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMeshSquare, AE_GFX_MDM_TRIANGLES);

		AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		//--------------------------------------------------
		// Drawing player object
		//--------------------------------------------------
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		// Set poisition for object 1
		AEGfxSetFullTransform(obj1X, obj1Y, playerAngle, 1.0f, 1.0f);
		//AEGfxSetPosition(obj1X, obj1Y);
		// No texture for object 1
		AEGfxTextureSet(NULL, 0, 0);
		// Drawing the mesh (list of triangles)
		AEGfxSetTransparency(alpha);
		AEGfxMeshDraw(pMesh1, AE_GFX_MDM_TRIANGLES);


		gridtexX += 0.01f;
		gridtexY += 0.01f;

		AEClamp(gridtexX, 0.0f, 255.0f);
		AEClamp(gridtexY, 0.0f, 255.0f);
		
		if (collisiontile) {
			//--------------------------------------------------
			// Drawing colored grid
			//--------------------------------------------------
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetPosition(100.0f, -60.0f);
			// Set texture for object 2
			AEGfxTextureSet(pTex2, gridtexX, gridtexY);
			AEGfxSetTransparency(1.0f);
			AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
			// Drawing the mesh (list of triangles)
			AEGfxMeshDraw(collisiontile->Mesh, AE_GFX_MDM_TRIANGLES);

			//--------------------------------------------------
			// Drawing planet
			//--------------------------------------------------
			// Set position for object 2
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetPosition(-100.0f, -60.0f);
			// Set texture for object 2
			AEGfxTextureSet(pTex1, obj1texX, obj1texY);
			AEGfxSetTransparency(1.0f);
			AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
			// Drawing the mesh (list of triangles)
			AEGfxMeshDraw(collisiontile->Mesh, AE_GFX_MDM_TRIANGLES);

			//--------------------------------------------------
			// Drawing reddish planet
			//--------------------------------------------------
			// Set position for object 2
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetPosition(-50.0f, -60.0f);
			// Set texture for object 2
			AEGfxTextureSet(pTex1, obj1texX, obj1texY);
			AEGfxSetTransparency(1.0f);
			AEGfxSetBlendColor(0.5f, 0.0f, 0.0, 0.75f);
			// Drawing the mesh (list of triangles)
			AEGfxMeshDraw(collisiontile->Mesh, AE_GFX_MDM_TRIANGLES);

			//--------------------------------------------------
			// Drawing running cat
			//--------------------------------------------------
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			// Set poisition for object 1
			AEGfxSetPosition(200.0f, -200.0f);
			// No texture for object 1
			AEGfxTextureSet(pTex3, textureOffsets[currentTextureOffset].mX, textureOffsets[currentTextureOffset].mY);
			// Drawing the mesh (list of triangles)
			AEGfxSetTransparency(1.0f);
			AEGfxMeshDraw(pMesh3, AE_GFX_MDM_TRIANGLES);


			//--------------------------------------------------
			// Drawing mouse object
			//--------------------------------------------------
			// Drawing object 1
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			// Set position for object 1
			AEGfxSetPosition(mouseInWorldX, mouseInWorldY);
			// No texture for object 1
			AEGfxTextureSet(NULL, 0, 0);
			// Drawing the mesh (list of triangles)
			AEGfxSetTransparency(1.0f);
			AEGfxMeshDraw(pMesh1, AE_GFX_MDM_TRIANGLES);
		}

		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}

	// Freeing the objects and textures
	AEGfxMeshFree(pMesh1);
	AEGfxMeshFree(pMesh3);
	AEGfxMeshFree(pMeshSquare);

	FreeTileInst(collisiontile);

	AEGfxTextureUnload(pTex1);
	AEGfxTextureUnload(pTex2);
	AEGfxTextureUnload(pTex3);

	// free the system
	AESysExit();

	return 1;
}

// ---------------------------------------------------------------------------

