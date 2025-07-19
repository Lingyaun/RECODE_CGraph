#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"

void tutorial_simple() {
    /* 创建图化 */
    GPipelinePtr pipeline = GPipelineFactory::create();
    CSTATUS status = STATUS_OK;//这只是一个状态量用来保证函数的正常运行
    GElementPtr a, b, c, d = nullptr;

    /* 注册节点，其中MyNode1和MyNode2必须为GNode的子类，否则无法通过编译。
     * MyNode1中run()执行内容为sleep(1s)
     * MyNode2中run()执行内容为sleep(2s) */
    status = pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");    // 将名为nodeA，无执行依赖的node信息，注册入pipeline中
    if (STATUS_OK != status) {
        return;    // 使用时，请对所有CGraph接口的返回值做判定。本例子中省略
    }
    status = pipeline->registerGElement<MyNode2>(&b, {a}, "nodeB");    // 将名为nodeB，依赖a执行的node信息，注册入pipeline中
    status = pipeline->registerGElement<MyNode1>(&c, {a}, "nodeC");
    status = pipeline->registerGElement<MyNode2>(&d, {b, c}, "nodeD");    // 将名为nodeD，依赖{b,c}执行的node信息，注册入pipeline中

    /* 图信息初始化，准备开始计算 */
    status = pipeline->init();

    /* 运行图计算。初始化后，支持多次循环计算 */
    for (int i = 0; i < 3; i++) {
        status = pipeline->run();
        std::cout << "[CGraph] tutorial_simple, loop : " << i + 1 << ", and run status = " << status << std::endl;
    }

    /* 图信息逆初始化，准备结束计算 */
    status = pipeline->deinit();
    GPipelineFactory::destroy(pipeline);
}

int main () {
    tutorial_simple();
    return 0;
}