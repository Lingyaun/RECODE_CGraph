#include "GParamManager.h"

GParamManager::GParamManager() {
    init();
}


GParamManager::~GParamManager() {
    deinit();
}


CSTATUS GParamManager::init() {
    CGRAPH_FUNCTION_BEGIN

    params_map_.clear();

    CGRAPH_FUNCTION_END
}


CSTATUS GParamManager::run() {
    CGRAPH_NO_SUPPORT
}


CSTATUS GParamManager::deinit() {
    CGRAPH_FUNCTION_BEGIN

    for (auto& param : params_map_) {
        CGRAPH_DELETE_PTR(param.second)
    }

    params_map_.clear();
    
    CGRAPH_FUNCTION_END
}


GParamPtr GParamManager::get(const std::string& key) {
    auto result = params_map_.find(key);
    if (result == params_map_.end()) {
        return nullptr;
    }

    return params_map_.find(key)->second;;
}


void GParamManager::reset() {
    for (auto cur : params_map_) {
        if (!cur.second) {
            continue;
        }

        cur.second->reset();
    }
}