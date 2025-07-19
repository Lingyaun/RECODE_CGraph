#ifndef CGRAPH_UTASKWRAPPER_H
#define CGRAPH_UTASKWRAPPER_H

#include <vector>
#include <memory>

#include "../UThreadObject.h"


class UTaskWrapper : public UThreadObject {
public:
    UTaskWrapper() = default;

    UTaskWrapper(UTaskWrapper&& task) noexcept:impl_(std::move(task.impl_)) {}//移动构造函数

    UTaskWrapper &operator=(UTaskWrapper&& task) {//移动赋值操作
        impl_ = std::move(task.impl_);
        return *this;
    }

    UTaskWrapper(const UTaskWrapper &) = delete;//禁止通过常量左值对象构造新对象
    UTaskWrapper(UTaskWrapper &) = delete;//禁止通过非常量左值对象构造新对象
    UTaskWrapper &operator=(const UTaskWrapper &) = delete;//禁止通过赋值操作复制对象状态

    template<typename F>
    UTaskWrapper(F&& f) : impl_(new implType<F>(std::forward<F>(f))) {
    }

    void operator()() {
        if (impl_ != nullptr) {
            impl_->call();
        }
    }
private:
    struct implBase 
    {
        virtual void call() = 0;
        virtual ~implBase() = default;
    };

    template<typename F>
    struct implType : implBase 
    {
        F func_;
        explicit implType(F&& func) : func_(std::move(func)) {}
        void call() override { func_(); }
    };

    std::unique_ptr<implBase> impl_ = nullptr;
};

using UTaskWrapperRef = UTaskWrapper &;
using UTaskWrapperPtr = UTaskWrapper *;
using UTaskWrapperArr = std::vector<UTaskWrapper>;

#endif