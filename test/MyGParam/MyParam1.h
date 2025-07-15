#ifndef CGRAPH_MYPARAM1_H
#define CGRAPH_MYPARAM1_H

#include "../../src/GraphCtrl/GraphInclude.h"

struct MyParam1 : public GParam {
    /**
     * reset方法，在pipeline执行一次结束的时候被调用。
     * 如果是pipeline多次执行，并且依赖之前pipeline运行的结果（如，通过iCount值，记录pipeline执行了多少次）
     * reset中，不要实现重置参数逻辑即可
     */
     void reset() override {
        iValue = 0;
        fValue = 0.0f;
    }

    int iValue {0};
    float fValue {0.0f};
};

#endif