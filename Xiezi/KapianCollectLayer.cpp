//
//  KapianCollectLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-1-15.
//
//

#include "KapianCollectLayer.h"
#include "resource.h"

KapianCollectLayer* KapianCollectLayer::create(CCSprite* collectedSprite)
{
    KapianCollectLayer* layer = new KapianCollectLayer;
    layer->m_collectedSprite=collectedSprite;
    if (layer && layer->init())
    {
        layer->autorelease();
    }
    else
    {
        delete layer;
        layer = NULL;
    }
    return layer;
}


bool KapianCollectLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
    m_ka=CCSprite::createWithSpriteFrameName("ka.png");
    m_ka->setPosition(ccp(-90,120));
    this->addChild(m_ka);
    this->addChild(m_collectedSprite);
    m_chouti=CCSprite::createWithSpriteFrameName("chouti.png");
    m_chouti->setPosition(ccp(-85,100));
    this->addChild(m_chouti);
    
    return true;
}


void KapianCollectLayer::collectAnimate(){
    CCPoint mainPoint=S_RM->getJpgBgPosition();
    
    m_ka->runAction(CCMoveTo::create(1, ccp(50, 120)));
    m_chouti->runAction(CCMoveTo::create(1, ccp(95,100)));
    
    //收集动画action
    CCPointArray *array = CCPointArray::create(3);
    array->addControlPoint(ccp(0, 0));
    array->addControlPoint(ccp(-mainPoint.x/2, -100));
    array->addControlPoint(ccpSub( ccp(100,150),mainPoint));
    CCActionInterval* atmullRomBy=CCCatmullRomBy::create(1, array);
    CCAction* action=CCSequence::create(CCSpawn::create(CCScaleTo::create(1, 0.12),atmullRomBy,CCRotateBy::create(1,  360),NULL),CCMoveTo::create(0.1, ccp(100, 117)),CCCallFunc::create(this, callfunc_selector(KapianCollectLayer::collectAnimateCallback)),NULL);
    
    m_collectedSprite->runAction(action);
    
    S_AE->playEffect("mario3.mp3");
    S_ALP->play("chouti.mp3",NULL);
}

void KapianCollectLayer::collectAnimateCallback(){
    m_ka->runAction(CCMoveTo::create(0.5, ccp(-90, 120)));
    m_chouti->runAction(CCMoveTo::create(0.5, ccp(-85,100)));
    m_collectedSprite->runAction(CCMoveTo::create(0.5, ccp(85-200,117)));
}
