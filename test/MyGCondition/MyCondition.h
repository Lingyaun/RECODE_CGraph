#ifndef CGRAPH_MYCONDITION_H
#define CGRAPH_MYCONDITION_H

#include "../../src/GraphCtrl/GraphInclude.h"

class MyCondition : public GCondition {

public:
    /**
     * 表示在condition中，需要执行第几个。
     * 0表示第0个；1表示第1个；以此类推
     * -1 表示最后一个
     * @return
     */
    int choose () override {
        return 1;
    }
};

#endif