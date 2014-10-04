//
//  LevelManager.h
//  Blackcat
//
//  Created by haojunhua on 14-5-9.
//
//

#ifndef __Blackcat__LevelManager__
#define __Blackcat__LevelManager__

#include "UserBarLayer.h"
#include "resource.h"
using namespace std;


class LevelManager :public CCObject{
public:
    static LevelManager* sharedLevelManager();
    int getLevelByExpAndRule(int exp);
public:
    LevelManager();
    ~LevelManager();
    
    /**
     * 获得经验
     *@param exp 增加的经验
     *@return 返回等级
     */
    int gainExp(int exp);
    
    void gain(const char* exp_action,CCPoint worldPoint,CCPointArray* array=NULL);
    
    void gain(int exp,CCPoint worldPoint,CCPointArray* array=NULL);
    
private:
    /**
     *设置等级，通过经验和等级表
     *@return 等级
     */
    int setLevelByExpAndRule();
    
protected:
    /**
     *经验模型对象
     */
    CC_SYNTHESIZE_READONLY(LevelObject*, m_levelObject, LevelObject);
    
    CC_SYNTHESIZE(UserBarLayer*, m_delegate, Delegate);
protected:
    vector<int> m_levelVector;
    CCDictionary* m_levelRuleDict;
};

#endif /* defined(__Blackcat__LevelManager__) */
