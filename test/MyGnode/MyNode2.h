#ifndef CGRAPH_MYNODE2_H
#define CGRAPH_MYNODE2_H

#include "../../src/GraphCtrl/GraphInclude.h"

class MyNode2 : public GNode {

public:
    CSTATUS run () override {
        CSTATUS status = STATUS_OK;
        CGRAPH_ECHO("[%s], enter MyNode2 run function. Sleep for 2 second ... ", this->getName().c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        return status;
    }
};


#endif