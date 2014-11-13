//
//  WelcomeScene.cpp
//  Blackcat
//
//  Created by haojunhua on 14-8-1.
//
//

#include "WelcomeScene.h"
#include "resource.h"
#include "PlatformAction.h"
#include "SummaryScene.h"
#include "LoadingScene.h"
#include "ClickableSprite.h"
#include "CopyRightLayer.h"
#include "NewPetScene.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "Java_com_kidsedu_KEHelper.h"
#endif

enum{
    kTagYouxi=0,
    kTagXuexi,
    kTagJianjie,
    kTagJiazhangcaidan,
    kTagXinshouyindao,
    kTagYouxibeijing,
    kTagLogo
};

const std::string g_itemsName[kTagLogo+1] = {
    "z_youxi",
    "z_xuexi",
    "jianji",
    "jiazhangcaidan",
    "xinshouzhidao",
    "youxibeijing",
    "LOGO"
};

WelcomeScene::WelcomeScene(){
    S_RM->addSceneRes("WelcomeScene", "yindao");
}

WelcomeScene::~WelcomeScene(){
    S_RM->removeSceneRes("WelcomeScene");
}

CCScene* WelcomeScene::scene()
{
    CCScene* scene = CCScene::create();
    WelcomeScene *layer = WelcomeScene::create();
    scene->addChild(layer);
    return scene;
}

bool WelcomeScene::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    CCSprite* bg=CCSprite::createWithSpriteFrameName("yindao_beijing.png");
    bg->setPosition(S_RM->getJpgBgPosition());
    this->addChild(bg);
    
    CCMenu* menu=CCMenu::create();
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    
    for (int i=kTagYouxi; i<kTagLogo+1; i++) {
        string point="yindao_"+g_itemsName[i];
        string sprite="yindao_"+g_itemsName[i]+".png";
        CCSprite* xiaobo=CCSprite::createWithSpriteFrameName(sprite.c_str());
        CCMenuItemSprite* xiaoboItem=CCMenuItemSprite::create(xiaobo, xiaobo,this,menu_selector(WelcomeScene::menuCallback));
        xiaoboItem->setPosition(S_RM->getPositionWithName(point.c_str()));
        xiaoboItem->setTag(i);
        menu->addChild(xiaoboItem);
    }
    
    {
        CCAnimation* animation=CCAnimation::create();
        for (int i=0;i<3;i++) {
            CCString* fileName=CCString::createWithFormat("yindao_heimao_%d.png",i+1);
            animation->addSpriteFrame(S_SF->spriteFrameByName(fileName->getCString()));
        }
        animation->setDelayPerUnit(0.1);
        animation->setRestoreOriginalFrame(true);
        CCAnimate* animate=CCAnimate::create(animation);
        ClickableSprite* heimao=ClickableSprite::createWithSpriteFrameName("yindao_heimao_1.png");
        heimao->setPosition(S_RM->getPositionWithName("yindao_heimao"));
        heimao->setTag(kTagXuexi);
        heimao->setTarget(this, menu_selector(WelcomeScene::menuCallback));
        this->addChild(heimao);
        heimao->runAction(CCRepeatForever::create(CCSequence::create(animate,CCDelayTime::create(3),NULL)));
    }
    
    {
        CCAnimation* animation=CCAnimation::create();
        for (int i=0;i<2;i++) {
            CCString* fileName=CCString::createWithFormat("yindao_xiaoxiongmao_%d.png",i+1);
            animation->addSpriteFrame(S_SF->spriteFrameByName(fileName->getCString()));
        }
        animation->setDelayPerUnit(0.1);
        animation->setRestoreOriginalFrame(true);
        CCAnimate* animate=CCAnimate::create(animation);
        ClickableSprite* xiaoxiongmao=ClickableSprite::createWithSpriteFrameName("yindao_xiaoxiongmao_1.png");
        xiaoxiongmao->setTag(kTagYouxi);
        xiaoxiongmao->setTarget(this, menu_selector(WelcomeScene::menuCallback));
        xiaoxiongmao->setPosition(S_RM->getPositionWithName("yindao_xiaoxiongmao"));
        this->addChild(xiaoxiongmao);
        xiaoxiongmao->runAction(CCRepeatForever::create(CCSequence::create(animate,CCDelayTime::create(2),NULL)));
    }
    
    {
        CCAnimation* animation=CCAnimation::create();
        for (int i=0;i<8;i++) {
            CCString* fileName=CCString::createWithFormat("yindao_fengche_%d.png",i+1);
            animation->addSpriteFrame(S_SF->spriteFrameByName(fileName->getCString()));
        }
        animation->setDelayPerUnit(0.1);
        animation->setRestoreOriginalFrame(true);
        CCAnimate* animate=CCAnimate::create(animation);
        CCSprite* fengche=CCSprite::createWithSpriteFrameName("yindao_fengche_1.png");
        fengche->setPosition(S_RM->getPositionWithName("yindao_fengche"));
        this->addChild(fengche);
        fengche->runAction(CCRepeatForever::create(animate));
    }
    
    
    //打开android按键响应
    this->setKeypadEnabled(true);
    
    return true;
    
}


void WelcomeScene::onEnter(){
    CCLayer::onEnter();
    S_DR->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
    
    bool isOpenInto=S_UD->getBoolForKey(NEW_OPEN,true);
    if (isOpenInto) {
        S_ALP->play("heimao001_1.mp3",NULL);
        
//        this->scheduleOnce(schedule_selector(WelcomeScene::playBgAduio), 4);
    }
    
    bool isBgMusicRunning=S_UD->getBoolForKey("BG_MUSIC",true);
    if (isBgMusicRunning) {
        S_AE->playBackgroundMusic("bg.mp3");
    }
}

void WelcomeScene::onExit(){
    S_ALP->stop();
    CCLayer::onExit();
    S_DR->getTouchDispatcher()->removeDelegate(this);
    S_AE->stopBackgroundMusic();
}

void WelcomeScene::keyBackClicked(){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    showTipDialogJNI("退出", "确定退出程序吗?");
#endif
}

void WelcomeScene::keyMenuClicked(){
    
}

void WelcomeScene::playBgAduio(float t){
    
    S_UD->setIntegerForKey(NEW_OPEN, false);
    S_UD->flush();
}

bool WelcomeScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    S_AE->playEffect("default.mp3");
    return false;
}

void WelcomeScene::menuCallback(cocos2d::CCObject *object){
    S_ALP->stop();
    CCNode* node=(CCNode*)object;
    
    bool isFirstOpen=S_UD->getBoolForKey("IS_FIRST_OPEN", true);
    if (isFirstOpen) {
        S_UD->setBoolForKey("HomeScene_YindaoOpen", true);
        S_UD->setBoolForKey("IS_FIRST_OPEN", false);
        S_UD->flush();
    }else{
        S_UD->setBoolForKey("HomeScene_YindaoOpen", false);
    }
    switch (node->getTag()) {
        case kTagJianjie:
            S_DR->pushScene(SummaryScene::scene(kSumHome));
            break;
        case kTagYouxibeijing:{
            PlatformAction::playMovie();
            S_AE->pauseBackgroundMusic();
        }
            break;
        case kTagXinshouyindao:{
            S_UD->setBoolForKey("HomeScene_YindaoOpen", true);
            S_UD->flush();
            S_UD->setStringForKey("HomeScene_First","heimao_1");
            S_DR->replaceScene(LoadingScene::scene("HomeScene"));
        }
            break;
        case kTagJiazhangcaidan:{
            S_UD->setBoolForKey("HomeScene_ParentOpen", true);
            S_UD->flush();
            S_DR->replaceScene(LoadingScene::scene("HomeScene"));
        }
            break;
        case kTagXuexi:{
            S_DR->replaceScene(LoadingScene::scene("HomeScene"));
        }
            break;
        case kTagYouxi:{
            S_UD->setIntegerForKey(PET_ENTER_MODE,kPetEnterNormal);
            S_DR->replaceScene(LoadingScene::scene("NewPetScene",false,kLoadingRoleXiaobo));
        }
            break;
        case kTagLogo:{
            CopyRightLayer* layer=CopyRightLayer::create();
            layer->setDelegate(this);
            this->getParent()->addChild(layer,ORDER_DIALOG);
        }
            break;
        default:
            break;
    }
    S_UD->flush();
}
