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
#include "UserBarLayer.h"
#include "KechengDialogLayer.h"
#include "AchieveLayer.h"
#include "KechengController.h"
#include "DownloadLayer.h"
#include "DownloadManager.h"
using namespace CocosDenshion;

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
    kPropsJiangbei,
    kPropsShuzuobian,
    kPropsChoutishang,
    kPropsChoutixia,
    kPropsXiaoqiche,
    //    kPropsMenpai,
    //    kPropsDitu,
    kMenuHanzi,
    kMenuBack
};

const std::string g_propsName[kPropsXiaoqiche-kPropsHezi+1] = {
    "hezi","laoshu","wawa","fangkuang","yuankuang","sanjiaokuang","jiangbei","shuzuobian","choutishang","choutixia","xiaoqiche"
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
        }else if(i+kPropsHezi==kPropsWawa){
            props->setIsImmediate(true);
        }
    }
    
    CCLabelTTF* label=CCLabelTTF::create("一", "KaiTi.ttf", 120.0,CCSizeMake(120, 120),kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    label->setColor(ccc3(255,255,255));
    m_hanziMenuItem=CCMenuItemLabel::create(label, this, menu_selector(HomeScene::menuCallBack));
    m_hanziMenuItem->setPosition(S_RM->getPositionWithName("home_heiban"));
    m_hanziMenuItem->setTag(kMenuHanzi);
    
    //返回按钮
    CCSprite *back= CCSprite::createWithSpriteFrameName("fanhui.png");
    CCMenuItem* backItem=CCMenuItemSprite::create(back, back, this, menu_selector(HomeScene::menuCallBack));
    backItem->setPosition(S_RM->getPositionWithName("global_back"));
    backItem->setTag(kMenuBack);
    
    CCMenu* pMenu = CCMenu::create(m_hanziMenuItem,backItem, NULL);
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
    
    m_heimao = Heimao::create();
    m_heimao->setPosition(S_RM->getPositionWithName("home_jingzhang"));
    this->addChild(m_heimao);
    m_heimao->doAction("z-daijizhayan:0", NULL);
    m_heimao->setTag(kHeimao);
    m_heimao->setBoxEnabled("z-shenti", true);
    m_heimao->setScale(1);
    m_heimao->setCallback(this, cartoon_selector(HomeScene::menuCallBack));
    
    
    //用户信息bar
    UserBarLayer* userBarLayer=UserBarLayer::create();
    userBarLayer->setPosition(S_RM->getPositionWithName("home_userbar"));
    this->addChild(userBarLayer,ORDER_USERBAR);
    
    //工具条
    m_parentLayer=ParentLayer::create();
    m_parentLayer->setDelegate(this);
    this->addChild(m_parentLayer,ORDER_DIALOG-1);
    
    //打开android按键响应
    this->setKeypadEnabled(true);

    return true;
}

void HomeScene::onEnter()
{
    GuideBaseLayer::onEnter();
    S_DR->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    
    bool isBgMusicRunning=S_UD->getBoolForKey("BG_MUSIC",true);
    if (isBgMusicRunning) {
        S_AE->playBackgroundMusic("bg_musicbox.mp3",true);
    }
    
    int hid=S_UD->getIntegerForKey(LAST_HANZI_ID,34);
    S_DM->getByKey(m_hanzi, hid);
    CCLabelTTF* label=(CCLabelTTF*)m_hanziMenuItem->getLabel();
    label->setString(m_hanzi->getzi().c_str());
    
    //排行榜
    m_rankingBarLayer=RankingBarLayer::create(true);
    m_rankingBarLayer->setDelegate(this);
    this->addChild(m_rankingBarLayer);
    
    //引导流程
    cc_timeval now=TimeUtils::millisecondNow();
    long nowTimeStamp=now.tv_sec;
//    long lastTimeStamp=(long)S_UD->getFloatForKey(LAST_OPEN_TIMESTAMP);
    bool yindaoOpen=S_UD->getBoolForKey("HomeScene_YindaoOpen");
    bool parentOpen=S_UD->getBoolForKey("HomeScene_ParentOpen");
    if(!DEBUG_OPEN){
        if (yindaoOpen) {
            S_UD->setBoolForKey("HomeScene_YindaoOpen", false);
            S_UD->flush();
            //        调试关闭引导
            this->startGuide("HomeScene_First","heimao_1",true);
        }else if(parentOpen){
            S_UD->setBoolForKey("HomeScene_ParentOpen", false);
            S_UD->flush();
            m_parentLayer->showOrHide(true);
        }else{
            Props* props=(Props*)this->getChildByTag(kPropsWawa);
            props->setIsImmediate(true);
            if (isBgMusicRunning) {
                props->runRandomAnimate(0,false,false);
            }
            
            //引导转到欢迎界面
//            cc_timeval time=TimeUtils::millisecondNow();
//            struct tm* locationTime= TimeUtils::getLocaltime(time.tv_sec);
//            int month=locationTime->tm_mon+1;
//            int day=locationTime->tm_mday;
//            
//            if(S_UD->getFloatForKey(LAST_OPEN_TIMESTAMP,0)>0){
//                if (month==1&&day==1) {
//                    //元旦
//                    this->startGuide("HomeScene_Normal","heimao_2");
//                }else if(month==6&&day==1){
//                    this->startGuide("HomeScene_Normal","heimao_3");
//                }else{
//                    if (nowTimeStamp-lastTimeStamp>2*24*3600){
//                        //              if (true){
//                        int random=(int) (CCRANDOM_0_1()*2);
//                        switch (random) {
//                            case 0:
//                                this->startGuide("HomeScene_Normal","heimao_6");
//                                break;
//                            case 1:
//                                this->startGuide("HomeScene_Normal","xiaobo_2");
//                                break;
//                            default:
//                                break;
//                        }
//                    }else if (S_KC->getDayRenwuCount()==0) {
//                        bool isNewOpen=S_UD->getBoolForKey(NEW_OPEN,true);
//                        if (isNewOpen) {
//                            this->startGuide("HomeScene_Normal","heimao_5");
//                        }
//                    }
//                }
//            }
        }
    }
    
    S_UD->setBoolForKey(NEW_OPEN, false);
    S_UD->setFloatForKey(LAST_OPEN_TIMESTAMP, nowTimeStamp);
    S_UD->flush();
    
    BaiduStat::onStatEvent(kBaiduOneEventStart,"SceneRetain","首页");
}

void HomeScene::onExit()
{
    BaiduStat::onStatEvent(kBaiduOneEventEnd,"SceneRetain","首页");
    
    GuideBaseLayer::onExit();
    S_DR->getTouchDispatcher()->removeDelegate(this);
    
    m_rankingBarLayer->removeFromParentAndCleanup(false);
    S_AE->stopBackgroundMusic();
}

void HomeScene::willEnterForeground(){
    CCLog("willEnterForeground");
}

void HomeScene::keyBackClicked(){
    S_DR->replaceScene(LoadingScene::scene("WelcomeScene"));
    S_ALP->stop();
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
        
        if (DEBUG_OPEN) {
            S_UD->setIntegerForKey(PET_ENTER_MODE, 0);
            S_UD->flush();
            this->startGuide("HomeScene_Normal","xiaobo_1");
        }else{
            int mode=CCRANDOM_0_1()*10;
            if (mode==0) {
                mode=kPetEnterGame;
            }else{
                mode=kPetEnterNormal;
            }
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
        }
        
    }else{
//        CCPoint point =pTouch->getLocationInView();
//        CCRect rect=CCRectMake(0, 320, 120, 100);
//        if (rect.containsPoint(point)) {
//            cc_timeval time=TimeUtils::millisecondNow();
//            if (time.tv_sec-m_time>1) {
//                m_count=0;
//            }else{
//                m_count+=1;
//            }
//            m_time=time.tv_sec;
//            if (m_count>=2) {
//                S_UD->setStringForKey("HomeScene_First","heimao_1");
//                S_UD->setIntegerForKey("EVENT_OPEN_KAPIAN_HANZI",0);
//                S_UD->setIntegerForKey("EVENT_OPEN_KAPIAN_TUPIAN",0);
//                S_UD->setIntegerForKey("EVENT_OPEN_CARDSET",0);
//                S_UD->flush();
//                this->startGuide("HomeScene_First","heimao_1",true);
//                //关闭娃娃
//                Props* props=(Props*)this->getChildByTag(kPropsWawa);
//                props->showStopPose();
//            }
//            
//        }
//        
//        //点击未相应区域
//        //        if(status){
//        //            this->setScale(1);
//        //            this->setPosition(CCPointZero);
//        //        }else{
//        //            this->setScale(2);
//        //            CCPoint point =pTouch->getLocationInView();
//        //            CCPoint midPoint=ccp(point.x*2,point.y*2);
//        //            this->setPosition(CCPointZero);
//        //        }
//        m_status=!m_status;
    }
}

void HomeScene::dialogCallBack(GuideDialogCMD cmd){
    if (this->getQueueKey()=="HomeScene_First") {
        if (this->getStepKey()=="heimao_11") {
            if (cmd==kDialogCMDOk) {
                KechengDialogLayer* floatLayer=KechengDialogLayer::create(m_heimao);
                floatLayer->setDelegate(this);
                this->addChild(floatLayer,ORDER_DIALOG);
                this->startGuide("HomeScene_Normal","heimao_13");
                return;
            }
        }
    }else if (this->getQueueKey()=="HomeScene_Normal"){
        if (this->getStepKey()=="heimao_1") {
            if (cmd==kDialogCMDYes) {
                KechengDialogLayer* floatLayer=KechengDialogLayer::create(m_heimao);
                floatLayer->setDelegate(this);
                this->addChild(floatLayer,ORDER_DIALOG);
                this->startGuide("HomeScene_Normal","heimao_13");
                return;
            }
        }else if (this->getStepKey()=="xiaobo_1"||this->getStepKey()=="xiaobo_3"){
            if (cmd==kDialogCMDYes) {
                S_DR->replaceScene(LoadingScene::scene("NewPetScene",true,kLoadingRoleXiaobo));
                return;
            }
        }
    }
    
    this->step();
    
    
    //临时修改代码
    m_heimao->setPosition(S_RM->getPositionWithName("home_jingzhang"));
}


void HomeScene::menuCallBack(CCObject* pSender){
    S_ALP->stop();
    
    CCMenuItem *menuItem = (CCMenuItem *)pSender;
    int tag=menuItem->getTag();
    
    switch (tag) {
        case kMenuBack:{
            S_ALP->stop();
            S_DR->replaceScene(LoadingScene::scene("WelcomeScene",true));
        }
            break;
        case kHeimao:{
            
            KechengDialogLayer* floatLayer=KechengDialogLayer::create(m_heimao);
            floatLayer->setDelegate(this);
            this->addChild(floatLayer,ORDER_DIALOG);
            
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
                S_AE->pauseBackgroundMusic();
                props->showStopPose();
            }else{
                S_AE->resumeBackgroundMusic();
                props->runRandomAnimate(0,false,false);
            }
            S_UD->setBoolForKey("BG_MUSIC",!isBgMusicRunning);
            S_UD->flush();
        }
            break;
        case kPropsJiangbei:{
            AchieveLayer* achieveLayer=AchieveLayer::create();
            achieveLayer->setDelegate(this);
            this->addChild(achieveLayer,ORDER_DIALOG);
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