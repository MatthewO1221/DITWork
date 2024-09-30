#include <pch.h>
#include "D_CheckEnoughNutritionToSpawn.h"

#define SPAWNREQUIREMENT 10.0f

D_CheckEnoughNutritionToSpawn::D_CheckEnoughNutritionToSpawn()
{}



void D_CheckEnoughNutritionToSpawn::on_update(float dt)
{
    float nutritionValue = agent->get_blackboard().get_value<float>("Nutrition");
	

	if (nutritionValue >= SPAWNREQUIREMENT)
	{
		

		on_success();
	}
	else
	{
		on_failure();
	}
}
