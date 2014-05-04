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

InstallScene::InstallScene(){
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
    
    CCSprite* logo=CCSprite::createWithSpriteFrameName("logo_32.png");
    logo->setPosition(S_RM->getPositionWithName("install_logo"));
    this->addChild(logo);
    
    m_labelProcess=CCLabelTTF::create("正在安装，请稍等:0%", "KaiTi.ttf", 25);
    m_labelProcess->setPosition(S_RM->getPositionWithName("install_text"));
    m_labelProcess->setColor(ccc3(0, 0, 0));
    this->addChild(m_labelProcess);
    
    return true;
}

void InstallScene::onEnter(){
    CCLayerColor::onEnter();
    
    m_installCount=2;
    ResInstall::sharedResInstall()->install("audio_pic.zip",this,callfunc_selector(InstallScene::installCallback));
    ResInstall::sharedResInstall()->install("ecards.zip",this,callfunc_selector(InstallScene::installCallback));
}

void InstallScene::gotoHomeScene(){
    S_UD->setBoolForKey(IS_INSTALLED, true);
    S_UD->flush();
    S_RM->addSceneRes("LoadingScene","loading");
    S_DR->replaceScene(LoadingScene::scene("HomeScene"));
}

void InstallScene::installCallback(){
    m_installCount--;
    CCString* str=CCString::createWithFormat("正在安装，请稍等:%d%%",(int)(m_installCount/2.0*100));
    m_labelProcess->setString(str->getCString());
    if (m_installCount==0) {
        this->gotoHomeScene();
    }
}
