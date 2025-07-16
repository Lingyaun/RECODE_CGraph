#ifndef CGRAPH_GELEMENT_H
#define CGRAPH_GELEMENT_H 

#include <atomic>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "../../UtilsCtrl/UtilsInclude.h"
#include "../../CObject/CObject.h"
#include "../GraphParam/GParamInclude.h"

class GElement : public CObject {
public:
    
    /* 获取name信息 */
    std::string getName() const;

    template<typename T>
    T* getGParam(const std::string& key);

    
    /**
     * 创建param信息，如果过了，则直接返回ok
     * @tparam T
     * @param key
     * @return
     */
    template<typename T>
    CSTATUS createGParam(const std::string& key);
    
protected:
    
    /* 构造函数 */
    explicit GElement();

    /* 析构函数 */
    ~GElement() override;

    /**
     * 实现拷贝构造函数
     * @param node
     */
    GElement(const GElement& node);

    /**
     * 实现赋值函数
     * @param element
     * @return
    */
    GElement& operator=(const GElement& element);

    /**
     * run方法执行之前的执行函数
     * @return
     */
    virtual CSTATUS beforeRun()=0;

    /**
     * run方法执行之后的执行函数
     * @return
     */
    virtual CSTATUS afterRun()=0;

    /**
     * 线程池中的运行函数，依次执行beforeRun，run和afterRun方法，
     * 其中有任何返回值问题，则直接返回
     * @param isMock 是否真实执行run函数。默认执行
     * @return
     */
    virtual CSTATUS process(bool isMock);

    /**
     * 设置name信息
     * @param name
     */
    void setName(const std::string& name);
    
    /**
     * 设置循环次数
     * @param loop
     */
    void setLoop(int loop=1);

    /**
     * 判定node是否可以运行
     * 可执行的条件为：自身未被执行且依赖节点全部被执行
     * nodiscard 表示返回值必须被判断
     * @return
     */
    [[nodiscard]]bool isRunnable() const;

    /**
      * 判定node是否可以和前节点链接执行
       * @return
    */
    [[nodiscard]]bool isLinkable() const;
    
    /**
     * 设置当前参数管理类信息
     * @param manager
     * @return
     */
    virtual CSTATUS setParamManager(GParamManagerPtr manager); 
    
    /**
     * 添加依赖节点信息
     * @param dependElements
     * @return
     */
    CSTATUS addDependElements(const std::set<GElement *> &dependElements);


protected:
    bool done_ { false };                     // 判定被执行结束
    bool is_init_ { false };                  // 是否初始化了
    bool linkable_ { false };                 // 判定是否可以连通计算
    int loop_ { 1 };                          // 节点执行次数
    std::string name_;                        // 节点名称
    std::string session;                      // 节点唯一session信息
    std::set<GElement *> run_before_;         // 被依赖的节点
    std::set<GElement *> dependence_;         // 依赖的节点信息
    std::atomic<int> left_depend_{ 0 };       // 当 left_depend_ 值为0的时候，即可以执行该node信息
    GParamManagerPtr param_manager_ { nullptr };     // 整体流程的参数管理类，所有pipeline中的所有节点共享
    
    friend class GNode;
    friend class GCluster;
    friend class GGroup;
    friend class GRegion;
    friend class GElementManager;
    friend class GPipeline;
    friend class GCondition;
};

using GElementPtr = GElement *;
using GElementPtrArr = std::vector<GElementPtr>;
using GElementPtrSet = std::set<GElementPtr>;


template<typename T>
inline CSTATUS GElement::createGParam(const std::string& key) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(this->param_manager_)

    status = this->param_manager_->create<T>(key);
    CGRAPH_FUNCTION_END
}


template<typename T>
inline T* GElement::getGParam(const std::string& key) {
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(this->param_manager_)

    T* ptr = dynamic_cast<T *>(this->param_manager_->get(key));
    return ptr;
}

#endif //CGRAPH_GELEMENT_H