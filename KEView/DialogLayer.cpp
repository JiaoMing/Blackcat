//
//  DialogLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-4-10.
//
//

#include "DialogLayer.h"
#include "resource.h"

#define S_DS DialogLayerStack::sharedDialogLayerStack()

DialogLayerStack* s_dialogLayerStack=NULL;
DialogLayerStack* DialogLayerStack::sharedDialogLayerStack(){
    if (s_dialogLayerStack==NULL) {
        s_dialogLayerStack=new DialogLayerStack();
    }
    return s_dialogLayerStack;
}


void DialogLayerStack::push(DialogLayer* layer){
    _dialogStack.push(layer);
}

DialogLayer* DialogLayerStack::popAndReturn(){
    if(!_dialogStack.empty()){
        DialogLayer* layer=_dialogStack.top();
        _dialogStack.pop();
        return layer;
    }else
        return  NULL;
}

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
    CCNode* parent=this->getParent();
    this->removeFromParent();
    dialogLayer->setPosition(parent->convertToNodeSpace(CCPointZero));
    parent->addChild(dialogLayer,ORDER_DIALOG);
}

void DialogLayer::pushDialog(DialogLayer *dialogLayer){
    this->retain();//remove前计数器+1防止释放
    S_DS->push(this);
    CCNode* parent=this->getParent();
    this->removeFromParent();
    dialogLayer->setDelegate(m_dialogLayerDelegate);
    dialogLayer->setPosition(parent->convertToNodeSpace(CCPointZero));
    parent->addChild(dialogLayer,ORDER_DIALOG);
}

void DialogLayer::popDialog(){
    DialogLayer* dialogLayer=S_DS->popAndReturn();
    if(dialogLayer){
        CCNode* parent=this->getParent();
        this->removeFromParent();
        if(dialogLayer->getParent()==NULL){
            parent->addChild(dialogLayer);
        }else{
            CCLog("child->m_pParent != __null");
        }
        CC_SAFE_RELEASE(dialogLayer);//恢复计数器
    }else{
        this->removeFromParent();
    }
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
        DialogLayer* layer=S_DS->popAndReturn();
        while (layer) {
            layer->removeFromParent();
            layer=S_DS->popAndReturn();
        }
        this->removeFromParent();
        S_ALP->stop();
        m_dialogLayerDelegate->dialogCloseCallBack();
        
    }
}

void DialogLayer::rePosition(){
    CCPoint middle=S_RM->getJpgBgPosition();
    CCSize size=m_bg->getContentSize();
    CCPoint orignInBg=ccp(35,60);
    this->setPosition(middle.x-size.width/2+orignInBg.x, middle.y-size.height/2+orignInBg.y);
    m_bg->setPosition(ccp(size.width/2-orignInBg.x, size.height/2-orignInBg.y));
}