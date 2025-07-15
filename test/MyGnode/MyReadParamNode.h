#ifndef CGRAPH_MyReadParamNode_H
#define CGRAPH_MyReadParamNode_H

#include "../../src/GraphCtrl/GraphInclude.h"
#include "../MyGParam/MyParam1.h"

class MyReadParamNode : public GNode {
public:
    CSTATUS run () {
        CSTATUS status = STATUS_OK;
        MyParam1* read_param = this->getGParam<MyParam1>("myParam1");//通过哈希表获取参数

        {
            CGRAPH_PARAM_READ_REGION(read_param)
            std::cout << "--> read : " << read_param->iValue << std::endl;
        }

        return status;
    }
};

#endif