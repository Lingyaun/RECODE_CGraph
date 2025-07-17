#include "MyGNode/MyWriteParamNode.h"
#include "MyGNode/MyReadParamNode.h"


void tutorial_param() {
    GPipelinePtr pipeline = GPipelineFactory::create();
    CSTATUS status = STATUS_OK;
    GElementPtr a, b, c, d = nullptr;

    status = pipeline->registerGElement<MyWriteParamNode>(&a, {}, "paramNodeA");    // 将名为nodeA，无执行依赖的node信息，注册入pipeline中
    if (STATUS_OK != status) {
        return;    // 使用时，请对所有CGraph接口的返回值做判定。本例子中省略
    }
    status = pipeline->registerGElement<MyReadParamNode>(&b, {a}, "paramNodeB", 9000000);    // 将名为nodeB，依赖a执行的node信息，注册入pipeline中
    status = pipeline->registerGElement<MyWriteParamNode>(&c, {a}, "paramNodeC", 9000000);
    status = pipeline->registerGElement<MyReadParamNode>(&d, {b, c}, "paramNodeD");    // 将名为nodeD，依赖{b,c}执行的node信息，注册入pipeline中

    /* 图信息初始化，准备开始计算 */
    status = pipeline->init();

    /* 运行图计算。初始化后，支持多次循环计算 */

    for (int i = 0; i < 1; i++) {
        status = pipeline->run();
    }

    /* 图信息逆初始化，准备结束计算 */
    status = pipeline->deinit();
    GPipelineFactory::destroy(pipeline);
}

int main() {
    tutorial_param();
    return 0;
}