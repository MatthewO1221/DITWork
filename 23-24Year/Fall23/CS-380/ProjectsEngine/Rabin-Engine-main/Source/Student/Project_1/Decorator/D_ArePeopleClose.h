#pragma once
#include "BehaviorNode.h"

class D_ArePeopleClose : public BaseNode<D_ArePeopleClose>
{
public:
    D_ArePeopleClose();

protected:
    


    virtual void on_update(float dt) override;
};