#ifndef CGRAPH_GELEMENT_INL
#define CGRAPH_GELEMENT_INL

#include "GElement.h"
template<typename T>
CSTATUS GElement::createGParam(const std::string& key) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(this->param_manager_)

    status = this->param_manager_->create<T>(key);
    CGRAPH_FUNCTION_END
}


template<typename T>
T* GElement::getGParam(const std::string& key) {
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(this->param_manager_)

    T* ptr = dynamic_cast<T *>(this->param_manager_->get(key));
    return ptr;
}


#endif