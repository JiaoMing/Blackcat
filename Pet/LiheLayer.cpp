//
//  LiheLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-6-3.
//
//

#include "LiheLayer.h"
#include "resource.h"
#include "KechengController.h"

/**
 * 每天最多领取三次礼物，第一次送、和两次任务过关
 * 但是如果任务过关了，之前尚有未领取的则一次领完
 */
#define UDKEY_LIWU_COUNT "UDKEY_LIWU_COUNT"
#define UDKEY_LAST_LIWU_DAY "UDKEY_LAST_LIWU_DAY"

LiheLayer::LiheLayer(){
    
}

LiheLayer::~LiheLayer(){
    
}

bool LiheLayer::init(){
    if (!CoverLayer::init(ccc4(0,0,0,0))) {
        return false;
    }
    
    m_lihe=CCSprite::create();
    m_lihe->setPosition(S_RM->getJpgBgPosition());
    this->addChild(m_lihe,2);
    
    
    
    return true;
}

void LiheLayer::onEnter(){
    CoverLayer::onEnter();
    
    m_xingxingCount=this->xingxingCount();
#pragma message "调试"
    m_xingxingCount=10;
    if (false) {
//    if (S_UD->getStringForKey(UDKEY_USER_USERNAME).length()==0||m_xingxingCount<=0) {
        this->removeFromParent();
    }else if (m_xingxingCount>0){
            if (S_KC->getDayRenwuCount()>0) {
                S_ALP->play("xiaobo016.mp3",NULL);
            }
        
            CCAnimation* animation=CCAnimation::create();
            for (int i=0; i<50; i++) {
                CCString* str;
                if (i<23) {
                    str=CCString::createWithFormat("lihe_%d.png",i+1);
                }else{
                    str=CCString::create("lihe_23.png");
                }
                animation->addSpriteFrameWithFileName(str->getCString());
            }
            
            animation->setDelayPerUnit(0.1);
            animation->setRestoreOriginalFrame(true);
            CCAnimate* animate=CCAnimate::create(animation);
            m_lihe->runAction(animate);
            
            m_guang1=CCSprite::create("guang.png");
            m_guang1->setPosition(S_RM->getJpgBgPosition());
            m_guang1->setOpacity(150);
            m_guang1->runAction(CCRotateBy::create(2.3, 360));
            this->addChild(m_guang1,1);
            
            m_guang2=CCSprite::create("guang.png");
            m_guang2->setPosition(S_RM->getJpgBgPosition());
            m_guang2->setOpacity(150);
            m_guang2->runAction(CCRotateBy::create(2.3, -360));
            this->addChild(m_guang2,1);
            
            this->scheduleOnce(schedule_selector(LiheLayer::delayGainExp), 2.3);
        
    }
}

void LiheLayer::onExit(){
    CoverLayer::onExit();
}

int LiheLayer::xingxingCount(){
    string time=TimeUtils::getYYYYMMDD();
    string lastTime=S_UD->getStringForKey(UDKEY_LAST_LIWU_DAY);
    int liwu_count=0;
    if (time==lastTime) {
        liwu_count=S_UD->getIntegerForKey(UDKEY_LIWU_COUNT, 0);
    }else{
        S_UD->setStringForKey(UDKEY_LAST_LIWU_DAY,time);
    }
    
    int dayRenwuCount=S_KC->getDayRenwuCount();
    
    int xingxingCount=0;
    if (dayRenwuCount<liwu_count) {
        return 0;
    }
    if (liwu_count==0) {
        xingxingCount=dayRenwuCount*5+(int)(CCRANDOM_0_1()*5)+1;
    }else if(liwu_count>=1){
        xingxingCount=(dayRenwuCount-liwu_count+1)*5;
    }
    
    S_UD->setIntegerForKey(UDKEY_LIWU_COUNT, dayRenwuCount+1);
    
    return xingxingCount;
}

void LiheLayer::delayGainExp(float t){
    
    m_guang1->runAction(CCSpawn::create(CCRotateBy::create(2.3, 360),CCFadeOut::create(1),NULL));
    m_guang2->runAction(CCSpawn::create(CCRotateBy::create(2.3, -360),CCFadeOut::create(1),NULL));
    m_lihe->runAction(CCSequence::create(CCDelayTime::create(1),CCFadeOut::create(1),NULL));
    
    S_LM->gain(m_xingxingCount, S_RM->getJpgBgPosition());
}


