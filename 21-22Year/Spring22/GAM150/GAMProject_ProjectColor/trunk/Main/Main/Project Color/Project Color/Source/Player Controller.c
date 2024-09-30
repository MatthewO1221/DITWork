//------------------------------------------------------------------------------
//
// File Name:	Player Data.h
// Author(s):	Rishabh Anand
// Project:		Project 2
// Course:		CS230S20
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "Player Controller.h"
#include "Vector2D.h"
#include "GameObject.h"
#include "Brush.h"
#include "Player Movement.h"
#include "Sprite.h"
#include "Physics.h"
#include "Player.h"
#include "Transform.h"
#include "Animation.h"
#include "AABB.h"
#include "EnemyAI.h"
#include "SpriteSource.h"
#include "Mesh.h"
#include "Tile.h"
#include "Level.h"
#include "World.h"
#include "Camera.h"
#include "Audio.h"
#include "GameStateManager.h"

static PlayerPtr player;
static GameObjectPtr paintBrush;
static AEGfxVertexList* playerMesh, *paintBrushMesh;
static AEGfxTexture* playerTexture, *paintBrushTexture;
static SpriteSourcePtr playerSpriteSource, paintBrushSpriteSource;

static AEGfxVertexList* meshes[8];
static AEGfxTexture* textures[11];
static SpriteSourcePtr spriteSources[11];
float interuptTime = 0.0F;
int playerDamage = 2;
float brushCollisionTimer = 0.25;
float iFrameTimeInit;

void LoadPlayer(void)
{
    // Load player info.
    playerMesh = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "Player Mesh");
    playerTexture = AEGfxTextureLoad("./Assets/player.png");

    // Load paintbrush info.
    paintBrushMesh = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "Paintbrush Mesh");
    paintBrushTexture = AEGfxTextureLoad("./Assets/Player/brush.png");

    // Initialize meshes.
    meshes[0] = MeshCreateQuad(0.5F, 0.5F, 1.0F, 1.0F, "One By One Player");
    meshes[1] = MeshCreateQuad(0.5F, 0.5F, 1.0F / 4, 1.0F, "Four By One Player");
    meshes[2] = MeshCreateQuad(0.5F, 0.5F, 1.0F / 6, 1.0F, "Six By One Player");
    meshes[3] = MeshCreateQuad(0.5F, 0.5F, 1.0F / 16, 1.0F, "Sixteen By One Player");
    meshes[4] = MeshCreateQuad(-0.5F, 0.5F, 1.0F, 1.0F, "One By One Player Reverse");
    meshes[5] = MeshCreateQuad(-0.5F, 0.5F, 1.0F / 4, 1.0F, "Four By One Player Reverse");
    meshes[6] = MeshCreateQuad(-0.5F, 0.5F, 1.0F / 6, 1.0F, "Six By One Player Reverse");
    meshes[7] = MeshCreateQuad(-0.5F, 0.5F, 1.0F / 16, 1.0F, "Sixteen By One Player Reverse");

    // Initialize textures.
    textures[0] = AEGfxTextureLoad("./Assets/Player/player_idle.png");
    textures[1] = AEGfxTextureLoad("./Assets/Player/player_walk.png");
    textures[2] = AEGfxTextureLoad("./Assets/Player/player_run.png");
    textures[3] = AEGfxTextureLoad("./Assets/Player/player_jump.png");
    textures[4] = AEGfxTextureLoad("./Assets/Player/player_fall.png");
    textures[5] = AEGfxTextureLoad("./Assets/Player/player_wallslide.png");
    textures[6] = AEGfxTextureLoad("./Assets/Player/player_walljump.png");
    textures[7] = AEGfxTextureLoad("./Assets/Player/player_swing.png");
    textures[8] = AEGfxTextureLoad("./Assets/Player/player_charge.png");
    textures[9] = AEGfxTextureLoad("./Assets/Player/player_hurt.png");
    textures[10] = AEGfxTextureLoad("./Assets/Player/player_death.png");

    // Initialize sprite sources.
    spriteSources[0] = SpriteSourceCreate(4, 1, textures[0]);
    spriteSources[1] = SpriteSourceCreate(4, 1, textures[1]);
    spriteSources[2] = SpriteSourceCreate(6, 1, textures[2]);
    spriteSources[3] = SpriteSourceCreate(4, 1, textures[3]);
    spriteSources[4] = SpriteSourceCreate(4, 1, textures[4]);
    spriteSources[5] = SpriteSourceCreate(1, 1, textures[5]);
    spriteSources[6] = SpriteSourceCreate(4, 1, textures[6]);
    spriteSources[7] = SpriteSourceCreate(4, 1, textures[7]);
    spriteSources[8] = SpriteSourceCreate(4, 1, textures[8]);
    spriteSources[9] = SpriteSourceCreate(1, 1, textures[9]);
    spriteSources[10] = SpriteSourceCreate(16, 1, textures[10]);

    //Load Player Sounds
    LoadSound("./Assets/SFX/player_hurt.mp3", sfx);
    LoadSound("./Assets/SFX/player_land.mp3", sfx);
    LoadSound("./Assets/SFX/d_1.ogg", sfx);
    LoadSound("./Assets/SFX/d_2.ogg", sfx);
    LoadSound("./Assets/SFX/d_3.ogg", sfx);
    LoadSound("./Assets/SFX/d_4.ogg", sfx);
    LoadSound("./Assets/SFX/d_5.ogg", sfx);
    LoadSound("./Assets/SFX/d_6.ogg", sfx);
    LoadSound("./Assets/SFX/d_7.ogg", sfx);

    LoadSound("./Assets/SFX/w_1.ogg", sfx);
    LoadSound("./Assets/SFX/w_2.ogg", sfx);
    LoadSound("./Assets/SFX/w_3.ogg", sfx);
    LoadSound("./Assets/SFX/w_4.ogg", sfx);
    LoadSound("./Assets/SFX/w_5.ogg", sfx);
    LoadSound("./Assets/SFX/w_6.ogg", sfx);
    LoadSound("./Assets/SFX/w_7.ogg", sfx);
    
}

void InitPlayer(void)
{
    player = calloc(1, sizeof(struct Player));

    if (player)
    {
        player->parent = GameObjectCreate();

        // Load player.
        LoadPlayerStats();
        iFrameTimeInit = player->iFrameTime;

        if (player->parent)
        {
            TransformPtr pTransform = TransformCreate();
            GameObjectAddTransform(player->parent, pTransform);

            PhysicsPtr pPhysics = PhysicsCreate();
            GameObjectAddPhysics(player->parent, pPhysics);

            SpritePtr pSprite = SpriteCreate();
            GameObjectAddSprite(player->parent, pSprite);

            AnimationPtr pAnim = AnimationCreate();
            GameObjectAddAnimation(player->parent, pAnim);

            Vector2D pSpawnPos = { (float)GetPlayer()->roomPosX, (float)GetPlayer()->roomPosY };
            Vector2D pScale = { 128, 128 };
            TransformSetTranslation(pTransform, &pSpawnPos);
            TransformSetRotation(pTransform, 0);
            TransformSetScale(pTransform, &pScale);

            AABB pAABB = FromPoints(pSpawnPos, pScale.x * 0.5F, pScale.y, 0, 0, false);
            GameObjectAddCollider(player->parent, pAABB);

            updateAABBPosition(GameObjectGetCollider(player->parent), GameObjectGetPhysics(player->parent));

            playerSpriteSource = SpriteSourceCreate(1, 1, playerTexture);

            SpriteSetMesh(pSprite, playerMesh);
            SpriteSetSpriteSource(pSprite, playerSpriteSource);
            SpriteSetFrame(pSprite, 0);
        }
    }

    paintBrush = GameObjectCreate();

    if (paintBrush)
    {
        TransformPtr bTransform = TransformCreate();
        GameObjectAddTransform(paintBrush, bTransform);

        PhysicsPtr bPhysics = PhysicsCreate();
        GameObjectAddPhysics(paintBrush, bPhysics);

        SpritePtr bSprite = SpriteCreate();
        GameObjectAddSprite(paintBrush, bSprite);

        AnimationPtr bAnim = AnimationCreate();
        GameObjectAddAnimation(paintBrush, bAnim);

        Vector2D bSpawnPos = { 0, -150 };
        Vector2D bScale = { 128, 128 };
        TransformSetTranslation(bTransform, &bSpawnPos);
        TransformSetRotation(bTransform, 0);
        TransformSetScale(bTransform, &bScale);

        AABB bAABB = FromPoints(bSpawnPos, bScale.x * 0.5F, bScale.y, 0, 0, false);
        GameObjectAddCollider(paintBrush, bAABB);

        updateAABBPosition(GameObjectGetCollider(paintBrush), GameObjectGetPhysics(paintBrush));

        paintBrushSpriteSource = SpriteSourceCreate(1, 1, paintBrushTexture);

        SpriteSetMesh(GameObjectGetSprite(paintBrush), paintBrushMesh);
        SpriteSetSpriteSource(GameObjectGetSprite(paintBrush), paintBrushSpriteSource);
        SpriteSetFrame(GameObjectGetSprite(paintBrush), 0);
    }
}

void PlayerUpdate(float objX, float objY, AABB* levelTiles, float dt)
{
	BrushUpdate(paintBrush, player->parent);
	PlayerMovementUpdate(player->parent, levelTiles);

    //brushCollisionTimer -= dt;

    EnemyPtr enemy = BrushCollidingWithEnemy(paintBrush);

    if (enemy && brushCollisionTimer <= 0 && BrushCollidingWithEnemyOnDownswing())
    {

        if (enemy)
        {
            DamageEnemy(player, enemy); // Check if colliding with the player
        }

        brushCollisionTimer = 0.25f;
    }
    else
    {
        brushCollisionTimer -= dt;
    }


    if (player->iFrames)
    {
        player->iFrameTime -= dt;


        if (player->iFrameTime <= 0.0f)
        {
            player->iFrames = false;
            player->iFrameTime = iFrameTimeInit;
        }

        if ((int)player->iFrameTime * 10 % 2 == 0)
        {
            if (SpriteGetAlpha(GameObjectGetSprite(player->parent)) == 1.0f)
            {
                SpriteSetAlpha((GameObjectGetSprite(player->parent)), 0.5f);
            }
            else if (SpriteGetAlpha(GameObjectGetSprite(player->parent)) == 0.5f)
            {
                SpriteSetAlpha((GameObjectGetSprite(player->parent)), 1.0f);
            }
        }
    }

    
    if (player->health <= 0)
    {
            
        GameStateManagerSetNextState(GsRestart);
            

    }
    
    

    
    
}

void ShutdownPlayer(void)
{
    // Free memory with player.
    SpriteSourceFree(&playerSpriteSource);
    GameObjectFree(&player->parent);
    

    // Free memory with paintbrush.
    SpriteSourceFree(&paintBrushSpriteSource);
    GameObjectFree(&paintBrush);
}

void UnloadPlayer(void)
{
    // Free meshes.
    for (int i = 0; i < 8; ++i)
    {
        // Free mesh.
        AEGfxMeshFree(meshes[i]);
    }

    // Free textures and sprite sources.
    for (int i = 0; i < 11; ++i)
    {
        // Free sprite source.
        SpriteSourceFree(&spriteSources[i]);

        // Free texture.
        AEGfxTextureUnload(textures[i]);
    }

    // Free paintbrush info.
    AEGfxMeshFree(paintBrushMesh);
    AEGfxTextureUnload(paintBrushTexture);

    // Free player info.
    AEGfxMeshFree(playerMesh);
    AEGfxTextureUnload(playerTexture);
}

void PlayerAnimate(enum PlayerMesh mesh, enum PlayerState state, int frameCount, float frameDuration, bool loop)
{
    interuptTime -= (float)AEFrameRateControllerGetFrameTime();

    if (interuptTime <= 0)
    {
        interuptTime = 0;
    }

    if (interuptTime > 0 || SpriteGetSpriteSource(GameObjectGetSprite(player->parent)) == spriteSources[state])
        return;

    SpriteSetMesh(GameObjectGetSprite(player->parent), meshes[mesh]);
    SpriteSetSpriteSource(GameObjectGetSprite(player->parent), spriteSources[state]);
    AnimationPlay(GameObjectGetAnimation(player->parent), frameCount, frameDuration, loop);

    if (state == SWING)
    {
        interuptTime = frameDuration * frameCount * 1.5F;
    }
}

void DamageEnemy(PlayerPtr player, EnemyPtr enemy)
{
    enemy->health -= playerDamage;
    AESysPrintf("Damaging Player");

    Vector2D pos = *TransformGetTranslation(GameObjectGetTransform(player->parent));
    EnemyHurt(enemy, &pos);

    //AEGfxVertexList* mesh = SpriteGetMesh(playerSprite);
    //SpriteSourcePtr spriteSource = SpriteGetSpriteSource(playerSprite);
}

void PlayerHurt(Vector2D* pos)
{
    PhysicsPtr physics = GameObjectGetPhysics(player->parent);

    TransformPtr playerTransform = GameObjectGetTransform(player->parent);

    Vector2D playerPos = *TransformGetTranslation(playerTransform);

    Vector2D launchAngle = { 750, 750 };
    launchAngle.x *= pos->x < playerPos.x ? 1 : -1;

    PhysicsSetVelocity(physics, &launchAngle);
    PlayerAnimate((int)launchAngle.x / 750 == -1 ? ONE_BY_ONE : ONE_BY_ONE_REVERSE, HURT, 1, 1.0F, false);
    PlaySFX("./Assets/SFX/player_hurt.mp3");

    player->iFrames = true;
}

PlayerPtr GetPlayer(void)
{
    if (player)
    {
        return player;
    }
    return 0;
}

GameObjectPtr GetBrush(void)
{
    if (paintBrush)
    {
        return paintBrush;
    }
    return 0;
}

// Checks if the player is out of bounds and loads the correct room accordingly.
void CheckForPlayerOutOfBounds(void)
{
    if (GetPlayer())
    {
        // Get player transform data.
        TransformPtr pTransform = GameObjectGetTransform(GetPlayer()->parent);
        Vector2D playerPos = *TransformGetTranslation(pTransform);

        // Get positions for parts of level.
        float bottomEdge = -448;
        float topEdge = GetCurrentTilemap()->height * TILE_SIZE - 448.0F + 32;
        float leftEdge = -768;
        float rightEdge = GetCurrentTilemap()->width * TILE_SIZE - 768.0F;
        float xMid = (leftEdge + rightEdge) / 2;
        float yMid = (bottomEdge + topEdge) / 2;

        // See if the player is out of the world.
        if (playerPos.x < leftEdge ||
            playerPos.x > rightEdge ||
            playerPos.y < bottomEdge ||
            playerPos.y > topEdge)
        {
            // Define new room.
            struct Room* newRoom = NULL;

            // Determine which room to load.
            if (playerPos.y > topEdge)
            {
                if (playerPos.x > xMid)
                {
                    SetRoom(&newRoom, GetCurrentRoom()->topRight ? GetCurrentRoom()->topRight : GetCurrentRoom()->top);
                }
                else
                {
                    SetRoom(&newRoom, GetCurrentRoom()->topLeft ? GetCurrentRoom()->topLeft : GetCurrentRoom()->top);
                }

                playerPos.y = -384;

                GameObjectPtr player = GetPlayer()->parent;
                PhysicsPtr physics = GameObjectGetPhysics(player);

                Vector2D newVel = { 0, 1000 };

                PhysicsSetVelocity(physics, &newVel);
            }
            else if (playerPos.y < bottomEdge)
            {
                if (playerPos.x > xMid)
                {
                    SetRoom(&newRoom, GetCurrentRoom()->bottomRight ? GetCurrentRoom()->bottomRight : GetCurrentRoom()->bottom);
                }
                else
                {
                    SetRoom(&newRoom, GetCurrentRoom()->bottomLeft ? GetCurrentRoom()->bottomLeft : GetCurrentRoom()->bottom);
                }

                playerPos.y = GetNextTilemap()->height * TILE_SIZE - 448.0F;
            }
            else if (playerPos.x < leftEdge)
            {
                if (playerPos.y > yMid)
                {
                    SetRoom(&newRoom, GetCurrentRoom()->topLeft ? GetCurrentRoom()->topLeft : GetCurrentRoom()->left);
                }
                else
                {
                    SetRoom(&newRoom, GetCurrentRoom()->bottomLeft ? GetCurrentRoom()->bottomLeft : GetCurrentRoom()->left);
                }

                playerPos.x = GetNextTilemap()->width * TILE_SIZE - 768.0F;
            }
            else
            {
                if (playerPos.y > yMid)
                {
                    SetRoom(&newRoom, GetCurrentRoom()->topRight ? GetCurrentRoom()->topRight : GetCurrentRoom()->right);
                }
                else
                {
                    SetRoom(&newRoom, GetCurrentRoom()->bottomRight ? GetCurrentRoom()->bottomRight : GetCurrentRoom()->right);
                }

                playerPos.x = -768.0F;
            }

            // Check if room beeds to be loaded.
            if (newRoom != NULL)
            {
                Vector2D roomDiffBottomLeft = { ((int)GetNextRoom()->x - (int)GetCurrentRoom()->x) * (float)TILE_SIZE,
                                                ((int)GetNextRoom()->y - (int)GetCurrentRoom()->y) * (float)TILE_SIZE };
                Vector2D roomDiffTopRight = { ((int)(GetNextRoom()->x + (int)GetNextRoom()->width) - ((int)GetCurrentRoom()->x + (int)GetCurrentRoom()->width)) * (float)TILE_SIZE,
                                              ((int)(GetNextRoom()->y + (int)GetNextRoom()->height) - ((int)GetCurrentRoom()->y + (int)GetCurrentRoom()->height)) * (float)TILE_SIZE };

                if (playerPos.x == -768.0F || playerPos.x == GetNextTilemap()->width * TILE_SIZE - 768.0F)
                {
                    playerPos.y -= roomDiffBottomLeft.y;
                }

                if (playerPos.y == -384 || playerPos.y == GetNextTilemap()->height * TILE_SIZE - 448.0F)
                {
                    playerPos.x -= roomDiffBottomLeft.x;
                }

                // Switch out current room for new room.
                SwitchRoom(newRoom);

                // Apply new player position.
                TransformSetTranslation(GameObjectGetTransform(GetPlayer()->parent), &playerPos);

                // Reset the camera.
                SetCamPos(&playerPos, true);
            }
        }

    }
}
