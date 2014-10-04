//
//  PageViewScene.cpp
//  Blackcat
//
//  Created by haojunhua on 14-9-21.
//
//
#include "PageViewScene.h"
#include "CardManager.h"
#include "Utils.h"
#include "CardShelfScene.h"
#include "XieziScene.h"
#include "SimpleAudioEngine.h"
#include "XieziCoverLayer.h"

using namespace CocosDenshion;

#define SPEED_THRESHOLD 1
#define TAG_MENU_BACK -1

USING_NS_CC;
USING_NS_CC_EXT;

PageViewScene::PageViewScene() {
    
}

PageViewScene::~PageViewScene() {
    CCString *path = CCString::createWithFormat("%secards/%s", CCFileUtils::sharedFileUtils()->getWritablePath().c_str(),CURRENT_ECARDPAGE->getimg().c_str());
    CCTextureCache::sharedTextureCache()->removeTextureForKey(path->getCString());
}

bool PageViewScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    //待修改
    m_pageSpriteRect=CCRectMake(0,0,W_SIZE.width,W_SIZE.height);
    
    CCMenu* pMenu = CCMenu::create();
    pMenu->setPosition( CCPointZero );
    CCSprite* fanhui=CCSprite::createWithSpriteFrameName("fanhui.png");
    CCMenuItemSprite* menuItem_back = CCMenuItemSprite::create(fanhui,
                                                               fanhui,
                                                               this,
                                                               menu_selector(PageViewScene::menuCallback) );
    menuItem_back->setPosition(S_RM->getPositionWithName("global_back"));
    pMenu->addChild(menuItem_back, 1, TAG_MENU_BACK);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 3, TAG_MENU_BACK);
    
    CCString* string=CCString::createWithFormat("%d/%d", CURRENT_ECARDPAGENUMBER+1, CURRENT_PAGECOUNT);
    CCLabelTTF *labelPageNumber = CCLabelTTF::create(string->getCString(), "KaiTi.ttf",  S_RM->getPositionWithName("CardList_pageNumberFontSize").x);
    labelPageNumber->setAnchorPoint(CCPointZero);
    labelPageNumber->setPosition(S_RM->getPositionWithName("CardList_pageNumberPosition"));
    labelPageNumber->setColor(ccc3(0, 0, 0));
    this->addChild(labelPageNumber,2);
    
    //打开android按键响应
    this->setKeypadEnabled(true);
    
    return true;
}

void PageViewScene::onEnter(){
    CCLayer::onEnter();
    S_DR->getTouchDispatcher()->addTargetedDelegate(this,0, false);
    BaiduStat::onStatEvent(kBaiduOneEventStart,"SceneRetain","PageViewScene");
}

void PageViewScene::onExit(){
    CCLayer::onExit();
    S_DR->getTouchDispatcher()->removeDelegate(this);
    S_DR->setDepthTest(false);
    BaiduStat::onStatEvent(kBaiduOneEventEnd,"SceneRetain","PageViewScene");
}

bool PageViewScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_fTouchMoveDistance=0;
    m_fTouchDirectionDistance=0;
    m_cctimeStart=TimeUtils::millisecondNow();
    return true;
}

void PageViewScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchLocation = pTouch->getLocation();
	CCPoint lastLocation =pTouch->getPreviousLocation();
    if (m_pageSpriteRect.containsPoint(touchLocation)&&m_pageSpriteRect.containsPoint(lastLocation)) {
        float moveDistance = touchLocation.x - lastLocation.x;
        if((moveDistance>0&&CURRENT_ECARDPAGENUMBER<0)||(moveDistance<0&&CURRENT_ECARDPAGENUMBER>CURRENT_PAGECOUNT-1)){//禁止第一页往回看以及最后一页往前看
            return;
        }
        if(moveDistance>0&&!m_bDirection){
            m_fTouchDirectionDistance=0;
            m_bDirection=true;
            m_cctimeStart=TimeUtils::millisecondNow();
            
        }else if(moveDistance<0&&m_bDirection){
            m_fTouchDirectionDistance=0;
            m_bDirection=false;
            m_cctimeStart=TimeUtils::millisecondNow();
            
        }else{
            m_fTouchDirectionDistance+=moveDistance;
        }
    }
}

void PageViewScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchLocation = pTouch->getLocation();
    if (m_pageSpriteRect.containsPoint(touchLocation)) {
        CCSize s=CCDirector::sharedDirector()->getWinSize();
        int lastPageIndex=CURRENT_ECARDPAGENUMBER;
        int newPageIndex=lastPageIndex;
        //最后一个方向的速度
        cc_timeval endTime=TimeUtils::millisecondNow();
        int timesInterval=(int)(endTime.tv_sec-m_cctimeStart.tv_sec)*1000+(endTime.tv_usec-m_cctimeStart.tv_usec)/1000;
        float speed=(float)m_fTouchDirectionDistance/timesInterval;
        
        if(fabsf(speed)>SPEED_THRESHOLD){
            //需再加上起点判断
            if(speed>SPEED_THRESHOLD&&lastPageIndex>0){
                newPageIndex-=1;
            }else if(speed<-SPEED_THRESHOLD&&lastPageIndex<CURRENT_PAGECOUNT-1){
                newPageIndex+=1;
            }
        }else if(fabsf(m_fTouchDirectionDistance)>s.width/4){
            if(m_fTouchDirectionDistance>s.width/4&&lastPageIndex>0){
                newPageIndex-=1;
            }else if(m_fTouchDirectionDistance<-s.width/4&&lastPageIndex<CURRENT_PAGECOUNT-1){
                newPageIndex+=1;
            }
        }
        
        if(newPageIndex!=lastPageIndex){
            S_CM->setCurrentECardPage(newPageIndex);
            CCScene* pScene=CCTransitionPageTurn::create(0.8f,this->changePage(),newPageIndex<lastPageIndex);
            S_DR->setDepthTest(true);
            this->beforeReplace();
            S_DR->replaceScene(pScene);
            S_AE->playEffect("fanye.mp3");
            return;
        }
    }
    
    //点击空白
//    S_AE->playEffect("default.mp3");
}

void PageViewScene::menuCallback(CCObject* pSender)
{
    CCMenuItem *menuItem = (CCMenuItem *)pSender;
    int tag=menuItem->getTag();
    if (menuItem->getTag()==TAG_MENU_BACK) {
        S_AE->stopBackgroundMusic();
        S_AE->playBackgroundMusic("bg_musicbox.mp3",true);
        S_DR->replaceScene(CardShelfScene::scene());
    }else{
        S_AE->playEffect("E4.mp3");
        
        XieziCoverLayer* xieziCoverLayer=XieziCoverLayer::create(tag,NULL);
        xieziCoverLayer->setDelegate(this);
        this->addChild(xieziCoverLayer,INT_MAX);
    }
}

void PageViewScene::keyBackClicked(){
    S_DR->replaceScene(CardShelfScene::scene());
}

void PageViewScene::keyMenuClicked(){
    
}
