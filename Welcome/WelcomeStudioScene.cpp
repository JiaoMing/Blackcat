//
//  WelcomeStudioScene.cpp
//  Blackcat
//
//  Created by haojunhua on 14-8-21.
//
//

#include "WelcomeStudioScene.h"
#include "PlatformAction.h"
#include "SummaryScene.h"
#include "LoadingScene.h"
#include "resource.h"

using namespace cocostudio::timeline;

WelcomeStudioScene::WelcomeStudioScene(){
    
}

WelcomeStudioScene::~WelcomeStudioScene(){
    
}

CCScene* WelcomeStudioScene::scene()
{
    CCScene* scene = CCScene::create();
    WelcomeStudioScene* layer=WelcomeStudioScene::create();
    scene->addChild(layer);
    return scene;
}

bool WelcomeStudioScene::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    m_rootNode=NodeReader::getInstance()->createNode("Blackcat_1/Blackcat_1.ExportJson");
    ActionTimeline* action = ActionTimelineCache::getInstance()->createAction("Blackcat_1/Blackcat_1.ExportJson");
    
    
    m_rootNode->runAction(action);
    action->gotoFrameAndPlay(0, action->getDuration(), true);
    
    for (int i=0; i<m_rootNode->getChildrenCount(); i++) {
        Widget* child=(Widget*)m_rootNode->getChildren()->objectAtIndex(i);
        if (child->getTag()==-1) {
            Button* button=(Button*)UIHelper::seekWidgetByTag(child, 136);
            if(button){
                button->addTouchEventListener(this, toucheventselector(WelcomeStudioScene::touchEvent));
            }
        }
    }
    
    this->addChild(m_rootNode);
    
    return true;
}


void WelcomeStudioScene::touchEvent(cocos2d::CCObject *sender, TouchEventType type){
    if (type ==TOUCH_EVENT_ENDED)
	{
		Button* btn = (Button*)sender;
        const char* name=btn->getName();
        
        S_UD->setBoolForKey("HomeScene_YindaoOpen", false);
        if (strcmp(name, "youxibeijing")==0) {
            PlatformAction::playMovie();
        }else if(strcmp(name, "xinshouyindao")==0){
            S_UD->setBoolForKey("HomeScene_YindaoOpen", true);
            S_UD->flush();
            S_UD->setStringForKey("HomeScene_First","heimao_1");
            S_DR->replaceScene(LoadingScene::scene("HomeScene"));
        }else if(strcmp(name, "xuexi")==0){
            S_DR->replaceScene(LoadingScene::scene("HomeScene"));
            
        }else if(strcmp(name, "youxi")==0){
            S_DR->replaceScene(LoadingScene::scene("NewPetScene",false,kLoadingRoleXiaobo));
        }else if(strcmp(name, "JZCD")==0){
            S_UD->setBoolForKey("HomeScene_ParentOpen", true);
            S_UD->flush();
            S_DR->replaceScene(LoadingScene::scene("HomeScene"));
        }else if(strcmp(name, "YXJJ")==0){
            S_DR->pushScene(SummaryScene::scene(kSumHome));
        }
        S_UD->flush();
	}
}

