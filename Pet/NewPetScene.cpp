//
//  NewPetScene.cpp
//  Blackcat
//
//  Created by haojunhua on 13-12-15.
//
//

#include "NewPetScene.h"
#include "RankingBarLayer.h"
#include "GameLayer.h"
#include "resource.h"
#include "KechengController.h"
#include "LoadingScene.h"
#include "CloudLayer.h"
#include "WelcomeScene.h"

enum {
    kBack=0,
    kXMDH1,/*四个免费熊猫动作+1个比赛*/
    kXMDH2,
    kXMDH3,
    kXMDH4,
    kXMDH5,
    kModeToggle,
    kXingxing
};


#define UDKEY_SONG_XINGXING_COUNT "UDKEY_SONG_XINGXING_COUNT"
#define UDKEY_SONG_XINGXING_LINGQU_COUNT "UDKEY_SONG_XINGXING_LINGQU_COUNT"
#define UDKEY_SONG_XINGXING_RENWU_COUNT "UDKEY_SONG_XINGXING_RENWU_COUNT"
#define UDKEY_SONG_LAST_DAY "UDKEY_SONG_LAST_DAY"

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
        CCSprite* lantian=CCSprite::createWithSpriteFrameName("lantian.png");
        lantian->setPosition(S_RM->getJpgBgPosition());
        this->addChild(lantian);
        
        CloudLayer* cloudLayer=CloudLayer::create();
        this->addChild(cloudLayer);
        
        CCSprite* qianjing=CCSprite::createWithSpriteFrameName("qianjing.png");
        qianjing->setPosition(S_RM->getJpgBgPosition());
        this->addChild(qianjing);
        
        
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
        
        //加载熊猫
        m_xiaobo = Cartoon::cartoonWithName("xiaoxiongmao");
        m_xiaobo->setPosition(S_RM->getPositionWithName("pet_panda"));
        m_xiaobo->setScale(1.2);
        m_xiaobo->setTag(1);
        //小波不加入playLayer，因假如Guide，需要在根layer
        this->addChild(m_xiaobo,2);
        m_xiaobo->setCallback(this, cartoon_selector(NewPetScene::pressXiaoboCallBack));
        m_xiaobo->setBoxEnabled("z-shenti", true);
        
        m_playLayer=CCLayer::create();//交互层，和游戏层相对应
        m_playLayer->setPosition(CCPointZero);
        this->addChild(m_playLayer,1);
        
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
        
        m_rankingBarLayer=RankingBarLayer::create();
        m_rankingBarLayer->setDelegate(this);
        m_playLayer->addChild(m_rankingBarLayer);
        
        UserBarLayer* userBarLayer=UserBarLayer::create();
        userBarLayer->setPosition(S_RM->getPositionWithName("pet_userbar"));
        S_LM->setDelegate(userBarLayer);
        m_playLayer->addChild(userBarLayer,ORDER_USERBAR);
        
        int xingxingCount=this->xingxingCount();
        if (xingxingCount>0) {
            CCRect rect=S_RM->getRectWithName("pet_xingxing_rect");
            for (int i=0; i<xingxingCount; i++) {
                ClickableSprite* star=ClickableSprite::createWithSpriteFrameName("star.png");
                star->setTouchableRect(S_RM->getGLRectWithName("global_xingxing_glrect"));
                star->setPosition(ccp(CCRANDOM_0_1()*rect.size.width+rect.origin.x,CCRANDOM_0_1()*rect.size.height+rect.origin.y));
                CCFiniteTimeAction* show=CCSpawn::create(CCScaleTo::create(0.5, 1),CCFadeIn::create(0.5),NULL);
                CCFiniteTimeAction* hide=CCSpawn::create(CCScaleTo::create(0.5, 0.01),CCFadeOut::create(0.5),NULL);
                star->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(CCRANDOM_0_1()*1),show,CCDelayTime::create(1),hide,NULL)));
                star->setScale(0.01);
                star->setTarget(this, menu_selector(NewPetScene::menuCallBack));
                m_playLayer->addChild(star,0,kXingxing);
            }
        }
        
        
        //打开android按键响应
        this->setKeypadEnabled(true);
        
        
        return true;
        
        
    }
    return false;
}

int NewPetScene::xingxingCount(){
    string time=TimeUtils::getYYYYMMDD();
    string lastTime=S_UD->getStringForKey(UDKEY_SONG_LAST_DAY,"");
    int songCount=S_UD->getIntegerForKey(UDKEY_SONG_XINGXING_COUNT, 0);
    int lingquCount=S_UD->getIntegerForKey(UDKEY_SONG_XINGXING_LINGQU_COUNT, 0);
    int xingxing_count=0;
    if (time==lastTime) {
        xingxing_count=songCount-lingquCount;
    }else{
        xingxing_count=(int)(CCRANDOM_0_1()*5)+1;
        S_UD->setIntegerForKey(UDKEY_SONG_XINGXING_COUNT, xingxing_count);
        S_UD->setStringForKey(UDKEY_SONG_LAST_DAY,time);
    }
    
    int dayRenwuCount=S_KC->getDayRenwuCount();
    int renwuSongCount=S_UD->getIntegerForKey(UDKEY_SONG_XINGXING_RENWU_COUNT, 0);
    
    if (dayRenwuCount>renwuSongCount) {
        int addXingxing=(dayRenwuCount-renwuSongCount)*5;
        xingxing_count+=addXingxing;
        S_UD->setIntegerForKey(UDKEY_SONG_XINGXING_COUNT, songCount+addXingxing);
        S_UD->setIntegerForKey(UDKEY_SONG_XINGXING_RENWU_COUNT, renwuSongCount+1);
    }
    S_UD->flush();
    //20上限
    if (xingxing_count>20) {
        xingxing_count=20;
    }
    return xingxing_count;
}


bool NewPetScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint point=pTouch->getLocation();
    CCSize size=m_xiaobo->getContentSize();
    CCPoint xiaoboPoint=m_xiaobo->getPosition();
    CCRect rect=CCRectMake(xiaoboPoint.x-size.width/2, xiaoboPoint.y-size.height/2, size.width, size.height);
    if (!CCRectMake(0, 0, W_SIZE.width, W_SIZE.height-200).containsPoint(point)) {
        return false;
    }else if(rect.containsPoint(point)){
        return false;
    }
    return true;
}

void NewPetScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint point=pTouch->getLocation();
    
    if (!CCRectMake(0, 0, W_SIZE.width, W_SIZE.height-200).containsPoint(point)) {
        return;
    }
    
    m_xiaobo->stopAllActions();
    S_ALP->stop();
    
    CCPoint xiaoboPosition=m_xiaobo->getPosition();
    
    CCArray* array=CCArray::create();
    float d=(point.x-xiaoboPosition.x)/100;
    if (d<0) {
        array->addObject(CCFlipX::create(true));
    }else{
        array->addObject(CCFlipX::create(false));
    }
    d=fabs(d);
    array->addObject(CCMoveTo::create(d, ccp(point.x,xiaoboPosition.y)));
    if (d<0) {
        array->addObject(CCFlipX::create(true));
    }else{
        array->addObject(CCFlipX::create(false));
    }
    array->addObject(CCCallFunc::create(this, callfunc_selector(NewPetScene::zouluEnd)));
    m_xiaobo->runAction(CCSequence::create(array));
    int time=d/0.8;
    CCString* str=CCString::createWithFormat("c-zoulu:%d",time);
    m_xiaobo->doAction(str->getCString(),NULL);
    
    m_xiaobo->saySomething("D2.mp3",2.5);
    
}

void NewPetScene::zouluEnd(){
    m_xiaobo->doAction("z-budongshuohua:0",NULL);
}

void NewPetScene::onEnter()
{
    GuideBaseLayer::onEnter();
    S_DR->getTouchDispatcher()->addTargetedDelegate(this,1, true);
    S_AE->playBackgroundMusic("bg_fengling.mp3",true);
    
    int enterMode=S_UD->getIntegerForKey(PET_ENTER_MODE);
    switch (enterMode) {
        case kPetEnterNormal:{
            if(S_UD->getBoolForKey("xiaobo_firstRun", true))
            {
                m_xiaobo->doAction("z-dazhaohu", "z-budongshuohua:0", NULL);
                m_xiaobo->saySomething("A1.mp3", 2);
                S_UD->setBoolForKey("xiaobo_firstRun", false);
                S_UD->flush();
            }
            else {
                m_xiaobo->doAction("z-dazhaohu", "z-budongshuohua:0", NULL);
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
    
    
    BaiduStat::onStatEvent(kBaiduOneEventStart,"SceneRetain","NewPetScene");
}

void NewPetScene::onExit()
{
    S_DR->getTouchDispatcher()->removeDelegate(this);
    bool isBgMusicRunning=S_UD->getBoolForKey("BG_MUSIC",true);
    if (isBgMusicRunning) {
        S_AE->playBackgroundMusic("bg_musicbox.mp3");
    }
    GuideBaseLayer::onExit();
    
    
    BaiduStat::onStatEvent(kBaiduOneEventEnd,"SceneRetain","NewPetScene");
}

void NewPetScene::pressXiaoboCallBack(CCObject *object, const char *pszBoxName)
{
    m_xiaobo->stopAllActions();
    if(m_xiaobo->isFlipX())m_xiaobo->setFlipX(false);
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
    CCNode* node=(CCNode*)pSender;
    int tag=node->getTag();
    if (tag>=kXMDH1&&tag<=kXMDH5) {
        m_xiaobo->stopAllActions();
        if (tag==kXMDH5) {
            //比赛
            this->startGuide("PetScene_Normal", "xiezi");
        }else{
            if(m_xiaobo->isFlipX())m_xiaobo->setFlipX(false);
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
                    m_xiaobo->doAction("z-wanqiqiu1","z-wanqiqiu2","z-wanqiqiu3", "z-budongshuohua:0", NULL);
                    m_xiaobo->saySomething("C3.mp3", 4.0f);
                    break;
                case kXMDH4:
                    m_xiaobo->doAction("z-chidongxi", "z-budongshuohua:0", NULL);
                    m_xiaobo->saySomething("D5.mp3", 4.0f);
                    break;
                default:
                    break;
            }
        }
    }else{
        switch (tag) {
            case kBack:{
                S_ALP->stop();
                S_DR->replaceScene(LoadingScene::scene("WelcomeScene",true,kLoadingRoleXiaobo));
            }
                break;
            case kModeToggle:{
                //                m_micDelegateMode=(MicDelegateMode)m_modeToggle->getSelectedIndex();
                //                m_micRecordPlayer->changeMode(m_micDelegateMode);
            }
            case kXingxing:{
                node->setVisible(false);
                int count=S_UD->getIntegerForKey(UDKEY_SONG_XINGXING_LINGQU_COUNT, 0);
                S_UD->setIntegerForKey(UDKEY_SONG_XINGXING_LINGQU_COUNT, ++count);
                S_UD->flush();
                S_LM->gain(1, node->getPosition());
            }
                break;
        }
    }
}


void NewPetScene::gameEnd(GamePlayer winner){
    m_gameLayer->removeFromParentAndCleanup(true);
    //    m_playLayer->setVisible(true);
    m_xiaobo->setVisible(true);
    switch (winner) {
        case kGamePlayerXiaobo:
            this->startGuide("PetScene_Normal", "xiaobo_win");
            break;
        case kGamePlayerWo:{
            int random=(int) (CCRANDOM_0_1()*4)+1;
            CCString* step=CCString::createWithFormat("wo_win%d",random);
            this->startGuide("PetScene_Normal",step->getCString());
            
            //提升成就
            S_AEM->achieveUp(kAchieveCSJJ);
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
    this->reorderChild(m_xiaobo, 0);
}

void NewPetScene::keyBackClicked(){
    S_DR->replaceScene(LoadingScene::scene("WelcomeScene",true,kLoadingRoleXiaobo));
}

void NewPetScene::keyMenuClicked(){
    
}