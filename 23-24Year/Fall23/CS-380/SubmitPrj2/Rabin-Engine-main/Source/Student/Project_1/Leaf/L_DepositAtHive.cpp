#include <pch.h>
#include "L_DepositAtHive.h"
#include "Agent/BehaviorAgent.h"

#define FULLVALUE 100.0f

void L_DepositAtHive::on_enter()
{
    // set animation, speed, etc
    float curNutrition = agent->get_blackboard().get_value<float>("Nutrition");

   
    // find the agent that is the furthest from this one
    float closestDistance = std::numeric_limits<float>().max();
    Vec3 closestPoint;
    bool targetFound = false;

    // get a list of all current agents
    const auto& hives = agents->get_all_agents_by_type("Hive");
    
    // and our agent's position
    const auto &currPos = agent->get_position();

    for (auto hive : hives)
    {
        // make sure it's not our agent
        
            const auto &agentPos = hive->get_position();
            const float distance = Vec3::Distance(currPos, agentPos);

            if (distance < closestDistance)
            {
                closestDistance = distance;
                closestPoint = agentPos;
                closestHive = static_cast<BehaviorAgent*>(hive);
                targetFound = true;
            }
        
    }

   

    if (targetFound == true && curNutrition >= FULLVALUE)
    {
        targetPoint = closestPoint;
		BehaviorNode::on_leaf_enter();
    }
    else // couldn't find a viable agent
    {
        on_failure();
    }
}

void L_DepositAtHive::on_update(float dt)
{
    const auto result = agent->move_toward_point(targetPoint, dt);

    if (result == true)
    {
        float curNutrition = agent->get_blackboard().get_value<float>("Nutrition");
        closestHive->get_blackboard().set_value("Nutrition", closestHive->get_blackboard().get_value<float>("Nutrition") + curNutrition);
        agent->get_blackboard().set_value("Nutrition", 0.0f);
        on_success();
    }

    display_leaf_text();
}

