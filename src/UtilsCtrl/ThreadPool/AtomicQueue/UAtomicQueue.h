#ifndef CGRAPH_UATOMIC_QUEUE_H
#define CGRAPH_UATOMIC_QUEUE_H

#include <memory>
#include<mutex>
#include<queue>
#include<condition_variable>

#include "../UThreadPoolDefine.h"
#include "../UThreadObject.h"


template<typename T>
class UAtomicQueue : public UThreadObject {
public:
    UAtomicQueue() = default;

    /**
     * 等待弹出
     * @param value
     */
    void waitPop(T& value) {
        CGRAPH_LOCK_GUARD lk(mutex_);
        cv_.wait(lk, [this] { return !queue_.empty(); });//线程会主动阻塞自己直到其他线程通过 cv_.notify_one() 唤醒它，不会占用 CPU 资源轮询队列。
        value = std::move(*queue_.front());
        queue_.pop();
    }


    /**
     * 尝试弹出
     * @param value
     * @return
     */
    bool tryPop(T& value) {
        CGRAPH_LOCK_GUARD lk(mutex_);
        if (queue_.empty()) {
            return false;
        }
        value = std::move(*queue_.front());
        queue_.pop();
        return true;
    }


    /**
     * 尝试弹出多个任务
     * @param values
     * @return
     */
    bool tryMultiPop(std::vector<T>& values) {
        CGRAPH_LOCK_GUARD lk(mutex_);
        if (queue_.empty()) {
            return false;
        }

        int i = CGRAPH_MAX_POOL_BATCH_SIZE;
        while (!queue_.empty() && i--) {
            values.emplace_back(std::move(*queue_.front()));
            queue_.pop();
        }

        return true;
    }


    std::unique_ptr<T> waitPop() {
        CGRAPH_LOCK_GUARD lk(mutex_);
        cv_.wait(lk, [this] { return !queue_.empty(); });
        std::unique_ptr<T> result = queue_.front();//这里保留先试试拷贝构造，
        queue_.pop();
        return result;
    }


    std::unique_ptr<T> tryPop() {
        CGRAPH_LOCK_GUARD lk(mutex_);
        if (queue_.empty()) {
            return std::unique_ptr<T>();//因为需要返回值所以返回一个空指针
        }

        std::unique_ptr<T> res = std::move(queue_.front());
        queue_.pop();
        return res;
    }


    /**
     * 传入数据
     * @param value
     */
    void push(T&& value) {
        std::unique_ptr<T> task(std::make_unique<T>(std::move(value)));
        CGRAPH_LOCK_GUARD lk(mutex_);
        queue_.push(std::move(task));
        cv_.notify_one();
    }

    bool empty() {
        CGRAPH_LOCK_GUARD lk(mutex_);
        return  queue_.empty();
    }

private:
    std::mutex mutex_;
    std::queue<std::unique_ptr<T>> queue_;
    std::condition_variable cv_;

};


#endif
