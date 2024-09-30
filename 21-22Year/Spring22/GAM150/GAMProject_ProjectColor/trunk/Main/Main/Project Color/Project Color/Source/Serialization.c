#include "stdafx.h"		// Should be included first in every .c file!
#include <sys/stat.h>
#include <direct.h>
#include "Serialization.h"
#include "Tile.h"
#include "Level.h"
#include "World.h"
#include "Player.h"
#include "Player Controller.h"
#include "Transform.h"
#include "GameObject.h"
#include "audio.h"
#include "fmod.h"

char localPath[MAX_PATH];
float saveTimeAlpha;

// Opens file picker dialogue.
BOOL OpenFilePicker(TCHAR[], SERIALIZEMODE, FILETYPE);

// Creates a path in ProgramData for storing save settings and level data.
int CreateLocalPath(void);

// Creates a new level based on file picker location.
int NewLevelWithPicker(const Level* level, const Tilemap* tilemap, char filePath[])
{
	CreateLocalPath();

	// Create wide char buffer.
	TCHAR buffer_w[MAX_PATH] = { 0 };

	// Tries to use file picker.
	if (!OpenFilePicker(buffer_w, SERIALIZEMODE_NEW, FILETYPE_LEVEL))
		return PICKERRESULT_CANCEL;

	// Converts file path to regular char.
	char buffer[MAX_PATH] = { 0 };
	size_t size;
	wcstombs_s(&size, buffer, sizeof(buffer), buffer_w, sizeof(buffer));

	// Save to file path.
	strcpy_s(filePath, MAX_PATH, buffer);
	return SaveLevelAtPath(level, tilemap, buffer);
}

// Saves a level based on file picker location.
int SaveLevelWithPicker(const Level* level, const Tilemap* tilemap)
{
	CreateLocalPath();

	// Create wide char buffer.
	TCHAR buffer_w[MAX_PATH] = { 0 };

	// Tries to use file picker.
	if (!OpenFilePicker(buffer_w, SERIALIZEMODE_SAVE, FILETYPE_LEVEL))
		return PICKERRESULT_CANCEL;

	// Converts file path to regular char.
	char buffer[MAX_PATH] = { 0 };
	size_t size;
	wcstombs_s(&size, buffer, sizeof(buffer), buffer_w, sizeof(buffer));

	// Save to file path.
	return SaveLevelAtPath(level, tilemap, buffer);
}

// Saves a level to a path.
int SaveLevelAtPath(const Level* level, const Tilemap* tilemap, const char* path)
{
	CreateLocalPath();

	// Open a file at filePath.
	FILE* file_ptr;
	fopen_s(&file_ptr, path, "w");

	// Check if file was successfully opened.
	if (file_ptr != NULL)
	{
		// Save the width and height of the level.
		putc(tilemap->width, file_ptr);
		putc(tilemap->height, file_ptr);
		putc(level->levelNum, file_ptr);

		// Iterate through y dimension.
		for (int y = 0; y < MAX_LEVEL_HEIGHT; ++y)
		{
			// Iterate through x dimension.
			for (int x = 0; x < MAX_LEVEL_WIDTH; ++x)
			{
				// Save the tile type for each position on the plane.
				putc((int)tilemap->tiles[x][y]->id, file_ptr);
			}
		}

		// Close the file.
		fclose(file_ptr);

		return PICKERRESULT_SUCCESS;
	}

	return PICKERRESULT_FAIL;
}

// Loads a level based on file picker location.
int LoadLevelWithPicker(Level* level, Tilemap* tilemap)
{
	CreateLocalPath();

	// Create wide char buffer.
	TCHAR buffer_w[MAX_PATH] = { 0 };

	// Tries to use file picker.
	if (!OpenFilePicker(buffer_w, SERIALIZEMODE_LOAD, FILETYPE_LEVEL))
		return PICKERRESULT_CANCEL;

	// Converts file path to regular char.
	char buffer[MAX_PATH] = { 0 };
	size_t size;
	wcstombs_s(&size, buffer, sizeof(buffer), buffer_w, sizeof(buffer));

	// Load from file path.
	return LoadLevelAtPath(level, tilemap, buffer);
}

// Loads a level from a path.
int LoadLevelAtPath(Level* level, Tilemap* tilemap, const char* path)
{
	CreateLocalPath();

	// Open a file at filePath.
	FILE* file_ptr;
	fopen_s(&file_ptr, path, "r");

	// Check if file was successfully opened.
	if (file_ptr != NULL)
	{
		// Load the level name.
		strcpy_s(level->filePath, sizeof(level->filePath), path);

		// Get information about the tiles.
		tilemap->width = (unsigned char)getc(file_ptr);
		tilemap->height = (unsigned char)getc(file_ptr);
		level->levelNum = (unsigned char)getc(file_ptr);

		// Iterate through y dimension.
		for (int y = 0; y < MAX_LEVEL_HEIGHT; ++y)
		{
			// Iterate through x dimension.
			for (int x = 0; x < MAX_LEVEL_WIDTH; ++x)
			{
				// Set the tile based on the tiles in the tile list.
				tilemap->tiles[x][y] = GetTileAt(getc(file_ptr));
			}
		}

		// Close the file.
		fclose(file_ptr);

		return PICKERRESULT_SUCCESS;
	}

	return PICKERRESULT_FAIL;
}

// Creates a new world based on file picker location.
int NewWorldWithPicker(const World* world, char filePath[])
{
	CreateLocalPath();

	// Create wide char buffer.
	TCHAR buffer_w[MAX_PATH] = { 0 };

	// Tries to use file picker.
	if (!OpenFilePicker(buffer_w, SERIALIZEMODE_NEW, FILETYPE_WORLD))
		return PICKERRESULT_CANCEL;

	// Converts file path to regular char.
	char buffer[MAX_PATH] = { 0 };
	size_t size;
	wcstombs_s(&size, buffer, sizeof(buffer), buffer_w, sizeof(buffer));

	// Save to file path.
	strcpy_s(filePath, MAX_PATH, buffer);
	return SaveWorldAtPath(world, buffer);
}

// Saves a world based on file picker location.
int SaveWorldWithPicker(const World* world)
{
	CreateLocalPath();

	// Create wide char buffer.
	TCHAR buffer_w[MAX_PATH] = { 0 };

	// Tries to use file picker.
	if (!OpenFilePicker(buffer_w, SERIALIZEMODE_SAVE, FILETYPE_WORLD))
		return PICKERRESULT_CANCEL;

	// Converts file path to regular char.
	char buffer[MAX_PATH] = { 0 };
	size_t size;
	wcstombs_s(&size, buffer, sizeof(buffer), buffer_w, sizeof(buffer));

	// Save to file path.
	return SaveWorldAtPath(world, buffer);
}

// Saves a world to a path.
int SaveWorldAtPath(const World* world, const char* path)
{
	CreateLocalPath();

	// Open a file at filePath.
	FILE* file_ptr;
	fopen_s(&file_ptr, path, "w");

	// Check if file was successfully opened.
	if (file_ptr != NULL)
	{
		// Save info about how many levels are in the world.
		fprintf_s(file_ptr, "%d\n", world->roomCount);

		// Save information about each of the areas.
		for (int i = 0; i < MAX_ROOM_COUNT; ++i)
		{
			fprintf_s(file_ptr, "%s\n", world->rooms[i].filePath);
			fprintf_s(file_ptr, "%u %u\n", world->rooms[i].x, world->rooms[i].y);
		}

		// Close the file.
		fclose(file_ptr);

		return PICKERRESULT_SUCCESS;
	}

	return PICKERRESULT_FAIL;
}

// Loads a world based on file picker location.
int LoadWorldWithPicker(World* world)
{
	CreateLocalPath();

	// Create wide char buffer.
	TCHAR buffer_w[MAX_PATH] = { 0 };

	// Tries to use file picker.
	if (!OpenFilePicker(buffer_w, SERIALIZEMODE_LOAD, FILETYPE_WORLD))
		return PICKERRESULT_CANCEL;

	// Converts file path to regular char.
	char buffer[MAX_PATH] = { 0 };
	size_t size;
	wcstombs_s(&size, buffer, sizeof(buffer), buffer_w, sizeof(buffer));

	// Load from file path.
	return LoadWorldAtPath(world, buffer);
}

// Loads a world from a path.
int LoadWorldAtPath(World* world, const char* path)
{
	CreateLocalPath();

	// Open a file at filePath.
	FILE* file_ptr;
	fopen_s(&file_ptr, path, "r");

	// Check if file was successfully opened.
	if (file_ptr != NULL)
	{
		// Load the world's name.
		strcpy_s(world->filePath, sizeof(world->filePath), path);

		// Save info about how many levels are in the world.
		fscanf_s(file_ptr, "%d\n", &world->roomCount);

		// Save information about each of the areas.
		for (int i = 0; i < MAX_ROOM_COUNT; ++i)
		{
			fscanf_s(file_ptr, "%s\n", &world->rooms[i].filePath, MAX_PATH);
			fscanf_s(file_ptr, "%u %u\n", &world->rooms[i].x, &world->rooms[i].y);
		}

		// Close the file.
		fclose(file_ptr);

		return PICKERRESULT_SUCCESS;
	}

	return PICKERRESULT_FAIL;
}

// Opens file picker dialogue.
BOOL OpenFilePicker(TCHAR buffer[], SERIALIZEMODE mode, FILETYPE type)
{
	// Define a new openfile.
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(ofn));

	// Set openfile values.
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = type == FILETYPE_LEVEL ? L"Level Files (.lvl)\0*.lvl\0\0" : L"World Files (.wld)\0*.wld\0\0";
	ofn.lpstrTitle = mode == SERIALIZEMODE_SAVE ? L"Save As" : mode == SERIALIZEMODE_NEW ? L"New" : L"Open";
	ofn.lpstrFile = buffer;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_PATH;

	// Use a different action based on serialiation mode.
	switch (mode)
	{
		case SERIALIZEMODE_NEW:
			// Set flags.
			ofn.Flags =
				OFN_CREATEPROMPT |
				OFN_DONTADDTORECENT |
				OFN_ENABLESIZING |
				OFN_EXPLORER |
				OFN_HIDEREADONLY |
				OFN_NONETWORKBUTTON |
				OFN_PATHMUSTEXIST |
				OFN_OVERWRITEPROMPT;

			// Open new file prompt.
			return GetSaveFileName(&ofn);
			break;

		case SERIALIZEMODE_SAVE:
			// Set flags.
			ofn.Flags =
				OFN_DONTADDTORECENT |
				OFN_ENABLESIZING |
				OFN_EXPLORER |
				OFN_FILEMUSTEXIST |
				OFN_HIDEREADONLY |
				OFN_NONETWORKBUTTON |
				OFN_OVERWRITEPROMPT;

			// Open save file prompt.
			return GetSaveFileName(&ofn);
			break;

		case SERIALIZEMODE_LOAD:
			// Set flags.
			ofn.Flags =
				OFN_DONTADDTORECENT |
				OFN_ENABLESIZING |
				OFN_EXPLORER |
				OFN_FILEMUSTEXIST |
				OFN_HIDEREADONLY |
				OFN_NONETWORKBUTTON;

			// Open open file prompt.
			return GetOpenFileName(&ofn);
			break;

		default:
			return 0;
			break;
	}
}

// Saves the player's information.
int SavePlayerData(const char* path)
{
	CreateLocalPath();

	// Appends file path to localPath.
	char buffer[MAX_PATH] = { 0 };
	sprintf_s(buffer, sizeof(buffer), "%s\\%s", localPath, path);

	// Open a file at filePath.
	FILE* file_ptr;
	fopen_s(&file_ptr, buffer, "w");

	// Check if file was successfully opened.
	if (file_ptr != NULL)
	{
		// Save if the player has the paintbrush.
		fprintf_s(file_ptr, "%d\n", GetPlayer()->hasBrush);

		// Save the player's max health.
		fprintf_s(file_ptr, "%d\n", GetPlayer()->maxHealth);
		// Save the player's current health.
		fprintf_s(file_ptr, "%d\n", GetPlayer()->health);

		// Save the player's max paint.
		fprintf_s(file_ptr, "%f\n", GetPlayer()->maxPaint);
		// Save the player's current paint.
		fprintf_s(file_ptr, "%f\n", GetPlayer()->paint);
		// Save the player's current selected color.
		fprintf_s(file_ptr, "%d\n", GetPlayer()->currColor);
		// Save the player's list of collected colors.
		fprintf_s(file_ptr, "%d\n", GetPlayer()->unlockedColors);

		// Save the player's location (room).
		fprintf_s(file_ptr, "%d\n", GetPlayer()->roomNum);
		// Save the player's location (x).
		fprintf_s(file_ptr, "%d\n", GetPlayer()->roomPosX);
		// Save the player's location (y).
		fprintf_s(file_ptr, "%d\n", GetPlayer()->roomPosY);

		// Save the player's default iFrame info
		fprintf_s(file_ptr, "%d\n", GetPlayer()->iFrames);
		fprintf_s(file_ptr, "%f\n", GetPlayer()->iFrameTime);

		// Save the player's gem info.
		fprintf_s(file_ptr, "%d\n", GetPlayer()->gemCount);

		// Close the file.
		fclose(file_ptr);

		return 0;
	}

	return -1;
}

// Loads the saved player information.
int LoadPlayerData(const char* path)
{
	CreateLocalPath();

	// Appends file path to localPath.
	char buffer[MAX_PATH] = { 0 };
	sprintf_s(buffer, sizeof(buffer), "%s\\%s", localPath, path);

	// Open a file at filePath.
	FILE* file_ptr;
	fopen_s(&file_ptr, buffer, "r");

	// Check if file was successfully opened.
	if (file_ptr != NULL)
	{
		// Load if the player has the paintbrush.
		fscanf_s(file_ptr, "%d\n", &GetPlayer()->hasBrush);

		// Load the player's max health.
		fscanf_s(file_ptr, "%d\n", &GetPlayer()->maxHealth);
		// Load the player's current health.
		fscanf_s(file_ptr, "%d\n", &GetPlayer()->health);

		// Load the player's max paint.
		fscanf_s(file_ptr, "%f\n", &GetPlayer()->maxPaint);
		// Load the player's current paint.
		fscanf_s(file_ptr, "%f\n", &GetPlayer()->paint);
		// Load the player's current selected color.
		fscanf_s(file_ptr, "%d\n", &GetPlayer()->currColor);
		// Load the player's list of collected colors.
		fscanf_s(file_ptr, "%d\n", &GetPlayer()->unlockedColors);

		// Load the player's location (room).
		fscanf_s(file_ptr, "%d\n", &GetPlayer()->roomNum);
		// Load the player's location (x).
		fscanf_s(file_ptr, "%d\n", &GetPlayer()->roomPosX);
		// Load the player's location (y).
		fscanf_s(file_ptr, "%d\n", &GetPlayer()->roomPosY);
		
		// Load the player's default iFrame info
		fscanf_s(file_ptr, "%d\n", &GetPlayer()->iFrames);
		fscanf_s(file_ptr, "%f\n", &GetPlayer()->iFrameTime);

		// Load the player's gem info.
		fscanf_s(file_ptr, "%d\n", &GetPlayer()->gemCount);

		// Close the file.
		fclose(file_ptr);

		return 0;
	}

	return -1;
}

// Saves the current settings.
int SaveOptionsData(const char* path)
{
	CreateLocalPath();

	// Appends file path to localPath.
	char buffer[MAX_PATH] = { 0 };
	sprintf_s(buffer, sizeof(buffer), "%s\\%s", localPath, path);

	// Open a file at filePath.
	FILE* file_ptr;
	fopen_s(&file_ptr, buffer, "w");

	// Check if file was successfully opened.
	if (file_ptr != NULL)
	{
		/*
		// Save the fullscreen status.
		fprintf_s(file_ptr, "%c\n", 1);
		
		// Save the music volume status.
		fprintf_s(file_ptr, "%f\n", );

		// Save the sfx volume status.
		fprintf_s(file_ptr, "%f\n", );

		// Save the ui volume status.
		fprintf_s(file_ptr, "%f\n", );
		*/

		// Close the file.
		fclose(file_ptr);

		return 0;
	}

	return -1;
}

// Loads the saved settings.
int LoadOptionsData(const char* path)
{
	CreateLocalPath();

	// Appends file path to localPath.
	char buffer[MAX_PATH] = { 0 };
	sprintf_s(buffer, sizeof(buffer), "%s\\%s", localPath, path);

	// Open a file at filePath.
	FILE* file_ptr;
	fopen_s(&file_ptr, buffer, "r");

	// Check if file was successfully opened.
	if (file_ptr != NULL)
	{
		/*
		// Load the fullscreen status.
		fscanf_s(file_ptr, "%c\n", 1);

		// Load the music volume status.
		fscanf_s(file_ptr, "%f\n", );

		// Load the sfx volume status.
		fscanf_s(file_ptr, "%f\n", );

		// Load the ui volume status.
		fscanf_s(file_ptr, "%f\n", );
		*/

		// Close the file.
		fclose(file_ptr);

		return 0;
	}

	return -1;
}

// Saves the boss data.
int SaveBossData(const char* path)
{
	CreateLocalPath();

	// Appends file path to localPath.
	char buffer[MAX_PATH] = { 0 };
	sprintf_s(buffer, sizeof(buffer), "%s\\%s", localPath, path);

	// Open a file at filePath.
	FILE* file_ptr;
	fopen_s(&file_ptr, buffer, "w");

	// Check if file was successfully opened.
	if (file_ptr != NULL)
	{
		// Save information about each boss (whether it's alive or not) recursively.

		// Close the file.
		fclose(file_ptr);

		return 0;
	}

	return -1;
}

// Loads the boss data.
int LoadBossData(const char* path)
{
	CreateLocalPath();

	// Appends file path to localPath.
	char buffer[MAX_PATH] = { 0 };
	sprintf_s(buffer, sizeof(buffer), "%s\\%s", localPath, path);

	// Open a file at filePath.
	FILE* file_ptr;
	fopen_s(&file_ptr, buffer, "r");

	// Check if file was successfully opened.
	if (file_ptr != NULL)
	{
		// Load information about each boss (whether it's alive or not) recursively.

		// Close the file.
		fclose(file_ptr);

		return 0;
	}

	return -1;
}

// Saves the collectible settings.
int SaveCollectibleData(const char* path)
{
	CreateLocalPath();

	// Appends file path to localPath.
	char buffer[MAX_PATH] = { 0 };
	sprintf_s(buffer, sizeof(buffer), "%s\\%s", localPath, path);

	// Open a file at filePath.
	FILE* file_ptr;
	fopen_s(&file_ptr, buffer, "w");

	// Check if file was successfully opened.
	if (file_ptr != NULL)
	{
		// Save information about all collected gems.
		for (int i = 0; i < MAX_GEM_COUNT; ++i)
		{
			fprintf_s(file_ptr, "%c\n", GetWorld()->gems[i]);
		}

		// Save information about all collected heart containers.
		for (int i = 0; i < MAX_HEART_CONTAINER_COUNT; ++i)
		{
			fprintf_s(file_ptr, "%c\n", GetWorld()->heartContainers[i]);
		}

		// Save information about all collected paint containers.
		for (int i = 0; i < MAX_PAINT_CONTAINER_COUNT; ++i)
		{
			fprintf_s(file_ptr, "%c\n", GetWorld()->paintContainers[i]);
		}

		// Save information about all activated checkpoints.
		for (int i = 0; i < MAX_CHECKPOINT_FLAG_COUNT; ++i)
		{
			fprintf_s(file_ptr, "%c\n", GetWorld()->checkpointFlags[i]);
		}

		// Close the file.
		fclose(file_ptr);

		return 0;
	}

	return -1;
}

// Loads the collectible settings.
int LoadCollectibleData(const char* path)
{
	CreateLocalPath();

	// Appends file path to localPath.
	char buffer[MAX_PATH] = { 0 };
	sprintf_s(buffer, sizeof(buffer), "%s\\%s", localPath, path);

	// Open a file at filePath.
	FILE* file_ptr;
	fopen_s(&file_ptr, buffer, "r");

	// Check if file was successfully opened.
	if (file_ptr != NULL)
	{
		// Save information about all collected gems.
		for (int i = 0; i < MAX_GEM_COUNT; ++i)
		{
			fscanf_s(file_ptr, "%c\n", &GetWorld()->gems[i], 1);
		}

		// Save information about all collected heart containers.
		for (int i = 0; i < MAX_HEART_CONTAINER_COUNT; ++i)
		{
			fscanf_s(file_ptr, "%c\n", &GetWorld()->heartContainers[i], 1);
		}

		// Save information about all collected paint containers.
		for (int i = 0; i < MAX_PAINT_CONTAINER_COUNT; ++i)
		{
			fscanf_s(file_ptr, "%c\n", &GetWorld()->paintContainers[i], 1);
		}

		// Save information about all activated checkpoints.
		for (int i = 0; i < MAX_CHECKPOINT_FLAG_COUNT; ++i)
		{
			fscanf_s(file_ptr, "%c\n", &GetWorld()->checkpointFlags[i], 1);
		}

		// Close the file.
		fclose(file_ptr);

		return 0;
	}

	return -1;
}

// Sets all settings and data back to the default (resets the game!)
int ResetAllData(void)
{
	char playerBuffer[MAX_PATH] = { 0 };
	sprintf_s(playerBuffer, sizeof(playerBuffer), "%s\\player_data.svd", localPath);
	remove(playerBuffer);

	char optionsBuffer[MAX_PATH] = { 0 };
	sprintf_s(optionsBuffer, sizeof(optionsBuffer), "%s\\options_data.svd", localPath);
	remove(optionsBuffer);

	char bossBuffer[MAX_PATH] = { 0 };
	sprintf_s(bossBuffer, sizeof(bossBuffer), "%s\\boss_data.svd", localPath);
	remove(bossBuffer);

	char collectibleBuffer[MAX_PATH] = { 0 };
	sprintf_s(collectibleBuffer, sizeof(collectibleBuffer), "%s\\collectible_data.svd", localPath);
	remove(collectibleBuffer);

	return 1;
}

// Saves all data.
int SaveAllData(void)
{
	// Record the player's current room num.
	GetPlayer()->roomNum = GetCurrentRoom()->roomNum;

	Vector2D playerPos = *TransformGetTranslation(GameObjectGetTransform(GetPlayer()->parent));
	GetPlayer()->roomPosX = (int)playerPos.x;
	GetPlayer()->roomPosY = (int)playerPos.y;

	GetPlayer()->health = GetPlayer()->maxHealth;
	GetPlayer()->paint = GetPlayer()->maxPaint;

	// Save.
	SavePlayerData("player_data.svd");
	SaveOptionsData("options_data.svd");
	SaveBossData("boss_data.svd");
	SaveCollectibleData("collectible_data.svd");

	saveTimeAlpha = 1.0F;

	return 1;
}

// Creates a path in ProgramData for storing save settings and level data.
int CreateLocalPath(void)
{
	if (strnlen_s(localPath, MAX_PATH) != 0)
		return -1;

	struct stat sb;

	char path[MAX_PATH];
	size_t required_size;
	getenv_s(&required_size, path, sizeof(path), "ProgramData");

	sprintf_s(localPath, sizeof(localPath), "%s\\TheBlindProgrammers", path);

	if (stat(localPath, &sb) == -1) {
		return _mkdir(localPath);
	}

	return -1;
}

// Returns the local path for storing save data.
const char* GetLocalPath(void)
{
	if (strnlen_s(localPath, MAX_PATH) == 0)
	{
		CreateLocalPath();
	}

	return localPath;
}

// Returns whether or not a file exists.
int DoesFileExist(const char* path)
{
	struct stat sb;

	char filePath[MAX_PATH] = { 0 };
	sprintf_s(filePath, sizeof(filePath), "%s\\%s", localPath, path);

	return stat(filePath, &sb);
}

float GetSaveTimeAlpha(void)
{
	return saveTimeAlpha;
}

void SetSaveTimeAlpha(float alpha)
{
	saveTimeAlpha = alpha;
}