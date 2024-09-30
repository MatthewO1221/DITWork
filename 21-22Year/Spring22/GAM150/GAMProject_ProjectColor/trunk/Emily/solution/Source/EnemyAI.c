#include "stdafx.h"
#include "EnemyAI.h"
#include "AABB.h"


//This struct holds the positions of the enemies' patrol points
struct PatrolData
{
	Vector2D leftPos; //Left patrol point position
	Vector2D rightPos; //Right patrol point position
};


//Struct holds enemy patrol stop points




//Controls what each enemy state does and when the enemy
//switches between states
void Existing(EnemyPtr enemy, tempPlayerPtr player)
{
	if (enemy->gameObject == NULL || player == NULL)
		return;

	//Enemies start in patrol mode

	PatrolDataPtr patrol = GetPatrolEnemy(enemy); //Get enemy patrol data
	//Get enemy and player translations
	Vector2D *translation = GetTranslationTransform(GetTransformGameObject(enemy->gameObject));
	Vector2D* playerTranslation = GetTranslationTransform(player->transform);

	PhysicsPtr physics = GetPhysicsGameObject(enemy->gameObject);
	Vector2D newVel = { GetVelocityPhysics(physics)->x, GetVelocityPhysics(physics)->y - 10 };

	if (1)
	{
		SetVelocityPhysics(GetPhysicsGameObject(enemy->gameObject), &newVel);
	}

	UpdateGameObject(enemy->gameObject, (float)AEFrameRateControllerGetFrameTime());

	//Region for patrol logic
#pragma region PatrolMode 
	if (enemy->mode == Patrol) //If enemy is set to patrol
	{
		bool movingRight = true; //Initially move right

		if (movingRight)
		{
			//Only jump on every "SlimeJumpRate"th frame
			if (AEFrameRateControllerGetFrameCount() % (int)SlimeJumpRate == 0)
			{
				SlimeJump(enemy, enemy->speed); //Do a slime jump(placeholder)
			}

			//If you're at or past the right patrol point
			//if (translation->x >= patrol->rightPos.x)
			//{
				//Don't go past patrol point
			//	translation->x = patrol->rightPos.x;
			//	movingRight = false; //Start moving left
			//}
		}
		else if (!movingRight) //Moving left
		{
			//Only jump on every "SlimeJumpRate"th frame
			if (AEFrameRateControllerGetFrameCount() % (int)SlimeJumpRate == 0)
			{
				SlimeJump(enemy, -enemy->speed); //Do a slime jump(placeholder)
			}
			//If you're at or past the left patrol point
			if (translation->x <= patrol->leftPos.x)
			{
				//Don't go past patrol point
				translation->x = patrol->leftPos.x;
				movingRight = true; //Moving right
			}
		}
	}
#pragma endregion

	//Region for following logic
#pragma region FollowMode
	if (enemy->mode == Following)
	{
		int frameCount = AEFrameRateControllerGetFrameCount();
		//If player is to the right
		if (translation->x < playerTranslation->x)
		{
			//Jump at player every "SlimeJumpRate"th frame
			if (frameCount % (int)SlimeJumpRate == 0 && translation->y == -200.0f)
			{
				//SlimeJump(enemy, enemy->speed); //Jump at player(placeholder)
			}
		}
		else if (translation->x > playerTranslation->x) //If player is to the left
		{
			if (frameCount % (int)SlimeJumpRate == 0 && translation->y == -200.0f)
			{
				//SlimeJump(enemy, -enemy->speed);
			}
		}
	}
#pragma endregion

	//Region for dying logic
#pragma region DyingMode
	//If the enemy is dying
	if (enemy->mode == Dying)
	{
		Death(enemy); //Die
	}
#pragma endregion

	//Region for dead logic
#pragma region DeadMode
	//Not yet
	if (enemy->mode == Dead)
	{

	}
#pragma endregion

	//Region for respawning logic
#pragma region RespawningMode
	//Not yet
	if (enemy->mode == Respawning)
	{

	}
#pragma endregion


	//Region which controls changing of enemy modes
#pragma region StateControl
	if (enemy->health <= 0 && enemy->mode != Dead) //If the enemy has 0 or less health
	{
		enemy->mode = Dying; //Enemy is dying
	}
	else if (InRange(enemy, DistToPlayer(enemy, player))) //If the player is in range
	{
		enemy->mode = Following; //Start following player
	}
	else if (!InRange(enemy, DistToPlayer(enemy, player))) //If the player is not in range
	{
		enemy->mode = Patrol; //Go back to patrolling
	}
	

#pragma endregion

}

//Damage the player
void Damage(EnemyPtr enemy, tempPlayerPtr player)
{
	player->health -= enemy->damage;
}

//Is the player in range
bool InRange(EnemyPtr enemy, float dist)
{
	//If the dist to the player is less than the range
	if (dist <= enemy->range && enemy)
	{
		return TRUE; //Player is in range
	}

	return FALSE; //Player is not in range
}

//Kill enemy
void Death(EnemyPtr enemy)
{
	if (enemy) //If pointer is valid
	{
		//Get transform and translation
		TransformPtr transform = GetTransformGameObject(enemy->gameObject);
		Vector2D* translation = GetTranslationTransform(transform);
		translation->x = 10000; //Throw enemy into the void
		enemy->mode = Dead; //Enemy is dead
	}
	
}

//Get distance to the player
float DistToPlayer(EnemyPtr enemy, tempPlayerPtr player)
{
	if (enemy && player)
	{
		//Get transform and translation
		TransformPtr transform = GetTransformGameObject(enemy->gameObject);
		Vector2D* translation = GetTranslationTransform(transform);
		Vector2D* playerTranslation = GetTranslationTransform(player->transform);

		//Distance formula, does not account for obstacles
		float dist = sqrtf(powf(translation->x - playerTranslation->x, 2.0f) +
			(powf(translation->y - playerTranslation->y, 2.0f)));

		return dist; //Return distance
	}
	
	return 0;
}

PatrolDataPtr GetPatrolEnemy(EnemyPtr enemy)
{
	if (enemy)
	{
		return enemy->patrol; //Get enemy patrol pointer
	}

	return 0;
}


EnemyPtr MakeEnemy(void)
{
	//Allocate a new enemy
	EnemyPtr newEnemy = calloc(1, sizeof(struct Enemy));

	if (newEnemy) //If successful
	{
		return newEnemy; //Return new enemy
	}

	return 0;
}

EnemyPtr SpawnEnemy(enum EnemyType type, enum Colors color, Vector2D spawnPos, char enemyName[])
{
	EnemyPtr newEnemy = MakeEnemy(); //Make a new enemy
	newEnemy->gameObject = MakeGameObject(); //Make a new game object for the enemy

	AddPhysicsGameObject(newEnemy->gameObject, MakePhysics()); //Make new physics for game object
	AddTransformGameObject(newEnemy->gameObject, MakeTransform()); //Make new transform for game object

	*newEnemy->name = *enemyName; //Set enemy and game object names
	SetNameGameObject(newEnemy->gameObject, enemyName);

	newEnemy->color = color; //Set enemy color

	//Set enemy current position and spawn position
	SetTranslationTransform(GetTransformGameObject(newEnemy->gameObject), &spawnPos);
	newEnemy->spawnPos = spawnPos;

	//SetPatrolPoints(newEnemy, patrolPoints); //Set the enemy's patrol points

	//Depending on the type of enemy initialize variables to certain values
	switch (type)
	{
	case Slime:
		DefaultSlime(newEnemy);
		break;

	}

	//Go through list of enemies and add this one to first empty slot
	for (int i = 0; i < MaxSpawnedEnemies; i++)
	{
		if (enemyList[i].gameObject == NULL)
		{
			enemyList[i] = *newEnemy;
			break;
		}
	}

	return newEnemy;
}

// MAKE DESPAWN FUNCTION TO REMOVE ALL ENEMIES FROM ENEMYLIST.

void DefaultSlime(EnemyPtr enemy)
{
	if (enemy) //If pointer is valid
	{
		//Set all variables to default values
		enemy->health = DefaultSlimeHealth;
		enemy->damage = DefaultSlimeDamage;
		enemy->speed = DefaultSlimeSpeed;
		enemy->range = DefaultSlimeRange;
		enemy->jumpH = DefaultSlimeJumpHeight;
	}
}

void SetPatrolPoints(EnemyPtr enemy, Vector2D patrolPoints[2])
{
	if (enemy) //If pointer is valid
	{
		//Get patrol data
		PatrolDataPtr patrol = GetPatrolEnemy(enemy);

		//Set left and right patrol points
		patrol->leftPos = patrolPoints[0];
		patrol->rightPos = patrolPoints[1];
	}
}

void SlimeJump(EnemyPtr enemy, float xDir)
{
	if (enemy)
	{
		PhysicsPtr physics = GetPhysicsGameObject(enemy->gameObject);
		TransformPtr transform = GetTransformGameObject(enemy->gameObject);

		if (physics && transform)
		{

			Vector2D velocity = *GetVelocityPhysics(physics);

			
			
			velocity.x = xDir * 1000;
			
			

			//Vector2D translation = *GetTranslationTransform(transform);
			//if (translation.y < groundHeight)
			//{
			//	translation.y = groundHeight;
			//	velocity.y = 0;
			//	PhysicsSetAcceleration(physics, &gravityNone);
			//	numLives1--;
			//	if (numLives1 <= 0)
			//	{
			//		GameStateManagerSetNextState(GsLevel2);
			//	}

		//}

			SetVelocityPhysics(physics, &velocity);
		}

	}
}

void DrawEnemy(EnemyPtr enemy, AEGfxVertexList *mesh, AEGfxTexture *temp)
{
	if (enemy->gameObject == NULL || mesh == NULL)
		return;

	// Set render mode.
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	// Set the position.
	TransformPtr transform = GetTransformGameObject(enemy->gameObject);
	Vector2D translation = *GetTranslationTransform(transform);
	AEGfxSetPosition(translation.x, translation.y);

	// Render the texture.
	AEGfxTextureSet(temp, 0.0F, 0.0F);

	// Set transparency.
	AEGfxSetTransparency(1.0F);

	// Draw the mesh.
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}