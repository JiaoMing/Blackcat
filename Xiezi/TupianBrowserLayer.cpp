//
//  TupianBrowserLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-1-2.
//
//

#include "TupianBrowserLayer.h"
#include "ResManager.h"
#include "resource.h"
#include "Hanzi.h"
#include "XieziScene.h"
#include "Utils.h"
#include "KapianCollectLayer.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

enum{
    kTagCurrentTupianSprite=0,
    kTagMenu,
    kTagTupianTableLayer,
    kTagWenziMenu
};

TupianBrowserLayer::TupianBrowserLayer(){
    m_isTableShow=true;
}

TupianBrowserLayer::~TupianBrowserLayer(){
    if (m_tupian!=NULL) {
        string path=CCFileUtils::sharedFileUtils()->getWritablePath()+m_tupian->geticonPath();
        CCTextureCache::sharedTextureCache()->removeTextureForKey(path.c_str());
    }
}

TupianBrowserLayer* TupianBrowserLayer::create(TupianBrowserDelegate* TupianBrowserDelegate,Tupian *tupian){
    TupianBrowserLayer* layer=new TupianBrowserLayer();
    layer->m_tupianBrowserDelegate=TupianBrowserDelegate;
    layer->m_tupian=tupian;
    if (layer && layer->init())
    {
        layer->autorelease();
    }
    else
    {
        delete layer;
        layer = NULL;
    }
    return layer;
}

bool TupianBrowserLayer::init(){
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 127), W_SIZE.width, W_SIZE.height)) {
        return false;
    }
    
    this->setMainTupianSprite(true);
    
    CCSprite* x=CCSprite::create("cha.png");
    x->setScale(1.5);
    CCMenuItemSprite* xItem=CCMenuItemSprite::create(x, x, this, menu_selector(TupianBrowserLayer::menuCallback));
    xItem->setPosition(S_RM->getPositionWithName("tupian_close"));
    
    CCMenu* menu=CCMenu::create(xItem,NULL);
    menu->setPosition(CCPointZero);
    menu->setTag(kTagMenu);
    this->addChild(menu);
    
    CCLayerColor *tableBg = CCLayerColor::create(ccc4(0, 0, 0, 180),W_SIZE.width,220);
    tableBg->setPosition(CCPointZero);
    tableBg->setTag(kTagTupianTableLayer);
    this->addChild(tableBg);
    
    TupianTablePosition tupianTablePosition=TupianTablePosition("tupian_table","tupian_table_size","tupian_table_cell_size","xiezi_Tupian_table_cell_thumb_size","xiezi_Tupiankuang_in_cell","xiezi_Tupianthumb_in_cell",kCCScrollViewDirectionHorizontal);
    m_tupianTabelLayer=TupianTableLayer::create(this,kTupian,m_tupian,tupianTablePosition);
    m_tupianTabelLayer->setPosition(ccp(0, 4));
    tableBg->setPosition(ccp(0,-280));
    tableBg->addChild(m_tupianTabelLayer);
    
    m_isTableShow=false;
    
    //打开android按键响应
    this->setKeypadEnabled(true);
    
    return true;
    
}

void TupianBrowserLayer::setMainTupianSprite(bool isFromInit){
    
    S_AE->playEffect((CCFileUtils::sharedFileUtils()->getWritablePath()+m_tupian->getcnAudioPath()).c_str());

    CCSprite* sprite=(CCSprite*)this->getChildByTag(kTagCurrentTupianSprite);
    if (sprite!=NULL) {
        this->removeChild(sprite, false);
    }
    
#define COLLECTED_SPRITE  string path=CCFileUtils::sharedFileUtils()->getWritablePath()+m_tupian->geticonPath();\
    sprite=CCSprite::create(path.c_str());\
    sprite->setPosition(S_RM->getPositionWithName("tupian_fangda"));\
    sprite->setTag(kTagCurrentTupianSprite);\
    sprite->setScale(S_RM->getScaleWithName("tupian_fangda"));\
//    CCSize mainSize=S_RM->getSizeWithName("tupian_main_size");\
//    m_scale=(mainSize.width+20)/600;\
    
    COLLECTED_SPRITE;
    this->addChild(sprite);
    
    if (!m_tupian->getisCollected()) {
        
        int count=S_DM->getCount("ciyu","isCollected>0");
        if (count>=COLLECT_LIMIT) {
            GuideDialog* guideDialog=new GuideDialog();
            guideDialog->autorelease();
            guideDialog->setText("非常抱歉，收藏的卡片数量已经超出了免费版本限制，请检查账号状态。");
            guideDialog->setMode(kGuideDialogOk);
            m_gudieDialogLayer=GuideDialogLayer::create(kDialogWithText);
            m_gudieDialogLayer->setDelegate(this);
            this->addChild(m_gudieDialogLayer,ORDER_DIALOG);
            m_gudieDialogLayer->setGuideDialogData(guideDialog);
        }else{
            m_tupian->setIntisCollected(1);
            CCString *sql=CCString::createWithFormat("update ciyu set isCollected=1 where id=%d;",m_tupian->getid());
            S_DM->executeSql(sql->getCString());
            
            static_userDefaultIncrement(COLLECT_TUPIAN_COUNT,15);
            
            CCSprite* sprite;
            COLLECTED_SPRITE;
            
            KapianCollectLayer* kapianCollectLayer=KapianCollectLayer::create(sprite,kTupian);
            this->addChild(kapianCollectLayer);
            kapianCollectLayer->collectAnimate();
            
//            if(isFromInit){
//                m_tupianBrowserDelegate->reloadTupianTable();
//            }
//            else{
//                m_tupianTabelLayer->reloadData();
//            }
            
        }
    }
    
    if (m_wenziMenu!=NULL) {
        this->removeChild(m_wenziMenu, false);
    }
    
    m_wenziMenu=CCMenu::create();
    string ciyu=m_tupian->getText();
    const char* zi=ciyu.c_str();
    char p[ciyu.length()/3][4];
    
    string where="";
    
    vector<Hanzi*>* hanzis=new vector<Hanzi*>();
    
    CLAUSE_INIT;
    
    whereClause.push_back(HANZI_VERIFY_PASS);
    
    
    for (int i = 0,j=0; i < ciyu.length(); i=i+3,j++)
    {
        char end='\0';
        sprintf(p[j], "%c%c%c%c",zi[i],zi[i+1],zi[i+2],end);
        if(i>0)where.append(" or ");
        where.append(" (zi like '").append(p[j]).append("') ");
    }
    
    whereClause.push_back(where.c_str());
    S_DM->findScrollData(hanzis, "id,zi", whereClause, orderbyClause, groupByClause);
    
    m_delay=0.0f;
    for (int i=0; i< ciyu.length()/3;i++) {
        CCLabelTTF *txtLabel = CCLabelTTF::create(p[i], "KaiTi.ttf", 60.0);
        txtLabel->setColor(ccc3(200, 200, 200));
    
        CCSprite* wenzi=CCSprite::createWithSpriteFrameName("zikuang.png");
        CCMenuItemSprite* pwenziItem=CCMenuItemSprite::create(wenzi, NULL, this, menu_selector(TupianBrowserLayer::wenziCallback));
        pwenziItem->setTag(0);
        for (vector<Hanzi *>::iterator it = hanzis->begin(); it != hanzis->end(); it ++){
            if (NULL != *it)
            {
                Hanzi* hanzi=*it;
                if (hanzi->getzi() ==p[i]) {
                    pwenziItem->setTag(hanzi->getid());
                    txtLabel->setColor(ccc3(0, 0, 0));
                    break;
                }
            }
        }
        txtLabel->setPosition(S_RM->getRelativePosition("tupian_wenzi_label", wenzi->getContentSize().height));
        txtLabel->runAction(CCSequence::create(CCDelayTime::create(m_delay),CCScaleTo::create(0.25, 1.5),CCScaleTo::create(0.25, 1),NULL));
        pwenziItem->addChild(txtLabel);
        pwenziItem->setPosition(ccp(i*140,0));
        m_wenziMenu->addChild(pwenziItem);
        m_delay+=0.5;
    }
    
    m_wenziMenu->setPosition(ccp(W_SIZE.width/2-(ciyu.length()/3-1)*70,S_RM->getPositionWithName("tupian_wenzi_fangda").y));
    m_wenziMenu->setTag(kTagWenziMenu);
    this->addChild(m_wenziMenu);

}

void TupianBrowserLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
//    scheduleOnce(schedule_selector(TupianBrowserLayer::enableTouch), m_delay);
    scheduleOnce(schedule_selector(TupianBrowserLayer::enableTouch), 0);
    m_enterTime=TimeUtils::millisecondNow();
}

void TupianBrowserLayer::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void TupianBrowserLayer::keyBackClicked(){
    this->menuCallback(NULL);
}

void TupianBrowserLayer::keyMenuClicked(){
    
}


bool TupianBrowserLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

void TupianBrowserLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
    m_wenziMenu->setEnabled(false);//关闭按钮事件防止崩溃
    CCFiniteTimeAction* enableWenziMenuAction=CCCallFunc::create(this, callfunc_selector(TupianBrowserLayer::enableTouch));
    
    CCPoint point = pTouch->getLocationInView();
    CCPoint touchPoint = S_DR->convertToGL(point);
    
    
    CCNode* tableLayer=(CCNode*) this->getChildByTag(kTagTupianTableLayer);
    CCSprite* sprite=(CCSprite*)this->getChildByTag(kTagCurrentTupianSprite);
    bool notTouchTable=touchPoint.y>tableLayer->getContentSize().height+tableLayer->getPosition().y;
    CCSize spriteSize=sprite->getContentSize();
    CCPoint spritePoint=sprite->getPosition();
    spriteSize=CCSizeMake(spriteSize.width*sprite->getScale(), spriteSize.height*sprite->getScale());
    
    CCRect rect=CCRectMake(spritePoint.x-spriteSize.width/2,spritePoint.y-spriteSize.height/2 , spriteSize.width, spriteSize.height);
    if (notTouchTable&&!rect.containsPoint(touchPoint)) {
        //点击空白
        cc_timeval now=TimeUtils::millisecondNow();
        long interval = (now.tv_sec-m_enterTime.tv_sec)* 1000 + (now.tv_usec - m_enterTime.tv_usec)/1000;
        if(interval>=m_delay*1000){
            //如果时间超过一秒则关闭
            this->removeFromParentAndCleanup(true);
            m_tupianBrowserDelegate->hideBrowserCallBack();
        }
        return;
    }
    
    if (!m_isTableShow||notTouchTable) {
        m_isTableShow=!m_isTableShow;
    }
    if (m_isTableShow) {
        tableLayer->runAction(CCMoveTo::create(0.1, ccp(0,-50)));
        CCFiniteTimeAction* action=CCSpawn::create(CCScaleTo::create(0.1, S_RM->getScaleWithName("tupian_main")),CCMoveTo::create(0.1, S_RM->getPositionWithName("tupian_main")),NULL);
        sprite->runAction(action);
        m_wenziMenu->runAction(CCSequence::create(CCMoveTo::create(0.1, ccp(m_wenziMenu->getPosition().x, S_RM->getPositionWithName("tupian_wenzi").y)),enableWenziMenuAction,NULL));
    }else{
        tableLayer->runAction(CCMoveTo::create(0.1, ccp(0,-280)));
        CCFiniteTimeAction* action=CCSpawn::create(CCScaleTo::create(0.1, S_RM->getScaleWithName("tupian_fangda")),CCMoveTo::create(0.1, S_RM->getPositionWithName("tupian_fangda")),NULL);
        sprite->runAction(action);
        m_wenziMenu->runAction(CCSequence::create(CCMoveTo::create(0.1, ccp(m_wenziMenu->getPosition().x, S_RM->getPositionWithName("tupian_wenzi_fangda").y)),enableWenziMenuAction,NULL));
    }
}

void TupianBrowserLayer::enableTouch(){
    CCMenu* menu=(CCMenu*) this->getChildByTag(kTagMenu);
    menu->setHandlerPriority(kCCMenuHandlerPriority-1);
    if(m_tupianTabelLayer!=NULL)m_tupianTabelLayer->setHandlerPriority(kCCMenuHandlerPriority-1);
    m_wenziMenu->setHandlerPriority(kCCMenuHandlerPriority-1);
    m_wenziMenu->setEnabled(true);
}

void TupianBrowserLayer::menuCallback(CCObject* pSender){
    this->removeFromParentAndCleanup(true);
    m_tupianBrowserDelegate->hideBrowserCallBack();
}

void TupianBrowserLayer::wenziCallback(cocos2d::CCObject *pSender){
    m_hanziID=((CCNode*)pSender)->getTag();
    if (m_hanziID>0) {
        GuideDialog* guideDialog=new GuideDialog();
        guideDialog->autorelease();
        guideDialog->setText("要跳转到相应的汉字吗？");
        guideDialog->setMode(kGuideDialogYesOrNo);
        m_gudieDialogLayer=GuideDialogLayer::create(kDialogWithText);
        m_gudieDialogLayer->setDelegate(this);
        this->addChild(m_gudieDialogLayer);
        m_gudieDialogLayer->setGuideDialogData(guideDialog);
    }
}

void TupianBrowserLayer::tupianLoadCallBack(int count){
    if (count<=6) {
//        
    }
}

void TupianBrowserLayer::tupianTouchCallBack(Tupian* tupian,bool isOverLimit){
    if (isOverLimit) {
        GuideDialog* guideDialog=new GuideDialog();
        guideDialog->autorelease();
        guideDialog->setText("非常抱歉，收藏的卡片数量已经超出了免费版本限制，请检查账号状态。");
        guideDialog->setMode(kGuideDialogOk);
        GuideDialogLayer* gudieDialogLayer=GuideDialogLayer::create(kDialogWithText);
        gudieDialogLayer->setDelegate(this);
        this->addChild(gudieDialogLayer,ORDER_DIALOG);
        gudieDialogLayer->setGuideDialogData(guideDialog);
    }else{
        if (m_isTableShow) {
            this->m_tupian=tupian;
            this->setMainTupianSprite(false);
        }
    }
}

void TupianBrowserLayer::dialogCallBack(GuideDialogCMD cmd){
    switch (cmd) {
        case kDialogCMDYes:{
            if (m_tupianBrowserDelegate->isPushToXieziScene()) {
                CCDirector::sharedDirector()->pushScene(XieziScene::scene(m_hanziID));
            }else{
                CCDirector::sharedDirector()->replaceScene(XieziScene::scene(m_hanziID));
            }
        }
            break;
        case kDialogCMDBlank:
        case kDialogCMDNo:{
            
        }
            break;
        default:
            break;
    }
}
