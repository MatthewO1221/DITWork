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
#include "Sprite.h"

ParticlePtr ParticleCreate(void)
{
	ParticlePtr particle = calloc(1, sizeof(Particle));

	if (particle)
	{
		particle->physics = PhysicsCreate();
		particle->sprite = SpriteCreate();
		particle->transform = TransformCreate();

		return particle;
	}
	else
	{
		return NULL;
	}
}





void ParticleDraw(ParticlePtr particle)
{
	if (particle)
	{
		SpriteDraw(particle->sprite, particle->transform);
	}
}





void ParticleUpdate(ParticlePtr particle, float dt)
{
	if (particle)
	{
		if (particle->lifeTime <= 0)
		{
			particle->isDeleted = true;
		}
		else
		{
			particle->lifeTime -= dt;

			if (particle->behavior)
			{
				particle->behavior(particle);
				PhysicsUpdate(particle->physics, particle->transform, dt);
				ParticleDraw(particle);
			}
		}
	}
}





void ParticleFree(ParticlePtr* particle)
{
	if ((*particle)->isDeleted)
	{
		PhysicsFree(&(*particle)->physics);
		SpriteFree(&(*particle)->sprite);
		TransformFree(&(*particle)->transform);

		free(*particle);
		*particle = NULL;
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

