#ifndef CGRAPH_MYNODE1_H
#define CGRAPH_MYNODE1_H

#include "../../src/GraphCtrl/GraphInclude.h"

class MyNode1 : public GNode {

public:
    CSTATUS run () override {
        CSTATUS status = STATUS_OK;
        CGRAPH_ECHO("[%s], enter MyNode1 run function. Sleep for 1 second ... ", this->getName().c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return status;
    }
};

#endif