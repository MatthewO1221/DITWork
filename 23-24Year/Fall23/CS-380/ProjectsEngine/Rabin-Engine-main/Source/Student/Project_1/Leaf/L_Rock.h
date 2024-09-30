#pragma once
#include "BehaviorNode.h"

class L_Rock : public BaseNode<L_Rock>
{
public:
    L_Rock();

protected:
  

    virtual void on_update(float dt) override;
};