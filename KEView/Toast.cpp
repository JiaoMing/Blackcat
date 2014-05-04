//
//  Toast.cpp
//  Blackcat
//
//  Created by haojunhua on 14-4-13.
//
//

#include "Toast.h"

#define TAG_TOAST 8888

Toast* Toast::sharedToast(){
    CCScene* scene=S_DR->getRunningScene();
    Toast* toast=(Toast*)scene->getChildByTag(TAG_TOAST);
    if (toast==NULL) {
        toast=Toast::create();
        scene->addChild(toast);
    }
    return toast;
}

bool Toast::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    m_label=NULL;
    
    m_bg=CCLayerColor::create(ccc4(0, 0, 0, 100), 300, 40);
    this->addChild(m_bg);
    
    return true;
}

void Toast::makeText(const char *text){
    if (m_label==NULL) {
        m_label=CCLabelTTF::create(text, "KaiTi.ttf", 20);
        m_label->setPosition(ccp(512, 50));
        m_label->setColor(ccWHITE);
        this->addChild(m_label);
    }else{
        m_label->setString(text);
    }
    float width=m_label->getContentSize().width+20;
    m_bg->setContentSize(CCSizeMake(width, 40));
    m_bg->setPosition(512-width/2, 50-40/2);
    
    CCObject* obj;
    CCARRAY_FOREACH(this->getChildren(), obj){
        CCNode* item=(CCNode*)obj;
        item->runAction(CCSequence::create(CCDelayTime::create(2),CCFadeOut::create(2),NULL));
    }
}