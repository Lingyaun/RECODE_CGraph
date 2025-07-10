#ifndef GGRAPH_GREGION_H
#define GGRAPH_GREGION_H 

#include"../GGroup.h"
#include"../../GElementManager.h"

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
    
     CSTATUS setThreadPool(GraphThreadPoolPtr pool);
    
private:
    GElementManagerPtr manager_;
    GraphThreadPoolPtr thread_pool_;    // 这里需要使用GPipeline类的线程池

    friend class GPipeline;
};

using GRegionPtr = GRegion *;


#endif