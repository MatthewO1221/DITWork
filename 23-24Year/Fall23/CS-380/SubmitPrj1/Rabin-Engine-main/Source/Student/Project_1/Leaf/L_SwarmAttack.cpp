#include <pch.h>
#include "L_SwarmAttack.h"
#include "Agent/BehaviorAgent.h"

#define NEARDIST 10.0f
#define AVOIDFACTOR 3.0f
#define ALIGNFACTOR 5.0f
#define ATTRACTFACTOR 100.0f
#define ATTACKFACTOR 5.0f
#define MOVESPEED 0.5f



void L_SwarmAttack::on_enter()
{
    // set animation, speed, etc

    // find the agent that is the furthest from this one
    
    float closestDistance = std::numeric_limits<float>().max();


    // get a list of all current agents
    const auto& swarmlings = agents->get_all_agents_by_type("Swarmling");

    const auto& people = agents->get_all_agents_by_type("Person");
    const auto& soldiers = agents->get_all_agents_by_type("Soldier");

    // and our agent's position
    const auto& currPos = agent->get_position();

    for (const auto& swarmling : swarmlings)
    {
        // make sure it's not our agent
        if (swarmling != agent)
        {
            const auto& agentPos = swarmling->get_position();
            const float distance = Vec3::Distance(currPos, agentPos);

            if (distance < NEARDIST)
            {
                nearbySwarmlings.push_back(static_cast<BehaviorAgent*>(swarmling));
            }
        }
    }

    for each (auto person in people)
    {
        if (Vec3::Distance(agent->get_position(), person->get_position()) < closestDistance)
        {
            closestDistance = Vec3::Distance(agent->get_position(), person->get_position());
            targetPoint = person->get_position();
        }
    }

    for each (auto soldier in soldiers)
    {
        if (Vec3::Distance(agent->get_position(), soldier->get_position()) < closestDistance)
        {
            closestDistance = Vec3::Distance(agent->get_position(), soldier->get_position());
            targetPoint = soldier->get_position();
        }
    }

    BehaviorNode::on_leaf_enter();
}

void L_SwarmAttack::on_update(float dt)
{
    Vec3 finalVelocity;
    //Separation Code
    Vec3 closeVec;

    for each (auto swarmling in nearbySwarmlings)
    {
        closeVec += agent->get_position() - swarmling->get_position();
    }

    finalVelocity += closeVec * AVOIDFACTOR;


    //Alignment Code

    Vec3 averageVec;

    for each (auto swarmling in nearbySwarmlings)
    {
        averageVec += swarmling->get_forward_vector();
    }

    if (nearbySwarmlings.size() > 0)
    {
        averageVec /= static_cast<float>(nearbySwarmlings.size());
    }
    

    finalVelocity += (averageVec - agent->get_forward_vector()) * ALIGNFACTOR;


    //Cohesion Code

    Vec3 posAverageVec;

        for each (auto swarmling in nearbySwarmlings)
        {
            posAverageVec += swarmling->get_position();
        }

        if (nearbySwarmlings.size() > 0)
        {
            posAverageVec /= static_cast<float>(nearbySwarmlings.size());
        }
        

        finalVelocity += (posAverageVec - agent->get_position()) * ATTRACTFACTOR;


        //Attack Code

        finalVelocity += (targetPoint - agent->get_position()) * ATTACKFACTOR;

        finalVelocity.Normalize();
        Vec3 movePoint = agent->get_position() + (finalVelocity * MOVESPEED);
        movePoint.y = 0.0f;

        auto result = agent->move_toward_point(movePoint, dt);

        if (result)
        {
            on_success();
        }

        display_leaf_text();
}

