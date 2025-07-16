#include <set>
#include "GPipeline.h"

GPipeline::GPipeline() {
    thread_pool_ = new(std::nothrow) GraphThreadPool();
    element_manager_ = new(std::nothrow) GElementManager();
    param_manager_ = new(std::nothrow) GParamManager();
    is_init_ = false;
}


GPipeline::~GPipeline() {
    CGRAPH_DELETE_PTR(thread_pool_)
    CGRAPH_DELETE_PTR(element_manager_)
    CGRAPH_DELETE_PTR(param_manager_)

    // 结束的时候，清空所有创建的节点信息。所有节点信息，仅在这一处释放
    for (GElementPtr element : element_repository_) {
        CGRAPH_DELETE_PTR(element);
    }
}


CSTATUS GPipeline::init() {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_ASSERT_NOT_NULL(thread_pool_)
    CGRAPH_ASSERT_NOT_NULL(element_manager_)
    CGRAPH_ASSERT_NOT_NULL(param_manager_)

    status = element_manager_->init();
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = true;
    CGRAPH_FUNCTION_END
}


CSTATUS GPipeline::run() {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_ASSERT_INIT(true)
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)
    CGRAPH_ASSERT_NOT_NULL(element_manager_)
    CGRAPH_ASSERT_NOT_NULL(param_manager_)

    int runElementSize = 0;
    std::vector<std::future<CSTATUS>> futures;

    for (GClusterArr& clusterArr : element_manager_->para_cluster_arrs_) {
        futures.clear();

        for (GCluster& cluster : clusterArr) {
            futures.push_back(std::move(this->thread_pool_->commit(cluster)));
            runElementSize += cluster.getElementNum();
        }

        for (auto& fut : futures) {
            status = fut.get();
            CGRAPH_FUNCTION_CHECK_STATUS
        }
    }

    param_manager_->reset();
    status = element_manager_->afterRunCheck(runElementSize);
    CGRAPH_FUNCTION_END
}


CSTATUS GPipeline::deinit() {
    CGRAPH_FUNCTION_BEGIN

    status = element_manager_->deinit();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = param_manager_->deinit();
    CGRAPH_FUNCTION_END
}

