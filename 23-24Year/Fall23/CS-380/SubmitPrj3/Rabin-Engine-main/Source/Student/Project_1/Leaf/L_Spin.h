#pragma once
#include "BehaviorNode.h"

class L_Spin : public BaseNode<L_Spin>
{
public:
    L_Spin();

protected:
  

    virtual void on_update(float dt) override;
};