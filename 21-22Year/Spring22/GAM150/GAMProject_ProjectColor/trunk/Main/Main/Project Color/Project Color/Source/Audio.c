//---------------------------------------------------------------------------\\
//
//	file	audio.c
//	author	Justin Chambers
//			Course: GAM100/GAM150
//			Copyright © 2021 DigiPen (USA) Corporation.
//	brief	This is a simple example of how to properly setup and use FMOD
//
//---------------------------------------------------------------------------\\

#include "audio.h"
#include "fmod.h"
#include "fmod_errors.h"
#include "stdafx.h"
#include "AEEngine.h"
#include "Random.h"
#include <stdio.h>		// printf()


#ifdef _DEBUG
#pragma comment (lib, "FMODL.dll")
#else // NDEBUG
#pragma comment (lib, "FMOD.dll")
#endif

int prevpos = 0;



// Helper function to check for errors
void ERRCHECK(FMOD_RESULT checkResult)
{
	if (checkResult != FMOD_OK)
	{
		AESysPrintf("There was an error in the audio system: (%d) %s\n", checkResult, FMOD_ErrorString(result));
		
	}
}

// Initialize the Audio System
void AudioInit()
{
	// Create and Initialize the FMOD System
	result = FMOD_System_Create(&soundSystem, 131584);
	ERRCHECK(result);

	result = FMOD_System_Init(soundSystem, 32, FMOD_INIT_NORMAL, 0);
	ERRCHECK(result);

	result = FMOD_System_GetMasterChannelGroup(soundSystem, &Master);
	ERRCHECK(result);
	result = FMOD_System_CreateChannelGroup(soundSystem, "Music", &Music);
	ERRCHECK(result);
	result = FMOD_System_CreateChannelGroup(soundSystem, "SFX", &SFX);
	ERRCHECK(result);

	/*DecreaseMaster();
	DecreaseMaster();
	DecreaseMaster();*/

	FMOD_ChannelGroup_SetVolume(SFX, 0.5f);
	FMOD_ChannelGroup_SetVolume(Music, 0.5f);
}

// Update the Audio System
// Note: this should be called frequently such as every game loop or
//       every time a user enters a command depending on the engine
void AudioUpdate()
{
	result = FMOD_System_Update(soundSystem);
	ERRCHECK(result);
	if (AEInputCheckTriggered(VK_OEM_PLUS))
		IncreaseVol(master);
	if (AEInputCheckTriggered(VK_OEM_MINUS))
		DecreaseVol(master);
}

// Cleanup the Audio System
void AudioCleanup()
{
	// Release all sounds that have been created
	for (int i = 0; i < MAX_SOUND_LOAD; ++i) {
		if (sound_list[i] != 0) {
			result = FMOD_Sound_Release(sound_list[i]);
			ERRCHECK(result);
		}
	}

	// Close and Release the FMOD system
	result = FMOD_System_Close(soundSystem);
	ERRCHECK(result);
	result = FMOD_System_Release(soundSystem);
	ERRCHECK(result);
}

int LoadSound(const char* name, enum channel_type channel)
{
	int pos = -1;
	//run thru the name list to see if this sound is already loaded in
	//run thru to see if there's a free space to load the sound into
	for (int i = 0; i < MAX_SOUND_LOAD; ++i) {
		if (sound_list[i] == 0) {
			pos = i;
			break;
		}
		if (strcmp(name, (const char*)(sound_names[i])) == 0)
			return 0;
	}

	//if this is true, all spaces are full
	if (pos == -1)
		return 0;

	FMOD_SOUND* sound;

	result = FMOD_System_CreateSound(soundSystem, name, FMOD_DEFAULT, 0, &sound);
	ERRCHECK(result);

	(sound_list[pos]) = sound;

	sound_types[pos] = channel;

	const int nameSize = strlen(name) + 1;
	sound_names[pos] = malloc(sizeof(char) * nameSize);
	if (sound_names[pos] != 0)
		strcpy_s(sound_names[pos], nameSize, name);

	return 1;
}

int LoadStream(const char* name)
{
	result = FMOD_System_CreateStream(soundSystem, name, FMOD_LOOP_NORMAL | FMOD_2D, 0, &mus_sound);

	ERRCHECK(result);

	if (result == FMOD_OK)
		return 1;
	else
		return 0;
}



int UnloadSound(const char* name)
{
	int pos = -1;
	for (int i = 0; i < MAX_SOUND_LOAD; ++i) {
		if (strcmp(name, sound_names[i]) == 0) {
			pos = i;
			break;
		}
	}

	//if this is true, can't find the sound to unload
	if (pos == -1)
		return 0;

	result = FMOD_Sound_Release(sound_list[pos]);
	ERRCHECK(result);

	free(sound_names[pos]);

	sound_names[pos] = 0;

	sound_types[pos] = 0;

	return 1;
}

int UnloadAllSound()
{
	int complete = 0;
	for (int i = 0; i < MAX_SOUND_LOAD; ++i) {
		if (sound_list != 0) {
			result = FMOD_Sound_Release(sound_list[i]);
			ERRCHECK(result);

			sound_list[i] = 0;
			free(sound_names[i]);

			sound_names[i] = 0;

			sound_types[i] = 0;

			complete = 1;
		}
	}
	return complete;
}

int PlaySFX(const char* name)
{
	int pos = -1;
	for (int i = 0; i < MAX_SOUND_LOAD; ++i) {
		if (strcmp(name, sound_names[i]) == 0) {
			pos = i;
			break;
		}
	}

	if (pos == -1)
		return 0;

	FMOD_CHANNEL* channel;

	//"./Assets/SFX/sample_beep.wav"
	result = FMOD_System_PlaySound(soundSystem, sound_list[pos], SFX, false, &channel);
	ERRCHECK(result);

	return 1;
}

int PlayStream(const char* name)
{
	FMOD_BOOL isPlaying = false;
	/*result = FMOD_Channel_IsPlaying(mus_channel, &isPlaying);
	ERRCHECK(result);*/

	if (isPlaying == false) {
		result = FMOD_System_PlaySound(soundSystem, mus_sound, SFX, false, &mus_channel);
		ERRCHECK(result);
		return 1;
	}
	else
		return 0;

}

void IncreaseVol(enum channel_type channel)
{
	float volume = 0.0f;
	switch (channel)
	{
	case sfx:
		FMOD_ChannelGroup_GetVolume(SFX, &volume);
		volume += VOL_ADJUST;
		volume = AEClamp(volume, 0, 1);
		FMOD_ChannelGroup_SetVolume(SFX, volume);
		break;
	case music:
		FMOD_ChannelGroup_GetVolume(Music, &volume);
		volume += VOL_ADJUST;
		volume = AEClamp(volume, 0, 1);
		FMOD_ChannelGroup_SetVolume(Music, volume);
		break;
	case master:
		FMOD_ChannelGroup_GetVolume(Master, &volume);
		volume += VOL_ADJUST;
		volume = AEClamp(volume, 0, 1);
		FMOD_ChannelGroup_SetVolume(Master, volume);
		break;
	}
	
	
}

void DecreaseVol(enum channel_type channel)
{
	float volume = 0.0f;
	switch (channel)
	{
	case sfx:
		FMOD_ChannelGroup_GetVolume(SFX, &volume);
		volume -= VOL_ADJUST;
		volume = AEClamp(volume, 0, 1);
		FMOD_ChannelGroup_SetVolume(SFX, volume);
		break;
	case music:
		FMOD_ChannelGroup_GetVolume(Music, &volume);
		volume -= VOL_ADJUST;
		volume = AEClamp(volume, 0, 1);
		FMOD_ChannelGroup_SetVolume(Music, volume);
		break;
	case master:
		FMOD_ChannelGroup_GetVolume(Master, &volume);
		volume -= VOL_ADJUST;
		volume = AEClamp(volume, 0, 1);
		FMOD_ChannelGroup_SetVolume(Master, volume);
		break;
	default:
		break;
	}

}

int PlayRandSound(char s) {
	bool found = false;
	char comp;
	int pos;
	while (!found) {
		pos = RandomRange(0, MAX_SOUND_LOAD - 1);
		if (sound_list[pos] != 0 && pos != prevpos){
			comp = sound_names[pos][13]; //amount of characters till the start of the filename
			if (comp == s) {
				PlaySFX(sound_names[pos]);
				prevpos = pos;
				found = true;

			}
		}
		
	}
	return 1;

}