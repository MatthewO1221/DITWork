#pragma once
#include "BehaviorNode.h"

class D_CheckNutrition : public BaseNode<D_CheckNutrition>
{
public:
    D_CheckNutrition();

protected:
    

    virtual void on_update(float dt) override;
};