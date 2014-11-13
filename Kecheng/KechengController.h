//
//  KechengController.h
//  Blackcat
//
//  Created by haojunhua on 14-6-3.
//
//

#ifndef __Blackcat__KechengController__
#define __Blackcat__KechengController__

#include "Kecheng.h"
#include "Hanzi.h"
#include <vector>
using namespace std;

#define S_KC KechengController::getInstance()

class KechengController {
public:
    KechengController();
    ~KechengController();
    static KechengController* getInstance();
    static void purgeInstance();
    
    /**
     * 修改kcid，并根据kcid生成m_hanziVector,
     * 返回是否刷新标记，如果kcid未变则返回false
     */
    bool changeKecheng(int kcid);
    
    /**
     * 完成任务后进行数据更新
     * 根据当前情况判断，如果任务是第一次被完成，返回true
     * 否则返回false
     */
    bool updateDataWhenCompleteKecheng();
    
    bool insertKechengIfNotExists();
    
    bool updateKecheng(bool isWin);
    
    
    void disOrderHanzi();//打乱顺序
    void reOrderHanzi();//重新排序
    
private:
    /**
     * 刷新m_kcid、m_lastKechengId、m_dayRenwuCount等数据,并根据m_kcid生成m_hanziVector
     */
    void initData();
    
    /**
     * 获取当天任务数量
     */
    int fetchDayRenwuCount();
    
protected:
    CC_SYNTHESIZE_READONLY(int, m_kcid, Kcid);//当前kcid
    
    CC_SYNTHESIZE_READONLY(int, m_kcCount, KcCount);//课程总数量
    
    CC_SYNTHESIZE_READONLY(int, m_lastKechengId, LastKechengId);//最新完成的任务id
    
    CC_SYNTHESIZE_READONLY(int, m_dayRenwuCount, DayRenwuCount);
    
    CC_SYNTHESIZE_READONLY(vector<Hanzi*>*, m_hanziVector, HanziVector);//课程汉字
    
};

#endif /* defined(__Blackcat__KechengController__) */
