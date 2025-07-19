#ifndef CGRAPH_UTHREAD_BASE_H
#define CGRAPH_UTHREAD_BASE_H 

#include <thread>

#include "../UThreadObject.h"
#include "../AtomicQueue\UAtomicQueue.h"
#include "../WorkStealingQueue/UWorkStealingQueue.h"

class UThreadBase : public UThreadObject { 
protected:
    explicit UThreadBase(){
        done_ = false;
        is_init_ = false;
        is_running_ = false;
        pool_task_queue_ =nullptr;
    }
    ~UThreadBase(){
        deinit();

    }
    /**
     * 所有线程类的deinit函数应该是一样的
     * 但是init函数不一样，因为线程构造函数不同
     * @return
     */
    CSTATUS deinit() override {
        CGRAPH_FUNCTION_BEGIN

        CGRAPH_ASSERT_INIT(true)

        done_ = false;
        if (thread_.joinable()) {
            thread_.join();    // 阻塞等待线程结束
        }
        is_init_ = false;
        is_running_ = false;

        CGRAPH_FUNCTION_END
    }
/**
     * 从线程池的队列中，获取任务
     * @param task
     * @return
     */
    virtual bool popPoolTask(UTaskWrapperRef task) {
        return (pool_task_queue_ && pool_task_queue_->tryPop(task));
    }


    /**
     * 从线程池的队列中中，获取批量任务
     * @param tasks
     * @return
     */
    virtual bool popPoolTasks(UTaskWrapperArr& tasks) {
        return (pool_task_queue_ && pool_task_queue_->tryMultiPop(tasks));
    }

private:
    bool done_;                                               // 线程状态标记
    bool is_init_;                                            // 标记初始化状态
    bool is_running_;                                         // 是否正在执行

    UAtomicQueue<UTaskWrapper>* pool_task_queue_{};           // 用于存放线程池中的普通任务
    std::thread thread_;                                      // 线程类

};



#endif