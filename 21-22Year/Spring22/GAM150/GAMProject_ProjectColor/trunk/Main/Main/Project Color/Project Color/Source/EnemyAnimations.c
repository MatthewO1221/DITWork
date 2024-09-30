#include "stdafx.h"
#include "EnemyAnimations.h"
#include "EnemyAI.h"
#include "AEEngine.h"
#include "SpriteSource.h"
#include "Animation.h"
#include "Sprite.h"
#include "Mesh.h"
#include "Tile.h"
#include "Audio.h"



void LoadAnimations(EnemyPtr enemy)
{
    // Initialize meshes.

    switch (enemy->enemyType)
    {
    case(Slime):
        enemy->meshes[0] = MeshCreateQuad(TILE_SIZE * 0.5f, TILE_SIZE * 0.5f, 1.0f, 1.0f, "Enemy Normal");
        enemy->meshes[1] = MeshCreateQuad(TILE_SIZE * -0.5f, TILE_SIZE * 0.5f, 1.0f, 1.0f, "Enemy Normal Reverse");
        break;
    case(Fleye):
        enemy->meshes[0] = MeshCreateQuad(TILE_SIZE * 0.5f, TILE_SIZE * 0.5f, 1.0f, 1.0f, "Enemy Normal");
        enemy->meshes[1] = MeshCreateQuad(TILE_SIZE * -0.5f, TILE_SIZE * 0.5f, 1.0f, 1.0f, "Enemy Normal Reverse");
    default:
        
        break;
    }

}

void EnemyAnimate(EnemyPtr enemy, int mesh)
{
    if (enemy)
    {
        SpriteSetMesh(GameObjectGetSprite(enemy->gameObject), enemy->meshes[mesh]);
    }
    
}
