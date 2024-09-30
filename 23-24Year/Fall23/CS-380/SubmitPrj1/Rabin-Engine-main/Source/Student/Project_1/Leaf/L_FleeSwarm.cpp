#include <pch.h>
#include "L_FleeSwarm.h"
#include "Agent/BehaviorAgent.h"


#define DISTANCE 20.0f
#define FLEEDISTANCE 20.0f

void L_FleeSwarm::on_enter()
{
    const auto& swarmlings = agents->get_all_agents_by_type("Swarmling");

    float closestDistance = std::numeric_limits<float>().max();
    bool foundSwarmling = false;
    Vec3 closestPoint;

    for each (auto swarmling in swarmlings)
    {
        if (Vec3::Distance(agent->get_position(), swarmling->get_position()) < closestDistance
            && Vec3::Distance(agent->get_position(), swarmling->get_position()) < DISTANCE)
        {
            closestDistance = Vec3::Distance(agent->get_position(), swarmling->get_position());
            closestPoint = swarmling->get_position();
            foundSwarmling = true;
        }
    }
    
    Vec3 fleeVec = agent->get_position() - closestPoint;
    fleeVec.Normalize();
    fleePoint = agent->get_position() + (fleeVec * FLEEDISTANCE);

    if (foundSwarmling == false)
    {
        on_failure();
    }
    else
    {
    BehaviorNode::on_leaf_enter();

    }
 
}

void L_FleeSwarm::on_update(float dt)
{
    const auto result = agent->move_toward_point(fleePoint, dt);

    if (result == true)
    {
        on_success();
    }

    

    display_leaf_text();
}
