#include <pch.h>
#include "Projects/ProjectOne.h"
#include "Agent/CameraAgent.h"
#include <random>

#define INITPEOPLE 20
#define INITSOLDIER 10
#define INITSWARMLINGS 50

void ProjectOne::setup()
{
    // Create your inital agents
    BehaviorAgent* newHive = agents->create_behavior_agent("Hive", BehaviorTreeTypes::HiveTree);
    newHive->set_color(Vec3(255, 0, 0));
    newHive->set_position(Vec3(50, 0, 50));
    newHive->get_blackboard().set_value("Nutrition", 0.0f);
    newHive->get_blackboard().set_value("SpawnRequirement", 20.0f);
    

    for (int i = 0; i < INITPEOPLE; i++)
    {
        BehaviorAgent* newPerson = agents->create_behavior_agent("Person", BehaviorTreeTypes::PersonTree);

        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> posRand(0.0f, 100.0f);
        std::uniform_real_distribution<float> angleRand(0.0f, 360.0f);
        
        Vec3 randPos = { posRand(gen), 0.0f, posRand(gen) };
        newPerson->set_position(randPos);
        newPerson->set_yaw(angleRand(gen));
        newPerson->set_scaling(0.5f);
    }

    for (int i = 0; i < INITSOLDIER; i++)
    {
        BehaviorAgent* newPerson = agents->create_behavior_agent("Soldier", BehaviorTreeTypes::SoldierTree);

        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> posRand(0.0f, 100.0f);
        std::uniform_real_distribution<float> angleRand(0.0f, 360.0f);

        Vec3 randPos = { posRand(gen), 0.0f, posRand(gen) };
        newPerson->set_position(randPos);
        newPerson->set_yaw(angleRand(gen));
        newPerson->set_color(Vec3(0, 255, 0));
        newPerson->set_scaling(0.5f);
    }

    for (int i = 0; i < INITSWARMLINGS; i++)
    {
        BehaviorAgent* newPerson = agents->create_behavior_agent("Swarmling", BehaviorTreeTypes::SwarmTree);

        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> posRand(0.0f, 20.0f);
        std::uniform_real_distribution<float> angleRand(0.0f, 360.0f);

        Vec3 randPos = { posRand(gen), 0.0f, posRand(gen) };
        newPerson->set_position(randPos);
        newPerson->set_yaw(angleRand(gen));
        newPerson->set_color(Vec3(0, 0, 0));
        newPerson->set_scaling(0.1f);
        newPerson->get_blackboard().set_value("Nutrition", 0.0f);
    }

    // you can technically load any map you want, even create your own map file,
    // but behavior agents won't actually avoid walls or anything special, unless you code that yourself
    // that's the realm of project 2 though
    terrain->goto_map(0);

    // you can also enable the pathing layer and set grid square colors as you see fit
    // works best with map 0, the completely blank map
    terrain->pathLayer.set_enabled(true);
    terrain->pathLayer.set_value(0, 0, Colors::Red);

    // camera position can be modified from this default as well
    auto camera = agents->get_camera_agent();
    camera->set_position(Vec3(-62.0f, 70.0f, terrain->mapSizeInWorld * 0.5f));
    camera->set_pitch(0.610865); // 35 degrees

    audioManager->SetVolume(0.5f);
    audioManager->PlaySoundEffect(L"Assets\\Audio\\retro.wav");
    // uncomment for example on playing music in the engine (must be .wav)
    // audioManager->PlayMusic(L"Assets\\Audio\\motivate.wav");
    // audioManager->PauseMusic(...);
    // audioManager->ResumeMusic(...);
    // audioManager->StopMusic(...);
}