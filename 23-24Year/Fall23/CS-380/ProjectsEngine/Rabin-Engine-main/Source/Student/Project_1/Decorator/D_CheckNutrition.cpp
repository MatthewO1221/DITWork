#include <pch.h>
#include "D_CheckNutrition.h"

#define FULLVALUE 100.0f

D_CheckNutrition::D_CheckNutrition()
{}



void D_CheckNutrition::on_update(float dt)
{
    float nutritionValue = agent->get_blackboard().get_value<float>("Nutrition");

	if (nutritionValue >= FULLVALUE)
	{
		on_success();
	}
	else
	{
		on_failure();
	}
}
