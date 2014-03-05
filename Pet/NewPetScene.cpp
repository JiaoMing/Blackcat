//
//  NewPetScene.cpp
//  Blackcat
//
//  Created by haojunhua on 13-12-15.
//
//

#include "NewPetScene.h"
#include "ResManager.h"
#include "LoadingScene.h"
#include "RankingBarLayer.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

enum {
    kBack=0,
    kXMDH1,/*四个免费熊猫动画*/
    kXMDH2,
    kXMDH3,
    kXMDH4,
    kModeToggle
};

NewPetScene::NewPetScene(){
//    m_micRecordPlayer=NULL;
}

NewPetScene::~NewPetScene(){
//    CC_SAFE_DELETE(m_micRecordPlayer);
}

CCScene* NewPetScene::scene()
{
    CCScene *scene = CCScene::create();
    NewPetScene* layer = NewPetScene::create();
    //初始化micplayer
//    layer->m_micRecordPlayer=new MicRecordPlayer(layer);
    scene->addChild(layer);
    return scene;
}

bool NewPetScene::init(){
    if (CCLayer::init()) {
        CCSprite* bg=CCSprite::create("bg.png");
        bg->setPosition(S_RM->getJpgBgPosition());
        this->addChild(bg);
        
        
        CCSprite* back=CCSprite::create("fanhui.png");
        CCMenuItemSprite* backItem = CCMenuItemSprite::create(back,
                                                              NULL,
                                                              this,
                                                              menu_selector(NewPetScene::menuCallBack) );
        backItem->setPosition(S_RM->getPositionWithName("global_back"));
        backItem->setTag(kBack);
        
        CCSprite *mode1 = CCSprite::createWithSpriteFrameName("mode1.png");
        CCMenuItem* item1=CCMenuItemSprite::create(mode1, mode1);
        CCSprite *mode2 = CCSprite::createWithSpriteFrameName("mode2.png");
        CCMenuItem* item2=CCMenuItemSprite::create(mode2, mode2);
        CCSprite *mode3 = CCSprite::createWithSpriteFrameName("mode3.png");
        CCMenuItem* item3=CCMenuItemSprite::create(mode3, mode3);
        m_modeToggle=CCMenuItemToggle::createWithTarget(this, menu_selector(NewPetScene::menuCallBack), item1,item2,item3,NULL);
        m_modeToggle->setPosition(S_RM->getPositionWithName("pet_mode"));
        m_modeToggle->setTag(kModeToggle);
        m_modeToggle->setVisible(false);
        
        CCMenu* menu = CCMenu::create(m_modeToggle,backItem, NULL);
        menu->setPosition(CCPointZero);
        this->addChild(menu);
        
        for (int i=0; i<4; i++) {
            CCString* str=CCString::createWithFormat("dh%d.png",i+1);
            CCSprite* sprite=CCSprite::createWithSpriteFrameName(str->getCString());
            CCMenuItemSprite* item=CCMenuItemSprite::create(sprite, sprite, this, menu_selector(NewPetScene::menuCallBack));
            CCString* pointStr=CCString::createWithFormat("pet_donghua%d",i+1);
            item->setPosition(S_RM->getPositionWithName(pointStr->getCString()));
            item->setTag(kXMDH1+i);
            menu->addChild(item);
        }
        
        //加载熊猫
        m_panda = Cartoon::cartoonWithName("xiaoxiongmao");
        m_panda->setPosition(S_RM->getPositionWithName("pet_panda"));
        m_panda->setScale(1.2);
        this->addChild(m_panda);
        m_panda->setCallback(this, cartoon_selector(NewPetScene::pressPandaCallBack));
        m_panda->setBoxEnabled("z-shenti", true);
        
        m_panda->doAction("z-dazhaohu", "z-budongshuohua:0", NULL);
        
        if(S_UD->getBoolForKey("panda_firstRun", true))
        {
            m_panda->doAction("z-dazhaohu", "z-budongshuohua:0", NULL);
            m_panda->saySomething("A1.mp3", 2);
            S_UD->setBoolForKey("panda_firstRun", false);
            S_UD->flush();
        }
        else {
            m_panda->doAction("z-budongshuohua:0", NULL);
        }
        
        
        m_rankingBarLayer=RankingBarLayer::create();
        this->addChild(m_rankingBarLayer);
    
        
        return true;
    }
    return false;
}

void NewPetScene::onEnter()
{
    CCLayer::onEnter();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void NewPetScene::onExit()
{
    bool isBgMusicRunning=S_UD->getBoolForKey("BG_MUSIC",true);
    if (isBgMusicRunning) {
        SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    }
    
    CCLayer::onExit();
}

void NewPetScene::pressPandaCallBack(CCObject *object, const char *pszBoxName)
{
    if(strcmp(pszBoxName, "z-shenti") == 0)
    {
        int i = rand() % 2;
        switch (i)
        {
            case 0:
                m_panda->doAction("z-dazhaohu", "z-budongshuohua:0", NULL);
                m_panda->saySomething("A1.mp3", 2);
                break;
            case 1:
                m_panda->doAction("z-dazhaohu", "z-budongshuohua:0", NULL);
                m_panda->saySomething("A2.mp3", 2);
            default:
                break;
        }
    }
}

void NewPetScene::menuCallBack(CCObject* pSender){
    int tag=((CCNode*)pSender)->getTag();
    
    if (tag>=kXMDH1&&tag<=kXMDH4) {
        int xingxing=S_UD->getIntegerForKey(OVER_XINGXING_COUNT);
        if (xingxing>0) {
            m_rankingBarLayer->subXingxing();
            switch (tag) {
                case kXMDH1:
                m_panda->doAction("z-niuzai:2", "z-budongshuohua:0", NULL);
                m_panda->saySomething("D1.mp3", 4.0f);
                break;
                case kXMDH2:
                m_panda->doAction("z-fengche", "z-budongshuohua:0", NULL);
                m_panda->saySomething("D3.mp3", 4.0f);
                break;
                case kXMDH3:
                m_panda->doAction("z-wanqiqiu01","z-wanqiqiu02","z-wanqiqiu03", "z-budongshuohua:0", NULL);
                m_panda->saySomething("C3.mp3", 4.0f);
                break;
                case kXMDH4:
                m_panda->doAction("z-chidongxi", "z-budongshuohua:0", NULL);
                m_panda->saySomething("D5.mp3", 4.0f);
                break;
                
                default:
                break;
            }
        }else{
            GuideDialog* guideDialog=new GuideDialog();
            guideDialog->setText("你没有星星了哦，去写汉字获得更多星星吧～");
            guideDialog->setMode(kGuideDialogOk);
            m_gudieDialogLayer=GuideDialogLayer::create(kDialogWithText);
            m_gudieDialogLayer->setDelegate(this);
            this->addChild(m_gudieDialogLayer);
            m_gudieDialogLayer->setGuideDialogData(guideDialog);
            CC_SAFE_DELETE(guideDialog);
        }
        
    }else{
        switch (tag) {
            case kBack:
            S_DR->replaceScene(LoadingScene::scene("HomeScene",true,kLoadingRolePanda));
            break;
            case kModeToggle:{
//                m_micDelegateMode=(MicDelegateMode)m_modeToggle->getSelectedIndex();
//                m_micRecordPlayer->changeMode(m_micDelegateMode);
            }
            break;
        }
    }
    
    
    
}

//void NewPetScene::micCallBack(MicCallBackCMD cmd){
//
//}

void NewPetScene::dialogCallBack(GuideDialogCMD cmd){
    m_gudieDialogLayer->removeFromParentAndCleanup(true);
}