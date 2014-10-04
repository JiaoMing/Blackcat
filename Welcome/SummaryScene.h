//
//  SummaryScene.h
//  Blackcat
//
//  Created by haojunhua on 14-8-4.
//
//

#ifndef __Blackcat__SummaryScene__
#define __Blackcat__SummaryScene__

#include "cocos2d.h"
USING_NS_CC;

enum {
    kSumHome=0,
    kSumShujia,
    kSumXiaoxiongmao,
    kSumHeiban,
    kSumHeimao,
    kSumChouti,
    kSumBisai
};

class SummaryScene : public CCLayer
{
public:
    static CCScene* scene(int key);
public:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    CREATE_FUNC(SummaryScene);
    
    virtual void keyBackClicked();
    virtual void keyMenuClicked();
    
    
    // touch event
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void menuCallback(CCObject* object);
private:
    int m_key;
    int m_touchedKey;
};

#endif /* defined(__Blackcat__SummaryScene__) */
