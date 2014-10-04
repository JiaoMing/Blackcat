//
//  HomeScene.h
//  Blackcat
//
//  Created by haojunhua on 13-10-21.
//
//

#ifndef Blackcat_HomeScene_h
#define Blackcat_HomeScene_h

#include "Cartoon.h"
#include "Heimao.h"
#include "cocos2d.h"
#include "GuideBaseLayer.h"
#include "RankingBarLayer.h"
#include "Hanzi.h"
#include "ParentLayer.h"
USING_NS_CC;

class HomeScene :public GuideBaseLayer
{
public:
    HomeScene();
    ~HomeScene();
    static CCScene* scene();
public:
    virtual bool init();
    CREATE_FUNC(HomeScene);
    
    // register touch
    virtual void onEnter();
    virtual void onExit();
    
    
    void willEnterForeground();
        
    // touch event
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void keyBackClicked();
    virtual void keyMenuClicked();
    
    virtual void dialogCallBack(GuideDialogCMD cmd);
    
    void menuCallBack(CCObject* pSender);
    
    void xiaoqicheDriveIn();
    
    virtual int topHandlerPriority(){return kCCMenuHandlerPriority-1;}
    
    void download(CCObject* obj);

private:
    Cartoon* m_Boy;
    Cartoon* m_Girl;
    Cartoon* m_Panda;
    Heimao* m_heimao;
    RankingBarLayer* m_rankingBarLayer;
    
    bool m_status;//缩放状态
    Hanzi* m_hanzi;//上次书写汉字
    CCMenuItemLabel* m_hanziMenuItem;
    
    int m_count;//用于引导
    long m_time;
    
    ParentLayer* m_parentLayer;
    
};

#endif
