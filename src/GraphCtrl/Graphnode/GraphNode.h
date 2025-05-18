#ifndef CGRAPH_GRAPHNODE_H
#define CGRAPH_GRAPHNODE_H

#include <list>

#include "../../CObject/CObject.h"
#include "../../UtilsCtrl/UtilsInclude.h"

class GraphNode : public CObject {
public:
    GraphNode();

    CSTATUS init() override;

    CSTATUS run() override;

    CSTATUS deinit() override;

    ~GraphNode() override;

public:
    /**
     * 返回被依赖的对应节点
     * @param object
     * @return
     */
    std::list<GraphNode *>& getRunBeforeList();

    /**
     * @brief 添加依赖
     * @return
     */
    CSTATUS addDependNode(GraphNode* node);
    
    /**
    * @brief 用于节点在执行完成后的处理
    * @return 
    */ 
    CSTATUS afterRun();
    // 依赖的内容是否已经执行完毕
    [[nodiscard]]bool enableRun() const;
    
    // 是否已经执行完毕
    [[nodiscard]]bool isDone() const;

protected:
    CSTATUS addRunBefore(GraphNode* node);
    CSTATUS addDependence(GraphNode* node);

private:
    bool done_;    // 标记被执行结束
    std::list<GraphNode *> dependence_;       // 依赖的节点
    std::list<GraphNode *> run_before_;       // 被依赖的节点
    int left_depend_;    // 当left_cnt_值为0的时候，即可以执行该node信息
};


#endif //CGRAPH_GRAPHNODE_H