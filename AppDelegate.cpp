//
//  blackcatAppDelegate.cpp
//  blackcat
//
//  Created by haojunhua on 13-8-22.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "ResManager.h"
#include "Utils.h"
#include "LoadingScene.h"
#include "resource.h"
#include "KechengListScene.h"
#include "HomeScene.h"
#include "WelcomeScene.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{
srand((unsigned int)time(NULL));
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::sharedEngine()->end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    S_DR->setOpenGLView(CCEGLView::sharedOpenGLView());
    S_DR->setAnimationInterval(1.0 / 60);

    
    float ratio=1.0;
    TargetPlatform target = getTargetPlatform();
    
    
    std::vector<std::string> searchPaths;
    if (target == kTargetIpad)
    {
//        searchPaths.push_back("ipad");
//        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(1024, 768, kResolutionShowAll);
        searchPaths.push_back("android");
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(1280, 720, kResolutionShowAll);
    }else{
        searchPaths.push_back("android");
        searchPaths.push_back("audio");
        searchPaths.push_back("audio/dialog");
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(1280, 720, kResolutionShowAll);
    }
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
    
    
    S_DR->setDepthTest(false);
    
    //初始化坐标
    S_RM->init("position",ratio);
    if (S_UD->getBoolForKey(IS_SHOW_GONGKAIXIN,true)) {
        S_DR->runWithScene(WelcomeScene::scene());
    }else{
        S_RM->addSceneRes("LoadingScene", "loading");
        S_DR->runWithScene(LoadingScene::scene("HomeScene"));
    }

    S_UD->setBoolForKey("BG_MUSIC",true);
    S_AE->playBackgroundMusic("blackcat-musicbox.mp3",true);
    
    S_UD->setBoolForKey(NEW_OPEN, true);
    S_UD->flush();
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    S_DR->pause();

    S_AE->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    S_DR->resume();
    
    HomeScene* scene=(HomeScene*)S_DR->getRunningScene();
    if(scene!=NULL)scene->willEnterForeground();
    
    bool isBgMusicRunning=S_UD->getBoolForKey("BG_MUSIC",true);
    if (isBgMusicRunning) {
        S_AE->resumeBackgroundMusic();
    }
    
}
