//
//  GuideCoverLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-1-8.
//
//

#include "GuideCoverLayer.h"

bool GuideCoverLayer::init(){
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 127))) {
        return false;
    }
    
    return true;
}

void GuideCoverLayer::onEnter()
{
    CCLayerColor::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_guideCoverDelegate->topHandlerPriority(), true);
}

void GuideCoverLayer::onExit()
{
    CCLayerColor::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool GuideCoverLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}


void GuideCoverLayer::setDelegate(GuideCoverDelegate* guideCoverDelegate){
    this->m_guideCoverDelegate=guideCoverDelegate;
}
