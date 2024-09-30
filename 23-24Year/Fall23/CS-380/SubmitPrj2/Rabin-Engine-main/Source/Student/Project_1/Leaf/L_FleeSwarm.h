#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_FleeSwarm : public BaseNode<L_FleeSwarm>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 fleePoint;
    
};