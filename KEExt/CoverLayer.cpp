//
//  CoverLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-3-2.
//
//

#include "CoverLayer.h"
#include "resource.h"

void CoverLayer::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

void CoverLayer::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool CoverLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void CoverLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    this->removeFromParent();
}