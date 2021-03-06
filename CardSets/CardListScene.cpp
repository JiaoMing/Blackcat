#include "CardListScene.h"
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


CCScene* CardListScene::scene()
{
    CCScene *scene = CCScene::create();
    CardListScene *layer = CardListScene::create();
    scene->addChild(layer);
    return scene;
}

CardListScene::CardListScene() {

}

CardListScene::~CardListScene() {
    CCString *path = CCString::createWithFormat("%secards/%s", CCFileUtils::sharedFileUtils()->getWritablePath().c_str(),CURRENT_ECARDPAGE->getimg().c_str());
    CCTextureCache::sharedTextureCache()->removeTextureForKey(path->getCString());
}

bool CardListScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    //待修改
    m_pageSpriteRect=CCRectMake(0,0,W_SIZE.width,W_SIZE.height);
    
    CCSprite* spriteBg=CCSprite::createWithSpriteFrameName("tuce_bg.png");
    spriteBg->setPosition(S_RM->getJpgBgPosition());
    this->addChild(spriteBg);
    
    CCSprite* spritePage=CCSprite::create(CCString::createWithFormat("%secards/%s", CCFileUtils::sharedFileUtils()->getWritablePath().c_str(),CURRENT_ECARDPAGE->getimg().c_str())->getCString());
    spritePage->setPosition(S_RM->getJpgBgPosition());
    float scale=W_SIZE.height/spritePage->getContentSize().height;
    spritePage->setScale(scale);
    this->addChild(spritePage);
    
    CCSprite* spriteKuang=CCSprite::createWithSpriteFrameName("tuce.png");
    spriteKuang->setPosition(S_RM->getJpgBgPosition());
    this->addChild(spriteKuang);
    
    CCMenu* pMenu = CCMenu::create();
    pMenu->setPosition( CCPointZero );
    CCSprite* fanhui=CCSprite::createWithSpriteFrameName("fanhui.png");
    CCMenuItemSprite* menuItem_back = CCMenuItemSprite::create(fanhui,
                                                               fanhui,
                                                               this,
                                                               menu_selector(CardListScene::menuCallback) );
    menuItem_back->setPosition(S_RM->getPositionWithName("global_back"));
    pMenu->addChild(menuItem_back, 1, TAG_MENU_BACK);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 3, TAG_MENU_BACK);
    
    float minX=(W_SIZE.width-spritePage->getContentSize().width*scale)/2;
    for (int i=0; i<S_CM->getECards()->size(); i++) {
        ECard* ecard = (*S_CM->getECards())[i];
        CCLabelTTF *labelWord = CCLabelTTF::create(ecard->getzi().c_str(), "KaiTi.ttf", S_RM->getPositionWithName("CardList_wordFontSize").x);
        labelWord->setColor(ccc3(0, 0, 0));
        CCMenuItemLabel* labelItem=CCMenuItemLabel::create(labelWord, this,menu_selector(CardListScene::menuCallback));
        labelItem->setAnchorPoint(CCPointZero);
        float x=minX+ecard->getx()*scale+2;
        float y=S_DR->getWinSize().height-ecard->gety()*scale-labelWord->getContentSize().height-2;
        labelItem->setPosition(ccp(x, y));
        labelItem->setTag(ecard->gethid());
        pMenu->addChild(labelItem);
    }
    
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

void CardListScene::onEnter(){
    CCLayer::onEnter();
    S_DR->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    BaiduStat::onStatEvent(kBaiduOneEventStart,"SceneRetain","CardListScene");
}

void CardListScene::onExit(){
    CCLayer::onExit();
    S_DR->getTouchDispatcher()->removeDelegate(this);
    S_DR->setDepthTest(false);
    BaiduStat::onStatEvent(kBaiduOneEventEnd,"SceneRetain","CardListScene");
}

bool CardListScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_fTouchMoveDistance=0;
    m_fTouchDirectionDistance=0;
    m_cctimeStart=TimeUtils::millisecondNow();
    return true;
}

void CardListScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
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

void CardListScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
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
            CCScene* pScene=CCTransitionPageTurn::create(0.8f,this->scene(),newPageIndex<lastPageIndex);
            S_DR->setDepthTest(true);
            S_DR->replaceScene(pScene);
            S_AE->playEffect("fanye.mp3");
            return;
        }
    }
    
    //点击空白
    S_AE->playEffect("default.mp3");
}

void CardListScene::menuCallback(CCObject* pSender)
{
    CCMenuItem *menuItem = (CCMenuItem *)pSender;
    int tag=menuItem->getTag();
    if (menuItem->getTag()==TAG_MENU_BACK) {
        S_DR->replaceScene(CardShelfScene::scene());
    }else{
        S_AE->playEffect("E4.mp3");
        
        XieziCoverLayer* xieziCoverLayer=XieziCoverLayer::create(tag,NULL);
        xieziCoverLayer->setDelegate(this);
        this->addChild(xieziCoverLayer,INT_MAX);
    }
}

void CardListScene::keyBackClicked(){
    S_DR->replaceScene(CardShelfScene::scene());
}

void CardListScene::keyMenuClicked(){
    
}