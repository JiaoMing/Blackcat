//
//  DialogLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-4-10.
//
//

#include "DialogLayer.h"
#include "resource.h"


bool DialogLayer::init(const ccColor4B& color){
    if (!CoverLayer::init(color)) {
        return false;
    }
    m_bg=CCSprite::createWithSpriteFrameName("dialog_bg.png");
    m_bg->setPosition(S_RM->getJpgBgPosition());
    this->addChild(m_bg);
    
    CCSprite* close=CCSprite::createWithSpriteFrameName("dialog_close.png");
    m_closeItem=CCMenuItemSprite::create(close, close, this, menu_selector(DialogLayer::menuCallback));
    m_closeItem->setPosition(S_RM->getPositionWithName("dialog_close"));
    m_menu=CCMenu::create(m_closeItem,NULL);
    m_menu->setPosition(CCPointZero);
    this->addChild(m_menu);
    
    m_contentLayer=CCLayer::create();
    m_contentLayer->setPosition(S_RM->getPositionWithName("dialog_content"));
    this->addChild(m_contentLayer);
    
    return true;
}

void DialogLayer::onEnter()
{
    CoverLayer::onEnter();
    scheduleOnce(schedule_selector(DialogLayer::enableTouch), 0.1);
}

void DialogLayer::onExit()
{
    CoverLayer::onExit();
}

int DialogLayer::topHandlerPriority(){
    return m_dialogLayerDelegate->topHandlerPriority()-1;
}

void DialogLayer::setDelegate(DialogLayerDelegate *delegate){
    CoverLayer::setDelegate(delegate);
    m_dialogLayerDelegate=delegate;
}

void DialogLayer::replaceDialog(DialogLayer *dialogLayer){
    dialogLayer->setDelegate(m_dialogLayerDelegate);
    this->getParent()->addChild(dialogLayer,ORDER_DIALOG);
    this->removeFromParent();
}

void DialogLayer::enableTouch(){
    m_menu->setHandlerPriority(this->getDelegate()->topHandlerPriority()-1);
}

void DialogLayer::setTitle(const char *title){
    if (m_title==NULL) {
        m_title=CCLabelTTF::create(title, "KaiTi.ttf", 40);
//        m_title->setFontFillColor(ccc3(255, 255, 255));
//        m_title->enableStroke(ccc3(0, 0, 0), 0.5);
        m_title->enableShadow(CCSizeMake(2, -2), 1, 0.5);
        m_title->setPosition(S_RM->getPositionWithName("dialog_title"));
        this->addChild(m_title);
    }else{
        m_title->setString(title);
    }
}


void DialogLayer::menuCallback(CCObject* obj){
    if (m_closeItem==obj) {
        if (m_forwardDialogLayer==NULL) {
            this->removeFromParent();
            S_ALP->stop();
            m_dialogLayerDelegate->dialogCloseCallBack();
        }else{
#pragma message "m_forwardDialogLayer不能初始化问题"
            this->replaceDialog(m_forwardDialogLayer);
        }
    }
}