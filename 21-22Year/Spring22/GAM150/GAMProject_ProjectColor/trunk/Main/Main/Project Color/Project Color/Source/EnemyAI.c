#include "stdafx.h"
#include "EnemyAI.h"
#include "AABB.h"
#include "Level.h"
#include "Debug.h"
#include "Sprite.h"
#include "Mesh.h"
#include "Tile.h"
#include "SpriteSource.h"
#include "Animation.h"
#include "Player Movement.h"
#include "Player Controller.h"
#include "EnemyAnimations.h"
#include "Random.h"
#include "Audio.h"
#include "PauseScreen.h"


static EnemyPtr enemyList[MaxSpawnedEnemies]; // List of all currently spawned enemies 

static AEGfxVertexList* enemyMesh;
static AEGfxTexture* enemyTexture;
static SpriteSourcePtr enemySpriteSource;

static bool isColliding = false;

static int collisionDirection = 0;

AABB* EnemyBoxCast(AABB col, int dir);

// Controls what each enemy state does and when the enemy
// switches between states
void Existing(EnemyPtr enemy, PlayerPtr player, AABB* levelTiles, float dt)
{
  if (enemy->gameObject == NULL || player == NULL || levelTiles == NULL || GetPaused())
    return;

  // Get various things
  TransformPtr enemyTransform = GameObjectGetTransform(enemy->gameObject);
  const Vector2D translation = *TransformGetTranslation(enemyTransform);
  const Vector2D playerTranslation = *TransformGetTranslation(GameObjectGetTransform(player->parent));

  AABB collider = GameObjectGetCollider(enemy->gameObject);

  
  CollisionChecks(enemy, levelTiles); //Check for collisions

  CheckPlayerCollision(enemy, player);

  if (enemy->playerCollision == TRUE)
    Damage(enemy, player); // Check if colliding with the player

  // Keep collider in the right spot
  updateAABBPosition(collider, GameObjectGetPhysics(enemy->gameObject));
  

#if DEBUG_MODE
  DrawBoundingBox(collider); // Make it so you can see the collision boxes
#endif

  // Region for patrol logic
#pragma region IdleMode
  if (enemy->mode == Idle) // If enemy is set to idle
    enemy->movement(enemy, player, enemy->speed, dt); // Call enemy's movement function
#pragma endregion

  // Region for following logic
#pragma region FollowMode
  if (enemy->mode == Following)
    if (translation.x < playerTranslation.x)      // If player is to the right
      enemy->movement(enemy, player, enemy->speed, dt); // Move towards player
    else if (translation.x > playerTranslation.x) // If player is to the left
      enemy->movement(enemy, player, -enemy->speed, dt);
#pragma endregion

  // Region for dying logic
#pragma region DyingMode

  // If the enemy is dying
  if (enemy->mode == Dying)
    Death(enemy); // Die
#pragma endregion

  // Region for dead logic
#pragma region DeadMode

  // Not yet
  if (enemy->mode == Dead)
  {
    // Do nothing
  }
#pragma endregion

  // Region for respawning logic
#pragma region RespawningMode

  // Not yet
  if (enemy->mode == Respawning)
  {
  }
#pragma endregion

  // Region which controls changing of enemy modes
#pragma region StateControl
  if (enemy->health <= 0 && enemy->mode != Dead) // If the enemy has 0 or less health
    enemy->mode = Dying; // Enemy is dying
  else if (InRange(enemy, DistToPlayer(enemy, player))) // If the player is in range
    enemy->mode = Following; // Start following player
  else if (!InRange(enemy, DistToPlayer(enemy, player))) // If the player is not in range
    enemy->mode = Idle; // Go back to idle
#pragma endregion
}

// Waldo has hid again
// Damage the player
void Damage(EnemyPtr enemy, PlayerPtr player)
{
    if (!player->iFrames)
    {
        player->health -= enemy->damage;
        AESysPrintf("Damaging Player");

        Vector2D pos = *TransformGetTranslation(GameObjectGetTransform(enemy->gameObject));
        PlayerHurt(&pos);

        //AEGfxVertexList* mesh = SpriteGetMesh(playerSprite);
        //SpriteSourcePtr spriteSource = SpriteGetSpriteSource(playerSprite);

    }
}

void EnemyHurt(EnemyPtr enemy, Vector2D* pos)
{
    
    if (enemy)
    {
        PhysicsPtr physics = GameObjectGetPhysics(enemy->gameObject);

        TransformPtr enemyTransform = GameObjectGetTransform(enemy->gameObject);

        Vector2D enemyPos = *TransformGetTranslation(enemyTransform);

        Vector2D launchAngle = { 500, 500 };
        launchAngle.x *= pos->x < enemyPos.x ? 1 : -1;

        PhysicsSetVelocity(physics, &launchAngle);
        enemy->floorCollision = false;
        //EnemyAnimate((int)launchAngle.x / 750 == -1 ? ONE_BY_ONE : ONE_BY_ONE_REVERSE, HURT, 1, 1.0F, false);
    }
}

// Is the player in range
bool InRange(EnemyPtr enemy, float dist)
{
  // If the dist to the player is less than the range
  if (dist <= enemy->range && enemy)
  {
    return TRUE; // Player is in range
  }

  return FALSE; // Player is not in range
}

// Kill enemy
void Death(EnemyPtr enemy)
{
  if (enemy) // If pointer is valid
  {
    for (int i = 0; i < MaxSpawnedEnemies; i++) // Go through enemy list
    {
      if (enemyList[i] == GetEnemyName(enemy->name)) // If you find the right enemy
      {
        enemyList[i] = NULL; // Set that list spot to NULL
      }
    }
    GameObjectFree(&enemy->gameObject); // Free enemy game object and everything attached to it
    free(enemy); // Free the enemy
    enemy = NULL; // Set the pointer to NULL
  }
}

// Get distance to the player
float DistToPlayer(EnemyPtr enemy, PlayerPtr player)
{
  if (enemy && player)
  {
    // Get transform and translation
    TransformPtr transform = GameObjectGetTransform(enemy->gameObject);
    const Vector2D* translation = TransformGetTranslation(transform);
    const Vector2D* playerTranslation = TransformGetTranslation(GameObjectGetTransform(player->parent));

    // Distance formula, does not account for obstacles
    float dist = sqrtf(powf(translation->x - playerTranslation->x, 2.0f) +
      (powf(translation->y - playerTranslation->y, 2.0f)));

    return dist; // Return distance
  }

  return 0;
}

// Calloc a new enemy pointer
EnemyPtr MakeEnemy(void)
{
  // Allocate a new enemy
  EnemyPtr newEnemy = calloc(1, sizeof(struct Enemy));

  if (newEnemy) // If successful
  {
    return newEnemy; // Return new enemy
  }

  return 0;
}

// Handles making a new enemy and setting all the enemy's values, including adding it to the list
EnemyPtr SpawnEnemy(enum EnemyType type, enum Colors color, Vector2D spawnPos, char enemyName[])
{
  int canSpawn = 0;
  for (int i = 0; i < MaxSpawnedEnemies; i++) // Check if there is an empty spot in the list
  {
    if (enemyList[i] == NULL) // If there is you can spawn an enemy
    {
      canSpawn = 1;
    }
  }

  if (canSpawn == 1)
  {
    EnemyPtr newEnemy = MakeEnemy(); // Make a new enemy
    newEnemy->gameObject = GameObjectCreate(); // Make a new game object for the enemy

    GameObjectAddPhysics(newEnemy->gameObject, PhysicsCreate()); // Make new physics for game object
    GameObjectAddTransform(newEnemy->gameObject, TransformCreate()); // Make new transform for game object
    GameObjectAddSprite(newEnemy->gameObject, SpriteCreate()); // Make new sprite for game object
    GameObjectAddAnimation(newEnemy->gameObject, AnimationCreate()); // Make new animaton for game object

    // Create a meshes and add default to the sprite
    LoadAnimations(newEnemy);
    SpriteSetMesh(GameObjectGetSprite(newEnemy->gameObject), newEnemy->meshes[0]);

    // Set the enemy's scale
    Vector2D scale = { TILE_SIZE, TILE_SIZE};
    TransformSetScale(GameObjectGetTransform(newEnemy->gameObject), &scale);

    // Set the enemy's name
    strcpy_s(newEnemy->name, sizeof(newEnemy->name), enemyName);
    GameObjectSetName(newEnemy->gameObject, enemyName);

    newEnemy->color = color; // Set enemy color
    newEnemy->enemyType = type;

    // Set enemy current position and spawn position
    TransformSetTranslation(GameObjectGetTransform(newEnemy->gameObject), &spawnPos);
    newEnemy->spawnPos = spawnPos;

    // Make a new collider for the enemy
    TransformPtr transform = GameObjectGetTransform(newEnemy->gameObject);
    AABB newAABB = FromPoints(spawnPos, TransformGetScale(transform)->x, TransformGetScale(transform)->y, 0, 0, false);
    GameObjectAddCollider(newEnemy->gameObject, newAABB);
    //AABB pAABB = FromPoints(pSpawnPos, pScale.x * 0.5F, pScale.y, 0, 0, false);
    // Depending on the type of enemy initialize variables to certain values
    switch (type)
    {
    case Slime:
      DefaultSlime(newEnemy);
      break;

    case Fleye:
      DefaultFleye(newEnemy);
      break;
    }

    // Set the enemy's sprite source
    SpriteSetSpriteSource(GameObjectGetSprite(newEnemy->gameObject), enemySpriteSource);

    // Go through list of enemies and add this one to first empty slot
    for (int i = 0; i < MaxSpawnedEnemies; i++)
    {
      if (enemyList[i] == NULL)
      {
        enemyList[i] = newEnemy;
        break;
      }
    }

    return newEnemy;
  }

  return 0;
}

// Sets all default slime values
void DefaultSlime(EnemyPtr enemy)
{
  if (enemy) // If pointer is valid
  {
    // Set all variables to default values
    enemy->health = DefaultSlimeHealth;
    enemy->damage = DefaultSlimeDamage;
    enemy->speed = RandomRangeFloat(DefaultSlimeMinSpeed, DefaultSlimeMaxSpeed);
    enemy->ogTimer = RandomRangeFloat(SlimeJumpRateMin, SlimeJumpRateMax);
    enemy->timer = enemy->ogTimer;
    enemy->range = DefaultSlimeRange;
    enemy->jumpH = DefaultSlimeJumpHeight;
    enemy->movement = &SlimeJump; // Set movement function pointer
    enemy->fleyeAttacked = FALSE;
    enemy->jumpSound = FALSE;
    
    // Set texture and sprite source
    switch (enemy->color)
    {
    case(Green):
        enemyTexture = AEGfxTextureLoad("./Assets/EnemySprites/GreenNormal.png");
        break;
    case(Blue):
        enemyTexture = AEGfxTextureLoad("./Assets/EnemySprites/BlueNormal.png");
        break;                                    
    case(Red):                                    
        enemyTexture = AEGfxTextureLoad("./Assets/EnemySprites/RedNormal.png");
        break;                                    
    case(Yellow):                                 
        enemyTexture = AEGfxTextureLoad("./Assets/EnemySprites/YellowNormal.png");
        break;                                    
    case(Orange):                                 
        enemyTexture = AEGfxTextureLoad("./Assets/EnemySprites/OrangeNormal.png");
        break;                                    
    case(Purple):                                 
        enemyTexture = AEGfxTextureLoad("./Assets/EnemySprites/PurpleNormal.png");
        break;
    default:
        break;
    }

    LoadSound("./Assets/SFX/slime_land.wav", sfx);
    LoadSound("./Assets/SFX/slime_jump.wav", sfx);
    
    enemySpriteSource = SpriteSourceCreate(1, 1, enemyTexture);

    // Slimes have gravity affecting them by default
    PhysicsSetAcceleration(GameObjectGetPhysics(enemy->gameObject), &Gravity);
  }
}

// Sets all default fleye values
void DefaultFleye(EnemyPtr enemy)
{
  if (enemy) // If pointer is valid
  {
    // Set all variables to default values
    enemy->health = DefaultFleyeHealth;
    enemy->damage = DefaultFleyeDamage;
    enemy->speed = DefaultFleyeSpeed;
    enemy->ogTimer = RandomRangeFloat(5.5f, 6.5f);
    enemy->timer = enemy->ogTimer;
    enemy->range = DefaultFleyeRange;
    enemy->jumpH = DefaultFleyeFlapHeight;
    enemy->movement = &FleyeMove; // Set movement function pointer
    enemy->fleyeAttacked = FALSE;
    enemy->jumpSound = FALSE;
    enemy->landSound = FALSE;

    enemyTexture = AEGfxTextureLoad("./Assets/Tiles/eyebat_spawn.png");
    enemySpriteSource = SpriteSourceCreate(1, 1, enemyTexture);

    // Fleyes have gravity affecting them by default
    PhysicsSetAcceleration(GameObjectGetPhysics(enemy->gameObject), &Gravity);

    LoadSound("./Assets/SFX/fleye_attack.mp3", sfx);
    LoadSound("./Assets/SFX/fleye_flap.mp3", sfx);
  }
}

// Movement function for slime enemies
void SlimeJump(EnemyPtr enemy, PlayerPtr player, float xDir, float dt)
{
  // Player variable exists for function pointer reasons
  UNREFERENCED_PARAMETER(player);

  if (enemy) // If pointer is valid
  {
    
    // Get physics, transform, and collider
    PhysicsPtr physics =     GameObjectGetPhysics(enemy->gameObject);
    TransformPtr transform = GameObjectGetTransform(enemy->gameObject);
    AABB collider =          GameObjectGetCollider(enemy->gameObject);

    if (physics && transform && collider) // If pointers are valid
    {
      // Make variable for velocity equal to current velocity
      Vector2D velocity = *PhysicsGetVelocity(physics);

      if (velocity.x > 0.0f || velocity.x < 0.0f)
      {
          xDir = velocity.x;
      }
      // Every timer seconds && if colliding with floor
      if (enemy->timer <= 0.0f && enemy->floorCollision)
      {
          if (xDir < 0)
          {
              EnemyAnimate(enemy, 0);
          }
          else if (xDir > 0)
          {
              EnemyAnimate(enemy, 1);
          }
        velocity.y = DefaultSlimeJumpHeight; // Do a jump
        PhysicsSetAcceleration(physics, &Gravity); // Turn on gravity

        // AESysPrintf("Jump");
        enemy->jumpSound = TRUE;
        enemy->landSound = TRUE;
      }
      else if (enemy->floorCollision) // If not the right frame but still on ground
      {
        enemy->timer -= dt;
        // AESysPrintf("Grounded");
        // Slime isn't moving
        if (velocity.x != 500.0f && velocity.x != -500.0f)
        {
            velocity.x = 0.0f;
        }
        
        

        // Turn off gravity
        //PhysicsSetAcceleration(physics, &noGravity);
      }
      
      Vector2D translation = *TransformGetTranslation(transform);
      if (enemy->timer <= 1.0f && enemy->timer >= 0.2f)
      {
          //translation.y -= 0.35f / sqrtf(enemy->timer);
          translation.y -= 0.3f / enemy->timer;
      }
      else if (enemy->timer < 0.2f && enemy->timer > 0.0f)
      {
          //translation.y += 0.35f / sqrtf(enemy->timer);
          translation.y += enemy->timer / 0.05f;
      }
      
      if (!enemy->floorCollision) // If not grounded
      {
          enemy->timer = enemy->ogTimer;
        // AESysPrintf("Airborne");
        // Move in the correct direction
          if (velocity.x == 0)
          {
              velocity.x = xDir;
          }
          velocity.x = velocity.x;
      }

      if (velocity.x > 0 && enemy->rightCollision)
      {
          velocity.x = 0;
      }
      else if (velocity.x < 0 && enemy->leftCollision)
      {
          velocity.x = 0;
      }

      if (velocity.y < 0 && enemy->floorCollision)
      {
          velocity.y = 0;
          velocity.x = 0;
          PhysicsSetAcceleration(physics, &noGravity);
      }
      else if (velocity.y > 0 && enemy->ceilingCollision)
      {
          velocity.y = 0;
      }

      if (!enemy->floorCollision)
      {
          PhysicsSetAcceleration(physics, &Gravity);
      }
      // Change slime's velocity to new value
      PhysicsSetVelocity(physics, &velocity);
      TransformSetTranslation(transform, &translation);
    }
  }
}

void FleyeMove(EnemyPtr enemy, PlayerPtr player, float xDir, float dt)
{
  if (enemy) // If pointer is valid
  {
    // Get physics, transform, and collider
    PhysicsPtr physics = GameObjectGetPhysics(enemy->gameObject);
    TransformPtr transform = GameObjectGetTransform(enemy->gameObject);
    AABB collider = GameObjectGetCollider(enemy->gameObject);

    if (physics && transform && collider) // If pointers are valid
    {
      // Make variables for velocity and position, set them to current values
      Vector2D velocity = *PhysicsGetVelocity(physics);
      Vector2D pos = *TransformGetTranslation(GameObjectGetTransform(enemy->gameObject));
      velocity.x = enemy->speed;
      if (enemy->mode == Following && DistToPlayer(enemy, player) <= DefaultFleyeAttackDist && enemy->fleyeAttacked == FALSE && enemy->timer <= 0.0f) // If the player is in attack range
      {
         
        // Get the angle from the fleye to the player, complicated
          Vector2D pTranslation = *TransformGetTranslation(GameObjectGetTransform(player->parent));
          Vector2D angle;
          Vector2DSub(&angle, &pTranslation, TransformGetTranslation(transform));
          Vector2DNormalize(&angle, &angle);
          TransformSetRotation(transform, Vector2DToAngleRad(&angle));
          Vector2DScale(&angle, &angle, DefaultFleyeAttackSpeed);

        


        // Temporary
          velocity = angle;

        
        if (pos.y <= TransformGetTranslation(GameObjectGetTransform(player->parent))->y || enemy->playerCollision == TRUE)
        {
            enemy->fleyeAttacked = TRUE;
            enemy->timer = enemy->ogTimer;
            PlaySFX("./Assets/SFX/fleye_attack.mp3");
        }
      }
      else if (pos.y <= enemy->spawnPos.y - 100.0f) // If 100 units below spawn pos
      {
          AESysPrintf("Fleye Attack");
        velocity.y = DefaultFleyeFlapHeight; // Do a flap
        PhysicsSetAcceleration(GameObjectGetPhysics(enemy->gameObject), &noGravity);
        PlaySFX("./Assets/SFX/fleye_flap.mp3");
        
        
      }
      if (pos.y >= enemy->spawnPos.y - 25.0f || pos.y <= enemy->spawnPos.y + 25.0f && enemy->fleyeAttacked == TRUE)
      {
          enemy->fleyeAttacked = FALSE;
      }
      else
      {
          PhysicsSetAcceleration(GameObjectGetPhysics(enemy->gameObject), &Gravity);
      }

      if (velocity.x < 0)
      {
          EnemyAnimate(enemy, 0);
      }
      else if (velocity.x > 0)
      {
          EnemyAnimate(enemy, 1);
      }
      
      enemy->timer -= dt;
      if (velocity.x > 0 && enemy->rightCollision)
      {
          enemy->speed = -enemy->speed;
          velocity.x = 0;
          pos.x = pos.x - 15.0f;
      }
      else if (velocity.x < 0 && enemy->leftCollision)
      {
          enemy->speed = -enemy->speed;
          velocity.x = 0;
          pos.x = pos.x + 15.0f;
      }

      if (velocity.y < 0 && enemy->floorCollision)
      {
          velocity.y = 0;
      }
      else if (velocity.y > 0 && enemy->ceilingCollision)
      {
          velocity.y = 0;
      }

      PhysicsSetVelocity(physics, &velocity);
      TransformSetTranslation(transform, &pos);
    }
  }
}

// Draws the given enemy
void DrawEnemy(EnemyPtr enemy)
{
  if (enemy->gameObject == NULL || SpriteGetMesh(GameObjectGetSprite(enemy->gameObject)) == NULL) // If pointers aren't valid
    return;

  // Set render mode.
  AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

  // Set the position.
  TransformPtr transform = GameObjectGetTransform(enemy->gameObject);
  Vector2D translation = *TransformGetTranslation(transform);
  AEGfxSetPosition(translation.x, translation.y);

  // Render the texture.
  AEGfxTextureSet(SpriteSourceGetTexture(SpriteGetSpriteSource(GameObjectGetSprite(enemy->gameObject))), 0.0F, 0.0F);

  // Set transparency.
  AEGfxSetTransparency(1.0F);

  
  
      if (enemy->timer <= 1.0f && enemy->timer >= 0.2f && enemy->enemyType == Slime)
      {
          AEGfxSetFullTransform(TransformGetTranslation(transform)->x, TransformGetTranslation(transform)->y,
              TransformGetRotation(GameObjectGetTransform(enemy->gameObject)), AEClamp(0.5f / enemy->timer, 0.5f, 2.0f), enemy->timer);
          if (enemy->jumpSound) {
              PlaySFX("./Assets/SFX/slime_jump.wav");
              enemy->jumpSound = false;
          }
          
      }
      else if (enemy->timer < 0.2f && enemy->timer > 0.0f && enemy->enemyType == Slime)
      {
          AEGfxSetFullTransform(TransformGetTranslation(transform)->x, TransformGetTranslation(transform)->y,
              TransformGetRotation(GameObjectGetTransform(enemy->gameObject)), AEClamp(enemy->timer/0.1f, 1.0f, 2.0f), AEClamp(0.05f / enemy->timer, 0.2f, 1.0f));
      }
  
  
  
  // Draw the mesh.
  AEGfxMeshDraw(SpriteGetMesh(GameObjectGetSprite(enemy->gameObject)), AE_GFX_MDM_TRIANGLES);
}

// Checks if the enemy is touching the ground
void CollisionChecks(EnemyPtr enemy, AABB* levelTiles)
{
  if (levelTiles && enemy) // If pointers are valid
  {
    // Get collider and translation
    AABB collider = GameObjectGetCollider(enemy->gameObject);
    float rotation = TransformGetRotation(GameObjectGetTransform(enemy->gameObject));
    AABB* List = EnemyBoxCast(collider, (int)(rotation / (PI / 2)));
    /*
    int colCount = 0;
    AABB collisions = ReturnAABBCollisionsInRange(collider, &colCount); // Get all tile colliders in range

    for (int i = 0; i < colCount; ++i) // Iterate through them all
    {
      if (CheckCollisionDirect(collider, &collisions[i]) == Right)
      {
        enemy->floorCollision = TRUE; // If you're grounded on any tiles then return
        return;
      }
      else
      {
        enemy->floorCollision = FALSE;
      }
    }

    if (collisions) // Free the array
    {
      free(collisions);
      collisions = NULL;
    }
    */
    //Waldo has been found for the 2nd time!

    if (levelTiles)
    {
        int colCount = 0;
        enum collision_direction direct;

        collisionDirection = 0;
        isColliding = false;
        enemy->floorCollision = false;
        enemy->ceilingCollision = false;
        enemy->rightCollision = false;
        enemy->leftCollision = false;

        for (int i = 1; i < 5; ++i) {
            if (AABBReturnColNumberinRange(enemy->gameObject, List[i]))
            {
                if (i == 4)
                {
                    isColliding = true;
                    direct = i;
                    enemy->floorCollision = true;
                    ++colCount;
                    if (enemy->landSound) {
                        PlaySFX("./Assets/SFX/slime_land.wav");
                        enemy->landSound = false;
                    }
                }

                if (i == 3)
                {
                    enemy->ceilingCollision = true;
                    ++colCount;
                }

                if (i == 1)
                {
                    enemy->leftCollision = true;
                    collisionDirection = -1;
                    ++colCount;
                    //velocity.x = 0;
                }
                else if (i == 2)
                {
                    enemy->rightCollision = true;
                    collisionDirection = 1;
                    ++colCount;
                }
            }
        }
    }

    /*
    for (int y = 0; y < MAX_LEVEL_HEIGHT; ++y)
    {
      for (int x = 0; x < MAX_LEVEL_WIDTH; ++x)
      {
        if (CheckCollisionDirect(collider, levelTiles[y * MAX_LEVEL_WIDTH + x]) == Right)
        {
          enemy->floorCollision = TRUE;
          return;
        }
        else
        {
          enemy->floorCollision = FALSE;
        }
      }
    }
    */
  }
}

// Check if enemy is colliding with player
void CheckPlayerCollision(EnemyPtr enemy, PlayerPtr player)
{
  if (enemy && player) // If pointers are valid
  {
    // Get player and enemy colliders
    AABB eCollider = GameObjectGetCollider(enemy->gameObject);
    AABB pCollider = GameObjectGetCollider(player->parent);

    if (CheckAABBCollision(eCollider, pCollider)) // If enemy and players are colliding
    {
      enemy->playerCollision = TRUE;
    }
    else
    {
      enemy->playerCollision = FALSE;
    }
  }
}

// Get enemy with a specific name from enemy list
EnemyPtr GetEnemyName(char name[16])
{
  for (int i = 0; i < MaxSpawnedEnemies; i++) // Iterate through enemy list
  {
    if (enemyList[i]->name == name) // If found the right name
    {
      return enemyList[i]; // Return the right enemy
    }
  }

  return 0;
}

// Get enemy at specific index of enemy list
EnemyPtr GetEnemyIndex(int index)
{
  if (index < MaxSpawnedEnemies && &enemyList[index] != NULL) // If the index is a valid enemy
  {
    return enemyList[index]; // Return the right enemy
  }

  return 0;
}

// Kill all enemies
void DespawnAll()
{
  for (int i = 0; i < MaxSpawnedEnemies; i++) // Iterate through enemy list
  {
    Death(GetEnemyIndex(i)); // Kill all enemies
  }
}

// Unload all meshes and textures
void UnloadAll()
{
  for (int i = 0; i < MaxSpawnedEnemies; i++) // Iterate through enemy list
  {
    if (GetEnemyIndex(i) != NULL) // If there is an enemy
    {
      if (SpriteGetMesh(GameObjectGetSprite(GetEnemyIndex(i)->gameObject)) != NULL) // If the enemy has a mesh
      {
          for (int j = 0; j < 2; j++)
          {
              AEGfxMeshFree(GetEnemyIndex(i)->meshes[j]); // Free the mesh
          }
      }
      if (SpriteSourceGetTexture(SpriteGetSpriteSource(GameObjectGetSprite(enemyList[i]->gameObject))) != NULL) // If the enemy has a texture
      {
        AEGfxTextureUnload(
          SpriteSourceGetTexture(SpriteGetSpriteSource(GameObjectGetSprite(enemyList[i]->gameObject)))); // Free the texture
      }
    }
  }
}

// Calls Existing() for all enemies
void UpdateAllEnemies(PlayerPtr player, AABB* levelTiles, float dt)
{
  for (int i = 0; i < MaxSpawnedEnemies; i++) // Iterate through enemy list
  {
    if (GetEnemyIndex(i) != NULL) // If there is an enemy at current position
    {
      Existing(GetEnemyIndex(i), player, levelTiles, dt); // Call existing for that enemy
    }
  }
}

// Calls DrawEnemy() for all enemies
void DrawAllEnemies()
{
  for (int i = 0; i < MaxSpawnedEnemies; i++) // Iterate through enemy list
  {
    if (GetEnemyIndex(i) != NULL) // If there is an enemy at current position
    {
      DrawEnemy(GetEnemyIndex(i)); // Draw that enemy
    }
  }
}


AABB* EnemyBoxCast(AABB col, int dir)
{
    AABB* List = (AABB*)calloc(5, sizeof(AABB));

    if (List) {
        List[0] = col;

        //make it easier to type
        float colx = col->WorldPos.x;
        float coly = col->WorldPos.y;


        Vector2D LeftPos = { 0, 0 };
        Vector2D RightPos = { 0, 0 };
        Vector2D TopPos = { 0, 0 };
        Vector2D BottomPos = { 0, 0 };

        switch (dir)
        {
            // down
        case 4:
            LeftPos.x = colx - col->width * 0.05F;
            LeftPos.y = coly;
            RightPos.x = colx + col->width * 0.75F;
            RightPos.y = coly;
            TopPos.x = colx;
            TopPos.y = coly + col->height * 0.5F;
            BottomPos.x = colx;
            BottomPos.y = coly - col->height * 0.265F;

            //left
            List[Right] = FromPoints(LeftPos, BOX_SIZE, col->height * 0.5F, 0, 0, false);
            //right
            List[Left] = FromPoints(RightPos, BOX_SIZE, col->height * 0.5F, 0, 0, false);
            //top
            List[Bottom] = FromPoints(TopPos, col->width * 0.5F, BOX_SIZE, 0, 0, false);
            //bottom
            List[Top] = FromPoints(BottomPos, col->width * 0.5F, BOX_SIZE, 0, 0, false);
            break;

            // up.
        default:
            LeftPos.x = colx - col->width * 0.05F;
            LeftPos.y = coly;
            RightPos.x = colx + col->width * 0.75F;
            RightPos.y = coly;
            TopPos.x = colx;
            TopPos.y = coly + col->height * 0.5F;
            BottomPos.x = colx;
            BottomPos.y = coly - col->height * 0.265F + 10.0f;

            //left
            List[Left] = FromPoints(LeftPos, BOX_SIZE, col->height * 0.5F, 0, 0, false);
            //right
            List[Right] = FromPoints(RightPos, BOX_SIZE, col->height * 0.5F, 0, 0, false);
            //top
            List[Top] = FromPoints(TopPos, col->width * 0.5F, BOX_SIZE, 0, 0, false);
            //bottom
            List[Bottom] = FromPoints(BottomPos, col->width * 0.5F, BOX_SIZE, 0, 0, false);
            break;
        }
        List[Not] = col;

        return List;
    }
    else
        return NULL;

}
