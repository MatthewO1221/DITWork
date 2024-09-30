#pragma once
#include "BehaviorNode.h"

class L_SwarmAttack : public BaseNode<L_SwarmAttack>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    float nearDist;
    float avoidFactor;
    float alignFactor;
    float attractFactor;
    float tetherFactor;
    float attackFactor;
    float tetherDist;
    float moveSpeed;
    Vec3 targetPoint;
    std::vector<BehaviorAgent*> nearbySwarmlings;
};