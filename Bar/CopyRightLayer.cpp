//
//  CopyRightLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-1-26.
//
//

#include "CopyRightLayer.h"
#include "resource.h"

bool CopyRightLayer::init(){
    if (CCLayerColor::initWithColor(ccc4(0, 0, 0, 127))) {
        CCSprite* copyright=CCSprite::create("copyright.png");
        copyright->setPosition(S_RM->getJpgBgPosition());
        this->addChild(copyright);
        
        return true;
    }
    return false;
}

bool CopyRightLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}

void CopyRightLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    this->removeFromParent();
}