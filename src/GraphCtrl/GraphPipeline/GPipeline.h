#ifndef CGRAPH_GPIPELINE_H
#define CGRAPH_GPIPELINE_H

#include <algorithm>
#include <vector>
#include <memory>
#include <list>


#include "../../CObject/CObject.h"
#include "../../UtilsCtrl/UtilsInclude.h"
#include "GPipelineDefine.h"


class GPipeline : public CObject {

public:
    explicit GPipeline();
    ~GPipeline() override;

    CSTATUS init() override;
    CSTATUS run() override;
    CSTATUS deinit() override;
    CSTATUS process(int runTimes = 1);
    /**
     * 根据传入的info信息，创建node节点
     * @tparam T
     * @param info
     * @return
     */
    template<typename T>
    GElementPtr createGNode(const GNodeInfo& info);

    /**
     * 根据传入的信息，创建节点集合（包含cluster和region）
     * @tparam T
     * @param elements
     * @param dependElements
     * @param name
     * @param loop
     * @return
     */
    template<typename T>
    GElementPtr createGGroup(const GElementPtrArr& elements,
                             const GElementPtrSet& dependElements = std::initializer_list<GElementPtr>(),
                             const std::string& name = "",
                             int loop = 1);

    /**
     * 在图中注册一个Element信息
     * @tparam T
     * @return
     */
    template<typename T>
    CSTATUS registerGElement(GElementPtr* elementRef,
                             const GElementPtrSet& dependElements = std::initializer_list<GElementPtr>(),
                             const std::string& name = "",
                             int loop = 1);

protected:



private:
    bool is_init_;                                          // 标志位
    GElementManagerPtr element_manager_;                    // 节点管理类（管理所有注册过的element信息）
    GraphThreadPoolPtr thread_pool_;                        // 线程池类
    GElementPtrSet element_repository_;                     // 标记创建的所有节点，最终释放使用
    GParamManagerPtr param_manager_;                        // 参数管理类
};

using GPipelinePtr = GPipeline *;
using GPipelinePtrList = std::list<GPipelinePtr>;



template<typename T>
inline CSTATUS GPipeline::registerGElement(GElementPtr* elementRef,
                                    const GElementPtrSet& dependElements,
                                    const std::string& name,
                                    int loop) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)

    if (element_manager_->hasElement(*elementRef)) {
        element_manager_->deleteElement(*elementRef);    // 每次注册，都默认为是新的节点
    }

/**
     * 如果是GNode类型，则直接创建节点
     * 如果不是GNode类型，则需要外部创建好，然后注册进来
     * */
    if (std::is_base_of<GNode, T>::value) {
        (*elementRef) = new(std::nothrow) T();
    }
    else if (std::is_base_of_v<GGroup, T>) {
        /**
         * 如果是GGroup类型的信息，则：
         * 1，必须外部创建
         * 2，未被注册到其他的pipeline中
         */
        if ((*elementRef) != nullptr
            && (*elementRef)->param_manager_ != nullptr) {
            CGRAPH_ECHO("This group has already been registered to another pipeline.");
            return STATUS_ERR;
        }
    }
    CGRAPH_ASSERT_NOT_NULL(*elementRef)
    status = (*elementRef)->setParamManager(this->param_manager_);
    CGRAPH_FUNCTION_CHECK_STATUS


    (*elementRef)->setName(name);
    (*elementRef)->setLoop(loop);
    status = (*elementRef)->addDependElements(dependElements);
    CGRAPH_FUNCTION_CHECK_STATUS

    status = element_manager_->addElement(dynamic_cast<GElementPtr>(*elementRef));//将节点(如cluster，Region,functionnode)转换成element类型后注册进Pipeline
    CGRAPH_FUNCTION_CHECK_STATUS
    element_repository_.insert(*elementRef);
    CGRAPH_FUNCTION_END
}


template<typename T>
inline GElementPtr GPipeline::createGNode(const GNodeInfo& info) {
    GNodePtr node = nullptr;
    if (std::is_base_of<GNode, T>::value) {
        node = new(std::nothrow) T();
        CSTATUS status = node->addDependElements( info.dependence);
        if (STATUS_OK != status) {
            return nullptr;
        }
        node->setName(info.name);
        node->setLoop(info.loop);
        node->setParamManager(this->param_manager_);    // 设置参数信息类
        element_repository_.insert(node);
    }

    return node;
}


template<typename T>
/**
 * @param elements 创建的元素
 * @param dependElements 依赖元素
 * @param name 元素名称
 * @param loop 循环次数
 */
inline GElementPtr GPipeline::createGGroup(const GElementPtrArr& elements,
                                    const GElementPtrSet& dependElements,
                                    const std::string& name,
                                    int loop) {
    // 如果不是所有的都非空，则创建失败
    if (std::any_of(elements.begin(), elements.end(),
                     [](GElementPtr element) {
                        return (nullptr == element);
                     })) {
        return nullptr;
    }

    if (std::any_of(dependElements.begin(), dependElements.end(),
                     [](GElementPtr element) {
                         return (nullptr == element);
                     })) {
        return nullptr;
    }
    
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(this->thread_pool_)    // 所有的pipeline必须有线程池
    GGroupPtr group = new(std::nothrow) T();
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(group)
    for (GElementPtr element : elements) {
        group->addElement(element);
    }

    if (std::is_same<GRegion, T>::value) {
        // 如果是GRegion类型，需要设置线程池信息。因为GRegion内部可能需要并行
        ((GRegion *)group)->setThreadPool(this->thread_pool_);
    }

    CSTATUS status = group->addDependElements(dependElements);
    if (STATUS_OK != status) {
        CGRAPH_DELETE_PTR(group)
        return nullptr;
    }
    group->setName(name);
    group->setLoop(loop);
    this->element_repository_.insert(group);
    return group;
}

#endif