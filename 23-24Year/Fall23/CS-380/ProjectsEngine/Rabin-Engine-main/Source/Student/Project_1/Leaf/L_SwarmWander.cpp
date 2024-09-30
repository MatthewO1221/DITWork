#include <pch.h>
#include "L_SwarmWander.h"
#include "Agent/BehaviorAgent.h"


#define NEARDIST 10.0f
#define AVOIDFACTOR 3.0f
#define ALIGNFACTOR 5.0f
#define ATTRACTFACTOR 100.0f
#define TETHERFACTOR 50.0f
#define TETHERDIST 50.0f
#define MOVESPEED 1.0f


#define DISTANCE 10.0f


void L_SwarmWander::on_enter()
{
    // set animation, speed, etc

    // find the agent that is the furthest from this one

    auto& people = agents->get_all_agents_by_type("Person");
    auto& soldiers = agents->get_all_agents_by_type("Soldier");

    int numPeopleFound = 0;

    const auto& pos = agent->get_position();

    for each (auto person in people)
    {
        if (Vec3::Distance(pos, person->get_position()) <= DISTANCE && person != agent)
        {
            numPeopleFound++;
        }
    }

    for each (auto soldier in soldiers)
    {
        if (Vec3::Distance(pos, soldier->get_position()) <= DISTANCE && soldier != agent)
        {
            numPeopleFound++;
        }
    }

    if (numPeopleFound > 0)
    {
        on_failure();
    }


    // get a list of all current agents
    const auto& swarmlings = agents->get_all_agents_by_type("Swarmling");

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

    if (nearbySwarmlings.empty())
    {
        on_failure();
    }
    else
    {
        BehaviorNode::on_leaf_enter();
    }

}

void L_SwarmWander::on_update(float dt)
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

    averageVec /= static_cast<float>(nearbySwarmlings.size());

    finalVelocity += (averageVec - agent->get_forward_vector()) * ALIGNFACTOR;


    //Cohesion Code

    Vec3 posAverageVec;

        for each (auto swarmling in nearbySwarmlings)
        {
            posAverageVec += swarmling->get_position();
        }

        posAverageVec /= static_cast<float>(nearbySwarmlings.size());

        finalVelocity += (posAverageVec - agent->get_position()) * ATTRACTFACTOR;


        //Tether Code

        if (Vec3::Distance(agent->get_position(), Vec3(50,0,50)) > TETHERDIST)
        {
            finalVelocity += (Vec3(50, 0, 50) - agent->get_position()) * TETHERFACTOR;
        }

        finalVelocity.Normalize();
        Vec3 movePoint = agent->get_position() + (finalVelocity * MOVESPEED);
        movePoint.y = 0.0f;

        const auto result = agent->move_toward_point(movePoint, dt);

        if (result)
        {
            on_success();
        }

        display_leaf_text();
}

