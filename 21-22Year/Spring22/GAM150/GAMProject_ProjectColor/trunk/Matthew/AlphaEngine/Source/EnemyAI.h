#include "AEVec2.h"
#include "targetver.h" /* required by Windows */
#include <AEEngine.h>
#include <stdlib.h> /* NULL, malloc/free */
#include <stdio.h> /* printf, fgets */
#include <stdbool.h> /* bool */
#include <string.h> /* strcmp, strcpy_s, strtok_s */
#include "Physics.h"
#include "Transform.h"
#include "Vector2D.h"
#include "GameObject.h"


#define SlimeJumpRate 5.0f //Jump rate, defined because only slimes use it
#define DefaultSlimeHealth 5 //Default health for slimes to spawn with
#define DefaultSlimeDamage 1 //Default damage for slimes to deal
#define DefaultSlimeSpeed 3 //Default speed for slimes to move at
#define DefaultSlimeJumpHeight 3 //Default height for slimes to jump
#define DefaultSlimeRange 50 //Default slime range
#define DefaultMode Patrol //Default mode for enemies to spawn in
#define MaxSpawnedEnemies 128


typedef struct Enemy* EnemyPtr; //Declaration of enemy struct + pointer
typedef struct PatrolData* PatrolDataPtr; //Declaration of patrol struct + pointer
typedef struct tempPlayer* tempPlayerPtr; //Declaration of tempPlayer struct + pointer
enum Modes; //Declaration of modes enum
enum EnemyType; //Declaration of enemyType enum
enum Colors; //Declaration of color enum

//This struct contains everything necessary for an enemy
struct Enemy
{
	char name[16]; //The name of the enemy
	enum EnemyType enemyType; //What kind of enemy is it(Slime or Knight)
	enum Colors color; //Color of enemy

	Vector2D spawnPos; //Enemy spawn position

	int health; //Health of enemy
	int damage; //How much damage enemy deals

	float speed; //How fast the enemy moves
	float jumpH; //Jump height of enemy
	float range; //How far away the enemy can see the player

	enum Modes mode; //Enum for all enemy states

	GameObjectPtr gameObject; //Pointer to the enemy game object
	PatrolDataPtr patrol; //Patrol stop points of enemy

	AEGfxTexture* texture; //Texture enemy uses

	//void (*attack)(int damage); //Attack function enemy uses
	//void (*movement)(struct Enemy); //Movement function enemy uses
	//void (*existing)(struct Enemy, struct tempPlayer); //Existing function for enemy
};

//Enum for all states enemy can be in
enum Modes
{
	Patrol, //Patrolling set points
	Following, //Following player
	Dying, //Enemy is dying
	Dead, //Enemy is dead
	Respawning //Enemy is respawning

};

//Enum for all enemy types
enum EnemyType
{
	Slime, //Slime enemy
	Knight //Knight enemy
};

//enum for all colors
enum Colors
{
	Red,
	Orange,
	Yellow,
	Green,
	Blue,
	Purple
};


struct Enemy enemyList[MaxSpawnedEnemies]; //List of all currently spawned enemies

//Enemy AI function declarations
void Existing(EnemyPtr enemy, tempPlayerPtr player);

void Attack(EnemyPtr enemy, tempPlayerPtr player);

bool InRange(EnemyPtr enemy, float dist);

void Death(EnemyPtr enemy);

float DistToPlayer(EnemyPtr enemy, tempPlayerPtr player);

void SlimeJump(EnemyPtr enemy, float xDir);

void Damage(EnemyPtr enemy, tempPlayerPtr player);

PatrolDataPtr GetPatrolEnemy(EnemyPtr enemy);

EnemyPtr MakeEnemy(void);

void SpawnEnemy(enum EnemyType type, enum Colors color, Vector2D spawnPos, Vector2D patrolPoints[2], char enemyName[]);

void DefaultSlime(EnemyPtr enemy);

void SetPatrolPoints(EnemyPtr enemy, Vector2D patrolPoints[2]);