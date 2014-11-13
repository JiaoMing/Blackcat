//
//  LoadingScene.cpp
//  Blackcat
//
//  Created by haojunhua on 13-10-21.
//
//

#include "LoadingScene.h"
#include "HomeScene.h"
#include "resource.h"
#include "AsyncManager.h"
#include "ResManager.h"

#define PROGRESS 100

CCScene* LoadingScene::scene(const char* sceneName,bool isReleaseCurrentScene,LoadingRole loadingRole)
{
    CCScene* scene = CCScene::create();
    LoadingScene *layer = new LoadingScene(sceneName,isReleaseCurrentScene,loadingRole);
    if (layer && layer->init())
    {
        layer->autorelease();
    }
    else
    {
        delete layer;
        layer = NULL;
    }
    scene->addChild(layer);
    return scene;
}

LoadingScene::LoadingScene(const char* sceneName,bool isReleaseCurrentScene,LoadingRole loadingRole){
    m_sceneName=sceneName;
    m_isReleaseCurrentScene=isReleaseCurrentScene;
    m_loadingRole=loadingRole;
    m_isLoadFinish=false;
    m_isTimeEnd=false;
}

LoadingScene::~LoadingScene()
{
}

bool LoadingScene::init()
{
    if(! CCLayer::init())
    {
        return false;
    }
    
    CCSprite *bg = CCSprite::createWithSpriteFrame(S_SF->spriteFrameByName("loading_bg.png"));
    bg->setPosition(S_RM->getJpgBgPosition());
    this->addChild(bg);
    
    Cartoon* role = Cartoon::cartoonWithName("kaishi");
    role->setPosition(S_RM->getPositionWithName("loading_role"));
    role->setScale(0.6);
    this->addChild(role);
    if (m_loadingRole==kLoadingRoleXiaobo) {
        role->doAction("jiazai:0", NULL);
    }else{
        role->doAction("heimaoguochang:0", NULL);
    }
    
    
    if (m_isReleaseCurrentScene) {
        //释放当前Scene资源
        S_AM->removeAsyncResource(S_AM->getCurrentSceneName().c_str());
    }
    S_AM->setCurrentSceneName(m_sceneName);
    
    //加载目标Scene资源
    m_resCurrent = m_resTotal = 0;
    m_resTotal=S_AM->getResourceCount(m_sceneName);
    
    this->scheduleOnce(schedule_selector(LoadingScene::countdown), 2);
    
    return true;
}

void LoadingScene::onEnter(){
    CCLayer::onEnter();
    if (m_resTotal==0)
    {
        m_isLoadFinish=true;
//        this->repalceScene();
    }else{
        S_AM->loadAsyncResource(m_sceneName, this, callfuncO_selector(LoadingScene::loadFinishedCallback));
    }
}


void LoadingScene::loadFinishedCallback(CCObject *pSender)
{
	m_resCurrent++;
	CCString *pStr = dynamic_cast<CCString *>(pSender);
	if (pStr != NULL)
	{
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(pStr->getCString());
        
	}
	if (m_resCurrent == m_resTotal)
	{
        m_isLoadFinish=true;
        if (m_isTimeEnd) {
            this->repalceScene();
        }
	}
}

void LoadingScene::countdown(float t){
    if (m_isLoadFinish) {
        this->repalceScene();
    }
    m_isTimeEnd=true;
}

void LoadingScene::repalceScene(){
    S_DR->replaceScene((*S_AM->getSelector(m_sceneName))());
}
