//
//  PageViewScene.h
//  Blackcat
//
//  Created by haojunhua on 14-9-21.
//
//

#ifndef __Blackcat__PageViewScene__
#define __Blackcat__PageViewScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CoverLayer.h"
using namespace cocos2d;

class PageViewScene : public CCLayer,public CoverLayerDelegate
{
public:
    virtual CCScene* changePage()=0;
    
    PageViewScene();
    ~PageViewScene();
    
    virtual void keyBackClicked();
    virtual void keyMenuClicked();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual void beforeReplace()=0;
    
    virtual void menuCallback(CCObject* pSender);
    
    
    virtual int topHandlerPriority(){return kCCMenuHandlerPriority-1;};
public:
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    CCRect m_pageSpriteRect;
    
    float m_fTouchMoveDistance;//总相对距离
    float m_fTouchDirectionDistance;//记录touchend触发前move最后一个方向touchmove的距离
    bool m_bDirection;//记录方向false为向左移(m_pThumbLayer坐标值变小),true反之
    cc_timeval m_cctimeStart;//记录最后一个方向的初始时间
};

#endif /* defined(__Blackcat__PageViewScene__) */
