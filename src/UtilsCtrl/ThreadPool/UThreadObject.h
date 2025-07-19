#ifndef CGRAPH_UTHREADOBJECT_H
#define CGRAPH_UTHREADOBJECT_H

#include "../UtilsDefine.h"
#include "../../CObject/CObject.h"

class UThreadObject : public CObject {

protected:
    /**
     * 部分thread中的算子，可以不实现run方法
     * @return
     */
    CSTATUS run() override {
        CGRAPH_NO_SUPPORT
    }
};

#endif