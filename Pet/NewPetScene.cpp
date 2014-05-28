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
#include "GameLayer.h"
using namespace CocosDenshion;

enum {
    kBack=0,
    kXMDH1,/*四个免费熊猫动作+1个比赛*/
    kXMDH2,
    kXMDH3,
    kXMDH4,
    kXMDH5,
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
        
        CCSprite* back=CCSprite::createWithSpriteFrameName("fanhui.png");
        CCMenuItemSprite* backItem = CCMenuItemSprite::create(back,
                                                              NULL,
                                                              this,
                                                              menu_selector(NewPetScene::menuCallBack) );
        backItem->setPosition(S_RM->getPositionWithName("global_back"));
        backItem->setTag(kBack);
        CCMenu* menuBack = CCMenu::create(backItem, NULL);
        menuBack->setPosition(CCPointZero);
        this->addChild(menuBack);
        
        m_playLayer=CCLayer::create();//交互层，和游戏层相对应
        m_playLayer->setPosition(CCPointZero);
        this->addChild(m_playLayer);
        
        CCSprite *mode1 = CCSprite::createWithSpriteFrameName("pet_mode1.png");
        CCMenuItem* item1=CCMenuItemSprite::create(mode1, mode1);
        CCSprite *mode2 = CCSprite::createWithSpriteFrameName("pet_mode2.png");
        CCMenuItem* item2=CCMenuItemSprite::create(mode2, mode2);
        CCSprite *mode3 = CCSprite::createWithSpriteFrameName("pet_mode3.png");
        CCMenuItem* item3=CCMenuItemSprite::create(mode3, mode3);
        m_modeToggle=CCMenuItemToggle::createWithTarget(this, menu_selector(NewPetScene::menuCallBack), item1,item2,item3,NULL);
        m_modeToggle->setPosition(S_RM->getPositionWithName("pet_mode"));
        m_modeToggle->setTag(kModeToggle);
        m_modeToggle->setVisible(false);
        
        CCMenu* menuXiaobo = CCMenu::create(m_modeToggle, NULL);
        menuXiaobo->setPosition(CCPointZero);
        m_playLayer->addChild(menuXiaobo);
        
        for (int i=0; i<5; i++) {
            CCString* str=CCString::createWithFormat("pet_action%d.png",i+1);
            CCSprite* sprite=CCSprite::createWithSpriteFrameName(str->getCString());
            CCMenuItemSprite* item=CCMenuItemSprite::create(sprite, sprite, this, menu_selector(NewPetScene::menuCallBack));
            CCString* pointStr=CCString::createWithFormat("pet_donghua%d",i+1);
            item->setPosition(S_RM->getPositionWithName(pointStr->getCString()));
            item->setTag(kXMDH1+i);
            menuXiaobo->addChild(item);
        }
        
        //加载熊猫
        m_xiaobo = Cartoon::cartoonWithName("xiaoxiongmao");
        m_xiaobo->setPosition(S_RM->getPositionWithName("pet_panda"));
        m_xiaobo->setScale(1.2);
        m_xiaobo->setTag(1);
        //小波不加入playLayer，因假如Guide，需要在根layer
        this->addChild(m_xiaobo);
        m_xiaobo->setCallback(this, cartoon_selector(NewPetScene::pressXiaoboCallBack));
        m_xiaobo->setBoxEnabled("z-shenti", true);
        
        m_rankingBarLayer=RankingBarLayer::create();
        
        m_playLayer->addChild(m_rankingBarLayer);
        
        UserBarLayer* userBarLayer=UserBarLayer::create();
        userBarLayer->setPosition(S_RM->getPositionWithName("pet_userbar"));
        S_LM->setDelegate(userBarLayer);
        m_playLayer->addChild(userBarLayer,ORDER_USERBAR);
        
        return true;
    }
    return false;
}

void NewPetScene::onEnter()
{
    GuideBaseLayer::onEnter();
    S_AE->playBackgroundMusic("bg_fengling.mp3");
    
    int enterMode=S_UD->getIntegerForKey(PET_ENTER_MODE);
    switch (enterMode) {
        case kPetEnterNormal:{
            
            m_xiaobo->doAction("z-dazhaohu", "z-budongshuohua:0", NULL);
            
            if(S_UD->getBoolForKey("xiaobo_firstRun", true))
            {
                m_xiaobo->doAction("z-dazhaohu", "z-budongshuohua:0", NULL);
                m_xiaobo->saySomething("A1.mp3", 2);
                S_UD->setBoolForKey("xiaobo_firstRun", false);
                S_UD->flush();
            }
            else {
                m_xiaobo->doAction("z-budongshuohua:0", NULL);
            }
        }
            
            break;
        case kPetEnterGame:{
            
            //开始比赛
            m_gameLayer=GameLayer::create();
            m_gameLayer->setDelegate(this);
            this->addChild(m_gameLayer);
            m_playLayer->setVisible(false);
            m_xiaobo->setVisible(false);
        }
            
            break;
    }
}

void NewPetScene::onExit()
{
    bool isBgMusicRunning=S_UD->getBoolForKey("BG_MUSIC",true);
    if (isBgMusicRunning) {
        S_AE->playBackgroundMusic("bg_musicbox.mp3");
    }
    GuideBaseLayer::onExit();
}

void NewPetScene::pressXiaoboCallBack(CCObject *object, const char *pszBoxName)
{
    if(strcmp(pszBoxName, "z-shenti") == 0)
    {
        int i = rand() % 2;
        switch (i)
        {
            case 0:
                m_xiaobo->doAction("z-dazhaohu", "z-budongshuohua:0", NULL);
                m_xiaobo->saySomething("A1.mp3", 2);
                break;
            case 1:
                m_xiaobo->doAction("z-dazhaohu", "z-budongshuohua:0", NULL);
                m_xiaobo->saySomething("A2.mp3", 2);
            default:
                break;
        }
    }
}

void NewPetScene::menuCallBack(CCObject* pSender){
    int tag=((CCNode*)pSender)->getTag();
    
    if (tag>=kXMDH1&&tag<=kXMDH5) {
        if (tag==kXMDH5) {
            //比赛
            this->startGuide("PetScene_Normal", "xiezi");
        }else{
            int xingxing=S_UD->getIntegerForKey(OVER_XINGXING_COUNT);
            if (xingxing>0) {
                m_rankingBarLayer->subXingxing();
                switch (tag) {
                    case kXMDH1:
                        m_xiaobo->doAction("z-niuzai:2", "z-budongshuohua:0", NULL);
                        m_xiaobo->saySomething("D1.mp3", 4.0f);
                        break;
                    case kXMDH2:
                        m_xiaobo->doAction("z-fengche", "z-budongshuohua:0", NULL);
                        m_xiaobo->saySomething("D3.mp3", 4.0f);
                        break;
                    case kXMDH3:
                        m_xiaobo->doAction("z-wanqiqiu01","z-wanqiqiu02","z-wanqiqiu03", "z-budongshuohua:0", NULL);
                        m_xiaobo->saySomething("C3.mp3", 4.0f);
                        break;
                    case kXMDH4:
                        m_xiaobo->doAction("z-chidongxi", "z-budongshuohua:0", NULL);
                        m_xiaobo->saySomething("D5.mp3", 4.0f);
                        break;
                    default:
                        break;
                }
            }else{
                //没有星星了
                this->startGuide("PetScene_Normal", "xingxing");
            }
        }
        
        
    }else{
        switch (tag) {
            case kBack:{
                S_DR->replaceScene(LoadingScene::scene("HomeScene",true,kLoadingRoleXiaobo));
            }
                break;
            case kModeToggle:{
                //                m_micDelegateMode=(MicDelegateMode)m_modeToggle->getSelectedIndex();
                //                m_micRecordPlayer->changeMode(m_micDelegateMode);
            }
                break;
        }
    }
}


void NewPetScene::gameEnd(GamePlayer winner){
    m_gameLayer->removeFromParentAndCleanup(true);
    //    m_playLayer->setVisible(true);
    m_xiaobo->setVisible(true);
    m_xiaobo->setPosition(ccp(512, 400));
    switch (winner) {
        case kGamePlayerXiaobo:
            this->startGuide("PetScene_Normal", "xiaobo_win");
            break;
        case kGamePlayerWo:{
            int random=(int) (CCRANDOM_0_1()*4)+1;
            CCString* step=CCString::createWithFormat("wo_win%d",random);
            this->startGuide("PetScene_Normal",step->getCString());
        }
            break;
        default:
            break;
    }
}

//void NewPetScene::micCallBack(MicCallBackCMD cmd){
//
//}

void NewPetScene::dialogCallBack(GuideDialogCMD cmd){
    switch (cmd) {
        case kDialogCMDOk:
            
            break;
        case kDialogCMDYes:{
            //开始比赛
            m_gameLayer=GameLayer::create();
            m_gameLayer->setDelegate(this);
            this->addChild(m_gameLayer);
            m_playLayer->setVisible(false);
            m_xiaobo->setVisible(false);
        }
            break;
        case kDialogCMDNo:{
            //不比赛
            m_playLayer->setVisible(true);
            m_xiaobo->setVisible(true);
        }
            break;
            
        default:
            break;
    }
    m_xiaobo->setPosition(S_RM->getPositionWithName("pet_panda"));
}