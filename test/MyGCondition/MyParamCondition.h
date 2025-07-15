#ifndef CGRAPH_MYPARAMCONDITION_H
#define CGRAPH_MYPARAMCONDITION_H

#include "../../src/GraphCtrl/GraphInclude.h"
#include "../MyGParam/MyParam1.h"

class MyParamCondition : public GCondition {

public:
    /**
     * 在这里主要演示condition中可以通过获取上方参数的形式，
     * 来决定执行执行当前的第几个逻辑
     * @return
     */
    int choose () override {
        MyParam1* myParamCondition = this->getGParam<MyParam1>("param1");
        if (nullptr == myParamCondition) {
            return GROUP_LAST_ELEMENT_INDEX;    // 如果没获取到，固定执行最后一个逻辑
        }

        int cnt = 0;
        {
            CGRAPH_PARAM_READ_REGION(myParamCondition)    // 如果当前算子，跟其他相关依赖算子不存在并行关系，则参数可以直接使用，不需要加锁
            cnt = myParamCondition->iValue;
        }
        return (cnt % getRange());
    }
};

#endif 