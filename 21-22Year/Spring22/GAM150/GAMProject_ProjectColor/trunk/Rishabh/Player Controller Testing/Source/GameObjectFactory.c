//------------------------------------------------------------------------------
//
// File Name:	GameObjectFactory.c
// Author(s):	Rishabh Anand
// Project:		Project 2
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObjectFactory.h"
#include "GameObject.h"
#include "AEEngine.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

GameObjectPtr GameObjectFactoryBuild(const char* filename)
{
	if (filename)
	{
		Stream factoryStream = StreamOpen(filename);
		if (factoryStream)
		{
			const char* factoryString = StreamReadToken(factoryStream);

			AESysPrintf("factoryString is %s\n", factoryString);

			if (strncmp(factoryString, "GameObject", _countof("GameObject")) == 0)
			{
				GameObjectPtr factoryObject = GameObjectCreate();
				GameObjectRead(factoryObject, factoryStream);
				StreamClose(&factoryStream);

				return factoryObject;
			}
			else
			{
				AESysPrintf("factoryString failed\n");
				return NULL;
			}
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

