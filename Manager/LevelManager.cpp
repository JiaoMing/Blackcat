//
//  LevelManager.cpp
//  Blackcat
//
//  Created by haojunhua on 14-5-9.
//
//

#include "LevelManager.h"

static LevelManager *g_instance = NULL;

LevelManager::LevelManager():m_delegate(NULL)
{
    m_levelRuleDict=S_PTM->getPlistDictionary("level.plist");
    CCArray* array=(CCArray*)m_levelRuleDict->objectForKey("level");
    CCObject *obj = NULL;
    CCARRAY_FOREACH(array, obj){
        CCString* item=(CCString*)obj;
        m_levelVector.push_back(item->intValue());
    }
    
    m_levelObject=new LevelObject();
    m_levelObject->setExp(S_UD->getIntegerForKey(USER_EXPERIENCE,0));
    
    this->setLevelByExpAndRule();
}

LevelManager::~LevelManager()
{
    CC_SAFE_DELETE(m_levelObject);
	CC_SAFE_DELETE(g_instance);
}

LevelManager* LevelManager::sharedLevelManager()
{
	if (g_instance == NULL)
	{
		g_instance = new LevelManager();
	}
	return g_instance;
}

int LevelManager::setLevelByExpAndRule(){
    int level=0;
    int exp=m_levelObject->getExp();
    for (vector<int>::iterator it=m_levelVector.begin(); it<m_levelVector.end(); it++) {
        if(exp>=*it){
            level++;
        }else{
            float upExp=*it;//升级目标经验
            float baseExp=*(--it);//当前基础经验,使用float类型，用于计算比例
            m_levelObject->setProcess((exp-baseExp)/(upExp-baseExp));
            m_levelObject->setExpNextLevel(upExp);
            break;
        }
    }
    m_levelObject->setLevel(level);
    return level;
}

int LevelManager::gainExp(int exp){
    m_levelObject->setExp(m_levelObject->getExp()+exp);
    S_UD->setIntegerForKey(USER_EXPERIENCE, m_levelObject->getExp());
    S_UD->flush();
    return this->setLevelByExpAndRule();
}

void LevelManager::gain(const char* exp_action,CCPoint worldPoint,CCPointArray* array){
    CCDictionary* actionDict=(CCDictionary*)m_levelRuleDict->objectForKey("exp_action");
    const CCString* exp=actionDict->valueForKey(exp_action);
    if (exp->length()>0) {
        this->gainExp(exp->intValue());
        if (m_delegate) {
            m_delegate->gainExp(worldPoint,array);
        }
    }
}