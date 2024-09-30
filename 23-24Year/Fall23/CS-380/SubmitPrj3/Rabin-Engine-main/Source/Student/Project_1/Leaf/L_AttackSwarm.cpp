#include <pch.h>
#include "L_AttackSwarm.h"
#include "Agent/BehaviorAgent.h"

#define DISTANCE 10.0f

void L_AttackSwarm::on_update(float dt)
{
    const auto& swarmlings = agents->get_all_agents_by_type("Swarmling");

    float closestDistance = std::numeric_limits<float>().max();
    bool foundSwarmling = false;
    Agent *closestSwarmling = nullptr;

    for each (auto swarmling in swarmlings)
    {
        if (Vec3::Distance(agent->get_position(), swarmling->get_position()) < closestDistance
            && Vec3::Distance(agent->get_position(), swarmling->get_position()) < DISTANCE)
        {
            closestDistance = Vec3::Distance(agent->get_position(), swarmling->get_position());
            closestSwarmling = swarmling;
            foundSwarmling = true;
        }
    }
    
    if (closestSwarmling)
    {
        AgentOrganizer* organizer = agents.get();
        organizer->destroy_agent(closestSwarmling);
        on_success();
    }
    else
    {
        on_failure();
    }
    
    display_leaf_text();
    
}


