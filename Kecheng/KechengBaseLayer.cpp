//
//  KechengBaseLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 13-12-19.
//
//

#include "KechengBaseLayer.h"
#include "resource.h"
#include "ResManager.h"

bool KechengBaseLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
//    CCSprite* lantian = CCSprite::createWithSpriteFrameName("renwu_lantian.png");
//    lantian->setPosition(S_RM->getJpgBgPosition());
//    this->addChild(lantian);
//
    CCSprite* pSprite = CCSprite::createWithSpriteFrameName("renwu_bg.png");
    pSprite->setPosition(S_RM->getJpgBgPosition());
    this->addChild(pSprite);
    
    CCParticleSnow* emitter = CCParticleSnow::create();
    this->addChild(emitter);
    
    emitter->setLife(3);
    emitter->setLifeVar(1);
    
    // gravity
    emitter->setGravity(ccp(0,-10));
    
    // speed of particles
    emitter->setSpeed(130);
    emitter->setSpeedVar(30);
    
    ccColor4F startColor = emitter->getStartColor();
    startColor.r = 0.9f;
    startColor.g = 0.9f;
    startColor.b = 0.9f;
    emitter->setStartColor(startColor);
    
    ccColor4F startColorVar = emitter->getStartColorVar();
    startColorVar.b = 0.1f;
    emitter->setStartColorVar(startColorVar);
    emitter->setEmissionRate(emitter->getTotalParticles()/emitter->getLife());
    emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("snow.png") );
    
    //黑猫
    m_heimao=Heimao::create();
    m_heimao->setPosition(S_RM->getPositionWithName("renwu_heimao"));
    m_heimao->setScale(1.2);
    this->addChild(m_heimao,10);
    m_heimao->action("heimao_stand");
    
    return true;
    
}

void KechengBaseLayer::onExit(){
    CCLayer::onExit();
}