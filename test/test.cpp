#include <iostream>
#include <thread>
#include "../src/GraphCtrl/GraphInclude.h"

mutex cout_mutex;
class MyNode1 : public GraphNode {

public:
    CSTATUS run () override {
        int status = STATUS_OK;
        INFO("enter node1 run function. sleep for 1 second ... ");
        this_thread::sleep_for(chrono::milliseconds(1000));
        return status;
    }
private:
    mutex out;
};

class MyNode2 : public GraphNode {

public:
    CSTATUS run () override {
        CSTATUS status = STATUS_OK;
        INFO("enter node2 run function. sleep for 2 second ... ");
        this_thread::sleep_for(chrono::milliseconds(2000));
        return status;
    }
private:
    mutex out;
};

void demo() {
    /* 创建图化 */
    Graphic* graphic = new Graphic();
    CSTATUS status = STATUS_OK;
    GraphNode* a = nullptr;
    GraphNode* b = nullptr;
    GraphNode* c = nullptr;
    GraphNode* d = nullptr;

    /* 注册节点，其中MyNode1和MyNode2必须为GraphNode的子类，否则无法通过编译。
     * MyNode1中run()执行内容为sleep(1s)
     * MyNode2中run()执行内容为sleep(2s) */
    status = graphic->registerGraphNode<MyNode1>(&a);    // a节点执行，没有任何依赖信息
    if (STATUS_OK != status) {
        return;    // 使用时，请对所有CGraph接口的返回值做判定。本例子中省略
    }
    status = graphic->registerGraphNode<MyNode2>(&b, {a});    // b节点执行，需要依赖a节点执行完毕
    status = graphic->registerGraphNode<MyNode1>(&c, {a});
    status = graphic->registerGraphNode<MyNode2>(&d, {b, c});    // d节点执行，需要依赖b和c节点执行完毕
    /* 图信息初始化，准备开始计算 */
    status = graphic->init();

    /* 运行图计算。初始化后，支持多次循环计算 */
    for (int i = 0; i < 3; i++) {
        cout << "CGraph test, loop : " << i << endl;
        status = graphic->run();
    }

    /* 图信息逆初始化，准备结束计算 */
    status = graphic->deinit();
    cout<<"done"<<endl;
    delete graphic;
}

int main () {
    demo();
    return 0;
}