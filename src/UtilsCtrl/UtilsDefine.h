#ifndef CGRAPH_UTILSDEFINE_H
#define CGRAPH_UTILSDEFINE_H

#include "../CObject/CObject.h"

/* 开启函数流程 */
#define CGRAPH_FUNCTION_BEGIN           \
    CSTATUS status = STATUS_OK;         \

/* 结束函数流程 */
#define CGRAPH_FUNCTION_END             \
    return status;                      \

/* 判断传入的指针信息是否为空 */
#define CGRAPH_ASSERT_NOT_NULL(ptr)     \
    if (nullptr == (ptr)) {             \
        return STATUS_RES;              \
    }                                   \

/* 判断函数流程是否可以继续 */
#define CGRAPH_FUNCTION_CHECK_STATUS    \
    if (STATUS_OK != status) {          \
        return status;                  \
    }                                   \

/* 删除资源信息 */
#define CGRAPH_DELETE_PTR(ptr)          \
    if ((ptr) != nullptr)               \
    {                                   \
        delete (ptr);                   \
        (ptr) = nullptr;                \
    }                                   \

#define CGRAPH_ASSERT_INIT(isInit)      \
    if (isInit != is_init_) {           \
        return STATUS_ERR;              \
    }                                   \

#define CGRAPH_PROCESS_ERROR            \
    return STATUS_ERR;                  \
/* 输出锁 */
#define INFO(msg) { \
    std::lock_guard<std::mutex> lock(cout_mutex); \
    std::cout << msg << std::endl; \
}

#endif //CGRAPH_UTILSDEFINE_H