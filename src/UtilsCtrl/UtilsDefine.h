#ifndef CGRAPH_UTILSDEFINE_H
#define CGRAPH_UTILSDEFINE_H

#include "../CObject/CObject.h"
#include<iostream>
#include <shared_mutex>
#include <mutex>
#include <ctime>
#include <string>
#include <cstdarg>
#include <thread>

using CGRAPH_READ_LOCK = std::shared_lock<std::shared_mutex>;
using CGRAPH_WRITE_LOCK = std::unique_lock<std::shared_mutex>;
using CGRAPH_LOCK_GUARD = std::lock_guard<std::mutex>;
using CGRAPH_UNIQUE_LOCK = std::unique_lock<std::mutex>;

static std::mutex g_check_status_mtx;
static std::mutex g_echo_mtx;

inline void CGRAPH_ECHO(const char *cmd, ...) {
#ifdef _CGRAPH_SILENCE_
    return;
#endif

    std::lock_guard<std::mutex> lock{ g_echo_mtx };
#ifndef _WIN32
    // 非windows系统，打印到毫秒
    timeb cur_time{};
    char timeInfo[32] = {0};

    ftime(&cur_time);
    tm *ptm = localtime(&cur_time.time);
    sprintf(timeInfo, " [%04d-%02d-%02d %02d:%02d:%02d.%03d] ",
            ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday,
            ptm->tm_hour, ptm->tm_min, ptm->tm_sec, cur_time.millitm);
    std::cout << "[CGraph]" << timeInfo;
#else
    // windows系统，打印到秒
    time_t cur_time = time(nullptr);
        std::string ct = ctime(&cur_time);
        std::cout << "[cgraph] ["
                  << ct.assign(ct.begin(), ct.end()-1)    // 去掉时间的最后一位\n信息
                  << "] ";
#endif

    va_list args;
    va_start(args, cmd);
    vprintf(cmd, args);
    va_end(args);
    std::cout << "\n";
}

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

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
#define INFO(msg) {                                 \
    std::lock_guard<std::mutex> lock(cout_mutex);   \
    std::cout << msg << std::endl;                  \
}
/* 不支持 */
#define CGRAPH_NO_SUPPORT                           \
    return STATUS_ERR;                              \


#define CGRAPH_PARAM_WRITE_REGION(param)            \
    CGRAPH_WRITE_LOCK paramWLock(param->lock_);     \

#define CGRAPH_PARAM_READ_REGION(param)             \
    CGRAPH_READ_LOCK paramRLock(param->lock_);      \

#define CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(ptr)     \
    if (nullptr == (ptr)) {                         \
        return nullptr;                             \
    }                                               \



    
#endif //CGRAPH_UTILSDEFINE_H