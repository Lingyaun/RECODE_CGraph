#ifndef GGRAPH_GREGION_H
#define GGRAPH_GREGION_H 

#include"../GGroup.h"
#include"../../GElement/GElementManager.h"

class GRegion : public GGroup { 

    protected:
    explicit GRegion();
    ~GRegion() override;

    GRegion(const GRegion& region);
    GRegion& operator=(const GRegion& region);

    CSTATUS init() override;
    CSTATUS deinit() override;
    CSTATUS run() override;
    CSTATUS process(bool isMock) override;

    CSTATUS addElement(GElementPtr element) override;

    friend class GPipeline;
};

using GRegionPtr = GRegion *;


#endif