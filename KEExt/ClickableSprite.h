//
//  ClickableSprite.h
//  YidiPlayerHD
//
//  Created by shuang on 12-7-12.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef YidiPlayerHD_ClickableSprite_h
#define YidiPlayerHD_ClickableSprite_h

#include "cocos2d.h"
using namespace cocos2d;

class ClickableSprite : public CCSprite, public CCTouchDelegate
{
public:
    ClickableSprite():m_bIsMoved(false),m_touchableRect(CCRectZero),m_pTarget(NULL),m_pSelector(NULL){};
    
    static ClickableSprite* create();
    static ClickableSprite* createWithSpriteFrameName(const char *pszSpriteFrameName);
    void setTarget(CCObject *target, SEL_MenuHandler selector);
    void setTouchableRect(CCRect rect){this->m_touchableRect=rect;};
    
    // register touch
    virtual void onEnter();
    virtual void onExit();
    
    // touch event
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    
protected:
    CCObject * m_pTarget;
    SEL_MenuHandler m_pSelector;
    CCPoint m_oPointBegan;
    bool m_bIsMoved;
    CCRect m_touchableRect;
};

#endif
