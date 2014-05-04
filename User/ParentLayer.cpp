//
//  UserLoginLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-4-3.
//
//

#include "ParentLayer.h"
#include "LoadingScene.h"
#include "XieziScene.h"
#include "KapianScene.h"
#include "CardShelfScene.h"
#include "Button9.h"
#include "SettingsLayer.h"
#include "HomeScene.h"

enum
{
    kTagShizi = 0,
    kTagZika,
    kTagYuedu,
    kTagXiezi,
    kTagTuka,
    kTagTansuo,
    kTagSettings,
    kTagMenu,
    kTagMenuClose
};

const std::string g_menuItem[kTagTansuo+1] = {
    "shizi.png","zika.png","yuedu.png","xiezi.png","tuka.png","tansuo.png"
};

#define SPEED_THRESHOLD 1

bool ParentLayer::init(){
    if (!DialogLayer::init()) {
        return false;
    }
    
    this->setTitle("家长设置");
    
    //设置背景透明度
    this->setOpacity(0);
    
//    this->hideClose();
    
    for (int i=0; i<kTagTansuo+1; i++) {
        CCSprite* sprite=CCSprite::createWithSpriteFrameName(g_menuItem[i].c_str());
        CCMenuItemSprite* item=CCMenuItemSprite::create(sprite, sprite, this, menu_selector(ParentLayer::menuCallback));
        item->setPosition(312+i%3*200, (5-i)/3*120+335);
        item->setTag(i+kTagShizi);
        this->addMenuItem(item);
    }
    
    Button9* button=Button9::create("设  置",CCSizeMake(80,40),20);
    CCMenuItemSprite* item=CCMenuItemSprite::create(button, button, this, menu_selector(ParentLayer::menuCallback));
    item->setPosition(514, 240);
    item->setTag(kTagSettings);
    this->addMenuItem(item);
    
    m_toggle=CCSprite::createWithSpriteFrameName("down.png");
    m_toggle->setPosition(ccp(512, 140));
    this->addChild(m_toggle);
    
    m_showParent=false;
    
    this->setPosition(0, 611);
    
    //检测是否从未打开
    int isNotFirstOpen=S_UD->getIntegerForKey(NEVER_OPEN_PARENT,0);
    if (isNotFirstOpen==0) {
        m_toggle->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(0.5),CCFadeIn::create(0.5),NULL)));
    }
    
    return true;
}

bool ParentLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCPoint nodePoint=m_toggle->convertTouchToNodeSpace(pTouch);
    CCRect rect=CCRectMake(0, 0, m_toggle->getContentSize().width, m_toggle->getContentSize().height);
    if (rect.containsPoint(nodePoint)) {
        m_isTouchToggle=true;
    }else{
        m_isTouchToggle=false;
        if (!m_showParent) {
            return false;
        }
    }
    
    m_fTouchMoveDistance=0;
    m_fTouchDirectionDistance=0;
    m_cctimeStart=TimeUtils::millisecondNow();
    
    return true;
}

void ParentLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (m_isTouchToggle) {
        CCPoint point=pTouch->getLocation();
        CCPoint prePoint=pTouch->getPreviousLocation();
        
        float moveDistance = point.y - prePoint.y;
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
        m_fTouchMoveDistance+=fabsf(moveDistance);
        
        CCPoint pointChange=ccp(0,moveDistance);
        if(this->getPosition().y+moveDistance<0){
            pointChange=ccp(0, moveDistance/2);
        }
        this->setPosition(ccpAdd(this->getPosition(), pointChange));
    }
}

void ParentLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    if (m_isTouchToggle) {
        if (m_fTouchMoveDistance<=2) {
            //点击事件
            m_showParent=!m_showParent;
        }else{
            //最后一个方向的速度
            cc_timeval endTime=TimeUtils::millisecondNow();
            int timesInterval=(int)((endTime.tv_sec-m_cctimeStart.tv_sec)*1000+(endTime.tv_usec-m_cctimeStart.tv_usec)/1000);
            float speed=(float)m_fTouchDirectionDistance/timesInterval;
            
            if(fabsf(speed)>SPEED_THRESHOLD){
                if(speed>SPEED_THRESHOLD){
                    m_showParent=false;
                }else if(speed<-SPEED_THRESHOLD){
                    m_showParent=true;
                }
            }else if(fabsf(m_fTouchDirectionDistance)>150){
                if(m_fTouchDirectionDistance>150){
                    m_showParent=false;
                }else if(m_fTouchDirectionDistance<-150){
                    m_showParent=true;
                }
            }
        }
        this->stopAllActions();
        if (m_showParent) {
            this->showOrHide(true);
        }else{
            this->showOrHide(false);
        }
    }
}

void ParentLayer::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    this->ccTouchEnded(pTouch, pEvent);
}

void ParentLayer::menuCallback(CCObject *obj){
    if (isCloseItem(obj)) {
        this->showOrHide(false);
    }else{
        DialogLayer::menuCallback(obj);
        CCNode* node=(CCNode*)obj;
        
        m_tag=node->getTag();
        if (m_tag==kTagSettings) {
            this->replaceDialog(SettingsLayer::create());
        }else{
            this->showOrHide(false);
            this->scheduleOnce(schedule_selector(ParentLayer::timingReplace), 0.6);
        }
    }
}

void ParentLayer::timingReplace(float t){
    switch (m_tag) {
        case kTagShizi:{
            S_DR->replaceScene(LoadingScene::scene("RenwuScene",false));
        }
            break;
        case kTagXiezi:{
            S_DR->replaceScene(KapianHanziScene::scene(kKapianShowModeCollect));
        }
            break;
        case kTagZika:
            S_DR->replaceScene(KapianHanziScene::scene(kKapianShowModeAll));
            break;
        case kTagTuka:
            S_DR->replaceScene(KapianTupianScene::scene(kKapianShowModeAll));
            break;
        case kTagYuedu:
            S_DR->replaceScene(CardShelfScene::scene());
            break;
        default:
            break;
    }
}

void ParentLayer::showOrHide(bool show){
    if (show) {
        S_UD->setIntegerForKey(NEVER_OPEN_PARENT, 1);
        S_UD->flush();
        m_toggle->stopAllActions();
        m_showParent=true;
        m_toggle->runAction(CCFadeOut::create(0.6));
        this->runAction(CCMoveTo::create(0.6, ccp(0, 0)));
        //            m_toggle->setDisplayFrame(S_SF->spriteFrameByName("up.png"));
    }else{
        m_showParent=false;
        m_toggle->runAction(CCFadeIn::create(0.6));
        this->runAction(CCMoveTo::create(0.6, ccp(0, 611)));
        //            m_toggle->setDisplayFrame(S_SF->spriteFrameByName("down.png"));
    }
}