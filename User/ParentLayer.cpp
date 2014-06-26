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
#include "CopyRightLayer.h"
enum
{
    kTagZika=0,
    kTagTuka,
    kTagYuedu,
    kTagSettings,
    kTagAbout
};

const std::string g_menuItem[kTagYuedu+1] = {
    "zika","tuka","yuedu"
};

#define SPEED_THRESHOLD 1

bool ParentLayer::init(){
    if (!DialogLayer::init()) {
        return false;
    }
    
    this->setTitle("家长菜单");
    
    //设置背景透明度
    this->setOpacity(0);
    
//    this->hideClose();
    
    for (int i=0; i<kTagYuedu+1; i++) {
        CCString* str=CCString::createWithFormat("%s.png",g_menuItem[i].c_str());
        CCSprite* sprite=CCSprite::createWithSpriteFrameName(str->getCString());
        CCMenuItemSprite* item=CCMenuItemSprite::create(sprite, sprite, this, menu_selector(ParentLayer::menuCallback));
        
        CCString* strPoint=CCString::createWithFormat("parent_%s",g_menuItem[i].c_str());
        item->setPosition(S_RM->getPositionWithName(strPoint->getCString()));
        item->setTag(i+kTagZika);
        item->setScale(1.2);
        this->addMenuItem(item);
        
    }
    
    Button9* button=Button9::create("设  置",CCSizeMake(120,50),25);
    CCMenuItemSprite* item=CCMenuItemSprite::create(button, button, this, menu_selector(ParentLayer::menuCallback));
    item->setPosition(S_RM->getPositionWithName("parent_setting"));
    item->setTag(kTagSettings);
    this->addMenuItem(item);
    
    Button9* about=Button9::create("关  于",CCSizeMake(120,50),25);
    CCMenuItemSprite* aboutItem=CCMenuItemSprite::create(about, about, this, menu_selector(ParentLayer::menuCallback));
    aboutItem->setPosition(S_RM->getPositionWithName("parent_about"));
    aboutItem->setTag(kTagAbout);
    this->addMenuItem(aboutItem);
    
    m_toggle=CCSprite::createWithSpriteFrameName("down.png");
    m_toggle->setPosition(S_RM->getPositionWithName("parent_down"));
    this->addChild(m_toggle);
    
    m_showParent=false;
    
    this->setPosition(S_RM->getPositionWithName("parent_hide"));
    
    //检测是否从未打开
    int isNotFirstOpen=S_UD->getIntegerForKey(NEVER_OPEN_PARENT,0);
    string first= S_UD->getStringForKey("HomeScene_First");
#pragma message "注意结束heimao_11"
    if (first=="heimao_11"&&isNotFirstOpen==0) {
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
        }else if(m_tag==kTagAbout){
            CopyRightLayer* layer=CopyRightLayer::create();
            layer->setDelegate(this);
            this->addChild(layer,ORDER_DIALOG);
        }else{
            this->showOrHide(false);
            this->scheduleOnce(schedule_selector(ParentLayer::timingReplace), 0.6);
        }
    }
}

void ParentLayer::timingReplace(float t){
    switch (m_tag) {
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
        this->runAction(CCMoveTo::create(0.6, CCPointZero));
        //            m_toggle->setDisplayFrame(S_SF->spriteFrameByName("up.png"));
    }else{
        m_showParent=false;
        m_toggle->runAction(CCFadeIn::create(0.6));
        this->runAction(CCMoveTo::create(0.6, S_RM->getPositionWithName("parent_hide")));
        //            m_toggle->setDisplayFrame(S_SF->spriteFrameByName("down.png"));
    }
}