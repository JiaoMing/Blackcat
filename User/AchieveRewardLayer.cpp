//
//  AchieveRewardLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-6-16.
//
//

#include "AchieveRewardLayer.h"
#include "resource.h"

AchieveRewardLayer* AchieveRewardLayer::create(AchieveKey key,int level){
    AchieveRewardLayer* layer=new AchieveRewardLayer();
    layer->autorelease();
    layer->init(key, level);
    return layer;
}

void AchieveRewardLayer::setTarget(cocos2d::CCObject *target, SEL_CallFunc selector){
    m_target=target;
    m_selector=selector;
}

bool AchieveRewardLayer::init(AchieveKey key,int level){
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 120))) {
        return false;
    }
    
    CCSprite* guang=CCSprite::createWithSpriteFrameName("reward_shanguangxingxing.png");
    guang->setPosition(S_RM->getJpgBgPosition());
    guang->setScale(2);
    this->addChild(guang);
    guang->runAction(CCRotateTo::create(3, 90));
    
    CCString* str=CCString::createWithFormat("reward_%s_%d.png",achieveStruts[key].icon.c_str(),level);
    CCSprite* xunzhang=CCSprite::createWithSpriteFrameName(str->getCString());
    xunzhang->setPosition(S_RM->getJpgBgPosition());
    xunzhang->setScale(0.1);
    this->addChild(xunzhang);
    xunzhang->runAction(CCSequence::create(CCScaleTo::create(1, 1),CCDelayTime::create(2),CCSpawn::create(CCScaleTo::create(1,0.01),CCFadeOut::create(1),NULL),NULL));
    
    this->runAction(CCSequence::create(CCDelayTime::create(3),CCFadeOut::create(1),NULL));
    
    this->scheduleOnce(schedule_selector(AchieveRewardLayer::callback), 4);
    
    return true;
}

void AchieveRewardLayer::onEnter(){
    CoverLayer::onEnter();
    
    S_OM->hideNativeView();
}

void AchieveRewardLayer::onExit(){
    CoverLayer::onExit();
    S_OM->resumeNativeView();
}


void AchieveRewardLayer::callback(float t){
    this->removeFromParent();
    if (m_target && m_selector) {
        (m_target->*m_selector)();
    }
}