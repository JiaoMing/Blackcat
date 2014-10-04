//
//  UpgradeLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-6-27.
//
//

#include "UpgradeLayer.h"
#include "ObjectManager.h"

UpgradeLayer::UpgradeLayer(){
    S_RM->addSceneRes("upgrade", "upgrade");
}

UpgradeLayer::~UpgradeLayer(){
    S_RM->removeSceneRes("upgrade");
}

bool UpgradeLayer::init(){
    if (!CoverLayer::init()) {
        return false;
    }
    
    m_shengji=CCSprite::createWithSpriteFrameName("shengji.png");
    m_shengji->setPosition(S_RM->getPositionWithName("upgrade_shengji"));
    m_shengji->setScale(0.1);
    this->addChild(m_shengji);
    
    
    this->scheduleOnce(schedule_selector(UpgradeLayer::scheduleToRemove), 3);
    
    return true;
}

void UpgradeLayer::onEnter(){
    CoverLayer::onEnter();
    
    CCAction* action=CCSequence::create(CCScaleTo::create(0.5, 1),CCCallFunc::create(this, callfunc_selector(UpgradeLayer::xingxingAnimate)),NULL);
    m_shengji->runAction(action);
    
    S_OM->hideNativeView();
}

void UpgradeLayer::onExit(){
    CoverLayer::onExit();
    S_OM->resumeNativeView();
}

void UpgradeLayer::scheduleToRemove(){
    this->removeFromParent();
}

void UpgradeLayer::xingxingAnimate(){
    CCAnimation* animation=CCAnimation::create();
    for (int i=0;i<17;i++) {
        CCString* fileName=CCString::createWithFormat("xingguang_%d.png",i+1);
        animation->addSpriteFrame(S_SF->spriteFrameByName(fileName->getCString()));
    }
    animation->setDelayPerUnit(0.1);
    animation->setRestoreOriginalFrame(true);
    CCAnimate* animate=CCAnimate::create(animation);
    CCSprite* xingguang=CCSprite::create();
    xingguang->setPosition(S_RM->getPositionWithName("upgrade_xingguang"));
    this->addChild(xingguang);
    xingguang->runAction(CCRepeatForever::create(animate));
    
    
    CCAnimation* animationShanguang=CCAnimation::create();
    for (int i=0;i<6;i++) {
        CCString* fileName=CCString::createWithFormat("shanguang_%d.png",i+1);
        animationShanguang->addSpriteFrame(S_SF->spriteFrameByName(fileName->getCString()));
    }
    animationShanguang->setDelayPerUnit(0.1);
    animationShanguang->setRestoreOriginalFrame(true);
    CCAnimate* animateShanguang=CCAnimate::create(animationShanguang);
    CCSprite* shanguang=CCSprite::create();
    shanguang->setPosition(S_RM->getPositionWithName("upgrade_xingguang"));
    this->addChild(shanguang);
    shanguang->runAction(animateShanguang);
    
}