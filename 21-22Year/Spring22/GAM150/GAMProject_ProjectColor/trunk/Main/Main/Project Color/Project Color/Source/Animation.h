//------------------------------------------------------------------------------
//
// File Name:	Animation.h
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 3
// Course:		CS230S22
//
// Copyright (C) 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

typedef struct GameObject* GameObjectPtr;
typedef struct Animation* AnimationPtr;
typedef struct Sprite* SpritePtr;

//typedef FILE* Stream;

//------------------------------------------------------------------------------
// Public Function Declarations:
//------------------------------------------------------------------------------

// Dynamically allocate a new animation object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
AnimationPtr AnimationCreate(void);

// Free the memory associated with an animation.
// (Also, set the animation pointer to NULL.)
// Params:
//	 animation = Pointer to the animation pointer.
void AnimationFree(AnimationPtr* animation);

// Read the properties of an Animation component from a file.
// [NOTE: Read the integer values for frameIndex and frameCount.]
// [NOTE: Read the float values for frameDelay and frameDuration.]
// [NOTE: Read the boolean values for isRunning and isLooping.]
// Params:
//	 animation = Pointer to the Animation component.
//	 stream = Pointer to the data stream used for reading.

//void AnimationRead(AnimationPtr animation, Stream stream);

// Set the parent game object for an animation component.
// Params:
//	 animation = Pointer to the animation pointer.
//	 parent = Pointer to the parent game object.
void AnimationSetParent(AnimationPtr animation, GameObjectPtr parent);

// Play a simple animation sequence (0 .. frameCount).
// (Hint: Make sure to initialize all variables, except for "parent".)
// Params:
//	 animation = Pointer to the animation object.
//	 frameCount = The number of frames in the sequence.
//	 frameDuration = The amount of time to wait between frames (in seconds).
//	 isLooping = True if the animation loops, false otherwise.
void AnimationPlay(AnimationPtr animation, int frameCount, float frameDuration, bool isLooping);

// Play a complex animation sequence (any frame/any delay).
// (Hint: Make sure to initialize all variables, except for "parent".)
// Params:
//	 animation = Pointer to the animation object.
//	 sequence = Pointer to the animation sequence to be played.
//void AnimationPlaySequence(AnimationPtr animation, AnimationSequencePtr sequence);

// Update the animation.
// Params:
//	 animation = Pointer to the animation object.
//	 dt = Change in time (in seconds) since the last game loop.
void AnimationUpdate(AnimationPtr animation, float dt);

// Determine if the animation has reached the end of its sequence.
// Params:
//	 animation = Pointer to the animation object.
// Returns:
//	 If the animation pointer is valid,
//		then return the value in isDone,
//		else return false.
bool AnimationIsDone(AnimationPtr animation);
