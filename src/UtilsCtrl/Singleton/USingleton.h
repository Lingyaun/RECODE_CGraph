#ifndef CGRAPH_USINGLETON_H
#define CGRAPH_USINGLETON_H

#include <shared_mutex>

#include "../UtilsDefine.h"
#include "../../CObject/CObject.h"

template<typename T>
class USingleton : public CObject {
public:
    explicit USingleton() {
        create();
    }

    ~USingleton() override {
        destroy();
    }

    T* get() {
        CGRAPH_LOCK_GUARD lock(lock_);
        T* handle = handle_;
        return handle;
    }

protected:
    CSTATUS create() {
        CGRAPH_FUNCTION_BEGIN
        if (nullptr == handle_) {
            CGRAPH_LOCK_GUARD lock(lock_);
            if (nullptr == handle_) {
                handle_ = CGRAPH_SAFE_MALLOC_COBJECT(T);
                CGRAPH_ASSERT_NOT_NULL(handle_)
            }
        }

        CGRAPH_FUNCTION_END
    }

    CSTATUS run() override {
        CGRAPH_NO_SUPPORT
    }

    CSTATUS destroy() {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_LOCK_GUARD lock(lock_);
        CGRAPH_DELETE_PTR(handle_)
        CGRAPH_FUNCTION_END
    }

private:
    T* handle_ { nullptr };
    std::mutex lock_;
};


#endif