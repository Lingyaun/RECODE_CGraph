#ifndef GNODE_H
#define GNODE_H 
#include"../GElement.h"
#include "../../GraphParam/GParamManager.h"

class GNode : public GElement {

public:

protected:
    explicit GNode();
    ~GNode() override;

private:
    GNode(const GNode& node);
    GNode& operator=(const GNode& node);


    CSTATUS beforeRun() override;
    CSTATUS process(bool isMock) override;
    CSTATUS afterRun() override;


    friend class GPipeline;

};

using GNodePtr = GNode *;
using GNodePtrArr = std::vector<GNodePtr>;


#endif