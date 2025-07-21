#include "UThreadPoolSingleton.h"

USingleton<UThreadPool> UThreadPoolSingleton::pool_;

UThreadPoolPtr UThreadPoolSingleton::get() {
    UThreadPoolPtr ptr = UThreadPoolSingleton::pool_.get();
    return ptr;
}