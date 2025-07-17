#ifndef CGRAPH_GPIPELINEFACTORY_H
#define CGRAPH_GPIPELINEFACTORY_H 

#include<list>
#include<mutex>

#include "GPipeline.h"
#include "../../CObject/CObject.h"
#include "../../UtilsCtrl/UtilsInclude.h"

class GPipelineFactory : public CObject { 

public:
    /**
     * 创建一个pipeline信息
     * @return
     */
    static GPipelinePtr create();

    /**
     * 销毁一个pipeline信息
     * @return
     */
    static void destroy(GPipelinePtr pipeline);

    /**
     * 清空所有的pipeline信息
     */
    static void clear();

protected:
    CSTATUS run();
    GPipelineFactory();

private:
    static GPipelinePtrList pipeline_list_;    // 记录所有的Pipeline信息
    static std::mutex lock_;                   // 独占锁保证线程安全


};


#endif 