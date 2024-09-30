#include <pch.h>
#include "L_Spin.h"

#define SPINSPEED 20.0f

L_Spin::L_Spin()
{}


void L_Spin::on_update(float dt)
{
    agent->set_yaw((agent->get_yaw() + SPINSPEED));

    on_success();

    display_leaf_text();
}
