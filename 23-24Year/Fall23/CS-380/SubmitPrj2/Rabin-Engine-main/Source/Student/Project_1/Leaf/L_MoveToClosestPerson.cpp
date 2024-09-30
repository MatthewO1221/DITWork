#include <pch.h>
#include "L_MoveToClosestPerson.h"
#include "Agent/BehaviorAgent.h"

#define DISTANCE 10.0f

void L_MoveToClosestPerson::on_enter()
{
    // set animation, speed, etc

    // find the agent that is the furthest from this one
    float closestDistance = std::numeric_limits<float>().max();
    Vec3 closestPoint;
    bool targetFound = false;

    // get a list of all current agents
    const auto& people = agents->get_all_agents_by_type("Person");
    const auto& soldiers = agents->get_all_agents_by_type("Soldier");

    // and our agent's position
    const auto &currPos = agent->get_position();

    for (const auto & person : people)
    {
        // make sure it's not our agent
        if (person != agent)
        {
            const auto &agentPos = person->get_position();
            const float distance = Vec3::Distance(currPos, agentPos);

            if (distance < closestDistance
                && Vec3::Distance(agent->get_position(), person->get_position()) > DISTANCE)
            {
                closestDistance = distance;
                closestPoint = agentPos;
                targetFound = true;
            }
        }
    }

    for each (auto soldier in soldiers)
    {
        if (soldier != agent)
        {
            const auto& agentPos = soldier->get_position();
            const float distance = Vec3::Distance(currPos, agentPos);

            if (distance < closestDistance
                && Vec3::Distance(agent->get_position(), soldier->get_position()) > DISTANCE)
            {
                closestDistance = distance;
                closestPoint = agentPos;
                targetFound = true;
            }
        }
    }

    if (targetFound == true)
    {
        targetPoint = closestPoint;
		BehaviorNode::on_leaf_enter();
    }
    else // couldn't find a viable agent
    {
        on_failure();
    }

}

void L_MoveToClosestPerson::on_update(float dt)
{
    const auto result = agent->move_toward_point(targetPoint, dt);

    if (result == true)
    {
        on_success();
    }

    display_leaf_text();
}

