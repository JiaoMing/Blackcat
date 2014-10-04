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
#include "KapianScene.h"
#include "MessageTableLayer.h"
#include "RecordManager.h"

#define OVER_XINGXING_TAG 10
#define SUBSCRIPT_NUM_TAG 10

enum{
    kTagMessage=100,
    kTagMessageShu,
    kTagMessageLabel
};

RankingBarLayer* RankingBarLayer::create(bool isFromHome){
    RankingBarLayer *pRet = new RankingBarLayer();
    pRet->m_isFromHome=isFromHome;
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool RankingBarLayer::init(){
    if (CCLayer::init()) {
        
        m_rankingDataLayer=CCLayer::create();
        
        for (int i=0; i<kRankXunzhang+1; i++) {
            m_rankingDataLayer->addChild(this->createSpriteWithRankStruct(i));
        }
        
        this->addChild(m_rankingDataLayer);
        
        CCSprite* logo=CCSprite::create("logo.png");
        CCMenuItemSprite* logoItem=CCMenuItemSprite::create(logo, logo, this, menu_selector(RankingBarLayer::logoCallBack));
        logoItem->setPosition(S_RM->getPositionWithName("global_logo"));
        logoItem->setVisible(false);
        logoItem->setOpacity(0);
        
        CCSprite* message=CCSprite::createWithSpriteFrameName("youjian.png");
        CCMenuItemSprite* messageItem=CCMenuItemSprite::create(message, message, this, menu_selector(RankingBarLayer::messageCallBack));
        messageItem->setPosition(S_RM->getPositionWithName("home_message"));
        messageItem->setTag(kTagMessage);
        
        
        CCSprite* messageShu=CCSprite::createWithSpriteFrameName("youjian_shu.png");
        messageShu->setPosition(ccp(62, 51));
        messageShu->setTag(kTagMessageShu);
        messageItem->addChild(messageShu);
        
        
        CCLabelTTF* label=CCLabelTTF::create("", "KaiTi.ttf", 15);
        label->setPosition(ccp(16, 15));
        label->setColor(ccc3(0, 0, 0));
        label->setTag(kTagMessageLabel);
        messageShu->addChild(label);
        
        if (m_isFromHome) {
            m_logoMenu=CCMenu::create(logoItem,messageItem,NULL);
            m_logoMenu->setPosition(CCPointZero);
            m_isLogoShow=false;
            this->addChild(m_logoMenu);
        }
        
        if (!DEBUG_OPEN) {
//            this->schedule(schedule_selector(RankingBarLayer::fresh),10.0f);
        }
        
        return true;
    }
    return false;
}

void RankingBarLayer::onEnter(){
    CCLayer::onEnter();
    S_RDM->setDelegate(this);
    freshRecord();
}

void RankingBarLayer::onExit(){
    CCLayer::onExit();
    S_RDM->setDelegate(NULL);
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
//    int overXingxingCount=S_UD->getIntegerForKey(OVER_XINGXING_COUNT,0);
//    CCNode* xingxing=m_rankingDataLayer->getChildByTag(OVER_XINGXING_TAG);
//    CCLabelTTF* subScript=(CCLabelTTF*)xingxing->getChildByTag(SUBSCRIPT_NUM_TAG);
//    if (overXingxingCount>0) {
//        overXingxingCount-=1;
//        S_UD->setIntegerForKey(OVER_XINGXING_COUNT, overXingxingCount);
//        S_UD->flush();
//        subScript->setString(CCString::createWithFormat("%d",overXingxingCount)->getCString());
//    }else{
//        subScript->removeFromParentAndCleanup(false);
//    }
}

void RankingBarLayer::logoCallBack(CCObject* object){
    if(m_isLogoShow){
        CopyRightLayer* layer=CopyRightLayer::create();
        layer->setDelegate(m_delegate);
        this->getParent()->addChild(layer,ORDER_DIALOG);
    }
}

void RankingBarLayer::messageCallBack(CCObject* object){
    MessageTableLayer* layer=MessageTableLayer::create();
    layer->setDelegate(m_delegate);
    this->getParent()->addChild(layer,ORDER_DIALOG);
    
    
    
}

void RankingBarLayer::rankCallBack(CCObject* object){
    CCNode* node=(CCNode*)object;
    
    RankingLayer* ranking=RankingLayer::create((Rank)node->getTag());
    ranking->setDelegate(m_delegate);
    this->getParent()->addChild(ranking,ORDER_DIALOG);
}

int RankingBarLayer::getMessageCount(){
    map<string, string>* data=new map<string, string>();
    map<string, string> expClause=map<string, string>();
    expClause.insert(pair<string, string>("newMessage","count(*)"));
    vector<const char*> whereClause=vector<const char*>();
    whereClause.push_back("isReaded=0");
    S_DM->statistic(data, "message", expClause,whereClause);
    
    int count=0;
    map<string, string>::iterator it;
    it=data->find("newMessage");
    if(it!=data->end()){
        count=atoi(it->second.c_str());
    }
    CC_SAFE_DELETE(data);
    
    return count;
}

void RankingBarLayer::freshRecord(){
    if(m_logoMenu!=NULL){
        CCNode* message=m_logoMenu->getChildByTag(kTagMessage);
        CCNode* messageShu=message->getChildByTag(kTagMessageShu);
        CCLabelTTF* messageLabel=(CCLabelTTF*)messageShu->getChildByTag(kTagMessageLabel);
        
        int count=getMessageCount();
        if (count==0) {
            messageShu->setVisible(false);
        }else{
            if (count>99) {
                count=99;
            }
            messageShu->setVisible(true);
            CCString* str=CCString::createWithFormat("%d",count);
            messageLabel->setString(str->getCString());
        }
    }
}