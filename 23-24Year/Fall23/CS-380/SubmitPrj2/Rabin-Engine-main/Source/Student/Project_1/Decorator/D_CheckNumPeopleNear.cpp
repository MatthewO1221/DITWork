#include <pch.h>
#include "D_CheckNumPeopleNear.h"


#define DISTANCE 20.0f
#define NUMPEOPLEREQUIRED 1

D_CheckNumPeopleNear::D_CheckNumPeopleNear() 
{}



void D_CheckNumPeopleNear::on_update(float dt)
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

	if (numPeopleFound > NUMPEOPLEREQUIRED)
	{
		on_failure();
	}
	else
	{
		
	}
	
	
}
