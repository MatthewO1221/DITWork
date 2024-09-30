//------------------------------------------------------------------------------
//
// File Name:	Stub.c
// Author(s):	Rishabh Anand
// Project:		Project 0
// Course:		CS230S22
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Particle.h"
#include "Vector2D.h"
#include "Transform.h"
#include "Physics.h"
#include "Sprite.h"
#include "ParticleEmitter.h"

ParticleEmitterPtr ParticleEmitterCreate()
{
	ParticleEmitterPtr particleEmitter = calloc(1, sizeof(ParticleEmitter));

	if (particleEmitter)
	{
		return particleEmitter;
	}
	else
	{
		return NULL;
	}
}





void ParticleEmitterUpdate(ParticleEmitterPtr particleEmitter, ParticlePtr particle, float dt)
{
	for (int i = 0; i < particleListSize; ++i)
	{
		if (particleEmitter->particleList[i])
		{
			ParticleUpdate(particleEmitter->particleList[i], dt);

			if (particleEmitter->particleList[i]->isDeleted)
			{
				ParticleFree(&(particleEmitter->particleList[i]));			//free particles that are deleted
				particleEmitter->particleList[i] = NULL;
				--(particleEmitter->particleCount);							//reduce particle count;
			}
		}
	}
}





void ParticleEmitterDraw(ParticleEmitterPtr particleEmitter)
{
	for (int i = 0; i < particleListSize; i++)
	{
		if (particleEmitter->particleList[i])
		{
			ParticleDraw(particleEmitter->particleList[i]);
		}
	}
}





void ParticleEmitterShutdown(ParticleEmitterPtr particleEmitter)
{
	for (int i = 0; i < particleListSize; ++i)
	{
		ParticleFree(&particleEmitter->particleList[i]);
		particleEmitter->particleList[i] = NULL;
	}
	particleEmitter->particleCount = 0;
}





void AddParticleToEmitter(ParticleEmitterPtr particleEmitter, ParticlePtr particle, float dt)
{
	if (particleEmitter && particle)
	{
		if (particleEmitter->currentTime <= 0)
		{
			for (unsigned i = 0; i < particleEmitter->particleReleaseRate; ++i)
			{
				int j = 0;
				while (particleEmitter->particleList[j])
				{
					++j;
				}

				if (!particleEmitter->particleList[j])
				{
					particleEmitter->particleList[j] = particle;
				}
			}
			particleEmitter->currentTime += particleEmitter->spawnTimer;
		}
		else
		{
			particleEmitter->currentTime -= dt;
		}
	}
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

