#ifndef GGRAPH_GGROUP_H
#define GGRAPH_GGROUP_H 

#include <vector>
#include "GGroupDefine.h"
#include "../GElement.h"

/* 所有节点组合的基类，所有节点组合功能，均继承自此类 */
class GGroup : public GElement {

public:
    virtual CSTATUS addElement(GElementPtr element) = 0;

protected:
    CSTATUS beforeRun() override;
    CSTATUS afterRun() override;
};

using GGroupPtr = GGroup *;

#endif