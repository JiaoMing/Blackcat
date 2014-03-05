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
//#include <pthread.h>

static int install_count=0;

//static pthread_t s_installThread;
//static pthread_mutex_t      s_userDefaultMutex;

static void* install(void* data){
    const char* fileName=(const char*)data;
//    pthread_mutex_lock(&s_userDefaultMutex);
    CCString* extract=CCString::createWithFormat("is%sExtracted",fileName);
    CCString* deposit=CCString::createWithFormat("is%sDeposited",fileName);
    bool isExtracted=S_UD->getBoolForKey(extract->getCString(),false);
    if (!isExtracted) {
        //转存卡片资源
        bool isDeposited=S_UD->getBoolForKey(deposit->getCString(),false);
        if(!isDeposited){
            if(FileUtils::depositResourceFile(fileName)) {
                CCLog("%s",deposit->getCString());
                S_UD->setBoolForKey(deposit->getCString(),true);
                S_UD->flush();
                CCLog("depositResourceFile");
                install_count++;
            }
        }
        
        //解压图片资源
        if (FileUtils::unzipOpenFile(fileName,false)) {
            FileUtils::removeWriteableFile(fileName);
            S_UD->setBoolForKey(extract->getCString(),true);
            S_UD->flush();
            CCLog("unzipOpenFile");
            install_count++;
        }
    }
//    pthread_mutex_unlock(&s_userDefaultMutex);
    return NULL;
}

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
    
    m_labelProcess=CCLabelTTF::create("正在安装，请稍等...", "KaiTi.ttf", 25);
    m_labelProcess->setPosition(S_RM->getPositionWithName("install_text"));
    m_labelProcess->setColor(ccc3(0, 0, 0));
    this->addChild(m_labelProcess);
    
    return true;
}

void InstallScene::onEnter(){
    CCLayerColor::onEnter();
    this->scheduleOnce(schedule_selector(InstallScene::gotoHomeScene), 1);
//    pthread_mutex_init(&s_userDefaultMutex, NULL);
//    pthread_create(&s_installThread, NULL, install, (void*)"audio_pic.zip");
////    pthread_create(&s_installThread, NULL, install, (void*)"ecards.zip");
    
    
}

void InstallScene::gotoHomeScene(float t){
    CCLog("gotoHomeScene %d",install_count);
    install((void*)"audio_pic.zip");
    install((void*)"ecards.zip");
    S_UD->setBoolForKey(IS_INSTALLED, true);
    S_UD->flush();
    S_RM->addSceneRes("LoadingScene","loading");
    S_DR->replaceScene(LoadingScene::scene("HomeScene"));
}

void InstallScene::fresh(float t){
    m_labelProcess->setString("fresh");
}
