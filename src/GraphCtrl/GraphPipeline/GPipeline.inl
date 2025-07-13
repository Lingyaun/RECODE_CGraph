#ifndef CGRAPH_GPIPELINE_INL
#define CGRAPH_GPIPELINE_INL

#include "GPipeline.h"

template<typename T>
CSTATUS GPipeline::registerGElement(GElementPtr* elementRef,
                                    const GElementPtrSet& dependElements,
                                    const std::string& name,
                                    int loop) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)

    if (element_manager_->hasElement(*elementRef)) {
        element_manager_->deleteElement(*elementRef);    // 每次注册，都默认为是新的节点
    }

    /**
     * 如果T类型是 GElement 的子类，则new T类型的对象，并且放到 element_manager_ 中去
     * 如果创建成功，则添加依赖信息。
     * 如果添加依赖信息失败，则默认创建节点失败，清空节点信息
     * */
    if (std::is_base_of<GNode, T>::value) {
        (*elementRef) = new(std::nothrow) T();
        CGRAPH_ASSERT_NOT_NULL(*elementRef)
        status = ((GNodePtr)(*elementRef))->setParamManager(this->param_manager_);
        CGRAPH_FUNCTION_CHECK_STATUS
    } else if (std::is_same_v<GCluster, T>) {
        CGRAPH_ASSERT_NOT_NULL(elementRef)
    } else if (std::is_same_v<GRegion, T>) {
        CGRAPH_ASSERT_NOT_NULL(elementRef)
    } else {
        return STATUS_ERR;
    }

    (*elementRef)->setName(name);
    (*elementRef)->setLoop(loop);
    status = addDependElements(*elementRef, dependElements);
    CGRAPH_FUNCTION_CHECK_STATUS

    status = element_manager_->addElement(dynamic_cast<GElementPtr>(*elementRef));//将节点(如cluster，Region,functionnode)转换成element类型后注册进Pipeline
    element_repository_.insert(*elementRef);
    CGRAPH_FUNCTION_END
}


template<typename T>
GElementPtr GPipeline::createGNode(const GNodeInfo& info) {
    GNodePtr node = nullptr;
    if (std::is_base_of<GNode, T>::value) {
        node = new(std::nothrow) T();
        CSTATUS status = addDependElements(node, info.dependence);
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
GElementPtr GPipeline::createGNodeS(const GElementPtrArr& elements,
                                    const GElementPtrSet& dependElements,
                                    const std::string& name,
                                    int loop) {
    // 如果不是所有的都非空，则创建失败
    if (!std::all_of(elements.begin(), elements.end(),
                     [](GElementPtr element) {
                        return (nullptr != element);
                     })) {
        return nullptr;
    }

    if (!std::all_of(dependElements.begin(), dependElements.end(),
                     [](GElementPtr element) {
                         return (nullptr != element);
                     })) {
        return nullptr;
    }

    GElementPtr ptr = nullptr;
    if (std::is_same<GCluster, T>::value) {
        ptr = new(std::nothrow) GCluster();
        CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(ptr)
        for (GElementPtr element : elements) {
            ((GCluster *)ptr)->addElement(element);
        }
    } else if (std::is_same<GRegion, T>::value) {
        CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(this->thread_pool_)
        ptr = new(std::nothrow) GRegion();
        CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(ptr)
        ((GRegion *)ptr)->setThreadPool(this->thread_pool_);
        for (GElementPtr element : elements) {
            ((GRegion *)ptr)->manager_->addElement(element);
        }
    }

    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(ptr)

    CSTATUS status = addDependElements(ptr, dependElements);
    if (STATUS_OK != status) {
        return nullptr;
    }
    ptr->setName(name);
    ptr->setLoop(loop);
    this->element_repository_.insert(ptr);
    return ptr;
}


#endif