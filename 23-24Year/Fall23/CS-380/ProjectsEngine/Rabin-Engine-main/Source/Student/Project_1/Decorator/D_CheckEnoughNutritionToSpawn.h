#pragma once
#include "BehaviorNode.h"

class D_CheckEnoughNutritionToSpawn : public BaseNode<D_CheckEnoughNutritionToSpawn>
{
public:
    D_CheckEnoughNutritionToSpawn();

protected:
    

    virtual void on_update(float dt) override;
};