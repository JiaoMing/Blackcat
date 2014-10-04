//
//  GuideBaseLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 13-12-29.
//
//

#include "GuideBaseLayer.h"
#include "resource.h"
#include "ResManager.h"
#include <stdint.h>
#include "GuideDialogLayer.h"
#include "Props.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;



enum {
    kDialogText=0,
    kDialogMenuOk
};

GuideBaseLayer::GuideBaseLayer(){
    m_lastReorderTag=INT_MIN;
    m_guide=NULL;
    m_cartoon=NULL;
}

GuideBaseLayer::~GuideBaseLayer(){
    CC_SAFE_RELEASE(m_guide);
}

bool GuideBaseLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
    return true;
}

void GuideBaseLayer::startGuide(const char *queueKey,const char* stepKey,bool isDefaultFirstStepKey){
    m_guide=NULL;
    m_queueKey=queueKey;
    if (isDefaultFirstStepKey) {
        m_stepKey=S_UD->getStringForKey(queueKey,stepKey);
    }else{
        m_stepKey=stepKey;
    }
    this->step();
}

void GuideBaseLayer::step(){
    S_ALP->stop();
    S_AE->stopAllEffects();
    
    if (m_guide!=NULL) {
        m_stepKey=m_guide->getToGuideKey();
        m_guide->release();
    }
    
    if (m_lastReorderTag!=INT_MIN) {
        CCNode* node= this->getChildByTag(m_lastReorderTag);
        this->reorderChild(node, m_lastReorderOrder);
        
        if (m_cartoonPosition.x!=0&&m_cartoonPosition.y!=0) {
            m_cartoon->setPosition(m_cartoonPosition);
        }
    }
    m_cartoonPosition=CCPointZero;
    
    m_guide=S_GM->getGuideDialogWithQueueKeyAndStepKey(m_queueKey.c_str(), m_stepKey.c_str());
    if (m_guide!=NULL) {
        m_guide->retain();
        S_UD->setStringForKey(m_queueKey.c_str(), m_stepKey);
        S_UD->flush();
    }else{
        //移除对话框层
        CCNode* guideDialogLayer=this->getChildByTag(GUIDE_DIALOG_LAYER);
        if (guideDialogLayer!=NULL) {
            guideDialogLayer->removeFromParentAndCleanup(true);
        }
        return;
    }
    
    int tag=0;
    if (m_guide->getType()==kGuideDialog) {
        GuideDialog* guideDialog=(GuideDialog*)m_guide;
        if (m_cartoon!=NULL) {
            //ENDING_POSE
        }
        m_cartoon=(Cartoon*)this->getChildByTag(guideDialog->getTag());
        if (m_cartoon!=NULL&&guideDialog->getActionArray()->count()>0) {
            m_cartoon->doAction(guideDialog->getActionArray());
            
        }
        
        //reorder卡通角色
        tag=guideDialog->getTag();
        m_lastReorderTag=tag;
        m_lastReorderOrder=m_cartoon->getZOrder();
        this->reorderChild(m_cartoon, INT_MAX);
        m_cartoonPosition=m_cartoon->getPosition();
        if (guideDialog->getCartoonPoint().length()>0) {
            m_cartoon->setPosition(S_RM->getPositionWithName(guideDialog->getCartoonPoint().c_str()));
        }else if(guideDialog->getDialogType()==kDialogWithText){
            m_cartoon->setPosition(S_RM->getPositionWithName("dialog_cartoon"));
        }
        
        //这里还没有关闭掉上个event对应的tag的点击事件
        if (guideDialog->getMode()!=kGuideDialogHidden) {
            
            GuideDialogLayer* guideDialogLayer=(GuideDialogLayer*)this->getChildByTag(GUIDE_DIALOG_LAYER);
            if (guideDialogLayer==NULL) {
                guideDialogLayer=GuideDialogLayer::create(guideDialog->getDialogType());
                guideDialogLayer->setDelegate(this);
                this->addChild(guideDialogLayer,TOP_ORDER_INDEX,GUIDE_DIALOG_LAYER);
            }
            
            if(!DEBUG_OPEN)
                guideDialogLayer->setVisible(false);
            guideDialogLayer->setGuideDialogData(guideDialog);
        }
        
//        if(guideDialog->getIsAutoPlayAudio())
            S_ALP->play(guideDialog->getAudioArray());
        
    }else{
        GuideEvent* guideEvent=(GuideEvent*)m_guide;
        tag=guideEvent->getTag();
        Props* props=(Props*)this->getChildByTag(tag);
        if (props!=NULL) {
            props->setHandlerPriority(this->topHandlerPriority()-1);
            //reorder道具
            m_lastReorderTag=tag;
            m_lastReorderOrder=props->getZOrder();
            this->reorderChild(props, INT_MAX);
            
        }
        //移除对话框层
        CCNode* node=this->getChildByTag(GUIDE_DIALOG_LAYER);
        if (node!=NULL) {
            node->removeFromParentAndCleanup(true);
        }
        
        CoverLayer* coverLayer=CoverLayer::create();
        coverLayer->setDelegate(this);
        this->addChild(coverLayer,TOP_ORDER_INDEX,GUIDE_EVENT_COVER_LAYER);
        
        int event=S_UD->getIntegerForKey(guideEvent->getKey().c_str(), kGuideEventDefault);
        
        if (event==kGuideEventPass) {
            //移除事件覆盖层
            CCNode* eventCover=this->getChildByTag(GUIDE_EVENT_COVER_LAYER);
            if (eventCover!=NULL) {
                eventCover->removeFromParentAndCleanup(true);
            }
            
            this->step();
            return;
        }else{
            S_UD->setIntegerForKey(guideEvent->getKey().c_str(), kGuideEventOpen);
            S_UD->flush();
        }
    }
    
}

void GuideBaseLayer::dialogCallBack(GuideDialogCMD cmd){
    this->step();
}
