#include "MyGCondition/MyCondition.h"
#include "MyGCondition/MyParamCondition.h"
#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"
#include "MyGNode/MyReadParamNode.h"
#include "MyGNode/MyWriteParamNode.h"

int tutorial_condition() {
    /* 根据MyCondition的信息， */
    CSTATUS status = STATUS_OK;
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b_condition, c, d_condition = nullptr;

    b_condition = pipeline->createGGroup<MyCondition>({
        pipeline->createGNode<MyNode1>(GNodeInfo("conditionNodeB0", 1)),
        pipeline->createGNode<MyNode2>(GNodeInfo("conditionNodeB1", 1)),
        pipeline->createGNode<MyNode1>(GNodeInfo("conditionNodeB2", 1))
    });    // 生成 b_condition。执行的时候，根据choose()的返回值，在B0,B1,B2中选择一个执行

    d_condition = pipeline->createGGroup<MyParamCondition>({
        pipeline->createGNode<MyNode1>(GNodeInfo("paramConditionNodeD0", 1)),
        pipeline->createGNode<MyNode1>(GNodeInfo("paramConditionNodeD1", 1)),
        pipeline->createGNode<MyNode1>(GNodeInfo("paramConditionNodeD2", 1))
    });

    if (nullptr == b_condition || nullptr == d_condition) {
        return status;
    }

    status = pipeline->registerGElement<MyWriteParamNode>(&a, {}, "writeNodeA", 1);
    status = pipeline->registerGElement<MyCondition>(&b_condition, {a}, "conditionB", 1);
    status = pipeline->registerGElement<MyReadParamNode>(&c, {b_condition}, "readNodeC", 1);
    status = pipeline->registerGElement<MyParamCondition>(&d_condition, {c}, "conditionD", 1);

    /* 图信息初始化，准备开始计算 */
    status = pipeline->init();

    status = pipeline->run();
    
    if (STATUS_OK != status) {
        return status;    // 使用时，请对所有CGraph接口的返回值做判定。本例子中省略
    }
    status = pipeline->deinit();
    if (STATUS_OK != status) {
        return status;    // 使用时，请对所有CGraph接口的返回值做判定。本例子中省略
    }
    GPipelineFactory::destroy(pipeline);
    return status;
}


int main() {
    std::cout<<tutorial_condition();

    return 0;
}