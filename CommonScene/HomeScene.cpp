//
//  HomeScene.cpp
//  Blackcat
//
//  Created by haojunhua on 13-10-21.
//
//

#include "HomeScene.h"
#include "resource.h"
#include "LoadingScene.h"
#include "Hanzi.h"
#include "XieziScene.h"
#include "PropsManager.h"
#include "KapianScene.h"
#include "CardShelfScene.h"
#include "Utils.h"
#include "RankingLayer.h"
#include "ActionFlowManager.h"
#include "SimpleAudioEngine.h"
#include "ParentLayer.h"
#include "NewPetScene.h"
using namespace CocosDenshion;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "Java_com_kidsedu_KEHelper.h"
#endif

enum {
    kBoy=0,
    kGirl,
    KPanda,
    kHeimao,
    kPropsHezi,
    kPropsLaoshu,
    kPropsWawa,
    kPropsFangkuang,
    kPropsYuankuang,
    kPropsSanjiaokuang,
    kPropsHuapen,
    kPropsShuzuobian,
    kPropsChoutishang,
    kPropsChoutixia,
    kPropsXiaoqiche,
    //    kPropsMenpai,
    //    kPropsDitu,
    kMenuHanzi
};

const std::string g_propsName[kPropsXiaoqiche-kPropsHezi+1] = {
    "hezi","laoshu","wawa","fangkuang","yuankuang","sanjiaokuang","huapen","shuzuobian","choutishang","choutixia","xiaoqiche"
};

HomeScene::HomeScene()
{
    m_status=false;
    m_time=TimeUtils::millisecondNow().tv_sec;
    m_hanzi=new Hanzi();
}

HomeScene::~HomeScene()
{
    CC_SAFE_DELETE(m_hanzi);
}

CCScene* HomeScene::scene()
{
    CCScene* scene = CCScene::create();
    HomeScene *layer = HomeScene::create();
    scene->addChild(layer);
    return scene;
}

bool HomeScene::init(){
    
    if ( !GuideBaseLayer::init() )
    {
        return false;
    }
    
    CCSprite *bg = CCSprite::createWithSpriteFrame(S_SF->spriteFrameByName("home_bg.png"));
    bg->setPosition(S_RM->getJpgBgPosition());
    this->addChild(bg);
    
    for (int i=0; i<kPropsXiaoqiche-kPropsHezi+1; i++) {
        string point="home_"+g_propsName[i];
        Props* props=S_PM->getPropsWithName(g_propsName[i].c_str());
        props->setPosition(S_RM->getPositionWithName(point.c_str()));
        props->showStopPose();
        this->addChild(props);
        props->setTarget(this, menu_selector(HomeScene::menuCallBack));
        props->setTag(i+kPropsHezi);
        
        if(i+kPropsHezi==kPropsXiaoqiche){
            props->setIsShowStopPosAfterARun(false);
        }
    }
    
    CCLabelTTF* label=CCLabelTTF::create("一", "KaiTi.ttf", 120.0,CCSizeMake(120, 120),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    label->setColor(ccc3(255,255,255));
    m_hanziMenuItem=CCMenuItemLabel::create(label, this, menu_selector(HomeScene::menuCallBack));
    m_hanziMenuItem->setPosition(S_RM->getPositionWithName("home_heiban"));
    m_hanziMenuItem->setTag(kMenuHanzi);
    
    CCMenu* pMenu = CCMenu::create(m_hanziMenuItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu);
    
    m_Boy = Cartoon::cartoonWithName("kaishi");
    m_Boy->setPosition(S_RM->getPositionWithName("home_xiaonanhai"));
    this->addChild(m_Boy);
    m_Boy->doAction("xiaonanhai2:0", NULL);
    m_Boy->setTag(kBoy);
    
    m_Girl = Cartoon::cartoonWithName("kaishi");
    m_Girl->setPosition(S_RM->getPositionWithName("home_xiaonvhai"));
    this->addChild(m_Girl);
    m_Girl->doAction("xiaonvhai2:0", NULL);
    m_Girl->setTag(kGirl);
    
    m_Panda = Cartoon::cartoonWithName("kaishi");
    m_Panda->setPosition(S_RM->getPositionWithName("home_xiaoxiongmao"));
    this->addChild(m_Panda);
    m_Panda->doAction("xiongmao2:0", NULL);
    m_Panda->setTag(KPanda);
    
    m_heimao = Cartoon::cartoonWithName("heimao");
    m_heimao->setPosition(S_RM->getPositionWithName("home_jingzhang"));
    this->addChild(m_heimao);
    m_heimao->doAction("z-daijizhayan:0", NULL);
    m_heimao->setTag(kHeimao);
    m_heimao->setBoxEnabled("z-shenti", true);
    m_heimao->setScale(1);
    m_heimao->setCallback(this, cartoon_selector(HomeScene::menuCallBack));
    
    //工具条
    ParentLayer* parentLayer=ParentLayer::create();
    parentLayer->setDelegate(this);
    this->addChild(parentLayer,INT_MAX-1);
    
    //打开android按键响应
    this->setKeypadEnabled(true);
    
    return true;
}

void HomeScene::onEnter()
{
    GuideBaseLayer::onEnter();
    S_DR->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    
    int hid=S_UD->getIntegerForKey(LAST_HANZI_ID,34);
    S_DM->getByKey(m_hanzi, hid);
    CCLabelTTF* label=(CCLabelTTF*)m_hanziMenuItem->getLabel();
    label->setString(m_hanzi->getzi().c_str());
    
    //排行榜
    m_rankingBarLayer=RankingBarLayer::create();
    this->addChild(m_rankingBarLayer);
    
    //引导流程
    cc_timeval now=TimeUtils::millisecondNow();
    long nowTimeStamp=now.tv_sec;
    long lastTimeStamp=(long)S_UD->getFloatForKey(LAST_OPEN_TIMESTAMP);
    string first= S_UD->getStringForKey("HomeScene_First");
    if (first!="heimao_10") {
//        调试关闭引导
        if(!DEBUG_OPEN)this->startGuide("HomeScene_First","heimao_1",true);
    }else{
        Props* props=(Props*)this->getChildByTag(kPropsWawa);
        props->setIsImmediate(true);
        bool isBgMusicRunning=S_UD->getBoolForKey("BG_MUSIC",true);
        if (isBgMusicRunning) {
            props->runRandomAnimate(0,false,false);
        }
        
        cc_timeval time=TimeUtils::millisecondNow();
        struct tm* locationTime= TimeUtils::getLocaltime(time.tv_sec);
        int month=locationTime->tm_mon+1;
        int day=locationTime->tm_mday;
        
        if(S_UD->getFloatForKey(LAST_OPEN_TIMESTAMP,0)>0){
            if (month==1&&day==1) {
                //元旦
                this->startGuide("HomeScene_Normal","heimao_2");
            }else if(month==6&&day==1){
                this->startGuide("HomeScene_Normal","heimao_3");
            }else{
                if (nowTimeStamp-lastTimeStamp>2*24*3600){
                    //              if (true){
                    int random=(int) (CCRANDOM_0_1()*2);
                    switch (random) {
                        case 0:
                            this->startGuide("HomeScene_Normal","heimao_6");
                            break;
                        case 1:
                            this->startGuide("HomeScene_Normal","xiaobo_2");
                            break;
                        default:
                            break;
                    }
                }else if (static_getDayRenwuCount()==0) {
                    bool isNewOpen=S_UD->getBoolForKey(NEW_OPEN,true);
                    if (isNewOpen) {
                        this->startGuide("HomeScene_Normal","heimao_5");
                    }
                }
            }
        }
    }
    
    S_UD->setBoolForKey(NEW_OPEN, false);
    S_UD->setFloatForKey(LAST_OPEN_TIMESTAMP, nowTimeStamp);
    S_UD->flush();
 
    BaiduStat::onStatEvent(kBaiduOneEventStart,"SceneRetain","HomeScene");
}

void HomeScene::onExit()
{
    BaiduStat::onStatEvent(kBaiduOneEventEnd,"SceneRetain","HomeScene");
    
    GuideBaseLayer::onExit();
    S_DR->getTouchDispatcher()->removeDelegate(this);
    
    m_rankingBarLayer->removeFromParentAndCleanup(false);
}

void HomeScene::willEnterForeground(){
    CCLog("willEnterForeground");
}

void HomeScene::keyBackClicked(){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	showTipDialogJNI("退出", "确定退出程序吗?");
#endif
}

void HomeScene::keyMenuClicked(){
    
}

bool HomeScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

void HomeScene::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCPoint touchPoint = convertTouchToNodeSpace(pTouch);
    m_Boy->doAction("xiaonanhai2:0",NULL);
    m_Girl->doAction("xiaonvhai2:0",NULL);
    if(m_Boy->boundingBox().containsPoint(touchPoint))
    {
        int random=(int) (CCRANDOM_0_1()*3)+1;
        CCString* step=CCString::createWithFormat("yidi_%d",random);
        this->startGuide("HomeScene_Normal",step->getCString());
        
    }else if(m_Girl->boundingBox().containsPoint(touchPoint))
    {
        int random=(int) (CCRANDOM_0_1()*4)+1;
        random=5;
        CCString* step=CCString::createWithFormat("feidi_%d",random);
        this->startGuide("HomeScene_Normal",step->getCString());
        
    }else if(m_Panda->boundingBox().containsPoint(touchPoint))
    {
        
        int mode=CCRANDOM_0_1()*2;
        S_UD->setIntegerForKey(PET_ENTER_MODE, mode);
        S_UD->flush();
        
        switch (mode) {
            case kPetEnterNormal:{
                this->startGuide("HomeScene_Normal","xiaobo_1");
            }
                break;
            case kPetEnterGame:{
                this->startGuide("HomeScene_Normal","xiaobo_3");
            }
                break;
        }
        
//        int count=static_getDayRenwuCount();
//        switch (count) {
//            case 0:
//                m_Panda->doAction("xiongmao3:6", "xiongmao2:0",NULL);
//                S_ALP->play("xiaobo003_1.mp3:4","xiaobo003_2.mp3:3",NULL);
//                break;
//            case 1:
//            case 2:
//                this->startGuide("HomeScene_Normal","xiaobo_1");
//                break;
//                
//            default:
//                break;
//        }
    }else{
        CCPoint point =pTouch->getLocationInView();
        CCRect rect=CCRectMake(0, 320, 120, 100);
        if (rect.containsPoint(point)) {
            cc_timeval time=TimeUtils::millisecondNow();
            if (time.tv_sec-m_time>1) {
                m_count=0;
            }else{
                m_count+=1;
            }
            m_time=time.tv_sec;
            if (m_count>=2) {
                S_UD->setStringForKey("HomeScene_First","heimao_1");
                S_UD->setIntegerForKey("EVENT_OPEN_KAPIAN_HANZI",0);
                S_UD->setIntegerForKey("EVENT_OPEN_KAPIAN_TUPIAN",0);
                S_UD->setIntegerForKey("EVENT_OPEN_CARDSET",0);
                S_UD->flush();
                this->startGuide("HomeScene_First","heimao_1",true);
                //关闭娃娃
                Props* props=(Props*)this->getChildByTag(kPropsWawa);
                props->showStopPose();
            }
            
        }
        
        //点击未相应区域
//        if(status){
//            this->setScale(1);
//            this->setPosition(CCPointZero);
//        }else{
//            this->setScale(2);
//            CCPoint point =pTouch->getLocationInView();
//            CCPoint midPoint=ccp(point.x*2,point.y*2);
//            this->setPosition(CCPointZero);
//        }
        m_status=!m_status;
    }
}

void HomeScene::dialogCallBack(GuideDialogCMD cmd){
    if (this->getQueueKey()=="HomeScene_First") {
        if (this->getStepKey()=="heimao_10") {
            if (cmd==kDialogCMDOk) {
                S_DR->replaceScene(LoadingScene::scene("RenwuScene",false));
                return;
            }
        }
    }else if (this->getQueueKey()=="HomeScene_Normal"){
        if (this->getStepKey()=="heimao_1") {
            if (cmd==kDialogCMDYes) {
                S_DR->replaceScene(LoadingScene::scene("RenwuScene",false));
                return;
            }
        }else if (this->getStepKey()=="xiaobo_1"||this->getStepKey()=="xiaobo_3"){
            if (cmd==kDialogCMDYes) {
                S_DR->replaceScene(LoadingScene::scene("NewPetScene",false,kLoadingRoleXiaobo));
                return;
            }
        }
    }
    
    this->step();
}


void HomeScene::menuCallBack(CCObject* pSender){
    S_AE->stopAllEffects();
//    S_AE->playEffect("click.mp3");
    
    CCMenuItem *menuItem = (CCMenuItem *)pSender;
    switch (menuItem->getTag()) {
        case kHeimao:{
            int heimaoCount=S_UD->getIntegerForKey("HEIMAO_CLICK_COUNT", 0);
            int eventHanzi=S_UD->getIntegerForKey("EVENT_OPEN_KAPIAN_HANZI",kGuideEventDefault);
            int eventTupian=S_UD->getIntegerForKey("EVENT_OPEN_KAPIAN_TUPIAN",kGuideEventDefault);
            if (heimaoCount==0) {
                if (eventHanzi!=kGuideEventPass&&eventTupian!=kGuideEventPass) {
                    this->startGuide("HomeScene_Normal","heimao_7");
                }else{
                    this->startGuide("HomeScene_Normal","heimao_8");
                    heimaoCount=1;
                }
            }else if (heimaoCount==1){
                this->startGuide("HomeScene_Normal","heimao_8");
            }else if (heimaoCount>=2){
                int count=static_getDayRenwuCount();
                if (count>=2&&m_hanzi->getwriteCount()>=3) {
                    int random=(int) (CCRANDOM_0_1()*2);
                    if (random==0) {
                        this->startGuide("HomeScene_Normal","heimao_9");
                    }else{
                        this->startGuide("HomeScene_Normal","heimao_10");
                    }
                }else if (count<2) {
                    this->startGuide("HomeScene_Normal","heimao_1");
                }else if(m_hanzi->getwriteCount()<3){
                    S_ALP->play("heimao037_1.mp3:2",(CCFileUtils::sharedFileUtils()->getWritablePath().append(m_hanzi->getcnAudioPath()).append(":1.2")).c_str(),"heimao037_2.mp3:2",NULL);
                    m_hanziMenuItem->runAction(CCSequence::create(CCDelayTime::create(2.0f),CCScaleTo::create(0.5, 1.3),CCScaleTo::create(0.5, 1),NULL));
                }
            }
            
            S_UD->setIntegerForKey("HEIMAO_CLICK_COUNT", heimaoCount+1);
            S_UD->flush();
        }
            break;
        case kPropsShuzuobian:{
            S_DR->replaceScene(CardShelfScene::scene());
//            S_DR->replaceScene(LoadingScene::scene("CardSetsScene",false));
        }
            break;
        case kMenuHanzi:{
            S_DR->pushScene(XieziScene::scene(m_hanzi->getid()));
        }
            break;
        case kPropsChoutishang:{
            S_DR->replaceScene(KapianHanziScene::scene());
//            S_DR->replaceScene(LoadingScene::scene("KapianHanziScene",false));
        }
            break;
        case kPropsChoutixia:{
            S_DR->replaceScene(KapianTupianScene::scene());
//            S_DR->replaceScene(LoadingScene::scene("KapianTupianScene",false));
        }
            break;
        case kPropsWawa:{
            bool isBgMusicRunning=S_UD->getBoolForKey("BG_MUSIC",true);
            Props* props=(Props*)menuItem;
            if (isBgMusicRunning) {
                SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
                props->showStopPose();
            }else{
                SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
                props->runRandomAnimate(0,false,false);
            }
            S_UD->setBoolForKey("BG_MUSIC",!isBgMusicRunning);
            S_UD->flush();
        }
            break;
        case kPropsXiaoqiche:{
            Props* xiaoqiche=(Props*)menuItem;
            AnimateAndAudio* aaa=(AnimateAndAudio*)xiaoqiche->getAnimates()->objectAtIndex(1);
            CCFiniteTimeAction* driveOut=CCSpawn::create(CCRepeat::create(aaa->getAnimate(), 5),CCMoveTo::create(2, ccp(W_SIZE.width, xiaoqiche->getPosition().y)),NULL);
            xiaoqiche->runAction(CCSequence::create(driveOut,CCCallFunc::create(this, callfunc_selector(HomeScene::xiaoqicheDriveIn)),NULL));
        }
            break;
        default:
            break;
    }
}

void HomeScene::xiaoqicheDriveIn(){
    CCPoint point=S_RM->getPositionWithName("home_xiaoqiche");
    CCPoint shache_point=S_RM->getPositionWithName("home_xiaoqiche_shache");
    Props* xiaoqiche=(Props*)this->getChildByTag(kPropsXiaoqiche);
    xiaoqiche->setPosition(ccp(-xiaoqiche->getContentSize().width, xiaoqiche->getPosition().y));
    AnimateAndAudio* aaa1=(AnimateAndAudio*)xiaoqiche->getAnimates()->objectAtIndex(1);
    AnimateAndAudio* aaa2=(AnimateAndAudio*)xiaoqiche->getAnimates()->objectAtIndex(2);
    CCFiniteTimeAction* driveIn1=CCSpawn::create(CCRepeat::create(aaa1->getAnimate(), 3),CCMoveTo::create(1,shache_point),NULL);
    CCFiniteTimeAction* driveIn2=CCSpawn::create(aaa2->getAnimate(),CCMoveTo::create(0.4,point),NULL);
    xiaoqiche->runAction(CCSequence::create(driveIn1,driveIn2,NULL));
}