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
#include "Player.h"

#pragma region DefaultSlimeValues

#define SlimeJumpRateMin 2.5 //Jump rate, defined because only slimes use it
#define SlimeJumpRateMax 3.5
#define DefaultSlimeHealth 5 //Default health for slimes to spawn with
#define DefaultSlimeDamage 1 //Default damage for slimes to deal
#define DefaultSlimeMinSpeed 200 //Default speed for slimes to move at
#define DefaultSlimeMaxSpeed 250
#define DefaultSlimeJumpHeight 750.0f //Default height for slimes to jump
#define DefaultSlimeRange 1000 //Default slime range
#define DefaultSlimeScale 90 //Default slime scale

#pragma endregion

#pragma region DefaultFleyeValues

#define DefaultFleyeHealth 5 //Default health for fleyes to spawn with
#define DefaultFleyeDamage 1 //Default damage for fleyes to deal
#define DefaultFleyeSpeed 200 //Default speed for fleyes to move at
#define DefaultFleyeAttackSpeed 500 //Default speed for fleyes to attack at
#define DefaultFleyeFlapHeight 600.0f //Default height for fleyes to flap
#define DefaultFleyeRange 600 //Default fleye range
#define DefaultFleyeScale 90 //Default fleye scale
#define DefaultFleyeAttackDist 600 //Default distance fleye will do an attack run

#pragma endregion


#define DefaultMode Idle //Default mode for enemies to spawn in
#define MaxSpawnedEnemies 16 //Maximum amount of enemies that can be spawned at once


typedef struct Enemy* EnemyPtr; //Declaration of enemy struct + pointer
typedef struct tempPlayer* tempPlayerPtr; //Declaration of tempPlayer struct + pointer
enum Modes; //Declaration of modes enum
enum EnemyType; //Declaration of enemyType enum
enum Colors; //Declaration of color enum

//This struct contains everything necessary for an enemy
struct Enemy
{
	char name[16]; //The name of the enemy
	enum EnemyType enemyType; //What kind of enemy is it(Slime, Knight, or Fleye)
	enum Colors color; //Color of enemy

	Vector2D spawnPos; //Enemy spawn position

	int health; //Health of enemy
	int damage; //How much damage enemy deals

	float speed; //How fast the enemy moves
	float jumpH; //Jump height of enemy
	float range; //How far away the enemy can see the player
	float ogTimer;
	float timer; //How often to jump or swoop

	bool floorCollision;
	bool playerCollision;
	bool ceilingCollision;
	bool rightCollision;
	bool leftCollision;

	bool fleyeAttacked;

	bool jumpSound;
	bool landSound;

	enum Modes mode; //Enum for all enemy states
	int enemyState;

	GameObjectPtr gameObject; //Pointer to the enemy game object

	void (*movement)(EnemyPtr, PlayerPtr, float, float); //Movement function enemy uses

	AEGfxVertexList* meshes[2];
	//AEGfxTexture* textures[11];
	//SpriteSourcePtr spriteSources[11];

};

//Temporary player struct for function creation
struct tempPlayer
{
	int health; //Temp health
	TransformPtr transform; //Temp transform
	AABB collider; //Temp collider
};
//Enum for all states enemy can be in
enum Modes
{
	Idle, //Idle, standing still
	Following, //Following player
	Dying, //Enemy is dying
	Dead, //Enemy is dead
	Respawning //Enemy is respawning

};


//Enum for all enemy types
enum EnemyType
{
	Slime, //Slime enemy
	Knight, //Knight miniboss
	Fleye //Bat enemy
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



//Enemy AI function declarations

//Handles all necessary functions for enemy, including movement, state control
//And ground/player collisions
void Existing(EnemyPtr enemy, PlayerPtr player, AABB* levelTiles, float dt);

//Function checks if enemy is collding with the player
//Changes enemy->playerCollision
void CheckPlayerCollision(EnemyPtr enemy, PlayerPtr player);

//Given the enemy and a range, returns true if that distance is withing the enemy's
//Range, false otherwise
bool InRange(EnemyPtr enemy, float dist);

//Kills an enemy
void Death(EnemyPtr enemy);

//Returns the distance from the enemy to the player
float DistToPlayer(EnemyPtr enemy, PlayerPtr player);

//Movement function for slime enemies
void SlimeJump(EnemyPtr enemy, PlayerPtr player, float xDir, float dt);

//Movement function for fleye enemies
void FleyeMove(EnemyPtr enemy, PlayerPtr player, float xDir, float dt);

//Damages the player using enemy->damage
void Damage(EnemyPtr enemy, PlayerPtr player);

void EnemyHurt(EnemyPtr enemy, Vector2D* pos);

//Callocs a new enemy
EnemyPtr MakeEnemy(void);

//Spawns an enemy, creates and adds all components, sets all default variables
//And adds the new enemy to the enemy list
//Creates enemy based on given parameters
EnemyPtr SpawnEnemy(enum EnemyType type, enum Colors color, Vector2D spawnPos, char enemyName[]);

//Sets struct variables to default for slime enemies
void DefaultSlime(EnemyPtr enemy);

//Sets struct variables to default for fleye enemies
void DefaultFleye(EnemyPtr enemy);

//Draws the given enemy
void DrawEnemy(EnemyPtr enemy);

//Checks if given enemy is collding with the ground
//Changes enemy->floorCollision
void CollisionChecks(EnemyPtr enemy, AABB* levelTiles);

//Returns true if the enemy is colliding with the player
void CheckPlayerCollision(EnemyPtr enemy, PlayerPtr player);

//Iterates through enemy list and returns first enemy with given name
EnemyPtr GetEnemyName(char name[16]);

//Returns enemy at the given index in enemy list
EnemyPtr GetEnemyIndex(int index);

//Calls death for all enemies in enemy list
void DespawnAll();

//Unloads all meshes and textures for enemies in enemy list
void UnloadAll();

//Calls existing for all enemies in enemy list
void UpdateAllEnemies(PlayerPtr player, AABB* levelTiles, float dt);

//Draws all enemies in enemy list
void DrawAllEnemies();