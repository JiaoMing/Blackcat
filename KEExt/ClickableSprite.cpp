//
//  ClickableSprite.cpp
//  YidiPlayerHD
//
//  Created by shuang on 12-7-12.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "ClickableSprite.h"

ClickableSprite *ClickableSprite::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
    ClickableSprite *sprite = new ClickableSprite();
    if (sprite && sprite->initWithSpriteFrameName(pszSpriteFrameName)) {
        sprite->autorelease();
    } else {
        CC_SAFE_RELEASE_NULL(sprite);
    }
    return sprite;
}

void ClickableSprite::setTarget(cocos2d::CCObject *target, SEL_MenuHandler selector)
{
    m_pTarget = target;
    m_pSelector = selector;
}

void ClickableSprite::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCNode::onEnter();
}

void ClickableSprite::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

bool ClickableSprite::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    //滑动前m_bIsMoved置为false
    m_bIsMoved=false;
    CCPoint pt = pTouch->getLocationInView();
    pt = CCDirector::sharedDirector()->convertToGL(pt);
    pt = convertToNodeSpace(pt);
    if (CCRect(0, 0, getContentSize().width, getContentSize().height).containsPoint(pt) && m_bVisible) {
        m_oPointBegan = pt;
        return true;
    }
    return false;
}

void ClickableSprite::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint touchLocation = pTouch->getLocationInView();	
	touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
	CCPoint lastLocation =pTouch->getPreviousLocationInView();
	lastLocation = CCDirector::sharedDirector()->convertToGL( lastLocation );
	float moveDistance = touchLocation.y - lastLocation.y;
    if(moveDistance<0)moveDistance=-moveDistance;
	if(moveDistance>3){
		m_bIsMoved=true;
	}
}

void ClickableSprite::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint pt = pTouch->getLocationInView();
    pt = CCDirector::sharedDirector()->convertToGL(pt);
    pt = convertToNodeSpace(pt);
    if (!m_bIsMoved&&CCRect(0, 0, getContentSize().width, getContentSize().height).containsPoint(pt) && m_bVisible && m_pTarget && m_pSelector) {
        (m_pTarget->*m_pSelector)(this);
    }
}

