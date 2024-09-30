#pragma once
#include "BehaviorNode.h"

class L_DepositAtHive : public BaseNode<L_DepositAtHive>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 targetPoint;
    BehaviorAgent* closestHive;
};