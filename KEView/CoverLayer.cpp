//
//  CoverLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-4-10.
//
//

#include "CoverLayer.h"
#include "resource.h"

bool CoverLayer::init(const ccColor4B& color){
    if (!CCLayerColor::initWithColor(color)) {
        return false;
    }
    return true;
}

void CoverLayer::onEnter()
{
    CCLayerColor::onEnter();
    if (m_coverLayerDelegate!=NULL) {
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_coverLayerDelegate->topHandlerPriority(), true);
    }
}

void CoverLayer::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool CoverLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void CoverLayer::setDelegate(CoverLayerDelegate* coverLayerDelegate){
    this->m_coverLayerDelegate=coverLayerDelegate;
}