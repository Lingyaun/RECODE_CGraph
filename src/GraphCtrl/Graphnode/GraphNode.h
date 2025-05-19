#ifndef CGRAPH_GRAPHNODE_H
#define CGRAPH_GRAPHNODE_H

#include <list>
#include <atomic>
#include "../../CObject/CObject.h"
#include "../../UtilsCtrl/UtilsInclude.h"

class GraphNode : public CObject {
public:
    explicit GraphNode();

    CSTATUS init() override;

    CSTATUS run() override;

    CSTATUS deinit() override;

    virtual ~GraphNode() override;

    /**
     * @brief 添加依赖
     * @return
     */
    CSTATUS addDependNode(GraphNode* node);

protected:
    /**
     * run方法执行之前的执行函数
     * @return
     */
    CSTATUS beforeRun();

    /**
    * @brief 用于节点在执行完成后的处理
    * @return 
    */
    CSTATUS afterRun();

    /**
     * 线程池中的运行函数，依次执行beforeRun，run和afterRun方法，其中有任何返回值问题，则直接返回
     * @return
     */
    CSTATUS process();

    /** 
     * 判定node是否可以运行
     * 可执行的条件为：自身未被执行且依赖节点全部被执行
     * nodiscard 表示返回值必须被判断
     * @return
     */
    [[nodiscard]]bool isRunnable() const;

private:
    std::atomic<bool> done_ {false};          // 标记被执行结束
    std::list<GraphNode *> dependence_;       // 依赖的节点
    std::list<GraphNode *> run_before_;       // 被依赖的节点
    int left_depend_;                         // 当left_cnt_值为0的时候，即可以执行该node信息
    
    /* 设置友元类，使得在Graphic和GraphThreadPool中可以获取本类的信息
     * 且外部无法继承或者修改对应函数 */
    friend class Graphic;
    friend class GraphThreadPool;
};


#endif //CGRAPH_GRAPHNODE_H