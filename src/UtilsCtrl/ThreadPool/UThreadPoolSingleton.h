#ifndef CGRAPH_UTHREADPOOLSINGLETON_H
#define CGRAPH_UTHREADPOOLSINGLETON_H

#include "UThreadPool.h"
#include "../Singleton/USingleton.h"


class UThreadPoolSingleton : public UThreadObject {
public:
    /**
     * 获取线程池指针内容
     * @return
     */
    static UThreadPoolPtr get();

private:
    UThreadPoolSingleton() = default;
    ~UThreadPoolSingleton() = default;

    static USingleton<UThreadPool> pool_;
};


#endif