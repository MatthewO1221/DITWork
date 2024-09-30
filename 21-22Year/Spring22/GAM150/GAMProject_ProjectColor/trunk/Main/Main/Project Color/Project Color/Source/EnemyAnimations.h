#pragma once
#include <stdbool.h>


typedef struct Enemy* EnemyPtr;





void EnemyAnimate(EnemyPtr enemy, int mesh);

void LoadAnimations(EnemyPtr enemy);