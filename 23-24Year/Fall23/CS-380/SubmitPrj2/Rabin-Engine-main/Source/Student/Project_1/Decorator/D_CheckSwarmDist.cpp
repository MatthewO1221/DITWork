#include <pch.h>
#include "D_CheckSwarmDist.h"


#define DISTANCE 20.0f

D_CheckSwarmDist::D_CheckSwarmDist()
{}



void D_CheckSwarmDist::on_update(float dt)
{
    auto& swarmlings = agents->get_all_agents_by_type("Swarmling");

	bool swarmlingFound = false;

	const auto& pos = agent->get_position();

	BehaviorNode* child = children.front();

	for each (auto swarmling in swarmlings)
	{
		if (Vec3::Distance(pos, swarmling->get_position()) <= DISTANCE)
		{
			

			child->tick(dt);

			if (swarmlingFound == false)
			{
				swarmlingFound = true;
			}

			break;
		}

		
	}

	if (swarmlingFound == false)
	{
		on_failure();
	}
	else
	{
		set_status(child->get_status());
		set_result(child->get_result());
	}
	
}
