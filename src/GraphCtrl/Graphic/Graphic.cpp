#include "Graphic.h"

Graphic::~Graphic() = default;

Graphic::Graphic() = default;

CSTATUS Graphic::init() {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_FUNCTION_END
}

CSTATUS Graphic::deinit() {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_FUNCTION_END
}

CSTATUS Graphic::addNode(GraphNode* node) {
    CGRAPH_FUNCTION_BEGIN

    this->nodes_.push_back(node);

    CGRAPH_FUNCTION_END
}

CSTATUS Graphic::run() {
    CGRAPH_FUNCTION_BEGIN

    int workedSize = 0;
    for (GraphNode* node : this->nodes_) {
        if (!node->enableRun()) {
            continue;    // 如果暂时无法执行，则继续
        }

        que_.push(node);
    }

    while (!que_.empty()) {
        GraphNode* node = que_.front();
        que_.pop();
        workedSize++;
        node->run();    // 到时候开线程池做掉这个
        node->afterRun();

        for (GraphNode* cur : node->getRunBeforeList()) {
            // 仅从和node有关的节点中寻找可以执行的节点信息
            if (!cur->isDone() && cur->enableRun()) {
                que_.push(cur);
            }
        }
    }

    if (workedSize != nodes_.size()) {
        status = STATUS_ERR;    // 判定是否是所有的节点均被执行
    }

    CGRAPH_FUNCTION_END
}