//
//  KapianScene.h
//  blackcat
//
//  Created by haojunhua on 13-9-2.
//
//

#ifndef blackcat_KapianScene_h
#define blackcat_KapianScene_h
#include "cocos2d.h"
#include "resource.h"
#include "ResManager.h"
#include "Kapian.h"
USING_NS_CC;


class KapianScene : public CCLayer
{
public:
    static CCScene* scene(KapianDataMode mode=kHanzi,KapianShowMode showMode=kKapianShowModeCollect);
    KapianScene(KapianDataMode mode,KapianShowMode showMode);
    ~KapianScene();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual void keyBackClicked();
    virtual void keyMenuClicked();
    
    
    void scheduleAnimate(float t);
    void processAnimate(KapianDataMode mode);
private:
    //设置公共节点元素
    void setMenuToggle(KapianShowMode mode);
    void changeShowModeCall(CCNode* pSender);
    
    void goBack(CCNode* pSender);
public:
    CC_SYNTHESIZE(KapianDataMode, m_mode, Mode);
    CC_SYNTHESIZE(KapianShowMode, m_showMode, ShowMode);
private:
    int m_hanziWriteCount;
    int m_hanziCount;
    int m_tupianViewCount;
    int m_tupianCount;
    CCMenuItemToggle* m_cltToggle;
    CCMenuItemToggle* m_allToggle;
};

class KapianHanziScene : public KapianScene
{
public:
    static CCScene* scene(KapianShowMode showMode=kKapianShowModeAll);
};

class KapianTupianScene : public KapianScene
{
public:
    static CCScene* scene(KapianShowMode showMode=kKapianShowModeCollect);
};


#endif
