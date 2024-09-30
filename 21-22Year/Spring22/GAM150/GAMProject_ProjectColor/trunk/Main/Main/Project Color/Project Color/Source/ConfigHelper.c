//------------------------------------------------------------------------------
//
// File Name:	ConfigHelper.c
// Author(s):	Emily Comes (emily.comes)
// Project:		"Project Color"
// Course:		GAM-150-B Spring 2022
//
// Copyright (C) 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "ConfigHelper.h"
#include "Serialization.h"
#include <stdio.h>
#include <Windows.h>

void ResetConfig(void)
{
  WriteConfigByte("game", "alreadysplashed", 0);

  // currently the only variable in the config file is scale, which is default 2
  WriteConfigByte("display", "scale", 2);
}

void WriteConfigByte(const char* section, const char* varName, char value)
{
  char* valueAsStr = (char*)calloc(5, sizeof(char));
  if (errno == ENOMEM || valueAsStr == NULL) // if memory allocation failed
    valueAsStr = NULL;
  else
  {
    sprintf_s(valueAsStr, 5, " %d", value);

    // Appends file path to localPath.
    char buffer[MAX_PATH] = { 0 };
    sprintf_s(buffer, sizeof(buffer), "%s\\%s", GetLocalPath(), "config.ini");

    WritePrivateProfileStringA(section, varName, valueAsStr, buffer);
    free(valueAsStr);
  }
}

char ReadConfigByte(const char* section, const char* varName, char minValue)
{
    // Appends file path to localPath.
    char buffer[MAX_PATH] = { 0 };
    sprintf_s(buffer, sizeof(buffer), "%s\\%s", GetLocalPath(), "config.ini");

  return (char)GetPrivateProfileIntA(section, varName, minValue, buffer);
}
