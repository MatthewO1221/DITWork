#pragma once
#include "BehaviorNode.h"

class D_CheckSwarmDist : public BaseNode<D_CheckSwarmDist>
{
public:
    D_CheckSwarmDist();

protected:
    

    virtual void on_update(float dt) override;
};