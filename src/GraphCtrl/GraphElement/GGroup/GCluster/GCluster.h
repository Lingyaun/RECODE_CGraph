#ifndef CGRAPH_GCLUSTER_H
#define CGRAPH_GCLUSTER_H

#include <vector>
#include"../../GElement/GElement.h"

class GCluster : public GElement {
public:
    explicit GCluster();
    ~GCluster() override;
    GCluster(const GCluster& cluster);
    GCluster& operator=(const GCluster& cluster);

    CSTATUS init() override;
    CSTATUS deinit() override;
    CSTATUS process(bool isMock) override;

    CSTATUS beforeRun() override;
    CSTATUS run() override;
    CSTATUS afterRun() override;

    CSTATUS addElement(GElementPtr element);
    
    /**
     * 获取元素数量
     * @return 
     */
    int getElementNum();

    /**
     * 判断是否所有element均执行结束了
     * @return
     */
    bool isElementsDone();

private:
    GElementPtrArr cluster_elements_;    // cluster中包含的元素信息

    friend class GRegion;
};

using GClusterPtr = GCluster *;

#endif //CGRAPH_GCLUSTER_H