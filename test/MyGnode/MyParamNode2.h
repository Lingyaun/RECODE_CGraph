#ifndef CGRAPH_MYPARAMNODE2_H
#define CGRAPH_MYPARAMNODE2_H

#include "../../src/GraphCtrl/GraphInclude.h"
#include "../MyGParam/MyParam1.h"

class MyParamNode2 : public GNode {
public:
    CSTATUS run () {
        CSTATUS status = STATUS_OK;
        MyParam1* param1 = this->getGParam<MyParam1>("myParam1");

        {
            CGRAPH_PARAM_READ_REGION(param1)
            std::cout << "--> read : " << param1->iValue << std::endl;
        }

        return status;
    }
};

#endif