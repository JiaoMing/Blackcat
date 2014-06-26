//
//  CoverLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-4-10.
//
//

#include "CoverLayer.h"
#include "resource.h"

CoverLayer::CoverLayer(){
    m_coverLayerDelegate=NULL;
}

CoverLayer::~CoverLayer(){
}

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
        S_DR->getTouchDispatcher()->addTargetedDelegate(this, m_coverLayerDelegate->topHandlerPriority(), true);
    }
}

void CoverLayer::onExit()
{
    CCLayerColor::onExit();
    S_DR->getTouchDispatcher()->removeDelegate(this);
}

bool CoverLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}