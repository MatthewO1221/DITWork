#include <pch.h>
#include "L_RandomColor.h"

L_RandomColor::L_RandomColor() 
{}


void L_RandomColor::on_update(float dt)
{
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> posRand(0.0f, 255.0f);

    agent->set_color(Vec3(posRand(gen), posRand(gen), posRand(gen)));

    on_success();

    display_leaf_text();
}
