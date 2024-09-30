//------------------------------------------------------------------------------
//
// File Name:	RunningParticles.h
// Author(s):	Rishabh Anand
// Project:		GAM150 Game
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Particle.h"
#include "ParticleEmitter.h"
#include "Vector2D.h"
#include "Transform.h"
#include "Physics.h"
#include "Sprite.h"
#include "RunningParticles.h"
#include "GameObject.h"
#include "Transform.h"

ParticleEmitterPtr runningParticleEmitter;

static float particleEmitterYOffset = 40.0f;





void RunningParticleSetTransform(GameObjectPtr playerGameObject)
{
	if (runningParticleEmitter && playerGameObject)
	{
		TransformPtr playerTransform = GameObjectGetTransform(playerGameObject);

		Vector2D particleEmitterTranslation = *TransformGetTranslation(playerTransform);			//player translation base for ptclemitter translation
		particleEmitterTranslation.y -= particleEmitterYOffset;										//set ptclemitter translation at player's feet

		TransformSetTranslation(runningParticleEmitter->transform, &particleEmitterTranslation);
	}
}





void RunningParticleInit(GameObjectPtr playerGameObject)
{
	runningParticleEmitter = ParticleEmitterCreate();												//create emitter

	if (runningParticleEmitter)
	{
		RunningParticleSetTransform(playerGameObject);
	}
}




void RunningParticleUpdate(GameObjectPtr playerGameObject, float dt)
{
	if (runningParticleEmitter && playerGameObject)
	{
		RunningParticleSetTransform(playerGameObject);


		//ParticleEmitterUpdate(runningParticleEmitter, dt);
	}
}





ParticlePtr RunningParticleCreate(ParticleEmitterPtr particleEmitter)
{
	ParticlePtr runningParticle = ParticleCreate();

	if (runningParticle && particleEmitter)
	{
		PhysicsPtr particlePhysics = runningParticle->physics;
		TransformPtr particleTransform = runningParticle->transform;

		TransformPtr particleEmitterTransform = particleEmitter->transform;

		Vector2D trajectory = { 0, 300 };
		Vector2D gravity = { 0, -20 };

		PhysicsSetVelocity(particlePhysics, &trajectory);
		PhysicsSetAcceleration(particlePhysics, &gravity);




		runningParticle->lifeTime = 3.0f;

		return runningParticle;
	}
	else
	{
		return NULL;
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

