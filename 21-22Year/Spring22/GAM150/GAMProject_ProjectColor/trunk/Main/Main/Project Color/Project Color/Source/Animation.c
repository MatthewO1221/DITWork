//------------------------------------------------------------------------------
//
// File Name:	Animation.c
// Author(s):	Emily Comes (emily.comes)
// Project:		Project 3
// Course:		CS230S22
//
// Copyright (C) 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Animation.h"
#include "Sprite.h"

// #include "Stream.h"
#include "GameObject.h"

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct Animation
{
  // Pointer to the parent game object associated with the animation.
  GameObjectPtr parent;

  // The current frame being displayed.
  unsigned frameIndex;

  // The maximum number of frames in the sequence.
  unsigned frameCount;

  // The time remaining for the current frame.
  float frameDelay;

  // The amount of time to display each successive frame.
  float frameDuration;

  // True if the animation is running; false if the animation has stopped.
  bool isRunning;

  // True if the animation loops back to the beginning.
  bool isLooping;

  // True if the end of the animation has been reached, false otherwise.
  // (Hint: This should be true for only one game loop.)
  bool isDone;

  // The animation sequence currently being played, if any.
  //AnimationSequencePtr	sequence;
} Animation;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new animation object.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
AnimationPtr AnimationCreate(void)
{
  AnimationPtr nuAnim = (AnimationPtr)calloc(1, sizeof(Animation));
  if (errno == ENOMEM || nuAnim == NULL) // memory allocation failed
    return NULL;
  nuAnim->parent = NULL;
  nuAnim->frameIndex = 0;
  nuAnim->frameCount = 0;
  nuAnim->frameDelay = 0.f;
  nuAnim->frameDuration = 0.f;
  nuAnim->isRunning = false;
  nuAnim->isLooping = false;
  nuAnim->isDone = false;
  return nuAnim;
}

// Free the memory associated with an animation.
// (Also, set the animation pointer to NULL.)
// Params:
//	 animation = Pointer to the animation pointer.
void AnimationFree(AnimationPtr* animation)
{
  if (*animation)
    free(*animation);
  *animation = NULL;
}

// Read the properties of an Animation component from a file.
// [NOTE: Read the integer values for frameIndex and frameCount.]
// [NOTE: Read the float values for frameDelay and frameDuration.]
// [NOTE: Read the boolean values for isRunning and isLooping.]
// Params:
//	 animation = Pointer to the Animation component.
//	 stream = Pointer to the data stream used for reading.

//void AnimationRead(AnimationPtr animation, Stream stream)
//{
//  animation->frameIndex = StreamReadInt(stream);
//  animation->frameCount = StreamReadInt(stream);
//  animation->frameDelay = StreamReadFloat(stream);
//  animation->frameDuration = StreamReadFloat(stream);
//  animation->isRunning = StreamReadBoolean(stream);
//  animation->isLooping = StreamReadBoolean(stream);
//}

// Set the parent game object for an animation component.
// Params:
//	 animation = Pointer to the animation pointer.
//	 parent = Pointer to the parent game object.
void AnimationSetParent(AnimationPtr animation, GameObjectPtr parent)
{
  animation->parent = parent;
}

// Play a simple animation sequence (0 .. frameCount).
// (Hint: Make sure to initialize all variables, except for "parent".)
// Params:
//	 animation = Pointer to the animation object.
//	 frameCount = The number of frames in the sequence.
//	 frameDuration = The amount of time to wait between frames (in seconds).
//	 isLooping = True if the animation loops, false otherwise.
void AnimationPlay(AnimationPtr animation, int frameCount, float frameDuration, bool isLooping)
{
  if (animation == NULL)
    return;
  animation->frameIndex = 0;
  animation->frameCount = frameCount;
  animation->frameDuration = frameDuration;
  animation->frameDelay = 0.f;
  animation->isLooping = isLooping;
  animation->isRunning = true;
  animation->isDone = false;
}

// Update the animation.
// Params:
//	 animation = Pointer to the animation object.
//	 dt = Change in time (in seconds) since the last game loop.
void AnimationUpdate(AnimationPtr animation, float dt)
{
  if (animation == NULL)
    return;
  animation->isDone = false;
  if (animation->isRunning)
  {
    animation->frameDelay -= dt;
    if (animation->frameDelay <= 0.f)
    {
      ++animation->frameIndex;
      if (animation->frameIndex >= animation->frameCount)
      {
        animation->frameIndex = 0;
        if (!animation->isLooping)
        {
          animation->frameIndex = animation->frameCount - 1;
          animation->isRunning = false;
        }
        animation->isDone = true;
      }
      animation->frameDelay = 0.f;
      if (animation->isRunning)
      {
        SpriteSetFrame(GameObjectGetSprite(animation->parent), animation->frameIndex);
        animation->frameDelay = animation->frameDuration;
      }
    }
  }
}

// Determine if the animation has reached the end of its sequence.
// Params:
//	 animation = Pointer to the animation object.
// Returns:
//	 If the animation pointer is valid,
//		then return the value in isDone,
//		else return false.
bool AnimationIsDone(AnimationPtr animation)
{
  if (animation)
    return animation->isDone;
  return false;
}
