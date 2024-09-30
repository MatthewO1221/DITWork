#include <pch.h>
#include "L_Rock.h"

#define SPINSPEED 0.1f

L_Rock::L_Rock()
{}


void L_Rock::on_update(float dt)
{

    if (agent->get_pitch() < 0.0f && agent->get_pitch() > -45.0f)
    {
        agent->set_pitch((agent->get_pitch() + SPINSPEED));
    }
    else if (agent->get_pitch() >= 0.0f && agent->get_pitch() < 45.0f)
    {
        agent->set_pitch((agent->get_pitch() - SPINSPEED));
    }
    

    on_success();

    display_leaf_text();
}
