//
//  TestLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-7-23.
//
//

#include "GushiScene.h"
#include "Utils.h"
#include "XieziCoverLayer.h"
using namespace std;

CCScene* GushiScene::scene()
{
    CCScene *scene = CCScene::create();
    GushiScene *layer = GushiScene::create();
    scene->addChild(layer);
    return scene;
}

bool GushiScene::init(){
    if (!CCLayer::init()) {
        return false;
    }
    string pinyin="sen lin li you yi tou shi wang . |n| ta ting shuo xian zai fei chang liu xing tang juan fa .";
    string text="森林里有一头狮王，|n|它听说现在非常流行烫卷发。";
    
    vector<std::string> pinyinVector;
    StringUtils::split(pinyin, " ", pinyinVector);
    
    CCMenu* menu=CCMenu::create();
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    
    int row=0;
    int column=0;
    for (int i=0; i<text.length()/3; i++) {
        string zi=text.substr(i*3,3);
        if (zi=="|n|") {
            row++;
            column=0;
        }else{
            CCLabelTTF* text=CCLabelTTF::create(zi.c_str(), "KaiTi.ttf", 25);
            text->setColor(ccc3(255,255,255));
            CCMenuItemLabel* item=CCMenuItemLabel::create(text, this, menu_selector(GushiScene::menuCallback));
            item->setPosition(ccp(100+column*60, 300-row*50));
            menu->addChild(item);
            
            if (pinyinVector[i]!=".") {
                CCLabelTTF* pinyin=CCLabelTTF::create(pinyinVector[i].c_str(), "Pinyinok.ttf", 15);
                pinyin->setColor(ccc3(255,255,255));
                pinyin->setPosition(ccp(100+column*60, 325-row*50));
                this->addChild(pinyin);
            }
            column++;
        }
    }
    
    return true;
}

void GushiScene::menuCallback(cocos2d::CCObject *sender){
    CCMenuItemLabel* item=(CCMenuItemLabel*)sender;
    CCLabelTTF* label=(CCLabelTTF*)item->getLabel();
    string zi=label->getString();
    
    XieziCoverLayer* layer=XieziCoverLayer::create(0, NULL,zi);
    layer->setDelegate(this);
    this->addChild(layer);
}