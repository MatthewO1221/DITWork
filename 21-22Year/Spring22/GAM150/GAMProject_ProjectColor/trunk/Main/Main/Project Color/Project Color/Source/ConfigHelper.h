//------------------------------------------------------------------------------
//
// File Name:	ConfigHelper.h
// Author(s):	Emily Comes (emily.comes)
// Project:		"Project Color"
// Course:		GAM-150-B Spring 2022
//
// Copyright (C) 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Public Function Declarations:
//------------------------------------------------------------------------------

// Resets variables in the config file to be their defaults
void ResetConfig(void);

// Writes an 8-bit integer (byte/char) to the config file
void WriteConfigByte(const char* section, const char* varName, char value);

// Reads an 8-bit integer (byte/char) to the config file
char ReadConfigByte(const char* section, const char* varName, char minValue);
