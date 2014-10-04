//
//  LoadscapeScene.cpp
//  Blackcat
//
//  Created by haojunhua on 14-8-18.
//
//

#include "LoadscapeScene.h"
#include "resource.h"
#include "WelcomeScene.h"

LoadscapeScene::~LoadscapeScene(){
    CCTextureCache::sharedTextureCache()->removeTextureForKey("Default.png");
}

CCScene* LoadscapeScene::scene()
{
    CCScene* scene = CCScene::create();
    LoadscapeScene *layer = new LoadscapeScene();
    if (layer && layer->init())
    {
        layer->autorelease();
    }
    scene->addChild(layer);
    return scene;
}

bool LoadscapeScene::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    CCSprite* bg=CCSprite::create("Default.png");
    bg->setPosition(S_RM->getJpgBgPosition());
    this->addChild(bg);
    
    this->scheduleOnce(schedule_selector(LoadscapeScene::gotoWelcomeScene), 2);
    return true;
}

void LoadscapeScene::gotoWelcomeScene(float t){
    S_DR->replaceScene(CCTransitionFade::create(1, WelcomeScene::scene()));
}
