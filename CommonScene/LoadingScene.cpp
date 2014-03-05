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
    
//    CCSprite *progressBg = CCSprite::create("jindutiao01.png");
//    progressBg->setPosition(ccp(512, 125));
//    this->addChild(progressBg);
//    
//    CCSprite *pic = CCSprite::create("jindutiao02.png");
//    CCProgressTimer* progress = CCProgressTimer::create(pic);
//    progress->setType(kCCProgressTimerTypeBar);
//    progress->setMidpoint(ccp(0, 0));
//    progress->setBarChangeRate(ccp(1, 0));
//    progress->setPercentage(0);
//    progress->setPosition(ccp(512, 125));
//    progress->setTag(PROGRESS);
//    this->addChild(progress);
    
    Cartoon* role = Cartoon::cartoonWithName("kaishi");
    role->setPosition(S_RM->getPositionWithName("loading_role"));
    role->setScale(0.6);
    this->addChild(role);
    if (m_loadingRole==kLoadingRolePanda) {
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
    
    return true;
}

void LoadingScene::onEnter(){
    CCLayer::onEnter();
    if (m_resTotal==0)
    {
        this->processReplaceScene();
    }else{
        S_AM->loadAsyncResource(m_sceneName, this, callfuncO_selector(LoadingScene::loadFinishedCallback));
        //未加载先走进度，Android下提升体验
//        CCProgressFromTo *progressFromTo = CCProgressFromTo::create(1.0f, 0, 1.0f/(m_resTotal+1)*100);
//        CCProgressTimer* progress=(CCProgressTimer*)this->getChildByTag(PROGRESS);
//        progress->runAction(progressFromTo);
    }
}


void LoadingScene::loadFinishedCallback(CCObject *pSender)
{
//    CCProgressTimer* progress=(CCProgressTimer*)this->getChildByTag(PROGRESS);
//    float percent = progress->getPercentage();
    
	m_resCurrent++;
	CCString *pStr = dynamic_cast<CCString *>(pSender);
	if (pStr != NULL)
	{
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(pStr->getCString());
        //未加载先走进度，Android下提升体验
//        CCProgressFromTo *progressFromTo = CCProgressFromTo::create(1.0f, percent, (float)(m_resCurrent+1)/(m_resTotal+1)*100);
//        progress->runAction(progressFromTo);
        
	}
	if (m_resCurrent == m_resTotal)
	{
        this->processReplaceScene();
	}
}

void LoadingScene::processReplaceScene(){
//    CCProgressTimer* progress=(CCProgressTimer*)this->getChildByTag(PROGRESS);
//    float percent = progress->getPercentage();
//    CCProgressFromTo *progressFromTo = CCProgressFromTo::create(0.5, percent, 100);
//    progress->runAction(progressFromTo);
//    CCDelayTime *waiting = CCDelayTime::create(0.5);
//    CCCallFunc *callBack = CCCallFunc::create(this, callfunc_selector(LoadingScene::repalceScene));
//    this->runAction(CCSequence::create(waiting, callBack, NULL));
    repalceScene();
}


void LoadingScene::repalceScene(){
    S_DR->replaceScene((*S_AM->getSelector(m_sceneName))());
}
