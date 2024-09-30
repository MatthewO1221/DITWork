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

	typedef void(*ParticleFunctionPtr)(ParticlePtr);

	typedef struct Particle Particle, * ParticlePtr;
	typedef struct Transform Transform, * TransformPtr;
	typedef struct Physics Physics, * PhysicsPtr;
	typedef struct Sprite Sprite, * SpritePtr;

	typedef struct Particle
	{
		TransformPtr transform;

		PhysicsPtr physics;

		SpritePtr sprite;

		ParticleFunctionPtr behavior;
		
		// How much longer the particle will exist for
		float lifeTime;

		// Determines if particle is to be deleted
		bool isDeleted;

	}Particle;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

	ParticlePtr ParticleCreate(void);

	void ParticleUpdate(ParticlePtr particle, float dt);

	void ParticleDraw(ParticlePtr particle);

	void ParticleFree(ParticlePtr* particle);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif

