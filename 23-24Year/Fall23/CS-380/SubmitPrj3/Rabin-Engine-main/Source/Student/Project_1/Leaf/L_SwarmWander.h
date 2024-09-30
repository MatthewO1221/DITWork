#pragma once
#include "BehaviorNode.h"

class L_SwarmWander : public BaseNode<L_SwarmWander>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    
    std::vector<BehaviorAgent*> nearbySwarmlings;
};