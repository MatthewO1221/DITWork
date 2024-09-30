#include "stdafx.h"
#include <stdarg.h>

#include "AEEngine.h"
#include "Stream.h"
#include "Vector2D.h"
#include "Trace.h"

static Stream streamFile;

static errno_t err = 5;
static char tokenBuffer[1024];
char *errorMsg;

Stream StreamOpen(const char* filename)
{
	err = fopen_s(&streamFile, filename, "rt");

	if (err == 0)
	{
		return streamFile;
	}
	else
	{
		strerror_s(errorMsg, 80, err);
		TraceMessage("Error: StreamOpen could not open file %s; %s", filename, errorMsg);
		return NULL;
	}
}

int StreamReadInt(Stream stream)
{
	int temp;

	if (stream != NULL)
	{
		fscanf_s(stream, "%i", &temp);
		return temp;
	}
	else
	{
		return 0;
	}
}

float StreamReadFloat(Stream stream)
{
	float temp;

	if (stream != NULL)
	{
		fscanf_s(stream, "%f", &temp);
		return temp;
	}
	else
	{
		return 0;
	}
}

void StreamReadVector2D(Stream stream, Vector2D* vector)
{
	if (stream != NULL && &vector != NULL)
	{
		vector->x = StreamReadFloat(stream);
		vector->y = StreamReadFloat(stream);
	}
}

// Read a token (a single word) from a file.
// Suggested steps:
//	 - Set the first value in tokenBuffer to 0
//	 - If the stream was opened successfully,
//	   - Read a string ("%s") into tokenBuffer using fscanf_s()
//	 - Return tokenBuffer
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 Pointer to tokenBuffer
const char* StreamReadToken(Stream stream)
{
	tokenBuffer[0] = '0';

	if (stream)
	{
		fscanf_s(stream, "%s", tokenBuffer, sizeof(tokenBuffer));
		return tokenBuffer;
	}
	else
	{
		return NULL;
	}
}

void StreamClose(Stream* stream)
{
	if (&stream != NULL)
	{
		AESysPrintf("FileClosed\n");
		fclose(*stream);
	}
}