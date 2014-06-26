//
//  ListScene.cpp
//  Blackcat
//
//  Created by haojunhua on 13-10-7.
//
//

#include "KechengListScene.h"
#include "KechengTableLayer.h"
#include "ResManager.h"
#include "resource.h"
#include "HomeScene.h"
#include "AsyncManager.h"
#include "Utils.h"

CCScene* KechengListScene::scene()
{
    CCScene *scene = CCScene::create();
    KechengListScene *layer = KechengListScene::create();
    scene->addChild(layer);
    return scene;
}

bool KechengListScene::init()
{
    if ( !KechengBaseLayer::init() )
    {
        return false;
    }
    
    CCSprite* pTableBg = CCSprite::createWithSpriteFrameName("renwu_renwukuang.png");
    pTableBg->setPosition(S_RM->getPositionWithName("renwu_list_bg"));
    this->addChild(pTableBg);
    
    CCMenu* pMenu = CCMenu::create();
    CCSprite* pFanhui_1=CCSprite::createWithSpriteFrameName("fanhui.png");
    CCMenuItemSprite* pbackItem=CCMenuItemSprite::create(pFanhui_1, pFanhui_1, this, menu_selector(KechengListScene::menuCallback));;
    pbackItem->setPosition(S_RM->getPositionWithName("global_back"));
    pMenu->addChild(pbackItem);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu);
    
    
    KechengTableLayer* tableLayer=KechengTableLayer::create(this);
    this->addChild(tableLayer);
    
    m_heimao->setBoxEnabled("z-shenti", true);
    m_heimao->setCallback(this, cartoon_selector(KechengListScene::heimaoCallBack));
    
    
    //打开android按键响应
    this->setKeypadEnabled(true);
    
    return true;
}

void KechengListScene::onEnter(){
    KechengBaseLayer::onEnter();
    this->heimaoAction();
    
    S_AE->pauseBackgroundMusic();
}

void KechengListScene::onExit(){
    KechengBaseLayer::onExit();
}

void KechengListScene::keyBackClicked(){
    this->menuCallback(NULL);
}

void KechengListScene::keyMenuClicked(){
    
}

void KechengListScene::kechengLoadCallBack(){
}

void KechengListScene::kechengTouchCallBack(){
    m_heimao->action("heimao_stand");
}

void KechengListScene::menuCallback(CCObject* pSender)
{
    S_AM->removeAsyncResource("RenwuScene");
    S_ALP->stop();
    S_AE->stopAllEffects();
    bool isBgMusicRunning=S_UD->getBoolForKey("BG_MUSIC",true);
    if (isBgMusicRunning) {
        S_AE->resumeBackgroundMusic();
    }
    S_DR->replaceScene(HomeScene::scene());
}

void KechengListScene::heimaoAction(){
    
    
    string time=TimeUtils::getYYYYMMDD();
    string lastTime=S_UD->getStringForKey(LAST_RENWU_DAY);
    int count=0;
    if (time==lastTime) {
        count=S_UD->getIntegerForKey(LAST_RENWU_DAY_COUNT, 0);
    }
    
    CCString* key=CCString::createWithFormat("heimao_renwuWelcome%d",count+1);
    m_heimao->action(key->getCString());
}

void KechengListScene::heimaoCallBack(CCObject* node, const char* buwei){
    this->heimaoAction();
}
