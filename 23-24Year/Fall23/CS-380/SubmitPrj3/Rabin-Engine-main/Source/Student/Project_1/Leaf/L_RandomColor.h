#pragma once
#include "BehaviorNode.h"

class L_RandomColor : public BaseNode<L_RandomColor>
{
public:
    L_RandomColor();

protected:
    

    
    virtual void on_update(float dt) override;
};