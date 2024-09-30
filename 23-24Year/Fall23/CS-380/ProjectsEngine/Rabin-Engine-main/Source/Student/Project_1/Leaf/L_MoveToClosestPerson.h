#pragma once
#include "BehaviorNode.h"

class L_MoveToClosestPerson : public BaseNode<L_MoveToClosestPerson>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

private:
    Vec3 targetPoint;
};