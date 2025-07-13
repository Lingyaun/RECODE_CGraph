#ifndef CGRAPH_GPIPELINEDEFINE_H
#define CGRAPH_GPIPELINEDEFINE_H

#include <utility>
#include <vector>
#include <string>
#include <set>

#include "../GraphElement/GElementInclude.h"

/**
 * 创建GCluster和GRegion的中的GNode的时候，使用到的信息
 * 创建GCluster的时候，dependence为空
 * 创建GRegion的时候，dependence不为空
 */
struct GNodeInfo {
    std::string name;
    int loop {1} ;
    GElementPtrSet dependence;

    // 无依赖版本，适用于cluster创建
    GNodeInfo(const std::string& name = "", int loop = 1) {
        this->name = name;
        this->loop = loop;
    }

    // 有依赖版本，适用于region创建
    /**
     * @param dependence 依赖的节点
     * @param name       节点名称
     * @param loop       循环次数
     */
    GNodeInfo(const GElementPtrSet& dependence = std::initializer_list<GElementPtr>(),
              const std::string& name = "",
              int loop = 1) {
        this->name = name;
        this->loop = loop;
        this->dependence = dependence;
    }
};


#endif