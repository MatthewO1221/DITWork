#include <pch.h>
#include "D_ArePeopleClose.h"

#define DISTANCE 10.0f

D_ArePeopleClose::D_ArePeopleClose()
{}



void D_ArePeopleClose::on_update(float dt)
{
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
		BehaviorNode* child = children.front();
		child->tick(dt);
	}
	else
	{
		on_failure();
	}
	
}
