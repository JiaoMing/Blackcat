//
//  Cartoon.h
//  yidiengine
//
//  Created by shuang on 12-5-2.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef yidiengine_Cartoon_h
#define yidiengine_Cartoon_h

#include "cocos2d.h"
using namespace cocos2d;

// 用来表示ani文件里的带有Order的一个动作配置
class AnimationConfig : public CCObject
{    
public:
    AnimationConfig() : m_nType(0), m_tOffset(CCPointZero), m_fTimer(.2f), m_strName(""), m_pOrderArray(NULL) {}
    ~AnimationConfig(){ CC_SAFE_RELEASE_NULL(m_pOrderArray); }
    inline void setType(int type) { m_nType = type; };
    inline int getType() { return m_nType; };
    inline void setOffset(const CCPoint& pt) { m_tOffset = pt; };
    inline const CCPoint& getOffset() { return m_tOffset; };
    inline void setTimer(float timer) { m_fTimer = timer; };
    inline float getTimer() { return m_fTimer; };
    inline void setName(const char *name) { m_strName = name; };
    inline const char * getName() { return m_strName.c_str(); };
    inline void setOrderArray(CCArray *pOrderArray) { m_pOrderArray = pOrderArray; };
    inline CCArray * getOrderArray() { return m_pOrderArray; };
    
protected:
    int m_nType;
    CCPoint m_tOffset;
    float m_fTimer;
    std::string m_strName;
    CCArray *m_pOrderArray;
};

// 用来表示ani文件里的带有中文注释的一个动作
class MotionClip : public CCObject
{
public:
    MotionClip() : m_strName(""), m_strBodyAnimation(""), m_strEyeAnimation(""), m_strMouthAnimation(""), m_nTimes(1) {}
    inline void setName(const char *name) { m_strName = name; }
    inline const char * getName() { return m_strName.c_str(); }
    inline void setBodyAnimation(const char *name) { m_strBodyAnimation = name; };
    inline const char * getBodyAnimation() { return m_strBodyAnimation.c_str(); };
    inline void setEyeAnimation(const char *name) { m_strEyeAnimation = name; };
    inline const char * getEyeAnimation() { return m_strEyeAnimation.c_str(); };
    inline void setMouthAnimation(const char *name) { m_strMouthAnimation = name; };
    inline const char * getMouthAnimation() { return m_strMouthAnimation.c_str(); };
    inline void setTimes( int times ) { m_nTimes = times; };
    inline int getTimes() { return m_nTimes; };
    
protected:
    std::string m_strName;
    std::string m_strBodyAnimation;
    std::string m_strEyeAnimation;
    std::string m_strMouthAnimation;
    int m_nTimes;
};

typedef void (CCObject::*SEL_CartoonHandler)(CCObject *, const char *);
#define cartoon_selector(_SELECTOR) (SEL_CartoonHandler)(&_SELECTOR)

class Cartoon : public CCSprite, public CCTouchDelegate
{
public:
    Cartoon() : m_strName(""), m_pEye(NULL), m_pMouth(NULL), m_pAnimationConfigs(NULL), m_pClipConfigs(NULL), m_nIndexOfAction(0), m_pActionsDoing(NULL), m_pBoxConfigs(NULL), m_strClipName(""), m_pListener(NULL), m_pfnSelector(NULL) {}
    ~Cartoon();
    static Cartoon * cartoonWithName(const char * pszName);
	bool initWithName(const char * pszName);
    inline const char *getName() { return m_strName.c_str(); }
    void setCallback(CCObject *pTarget, SEL_CartoonHandler pSelector);
    void setBoxEnabled(const char * pszBoxName, bool enabled);
    bool getBoxEnabled(const char * pszBoxName);
    // do action by defined in shuwawa-anis.ani file 
    void doAction(CCArray * pActions);
    void doAction(const char * pszAction1, ... );
    void saySomething(const char * pszFileName, float duration);
    void doCallFuncOAction(CCString *pAction);
    void updateMouthFrame(short peak);
    
    // 用来执行CCAction中的与fade相关的动作
    CCAction * runFadeAction(CCAction * action);
    
    
    //声音+动作，同时执行，从配置文件加载数据
    float action(const char* key,bool random=true,int actionIndex=0,int audioIndex=0);
    
    //super methods
    void onEnter();
    void onExit();
    
    /**
     @brief For phone event handle functions
     */
    bool ccTouchBegan(CCTouch* touch, CCEvent* event);
//    void ccTouchEnded(CCTouch* touch, CCEvent* event);
//    void ccTouchCancelled(CCTouch *touch, CCEvent* event);
//    void ccTouchMoved(CCTouch* touch, CCEvent* event);
    
    //
    inline CCSprite * getEye() const { return m_pEye; }
    inline CCSprite * getMouth() const { return m_pMouth; }
    inline CCDictionary * getAnimationConfigs() const { return m_pAnimationConfigs; }
    inline CCDictionary * getClipConfigs() const { return m_pClipConfigs; }
    inline void setClipName(const char *pszClipName) { m_strClipName = pszClipName; }
    
    // 调试动作
    int debugActionIndex;
    void debugFirstAction();
    void debugNextAction();
    void debugLastAction();
    const char *getDebugingAction();
    
private:
    // pszAction format: "actionname:actiontimes" or "actionname"
    void doSingleAction(const char *pszAction);
    void doNextAction();
    void doNextBlink(CCObject * object);
    bool parseConfigFile2();
    
private:
    std::string m_strName;
    CCSprite * m_pEye;
    CCSprite * m_pMouth;
    CCDictionary * m_pAnimationConfigs;
    CCDictionary * m_pClipConfigs;
    CCDictionary * m_pBoxConfigs;
    int m_nIndexOfAction;
    CCArray * m_pActionsDoing;
    std::string m_strClipName;
    
    CCObject * m_pListener;
    SEL_CartoonHandler m_pfnSelector;
    std::map<std::string, bool> m_mapBoxEnabled;
};

//用法
//Cartoon *shuwawa = Cartoon::cartoonWithName("shuwawa");
//shuwawa->setPosition(ccp(200, 200));
//addChild(shuwawa);
//shuwawa->doAction("z-shuohua:0", NULL);
//shuwawa->doAction("z-bengtiaohuanhu:1", "z-shuohua:0", NULL);
//doAction中参数 字符串格式为动作名:动作次数 动作名可以从ani文件得知
//如果是移动等cocos2d相关的动作CCAction需要使用runAction函数来执行，但与fade相关的动作需要用runFadeAction来执行，如shuwawa->runFadeAction(CCFadeOut::actionWithDuration(4));

//box，是为了点击卡通人物的某个部分执行相应的动作，具体由那些身体区域可以触摸，详见ani文件的box config
//怎么使用？1.setCallback设置每个部分点击后的回调函数，回调函数形式为void CCObject::cartoonCallFunc(CCObject *pSender, const char *pszBoxName)
//该函数是CCObject类的子类的成员函数，第一个参数为所构造的shuwawa对象的指针，第二个参数为box的名字，见box config中的定义
//2.setBoxEnabled设置哪个box使能
//例子
//shuwawa->setCallback(this, cartoon_selector(FengMianScene::cartoonCallFunc));
//shuwawa->setBoxEnabled("z-shenti", true);
//
//void FengMianScene::cartoonCallFunc(CCObject *pSender, const char *pszBoxName)
//{
//    Cartoon *cartoon = (Cartoon *)pSender;
//    if (strcmp(pszBoxName, "z-shenti") == 0) {
//        // do something
//    } else if (strcmp(pszBoxName, "z-shoubi") == 0) {
//        
//    }
//}

#endif
