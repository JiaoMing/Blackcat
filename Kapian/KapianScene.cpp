//
//  KapianScene.cpp
//  blackcat
//
//  Created by haojunhua on 13-9-2.
//
//

#include "KapianScene.h"
#include "KapianTableLayer.h"
#include "DBManager.h"
#include "LoadingScene.h"
#include "HomeScene.h"
using namespace std;

enum{
    kBG=0,
    kShowTupian,
    kShowHanzi,
    kTableLayer
};
#define COUNT 9

#define TAG_PROGRESS 100
#define TAG_PROGRESS_LABEL 101

static void getProcess(const char* model,const char* column,int* count,int* sumCount){
    map<string, string>* data=new map<string, string>();
    map<string, string> expClause=map<string, string>();
    map<string, string>::iterator it;
    CCString* exp=CCString::createWithFormat("count(%s)",column);
    expClause.insert(pair<string, string>("count",exp->getCString()));
    exp=CCString::createWithFormat("Sum(case when %s>0 then 1 else 0 end)",column);
    expClause.insert(pair<string, string>("sumCount",exp->getCString()));
    S_DM->statistic(data, model, expClause);
    for(it=data->begin();it!=data->end();++it){
        if(it->first=="count"){
            *count=atoi(it->second.c_str());
        }else if(it->first=="sumCount"){
            *sumCount=atoi(it->second.c_str());
        }
        data->erase(it);
    }
    CC_SAFE_DELETE(data);
}

CCScene* KapianScene::scene(KapianDataMode mode)
{
    CCScene *scene = CCScene::create();
    KapianScene *layer = new KapianScene(mode);
    
    if (layer && layer->init())
    {
        layer->autorelease();
    }
    else
    {
        delete layer;
        layer = NULL;
    }
    scene->addChild(layer);
    return scene;
}

KapianScene::KapianScene(KapianDataMode mode){
    S_RM->addSceneRes("KapianScene", "new_kapian");
    this->m_mode=mode;
    switch (mode) {
        case kHanzi:{
            int event=S_UD->getIntegerForKey("EVENT_OPEN_KAPIAN_HANZI",kGuideEventDefault);
            if (event!=kGuideEventPass) {
                if(event==kGuideEventOpen)S_ALP->play("heimao036.mp3",NULL);
                S_UD->setIntegerForKey("EVENT_OPEN_KAPIAN_HANZI", kGuideEventPass);
                S_UD->flush();
            }
        }
            break;
        case kTupian:{
            int event=S_UD->getIntegerForKey("EVENT_OPEN_KAPIAN_TUPIAN",kGuideEventDefault);
            if (event!=kGuideEventPass) {
                if(event==kGuideEventOpen)S_ALP->play("heimao036.mp3",NULL);
                S_UD->setIntegerForKey("EVENT_OPEN_KAPIAN_TUPIAN", kGuideEventPass);
                S_UD->flush();
            }
        }
            break;
        default:
            break;
    }
}

KapianScene::~KapianScene(){
    S_RM->removeSceneRes("KapianScene");
}

bool KapianScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSprite *bg = CCSprite::createWithSpriteFrameName("kapian_bg.png");
    bg->setPosition(S_RM->getJpgBgPosition());
    this->addChild(bg);
    
    CCMenu* menu=CCMenu::create();
    menu->setPosition(CCPointZero);
    //返回按钮
    CCSprite *back= CCSprite::create("fanhui.png");
    CCMenuItem* backItem=CCMenuItemSprite::create(back, back, this, menu_selector(KapianScene::goBack));
    backItem->setPosition(S_RM->getPositionWithName("global_back"));
    menu->addChild(backItem);
    this->addChild(menu);
    
    CCSprite* spriteLabel=CCSprite::create();
    switch (m_mode) {
        case kHanzi:
            spriteLabel->setDisplayFrame(S_SF->spriteFrameByName("kapian_zi.png"));
            break;
        case kTupian:
            spriteLabel->setDisplayFrame(S_SF->spriteFrameByName("kapian_tu.png"));
            break;
        default:
            break;
    }
    spriteLabel->setPosition(S_RM->getPositionWithName("kapian_label"));
    this->addChild(spriteLabel);
    
    //初始化公共精灵
    CCSprite *showClt1 = CCSprite::createWithSpriteFrameName("shouji-lan.png");
    CCMenuItem* showCltItem1=CCMenuItemSprite::create(showClt1, showClt1);
    CCSprite *showClt2 = CCSprite::createWithSpriteFrameName("shouji-huang.png");
    CCMenuItem* showCltItem2=CCMenuItemSprite::create(showClt2, showClt2);
    m_cltToggle=CCMenuItemToggle::createWithTarget(this, menu_selector(KapianScene::changeShowModeCall), showCltItem1,showCltItem2,NULL);
    m_cltToggle->setPosition(S_RM->getPositionWithName("kapian_collect"));
    menu->addChild(m_cltToggle);
    
    CCSprite *showAll1 = CCSprite::createWithSpriteFrameName("quanbu-lan.png");
    CCMenuItem* showAllItem1=CCMenuItemSprite::create(showAll1, showAll1);
    CCSprite *showAll2 = CCSprite::createWithSpriteFrameName("quanbu-huang.png");
    CCMenuItem* showAllItem2=CCMenuItemSprite::create(showAll2, showAll2);
    m_allToggle=CCMenuItemToggle::createWithTarget(this, menu_selector(KapianScene::changeShowModeCall), showAllItem1,showAllItem2,NULL);
    m_allToggle->setPosition(S_RM->getPositionWithName("kapian_all"));
    menu->addChild(m_allToggle);
    
    //设置模型
    this->setMenuToggle(m_showMode=kCollect);
    
    //进度
    CCSprite* pProcessBg=CCSprite::createWithSpriteFrameName("jindukuang.png");
    pProcessBg->setPosition(S_RM->getPositionWithName("kapian_jindu_bg"));
    this->addChild(pProcessBg);
    CCSprite* pProcess=CCSprite::createWithSpriteFrameName("mange.png");
    CCProgressTimer* progress = CCProgressTimer::create(pProcess);
    progress->setType(kCCProgressTimerTypeBar);
    progress->setMidpoint(ccp(0, 0));
    progress->setBarChangeRate(ccp(1, 0));
    progress->setPercentage(0);
    progress->setPosition(S_RM->getPositionWithName("kapian_jindu"));
    progress->setTag(TAG_PROGRESS);
    this->addChild(progress);
    
    CCLabelTTF* label=CCLabelTTF::create("", "KaiTi.ttf", 25);
    CCLabelTTF::create("", "KaiTi.ttf", 20, CCSizeMake(326, 58), kCCTextAlignmentCenter);
    label->setPosition(S_RM->getPositionWithName("kapian_jindu"));
    label->setTag(TAG_PROGRESS_LABEL);
    label->setColor(ccc3(116, 61, 37));
    this->addChild(label);
    
    
    
    //初始化table层
    KapianTableLayer* kapianTableLayer=KapianTableLayer::create(m_mode);
    kapianTableLayer->setTag(kTableLayer);
    this->addChild(kapianTableLayer);
    
    //进度
    this->processAnimate(m_mode);
    
    
    //打开android按键响应
    this->setKeypadEnabled(true);
    
    return true;
}

void KapianScene::processAnimate(KapianDataMode mode){
    float process;
    int count;
    int sum;
    CCString* labelTxt;
    switch (mode) {
        case kHanzi:
            getProcess("hanzi","isCollected",&count,&sum);
            break;
        case kTupian:
            getProcess("ciyu","isCollected",&count,&sum);
            break;
        default:
            break;
    }
    process=((float)sum)/count*100;
    labelTxt=CCString::createWithFormat("%d/%d",sum,count);
    
    if (process>0&&process<5) {
        process=5;
    }
    CCProgressFromTo *progressFromTo = CCProgressFromTo::create(1.0f, 0, process);
    CCProgressTimer* progress=(CCProgressTimer*)this->getChildByTag(TAG_PROGRESS);
    progress->runAction(progressFromTo);
    
    CCLabelTTF* label=(CCLabelTTF*)this->getChildByTag(TAG_PROGRESS_LABEL);
    label->setString(labelTxt->getCString());
}

void KapianScene::setMenuToggle(KapianShowMode mode){
    switch (mode) {
        case kCollect:
            m_cltToggle->setSelectedIndex(1);
            m_allToggle->setSelectedIndex(0);
            break;
        case kAll:
            m_cltToggle->setSelectedIndex(0);
            m_allToggle->setSelectedIndex(1);
            break;
        default:
            break;
    }
}


void KapianScene::changeShowModeCall(CCNode* pSender){
    KapianShowMode showMode=kAll;
    CCMenuItemToggle* sender=(CCMenuItemToggle*)pSender;
    if (sender==m_allToggle) {
        showMode=kAll;
    }else if(sender==m_cltToggle){
        
        showMode=kCollect;
    }
    this->setMenuToggle(showMode);//放在前面解决CCMenuItemToggle切换自身
    if (m_showMode==showMode) {
        return;
    }else{
        m_showMode=showMode;
        ((KapianTableLayer*)(this->getChildByTag(kTableLayer)))->changeShowMode(m_showMode);
    }
}

void KapianScene::goBack(CCNode* pSender){
    CCDirector::sharedDirector()->replaceScene(HomeScene::scene());
}

CCScene* KapianHanziScene::scene()
{
    return KapianScene::scene(kHanzi);
}

CCScene* KapianTupianScene::scene()
{
    return KapianScene::scene(kTupian);
}

void KapianScene::keyBackClicked(){
    this->goBack(NULL);
}

void KapianScene::keyMenuClicked(){
    
}
