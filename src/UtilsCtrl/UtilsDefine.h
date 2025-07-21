#ifndef CGRAPH_UTILSDEFINE_H
#define CGRAPH_UTILSDEFINE_H

#include<iostream>
#include <ctime>
#include <string>
#include <cstdarg>

#include <thread>
#include <shared_mutex>
#include <mutex>

#include "../CObject/CObject.h"
//这里删除了线程池的引入
#include "ThreadPool/UThreadPoolDefine.h"

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

    std::lock_guard<std::mutex> lock{ g_echo_mtx };//在当前作用域内创建 lock 对象时，立即锁定 g_echo_mtx，当 lock 离开作用域时（如函数返回），自动释放锁，无需手动调用 unlock()。


#ifndef _WIN32
    // 非windows系统，打印到毫秒
    auto now = std::chrono::system_clock::now();
    //通过不同精度获取相差的毫秒数
    uint64_t disMs = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()
                      - std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() * 1000;
    time_t tt = std::chrono::system_clock::to_time_t(now);
    auto time_tm = localtime(&tt);
    char strTime[32] = { 0 };
    sprintf(strTime, " [%d-%02d-%02d %02d:%02d:%02d.%03d]", time_tm->tm_year + 1900,
            time_tm->tm_mon + 1, time_tm->tm_mday, time_tm->tm_hour,
            time_tm->tm_min, time_tm->tm_sec, (int)disMs);
    std::cout << "[CGraph]" << strTime << " ";
#else
    // windows系统，打印到秒
    time_t cur_time = time(nullptr);
    char time_buffer[32];
    ctime_s(time_buffer,sizeof(time_buffer),&cur_time);
    std::string ct(time_buffer);
    ct.pop_back();// 去掉时间的最后一位\n信息
        std::cout << "[cgraph] ["
                  << ct  
                  << "] ";
#endif


    va_list args;
    va_start(args, cmd);
    vprintf(cmd, args);
    va_end(args);
    std::cout << "\n";
}

//用于优化高频执行路径的代码布局，减少分支预测失败导致的流水线停滞。
#define likely(x)   __builtin_expect(!!(x), 1)//明确告诉编译器，条件 x 很可能为真
#define unlikely(x) __builtin_expect(!!(x), 0)//明确告诉编译器，条件 x 很可能为假

template<typename T>
inline T* SafeMallocCObject() {
    T* ptr = nullptr;
    while (!ptr && std::is_base_of_v<CObject, T>) {
        ptr = new(std::nothrow) T();
    }
    return ptr;
}


/* 创建CObject对象 */
#define CGRAPH_SAFE_MALLOC_COBJECT(TYPE)             \
    SafeMallocCObject<TYPE>();                       \


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


    /* 判断初始化状态*/
#define CGRAPH_ASSERT_INIT(isInit)      \
    if (isInit != is_init_) {           \
        return STATUS_ERR;              \
    }                                   \


    /* 处理错误*/
#define CGRAPH_PROCESS_ERROR            \
    return STATUS_ERR;                  \

    
/* 不支持 */
#define CGRAPH_NO_SUPPORT                           \
    return STATUS_ERR;                              \


/* 写入参数锁*/
#define CGRAPH_PARAM_WRITE_REGION(param)            \
    CGRAPH_WRITE_LOCK paramWLock((param)->_param_shared_lock_);     \


/* 读取参数锁*/
#define CGRAPH_PARAM_READ_REGION(param)             \
    CGRAPH_READ_LOCK paramRLock((param)->_param_shared_lock_);      \

    
/* 判断传入的指针信息是否为空*/
#define CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(ptr)     \
    if (nullptr == (ptr)) {                         \
        return nullptr;                             \
    }                                               \


/* 线程休眠毫秒 */
#define CGRAPH_SLEEP_MILLISECOND(ms)                                    \
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));         \


/* 线程休眠秒 */
#define CGRAPH_SLEEP_SECOND(s)                                          \
    std::this_thread::sleep_for(std::chrono::seconds(s));               \

    
#endif //CGRAPH_UTILSDEFINE_H