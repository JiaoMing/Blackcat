//
//  InstallScene.cpp
//  Blackcat
//
//  Created by haojunhua on 14-2-27.
//
//

#include "InstallScene.h"
#include "resource.h"
#include "LoadingScene.h"
#include "ResInstall.h"
#include "WelcomeScene.h"

#define INSTALL_COUNT 2

InstallScene::InstallScene(){
    m_count=0;
    S_RM->addSceneRes("InstallScene", "install");
}

InstallScene::~InstallScene(){
    S_RM->removeSceneRes("InstallScene");
}

CCScene* InstallScene::scene()
{
    CCScene* scene = CCScene::create();
    InstallScene *layer = InstallScene::create();
    scene->addChild(layer);
    return scene;
}

bool InstallScene::init()
{
    if(! CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)))
    {
        return false;
    }
    
    CCSprite* logo=CCSprite::create();
    logo->setPosition(S_RM->getPositionWithName("install_logo"));
    this->addChild(logo);
    CCAnimation* animation=CCAnimation::create();
    for (int i=0;i<32;i++) {
        CCString* fileName=CCString::createWithFormat("logo_%d.png",i+1);
        animation->addSpriteFrame(S_SF->spriteFrameByName(fileName->getCString()));
    }
    animation->setDelayPerUnit(0.1);
    animation->setRestoreOriginalFrame(true);
    CCAnimate* animate=CCAnimate::create(animation);
    logo->runAction(CCRepeatForever::create(animate));
    
    m_labelProcess=CCLabelTTF::create("正在安装，请稍等:0%", "KaiTi.ttf", 25);
    m_labelProcess->setPosition(S_RM->getPositionWithName("install_text"));
    m_labelProcess->setColor(ccc3(0, 0, 0));
    this->addChild(m_labelProcess);
    
    this->schedule(schedule_selector(InstallScene::loadingProcessFresh),0.1);
    return true;
}

void InstallScene::onEnter(){
    CCLayerColor::onEnter();
    
    m_installedCount=0;
    ResInstall::sharedResInstall()->install("audio_pic.zip",this,callfunc_selector(InstallScene::installCallback));
    ResInstall::sharedResInstall()->install("ecards.zip",this,callfunc_selector(InstallScene::installCallback));
}

void InstallScene::gotoHomeScene(){
    S_UD->setBoolForKey(IS_INSTALLED, true);
    S_UD->flush();
    S_RM->addSceneRes("LoadingScene","loading");
    S_DR->replaceScene(WelcomeScene::scene());
}

void InstallScene::loadingProcessFresh(float t){
    int nowThumb=(int)((float)(m_installedCount+1)/INSTALL_COUNT*100);
    if(m_count<nowThumb-1){
        m_count++;
    }
    
    if (m_count>100) {
        m_count=100;
    }
    
    CCString* str=CCString::createWithFormat("正在安装，请稍等:%d%%",m_count);
    m_labelProcess->setString(str->getCString());
}

void InstallScene::installCallback(){
    m_installedCount++;
    if (m_installedCount==INSTALL_COUNT) {
        m_labelProcess->setString("正在安装，请稍等:100%");
        this->unschedule(schedule_selector(InstallScene::loadingProcessFresh));
        this->gotoHomeScene();
    }
}