#ifndef GNODE_H
#define GNODE_H 
#include"../GElement/GElement.h"

class GNode : public GElement{
protected:

    CSTATUS beforeRun() override;

    CSTATUS afterRun() override;
    
    CSTATUS process(bool isMock) override;

    explicit GNode();
    ~GNode() override;

    GNode(const GNode& node);
    GNode& operator=(const GNode& node);


};

using GNodePtr= GNode*;
using GNodePtrArr=std::vector<GNodePtr>;

#endif