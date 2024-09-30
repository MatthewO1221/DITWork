#pragma once
#include "BehaviorNode.h"

class D_CheckNumPeopleNear : public BaseNode<D_CheckNumPeopleNear>
{
public:
    D_CheckNumPeopleNear();

protected:
    

    virtual void on_update(float dt) override;
};