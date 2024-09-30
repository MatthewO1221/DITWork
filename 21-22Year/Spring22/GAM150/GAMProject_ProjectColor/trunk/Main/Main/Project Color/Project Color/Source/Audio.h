//---------------------------------------------------------------------------\\
//
//	file	audio.h
//	author	Justin Chambers
//			Course: GAM100/GAM150
//			Copyright © 2021 DigiPen (USA) Corporation.
//	brief	This is a simple example of how to properly setup and use FMOD
//
//---------------------------------------------------------------------------\\



#define MAX_SOUND_LOAD 32
#define VOL_ADJUST 0.1f

#include "fmod.h"
#include "fmod_errors.h"

enum cringe_based { Cringe, Based };
					//0      1

enum channel_type { sfx = 1, music = 2, master = 3};

FMOD_SOUND* sound_list[MAX_SOUND_LOAD];
int sound_types[MAX_SOUND_LOAD];
char* sound_names[MAX_SOUND_LOAD];

FMOD_SOUND* mus_sound;
FMOD_CHANNEL* mus_channel;

FMOD_CHANNELGROUP* SFX, * Music, * Master;

FMOD_SYSTEM* soundSystem;
FMOD_RESULT result;

void AudioInit();
void AudioUpdate();
void AudioCleanup();

//loads a sound after being given the file name in the input
//returns a 1 if load successful, 0 if sound list is full or if the sound is already loaded
int LoadSound(const char* name, enum channel_type channel);
int LoadStream(const char* name);

int UnloadSound(const char* name);
int UnloadAllSound();

int PlaySFX(const char* name);
int PlayStream(const char* name);

void IncreaseVol(enum channel_type channel);
void DecreaseVol(enum channel_type channel);

int PlayRandSound(char s);
