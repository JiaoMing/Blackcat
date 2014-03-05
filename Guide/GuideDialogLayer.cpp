//
//  GuideDelegate.cpp
//  Blackcat
//
//  Created by haojunhua on 13-12-27.
//
//

#include "GuideDialogLayer.h"
#include "resource.h"
#include "ResManager.h"
#include <stdint.h>

enum {
    kDialogText=0,
    kDialogMenu
};
enum {
    kDialogMenuOk=0,
    kDialogMenuYes,
    kDialogMenuNo
};

GuideDialogLayer* GuideDialogLayer::create(GuideDialogType guideDialogType)
{
    GuideDialogLayer *layer = new GuideDialogLayer();
    layer->m_guideDialogType=guideDialogType;
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


void GuideDialogLayer::setDelegate(GuideDialogDelegate* guideDialogDelegate){
    GuideCoverLayer::setDelegate(guideDialogDelegate);
    this->m_guideDialogDelegate=guideDialogDelegate;
}


bool GuideDialogLayer::init(){
    if (!GuideCoverLayer::init()) {
        return false;
    }
    switch (m_guideDialogType) {
        case kDialogBtuOnly:{
            CCSprite* ok=CCSprite::createWithSpriteFrameName("dialog_ok.png");
            CCMenuItemSprite* okItem=CCMenuItemSprite::create(ok, ok, this, menu_selector(GuideDialogLayer::menuCallBack));
            okItem->setPosition(S_RM->getPositionWithName("dialog_ok"));
            okItem->setTag(kDialogMenuOk);
            
            CCSprite* yes=CCSprite::createWithSpriteFrameName("dialog_yes.png");
            CCMenuItemSprite* yesItem=CCMenuItemSprite::create(yes,yes, this, menu_selector(GuideDialogLayer::menuCallBack));
            yesItem->setPosition(S_RM->getPositionWithName("dialog_yes"));
            yesItem->setTag(kDialogMenuYes);
            
            CCSprite* no=CCSprite::createWithSpriteFrameName("dialog_no.png");
            CCMenuItemSprite* noItem=CCMenuItemSprite::create(no,no, this, menu_selector(GuideDialogLayer::menuCallBack));
            noItem->setPosition(S_RM->getPositionWithName("dialog_no"));
            noItem->setTag(kDialogMenuNo);
            
            CCMenu* menu=CCMenu::create(okItem,yesItem,noItem,NULL);
            menu->setPosition(CCPointZero);
            menu->setTag(kDialogMenu);
            this->addChild(menu);
        }
            break;
        case kDialogWithText:{
            CCSprite* dialogBG=CCSprite::createWithSpriteFrameName("dialog.png");
            dialogBG->setPosition(S_RM->getJpgBgPosition());
            dialogBG->setPosition(S_RM->getPositionWithName("dialog"));
            this->addChild(dialogBG);
            
            CCLabelTTF* text=CCLabelTTF::create("", "KaiTi.ttf", 25,CCSizeMake(465, 155),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
            text->setColor(ccc3(58,37,4));
            text->setTag(kDialogText);
            text->setPosition(S_RM->getPositionWithName("dialog_text"));
            this->addChild(text);

            CCSprite* ok=CCSprite::createWithSpriteFrameName("dialog_ok2.png");
            CCMenuItemSprite* okItem=CCMenuItemSprite::create(ok, ok, this, menu_selector(GuideDialogLayer::menuCallBack));
            okItem->setPosition(S_RM->getPositionWithName("dialog_ok2"));
            okItem->setTag(kDialogMenuOk);
            
            CCSprite* yes=CCSprite::createWithSpriteFrameName("dialog_yes2.png");
            CCMenuItemSprite* yesItem=CCMenuItemSprite::create(yes,yes, this, menu_selector(GuideDialogLayer::menuCallBack));
            yesItem->setPosition(S_RM->getPositionWithName("dialog_yes2"));
            yesItem->setTag(kDialogMenuYes);
            
            CCSprite* no=CCSprite::createWithSpriteFrameName("dialog_no2.png");
            CCMenuItemSprite* noItem=CCMenuItemSprite::create(no,no, this, menu_selector(GuideDialogLayer::menuCallBack));
            noItem->setPosition(S_RM->getPositionWithName("dialog_no2"));
            noItem->setTag(kDialogMenuNo);
            
            CCMenu* menu=CCMenu::create(okItem,yesItem,noItem,NULL);
            menu->setPosition(CCPointZero);
            menu->setTag(kDialogMenu);
            this->addChild(menu);
        }
        default:
            break;
    }
    
    return true;
}

void GuideDialogLayer::onEnter()
{
    GuideCoverLayer::onEnter();
    
    scheduleOnce(schedule_selector(GuideDialogLayer::enableTouch), 0.1);
}
void GuideDialogLayer::onExit()
{
    GuideCoverLayer::onExit();
    S_DR->getTouchDispatcher()->removeDelegate(this);
}

void GuideDialogLayer::enableTouch(){
    CCMenu* menu=(CCMenu*) this->getChildByTag(kDialogMenu);
    menu->setHandlerPriority(m_guideDialogDelegate->topHandlerPriority()-1);
}

bool GuideDialogLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
//    m_guideDialogDelegate->dialogCallBack(kDialogCMDBlank);
    return true;
}

void GuideDialogLayer::setGuideDialogData(GuideDialog* guideDialog){
    m_guideDialog=guideDialog;
    CCLabelTTF* text=(CCLabelTTF*) this->getChildByTag(kDialogText);
    if(text!=NULL)text->setString(guideDialog->getText().c_str());
    
    CCMenu* menu=(CCMenu*) this->getChildByTag(kDialogMenu);
    CCMenuItem* ok=(CCMenuItem*)menu->getChildByTag(kDialogMenuOk);
    CCMenuItem* yes=(CCMenuItem*)menu->getChildByTag(kDialogMenuYes);
    CCMenuItem* no=(CCMenuItem*)menu->getChildByTag(kDialogMenuNo);
    switch (m_guideDialog->getMode()) {
        case kGuideDialogOk:{
            ok->setVisible(true);
            yes->setVisible(false);
            no->setVisible(false);
        }
            break;
        case kGuideDialogYesOrNo:{
            ok->setVisible(false);
            yes->setVisible(true);
            no->setVisible(true);
        }
            break;
        default:
            break;
    }
    if (guideDialog->getDuration()>0) {
        //!!使用scheduleOnce在自动移除的时候会出现问题
        schedule(schedule_selector(GuideDialogLayer::sayFinished), guideDialog->getDuration());
    }
    
}

void GuideDialogLayer::menuCallBack(CCObject* object){
    unschedule(schedule_selector(GuideDialogLayer::sayFinished));
    S_AE->stopAllEffects();
    CCNode* node=(CCNode*)object;
    m_guideDialogDelegate->dialogCallBack((GuideDialogCMD)node->getTag());
}

void GuideDialogLayer::sayFinished(){
    this->setVisible(true);
    unschedule(schedule_selector(GuideDialogLayer::sayFinished));
    if (m_guideDialog->getIsAutoStep()) {
        m_guideDialogDelegate->dialogCallBack(kDialogCMDOk);
    }else{
        
    }
}
