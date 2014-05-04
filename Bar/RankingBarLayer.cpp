//
//  DataShowLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-1-26.
//
//

#include "RankingBarLayer.h"
#include "resource.h"
#include "CopyRightLayer.h"
#include "ClickableSprite.h"
#include "HomeScene.h"

#define OVER_XINGXING_TAG 10
#define SUBSCRIPT_NUM_TAG 10

bool RankingBarLayer::init(){
    if (CCLayer::init()) {
        
        m_rankingDataLayer=CCLayer::create();
        
        
        for (int i=0; i<kRankXingxing+1; i++) {
            m_rankingDataLayer->addChild(this->createSpriteWithRankStruct(i));
        }
        
        this->addChild(m_rankingDataLayer);
        
        CCSprite* logo=CCSprite::create("logo.png");
        CCMenuItemSprite* logoItem=CCMenuItemSprite::create(logo, logo, this, menu_selector(RankingBarLayer::logoCallBack));
        logoItem->setPosition(S_RM->getPositionWithName("global_logo"));
        logoItem->setVisible(false);
        logoItem->setOpacity(0);
        m_logoMenu=CCMenu::create(logoItem,NULL);
        m_logoMenu->setPosition(CCPointZero);
        m_isLogoShow=false;
        this->addChild(m_logoMenu);
        
        if (!DEBUG_OPEN) {
            this->schedule(schedule_selector(RankingBarLayer::fresh),10.0f);
        }
        
        return true;
    }
    return false;
}

CCSprite* RankingBarLayer::createSpriteWithRankStruct(int index){
    RankStruct rs=rankStruts[index];
    CCSprite* sprite=CCSprite::create();
    sprite->setPosition(S_RM->getPositionWithName(rs.position.c_str()));
    
    CCSprite* diwen=CCSprite::createWithSpriteFrameName("diwen.png");
    diwen->setPosition(ccp(0, 0));
    sprite->addChild(diwen);
    
    ClickableSprite* main=ClickableSprite::createWithSpriteFrameName(rs.rankIcon.c_str());
    main->setTarget(this, menu_selector(RankingBarLayer::rankCallBack));
    main->setTag(index);
    main->setPosition(ccp(0, 38));
    sprite->addChild(main);
    
    int count=S_UD->getIntegerForKey(rs.UD_key.c_str(),rs.UD_default);
    if (index==kRankXunzhang) {
        count=count*6;
    }
    CCString* dataStr1=CCString::createWithFormat("%d",count);
    CCLabelTTF* label1=CCLabelTTF::create(dataStr1->getCString(), "Arial", 18);
    label1->setColor(ccc3(255, 255, 255));
    label1->setPosition(ccp(0, 0));
    sprite->addChild(label1);
    
    if (index==kRankXingxing) {
        CCString* dataStr2=CCString::createWithFormat("%d",S_UD->getIntegerForKey(OVER_XINGXING_COUNT,0));
        CCLabelTTF* label2=CCLabelTTF::create(dataStr2->getCString(), "Arial", 20);
        label2->setColor(ccc3(255, 0, 0));
        label2->setPosition(ccp(25, 55));
        label2->setTag(SUBSCRIPT_NUM_TAG);
        sprite->addChild(label2);
        sprite->setTag(OVER_XINGXING_TAG);
    }
    return sprite;
}

void RankingBarLayer::fresh(float t){
    if (m_isLogoShow) {
        m_isLogoShow=false;
        CCObject* obj;
        CCARRAY_FOREACH(m_rankingDataLayer->getChildren(), obj){
            CCObject* object;
            CCARRAY_FOREACH(((CCNode*)obj)->getChildren(), object){
                CCNode* sprite=(CCNode*)object;
                sprite->runAction(CCFadeIn::create(1.0f));
            }
        }
        
        CCARRAY_FOREACH(m_logoMenu->getChildren(), obj){
            CCNode* item=(CCNode*)obj;
            item->runAction(CCSequence::createWithTwoActions(CCFadeOut::create(1.0f), CCCallFuncN::create(this, callfuncN_selector(RankingBarLayer::logoFreshCallBack))));
        }
        
    }else{
        m_isLogoShow=true;
        CCObject* obj;
        CCARRAY_FOREACH(m_rankingDataLayer->getChildren(), obj){
            CCObject* object;
            CCARRAY_FOREACH(((CCNode*)obj)->getChildren(), object){
                CCNode* sprite=(CCNode*)object;
                sprite->runAction(CCFadeOut::create(1.0f));
            }
        }
        
        CCARRAY_FOREACH(m_logoMenu->getChildren(), obj){
            CCNode* item=(CCNode*)obj;
            item->setVisible(true);
            item->runAction(CCFadeIn::create(1.0f));
        }
    }
}

void RankingBarLayer::logoFreshCallBack(CCNode* node){
    node->setVisible(m_isLogoShow);
}

void RankingBarLayer::subXingxing(int count){
    int overXingxingCount=S_UD->getIntegerForKey(OVER_XINGXING_COUNT,0);
    CCNode* xingxing=m_rankingDataLayer->getChildByTag(OVER_XINGXING_TAG);
    CCLabelTTF* subScript=(CCLabelTTF*)xingxing->getChildByTag(SUBSCRIPT_NUM_TAG);
    if (overXingxingCount>0) {
        overXingxingCount-=1;
        S_UD->setIntegerForKey(OVER_XINGXING_COUNT, overXingxingCount);
        S_UD->flush();
        subScript->setString(CCString::createWithFormat("%d",overXingxingCount)->getCString());
    }else{
        subScript->removeFromParentAndCleanup(false);
    }
}

void RankingBarLayer::logoCallBack(CCObject* object){
    if(m_isLogoShow)this->getParent()->addChild(CopyRightLayer::create(),INT_MAX);
}

void RankingBarLayer::rankCallBack(CCObject* object){
    CCNode* node=(CCNode*)object;
    RankingLayer* ranking=RankingLayer::create((Rank)node->getTag());
    ranking->setDelegate((HomeScene*)this->getParent());
    this->getParent()->addChild(ranking,INT_MAX);
}