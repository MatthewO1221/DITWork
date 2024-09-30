#include <pch.h>
#include "L_SpawnSwarmling.h"
#include "Agent/BehaviorAgent.h"
#include "Projects/ProjectOne.h"


void L_SpawnSwarmling::on_update(float dt)
{
	float spawnCost = agent->get_blackboard().get_value<float>("SpawnRequirement");
	float curNutrition = agent->get_blackboard().get_value<float>("Nutrition");

	if (curNutrition < spawnCost)
	{
		on_failure();
	}
	else
	{
		on_success();
	}

	agent->get_blackboard().set_value("Nutrition", agent->get_blackboard().get_value<float>("Nutrition") - spawnCost);

	BehaviorAgent* newSwarmling = agents.get()->create_behavior_agent("Swarmling", BehaviorTreeTypes::SwarmTree);

	newSwarmling->get_blackboard().set_value("Nutrition", 0.0f); 
	newSwarmling->set_position(Vec3(50,0,50));
	newSwarmling->set_scaling(0.1f);


}

