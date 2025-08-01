#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"


void tutorial_pipeline_1(GPipelinePtr pipeline_1) {
    if (nullptr == pipeline_1) {
        return;
    }

    CSTATUS status = STATUS_OK;
    GElementPtr node1A, node1B, node1C = nullptr;

    /**
     * 构造 pipeline_1 需要的信息，并执行
     * 使用时，请注意对返回值做判定
     */
    status = pipeline_1->registerGElement<MyNode1>(&node1A, {}, "node1A");
    status = pipeline_1->registerGElement<MyNode1>(&node1B, {node1A}, "node1B");
    status = pipeline_1->registerGElement<MyNode1>(&node1C, {node1B}, "node1C");

    pipeline_1->process(5);    // 执行n次，本例中 n=5
}


void tutorial_pipeline_2(GPipelinePtr pipeline_2) {
    if (nullptr == pipeline_2) {
        return;
    }

    CSTATUS status = STATUS_OK;
    GElementPtr node2A, node2B, node2C = nullptr;

    status = pipeline_2->registerGElement<MyNode2>(&node2A, {}, "node2A");
    status = pipeline_2->registerGElement<MyNode2>(&node2B, {}, "node2B");
    status = pipeline_2->registerGElement<MyNode2>(&node2C, {}, "node2C");

    pipeline_2->process(3);
}


void tutorial_pipeline_3(GPipelinePtr pipeline_3) {
    if (nullptr == pipeline_3) {
        return;
    }

    CSTATUS status = STATUS_OK;
    GElementPtr node3A, node3B, node3C, node3D = nullptr;
    GElementPtr region = nullptr;

    node3A = pipeline_3->createGNode<MyNode1>(GNodeInfo({}, "node3A", 1));
    node3B = pipeline_3->createGNode<MyNode2>(GNodeInfo({node3A}, "node3B", 1));
    node3C = pipeline_3->createGNode<MyNode1>(GNodeInfo({node3A}, "node3C", 1));
    node3D = pipeline_3->createGNode<MyNode1>(GNodeInfo({node3B, node3C}, "node3D", 1));

    region = pipeline_3->createGGroup<GRegion>({node3A, node3B, node3C, node3D});

    status = pipeline_3->registerGElement<GRegion>(&region);
    pipeline_3->process(2);
}


void tutorial_multi_pipeline() {
    /**
     * 通过多线程，同时构造3个pipeline，并执行
     * CGraph底层，pipeline参数系统相互独立，线程池资源共享
     */
    GPipelinePtr pipeline_1 = GPipelineFactory::create();
    GPipelinePtr pipeline_2 = GPipelineFactory::create();
    GPipelinePtr pipeline_3 = GPipelineFactory::create();

    std::thread thd1 = std::move(std::thread(tutorial_pipeline_1, pipeline_1));
    std::thread thd2 = std::move(std::thread(tutorial_pipeline_2, pipeline_2));
    std::thread thd3 = std::move(std::thread(tutorial_pipeline_3, pipeline_3));

    thd1.join();
    thd2.join();
    thd3.join();

    GPipelineFactory::clear();
}


int main() {
    tutorial_multi_pipeline();
    return 0;
}