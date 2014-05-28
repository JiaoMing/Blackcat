#ifndef __ebooks__CardList__
#define __ebooks__CardList__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CoverLayer.h"
using namespace cocos2d;

class CardListScene : public CCLayer,public CoverLayerDelegate
{
public:
    CardListScene();
    ~CardListScene();
    
    virtual void keyBackClicked();
    virtual void keyMenuClicked();
    
    virtual bool init();
    static CCScene* scene();
    virtual void menuCallback(CCObject* pSender);
    CREATE_FUNC(CardListScene);
    
    virtual int topHandlerPriority(){return kCCMenuHandlerPriority-1;};
public:
    virtual void registerWithTouchDispatcher();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    CCRect m_pageSpriteRect;
    
    float m_fTouchMoveDistance;//总相对距离
    float m_fTouchDirectionDistance;//记录touchend触发前move最后一个方向touchmove的距离
    bool m_bDirection;//记录方向false为向左移(m_pThumbLayer坐标值变小),true反之
    cc_timeval m_cctimeStart;//记录最后一个方向的初始时间
};

#endif // __ebooks__CardList__
