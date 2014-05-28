//
//  UserBarLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-5-9.
//
//

#ifndef __Blackcat__UserBarLayer__
#define __Blackcat__UserBarLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class LevelObject :public CCObject{
public:
    LevelObject():m_exp(0),m_level(1),m_process(0){
        
    }
    
    CCObject* copyWithZone(CCZone* pZone)
    {
        CCAssert(pZone == NULL, "CCArray should not be inherited.");
        LevelObject* levelObject = new LevelObject();
        levelObject->setExp(this->m_exp);
        levelObject->setExpNextLevel(m_expNextLevel);
        levelObject->setLevel(m_level);
        levelObject->setProcess(m_process);
        return levelObject;
    }
    
    /**
     *当前经验值
     */
    CC_SYNTHESIZE(int, m_exp, Exp);
    /**
     *当前等级
     */
    CC_SYNTHESIZE(int, m_level, Level);
    /**
     *当前等级的进度
     */
    CC_SYNTHESIZE(float, m_process, Process);
    /**
     *下一级经验
     */
    CC_SYNTHESIZE(int, m_expNextLevel, ExpNextLevel);
    
};


class LevelDelegate{
public:
    virtual void gainExp(CCPoint point,CCPointArray* array=NULL)=0;
    virtual void setPosition(const CCPoint &position)=0;
};

class UserBarLayer :public CCLayer,public LevelDelegate{
public:
    UserBarLayer();
    ~UserBarLayer();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    CREATE_FUNC(UserBarLayer);
    
    void fresh();
    
    /**
     *获得经验动画，星星飞
     */
    virtual void gainExp(CCPoint worldPoint,CCPointArray* array=NULL);
    
    /**
     *刷新等级数据
     */
    virtual void expUp();
    
    virtual void setPosition(const CCPoint &position){CCNode::setPosition(position);};
private:
    void levelUp();
private:
    CCProgressTimer* m_progress;
    CCLabelTTF* m_username;
    CCLabelTTF* m_levelLabel;
    CCLabelTTF* m_expLabel;
    CCPoint m_xingDisplayPoint;//星星消失的坐标
    LevelObject* m_levelObject;
    CCScale9Sprite* m_mingzi9Sprite;
    int m_zOrder;//保存历史zorder
};

#endif /* defined(__Blackcat__UserBarLayer__) */
