#ifndef CGRAPH_MYPARAMNODE1_H
#define CGRAPH_MYPARAMNODE1_H

#include "../../src/GraphCtrl/GraphInclude.h"
#include "../MyGParam/MyParam1.h"
class MyParamNode1 : public GNode {
public:
    CSTATUS init () {
        CSTATUS status = STATUS_OK;
        status = this->createGParam<MyParam1>("myParam1");    // 推荐在init的时候，将可能用到的参数创建好
        return status;
    }

    CSTATUS run () {
        CSTATUS status = STATUS_OK;

        MyParam1* write_param = this->getGParam<MyParam1>("myParam1");//通过哈希表获取参数
        {
            CGRAPH_PARAM_WRITE_REGION(write_param)
            std::cout << "--> write : " << ++write_param->iValue << std::endl;
        }

        return status;
    }
};

#endif