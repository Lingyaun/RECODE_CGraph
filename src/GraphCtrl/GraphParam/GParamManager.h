#ifndef GPARAM_MANAGER_H
#define GPARAM_MANAGER_H 

#include <unordered_map>
#include <string>


#include "../../CObject/CObject.h"
#include "../../UtilsCtrl/UtilsInclude.h"
#include "GParam.h"
#include"../GraphThread/GraphThreadPool.h"

class GParamManager : public CObject
{
public:
    /**
     * 创建一个特定类型的参数
     * @tparam T
     * @param key
     * @return
     */
    template<typename T>
    CSTATUS create(const std::string& key);

    /**
     * 获取一个特定类型的参数
     * @param key
     * @return
     */
    GParamPtr get(const std::string& key);


protected:
    explicit GParamManager();
    virtual ~GParamManager() override;
    CSTATUS run() override;
    CSTATUS init() override;
    CSTATUS deinit() override;
    void reset();

    
private:

    /**
     * 记录param信息的hash表
     * @param string first
     * @param GParam second
     */
	std::unordered_map<std::string, GParam*> params_map_;
    std::shared_mutex lock_;                                          // 读写锁
    bool is_init_;                                                    // 标记是否初始化结束

    friend class GPipeline;
};

using GParamManagerPtr = GParamManager *;

#include "GParamManager.inl"

#endif