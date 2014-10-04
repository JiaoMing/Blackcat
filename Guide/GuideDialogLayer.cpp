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
#include "Button9.h"

enum {
    kDialogText=0,
    kDialogMenu
};
enum {
    kDialogMenuOk=0,
    kDialogMenuYes,
    kDialogMenuNo,
    kDialogMenuGuangbo
};

GuideDialogLayer* GuideDialogLayer::create(GuideDialogType guideDialogType,GuideDialogWithTextButtonPosiztion dialogButtonPosiztion)
{
    GuideDialogLayer *layer = new GuideDialogLayer();
    layer->m_guideDialogType=guideDialogType;
    layer->m_dialogButtonPosiztion=dialogButtonPosiztion;
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
    DialogLayer::setDelegate(guideDialogDelegate);
    this->m_guideDialogDelegate=guideDialogDelegate;
}

bool GuideDialogLayer::init(){
    if (!DialogLayer::init()) {
        return false;
    }
    switch (m_guideDialogType) {
        case kDialogBtuOnly:{
            this->hideBg();
            
            CCSprite* ok=CCSprite::createWithSpriteFrameName("dialog_ok.png");
            CCMenuItemSprite* okItem=CCMenuItemSprite::create(ok, ok, this, menu_selector(GuideDialogLayer::menuCallback));
            okItem->setPosition(S_RM->getPositionWithName("dialog_ok"));
            okItem->setTag(kDialogMenuOk);
            this->addMenuItem(okItem);
            
            CCSprite* yes=CCSprite::createWithSpriteFrameName("dialog_yes.png");
            CCMenuItemSprite* yesItem=CCMenuItemSprite::create(yes,yes, this, menu_selector(GuideDialogLayer::menuCallback));
            yesItem->setPosition(S_RM->getPositionWithName("dialog_yes"));
            yesItem->setTag(kDialogMenuYes);
            this->addMenuItem(yesItem);
            
            CCSprite* no=CCSprite::createWithSpriteFrameName("dialog_no.png");
            CCMenuItemSprite* noItem=CCMenuItemSprite::create(no,no, this, menu_selector(GuideDialogLayer::menuCallback));
            noItem->setPosition(S_RM->getPositionWithName("dialog_no"));
            noItem->setTag(kDialogMenuNo);
            this->addMenuItem(noItem);
            
//            if (m_cartoon!=NULL) {
//                CCSprite* guangbo=CCSprite::createWithSpriteFrameName("guangbo.png");
//                CCMenuItemSprite* guangboItem=CCMenuItemSprite::create(guangbo,guangbo, this, menu_selector(GuideDialogLayer::menuCallback));
//                guangboItem->setPosition(S_RM->getPositionWithName("dialog_title"));
//                guangboItem->setTag(kDialogMenuGuangbo);
//                this->addMenuItem(guangboItem);
//            }
        }
            break;
        case kDialogWithText:{
            
            CCLabelTTF* text=CCLabelTTF::create("", "KaiTi.ttf", 25,CCSizeMake(560, 245),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
            text->setColor(ccc3(58,37,4));
            text->setTag(kDialogText);
            text->setPosition(S_RM->getPositionWithName("dialog_text"));
            this->addChild(text);
            
            Button9* ok=Button9::create("OK");
            CCMenuItemSprite* okItem=CCMenuItemSprite::create(ok, ok, this, menu_selector(GuideDialogLayer::menuCallback));
            okItem->setTag(kDialogMenuOk);
            this->addMenuItem(okItem);
            
            Button9* yes=Button9::create("YES");
            CCMenuItemSprite* yesItem=CCMenuItemSprite::create(yes,yes, this, menu_selector(GuideDialogLayer::menuCallback));
            yesItem->setTag(kDialogMenuYes);
            this->addMenuItem(yesItem);
            
            Button9* no=Button9::create("NO");
            CCMenuItemSprite* noItem=CCMenuItemSprite::create(no,no, this, menu_selector(GuideDialogLayer::menuCallback));
            noItem->setTag(kDialogMenuNo);
            this->addMenuItem(noItem);
            
            okItem->setPosition(S_RM->getPositionWithName("dialog_text_ok"));
            yesItem->setPosition(S_RM->getPositionWithName("dialog_text_yes"));
            noItem->setPosition(S_RM->getPositionWithName("dialog_text_no"));
            
        }
        default:
            break;
    }
    
    return true;
}

//bool GuideDialogLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
////    S_ALP->play(m_guideDialog->getAudioArray());
//    return true;
//}

void GuideDialogLayer::setGuideDialogData(GuideDialog* guideDialog){
    m_guideDialog=guideDialog;
    m_guideDialog->retain();
    CCLabelTTF* text=(CCLabelTTF*) this->getChildByTag(kDialogText);
    if(text!=NULL)text->setString(guideDialog->getText().c_str());
    
    CCMenu* menu=this->getMainMenu();
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

void GuideDialogLayer::menuCallback(CCObject* object){
    CCNode* node=(CCNode*)object;
    if (node->getTag()==kDialogMenuGuangbo) {
        S_ALP->play(m_guideDialog->getAudioArray());
    }else{
        unschedule(schedule_selector(GuideDialogLayer::sayFinished));
        S_ALP->stop();
        S_AE->stopAllEffects();
        
        GuideDialogCMD cmd;
        if (isCloseItem((CCMenuItem*)object)) {
            switch (m_guideDialog->getMode()) {
                case kGuideDialogOk:{
                    cmd=kDialogCMDOk;
                }
                    break;
                case kGuideDialogYesOrNo:{
                    cmd=kDialogCMDNo;
                }
                    break;
                default:
                    break;
            }
        }else{
            cmd=(GuideDialogCMD)node->getTag();
        }
        this->removeFromParent();
        DialogLayer::menuCallback(object);
        
        m_guideDialogDelegate->dialogCallBack(cmd);
    }
    
}

void GuideDialogLayer::sayFinished(){
    this->setVisible(true);
    unschedule(schedule_selector(GuideDialogLayer::sayFinished));
    if (m_guideDialog->getIsAutoStep()) {
        m_guideDialogDelegate->dialogCallBack(kDialogCMDOk);
    }else{
        
    }
}
