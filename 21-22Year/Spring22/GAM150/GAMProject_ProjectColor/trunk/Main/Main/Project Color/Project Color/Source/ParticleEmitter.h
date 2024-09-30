//------------------------------------------------------------------------------
//
// File Name:	Stub.h
// Author(s):	Rishabh Anand
// Project:		Project 0
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
	/* Assume C declarations for C++ */
#endif

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Constants:

#define particleListSize 1000

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:

	typedef struct ParticleEmitter* ParticleEmitterPtr;
	typedef struct Particle Particle, * ParticlePtr;
	typedef struct Transform Transform, * TransformPtr;
	typedef struct Physics Physics, * PhysicsPtr;
	typedef struct Sprite Sprite, * SpritePtr;

	typedef struct ParticleEmitter
	{
		TransformPtr transform;

		// The current number of game objects currently in the list.
		unsigned int particleCount;

		// Time till next batch of particles is added
		float spawnTimer;

		// Current time
		float currentTime;

		// Number of Particles to Spawn every call
		unsigned int particleReleaseRate;

		// The maximum number of particles ever in the list, at any given moment.
		unsigned int particleMax;

		// or a dynamically sized array, such as a linked list.
		ParticlePtr particleList[particleListSize];

	}ParticleEmitter;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

	ParticleEmitterPtr ParticleEmitterCreate(void);

	void ParticleEmitterUpdate(ParticleEmitterPtr particleEmitter, ParticlePtr particle, float dt);

	void ParticleEmitterDraw(ParticleEmitterPtr particleEmitter);

	void ParticleEmitterShutdown(ParticleEmitterPtr particleEmitter);

	void AddParticleToEmitter(ParticleEmitterPtr particleEmitter, ParticlePtr particle, float dt);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

