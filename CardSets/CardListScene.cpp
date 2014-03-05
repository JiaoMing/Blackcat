#include "CardListScene.h"
#include "CardManager.h"
#include "Utils.h"
#include "CardShelfScene.h"
#include "XieziScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;



#define SPEED_THRESHOLD 1
#define TAG_MENU_BACK -1

USING_NS_CC;
USING_NS_CC_EXT;

//左右切换
//class PageTransitionForward : public CCTransitionMoveInR
//{
//public:
//    static CCTransitionScene* create(float t, CCScene* s)
//    {
//        CCDirector::sharedDirector()->setDepthTest(true);
//        return CCTransitionMoveInR::create(t, s);
//    }
//};
//
//class PageTransitionBackward : public CCTransitionMoveInL
//{
//public:
//    static CCTransitionScene* create(float t, CCScene* s)
//    {
//        CCDirector::sharedDirector()->setDepthTest(true);
//        return CCTransitionMoveInL::create(t, s);
//    }
//};

class PageTransitionForward : public CCTransitionPageTurn
{
public:
    static CCTransitionScene* create(float t, CCScene* s)
    {
        CCDirector::sharedDirector()->setDepthTest(true);
        return CCTransitionPageTurn::create(t, s, false);
    }
};

class PageTransitionBackward : public CCTransitionPageTurn
{
public:
    static CCTransitionScene* create(float t, CCScene* s)
    {
        CCDirector::sharedDirector()->setDepthTest(true);
        return CCTransitionPageTurn::create(t, s, true);
    }
};

CCScene* CardListScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    CardListScene *layer = CardListScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

CardListScene::CardListScene() {}
CardListScene::~CardListScene() {
    CCString *path = CCString::createWithFormat("%secards/%s", CCFileUtils::sharedFileUtils()->getWritablePath().c_str(),CURRENT_ECARDPAGE->getbackground_image().c_str());
    CCTextureCache::sharedTextureCache()->removeTextureForKey(path->getCString());
}

// on "init" you need to initialize your instance
bool CardListScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCString* string=CCString::createWithFormat("%d/%d", CURRENT_ECARDPAGENUMBER+1, CURRENT_PAGECOUNT);
    
    CCSprite *spriteBg=CCSprite::createWithSpriteFrameName("tuce_bg.png");
    spriteBg->setPosition(S_RM->getJpgBgPosition());
    CCSprite *spritePage=CCSprite::create(CCString::createWithFormat("%secards/%s", CCFileUtils::sharedFileUtils()->getWritablePath().c_str(),CURRENT_ECARDPAGE->getbackground_image().c_str())->getCString());
    spritePage->setPosition(S_RM->getJpgBgPosition());
    float scale=S_DR->getWinSize().height/spritePage->getContentSize().height;
    spritePage->setScale(scale);
//    CCRect PageSize = spritePage->boundingBox();
//    CCLog("getMinX: %f, getMidX: %f, getMaxX: %f", PageSize.getMinX(), PageSize.getMidX(), PageSize.getMaxX());
    //CCLog("getMinY: %f, getMidY: %f, getMaxY: %f", PageSize.getMinY(), PageSize.getMidY(), PageSize.getMaxY());
//    m_pageSpriteRect = CCRectMake(PageSize.getMinX(), PageSize.getMinY(), spritePage->getContentSize().width, spritePage->getContentSize().height);
    //待修改
    m_pageSpriteRect=CCRectMake(0,0,S_DR->getWinSize().width,S_DR->getWinSize().height);
    spriteBg->addChild(spritePage, 1);
    CCSprite *spriteKuang=CCSprite::createWithSpriteFrameName("tuce.png");
    spriteKuang->setPosition(S_RM->getJpgBgPosition());
    spriteBg->addChild(spriteKuang,2);
    this->addChild(spriteBg,1);
    
    CCMenu* pMenu = CCMenu::create(NULL);
    pMenu->setPosition( CCPointZero );
    CCSprite *spriteMenuBackUnpressed=CCSprite::create("fanhui.png");
    CCMenuItemSprite *menuItem_back = CCMenuItemSprite::create(spriteMenuBackUnpressed,
                                                               NULL,
                                                               this,
                                                               menu_selector(CardListScene::menuCallback) );
    menuItem_back->setPosition(S_RM->getPositionWithName("global_back"));
    pMenu->addChild(menuItem_back, 1, TAG_MENU_BACK);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 3, TAG_MENU_BACK);
    
    
    float minX=(S_DR->getWinSize().width-spritePage->getContentSize().width*scale)/2;
    for (int i=0; i<S_CM->getECards()->size(); i++) {
        ECard* ecard = (*S_CM->getECards())[i];
        CCLabelTTF *labelWord = CCLabelTTF::create(ecard->getcontent().c_str(), "KaiTi.ttf", S_RM->getPositionWithName("CardList_wordFontSize").x);
        labelWord->setColor(ccc3(0, 0, 0));
        CCMenuItemLabel* labelItem=CCMenuItemLabel::create(labelWord, this,menu_selector(CardListScene::menuCallback));
        labelItem->setAnchorPoint(CCPointZero);
        float x=minX+ecard->getx()*scale+2;
        float y=S_DR->getWinSize().height-ecard->gety()*scale-labelWord->getContentSize().height-2;
        labelItem->setPosition(ccp(x, y));
        labelItem->setTag(ecard->getwid());
        pMenu->addChild(labelItem);
    }
    
    CCLabelTTF *labelPageNumber = CCLabelTTF::create(string->getCString(), "KaiTi.ttf",  S_RM->getPositionWithName("CardList_pageNumberFontSize").x);
    labelPageNumber->setAnchorPoint(CCPointZero);
    labelPageNumber->setPosition(S_RM->getPositionWithName("CardList_pageNumberPosition"));
    labelPageNumber->setColor(ccc3(0, 0, 0));
    this->addChild(labelPageNumber,2);
    
    //打开android按键响应
    this->setKeypadEnabled(true);
    
    this->setTouchEnabled(true);
    return true;
}

void CardListScene::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
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
    CCPoint touchLocation = pTouch->getLocationInView();
	touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
	CCPoint lastLocation =pTouch->getPreviousLocationInView();
	lastLocation = CCDirector::sharedDirector()->convertToGL( lastLocation );
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
    CCPoint original_touchLocation = pTouch->getLocationInView();
	CCPoint touchLocation = CCDirector::sharedDirector()->convertToGL( original_touchLocation );
    if (m_pageSpriteRect.containsPoint(touchLocation)) {
        CCSize s=CCDirector::sharedDirector()->getWinSize();
        int lastPageIndex=CURRENT_ECARDPAGENUMBER;
        int newPageIndex=lastPageIndex;
        //最后一个方向的速度
        cc_timeval endTime=TimeUtils::millisecondNow();
        int timesInterval=(endTime.tv_sec-m_cctimeStart.tv_sec)*1000+(endTime.tv_usec-m_cctimeStart.tv_usec)/1000;
        float speed=(float)m_fTouchDirectionDistance/timesInterval;
        
        if(fabsf(speed)>SPEED_THRESHOLD){
            //需再加上起点判断
            if(speed>SPEED_THRESHOLD&&lastPageIndex>0){
                newPageIndex-=1;
//                CCLog("迅速左右滑: %d", newPageIndex);
                updatePageReadNumber(newPageIndex);
                prevPageTurn();
                return;
            }else if(speed<-SPEED_THRESHOLD&&lastPageIndex<CURRENT_PAGECOUNT-1){
                newPageIndex+=1;
//                CCLog("迅速右左滑: %d", newPageIndex);
                updatePageReadNumber(newPageIndex);
                nextPageTurn();
                return;
            }
        }else if(fabsf(m_fTouchDirectionDistance)>s.width/4){
            if(m_fTouchDirectionDistance>s.width/4&&lastPageIndex>0){
                newPageIndex-=1;
//                CCLog("左右滑: %d", newPageIndex);
                updatePageReadNumber(newPageIndex);
                prevPageTurn();
                
                return;
            }else if(m_fTouchDirectionDistance<-s.width/4&&lastPageIndex<CURRENT_PAGECOUNT-1){
                newPageIndex+=1;
//                CCLog("右左滑: %d", newPageIndex);
                updatePageReadNumber(newPageIndex);
                nextPageTurn();
                return;
            }
        }
    }
    
    //点击空白
    SimpleAudioEngine::sharedEngine()->playEffect("default.mp3");
    
//    for (int i=0; i<S_CM->getECards()->size(); i++) {
//        ECard* ecard = (*S_CM->getECards())[i];
//        //CCLog("页码: %d, 卡片: %s, 位置: %d,%d",CURRENT_ECARDPAGENUMBER+1,ecard->getcontent().c_str(),ecard->getx(),ecard->gety());
//        CCRect ecardRectArea =  CCRectMake(ecard->getx(),ecard->gety(),ecard->getsize(),ecard->getsize());
//        if (ecardRectArea.containsPoint(original_touchLocation)) {
//            if (ecard->getwid()!=0) {
//                CCDirector::sharedDirector()->pushScene(XieziScene::scene(ecard->getwid()));
//            }
//            SimpleAudioEngine::sharedEngine()->playEffect("E4.mp3");
//            return;
//        }else{
//            if (i==S_CM->getECards()->size()-1) {
//                
//            }
//        }
//    }
}

void CardListScene::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void CardListScene::updatePageReadNumber(int pageNumber) {
    S_CM->setCurrentECardPage(pageNumber);
}

void CardListScene::nextPageTurn()
{
    CCScene* pScene = PageTransitionForward::create(0.8f, scene());
    if (pScene)
    {
        CCDirector::sharedDirector()->replaceScene(pScene);
        SimpleAudioEngine::sharedEngine()->playEffect("fanye.mp3");
    }
}

void CardListScene::prevPageTurn()
{
    CCScene* pScene = PageTransitionBackward::create(0.8f, scene());
    if (pScene)
    {
        CCDirector::sharedDirector()->replaceScene(pScene);
        SimpleAudioEngine::sharedEngine()->playEffect("fanye.mp3");
    }
}

void CardListScene::menuCallback(CCObject* pSender)
{
    CCMenuItem *menuItem = (CCMenuItem *)pSender;
    int tag=menuItem->getTag();
    if (menuItem->getTag()==TAG_MENU_BACK) {
        CCDirector::sharedDirector()->replaceScene(CardShelfScene::scene());
    }else{
        SimpleAudioEngine::sharedEngine()->playEffect("E4.mp3");
        CCDirector::sharedDirector()->pushScene(XieziScene::scene(tag));
    }
}

void CardListScene::keyBackClicked(){
    S_DR->replaceScene(CardShelfScene::scene());
}

void CardListScene::keyMenuClicked(){
    
}