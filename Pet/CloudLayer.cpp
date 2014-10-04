//
//  CloudLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-6-22.
//
//

#include "CloudLayer.h"
#include "resource.h"
using namespace std;

typedef struct _CloudStruct
{
    string pic;
    string startPoint;
    float moveX;
}CloudStruct;

// 在这里配置每个场景要加载的资源
static CloudStruct clouds[] = {
    {
       "yun1.png",
       "pet_yun1",
       0.2
    },
    {
        "yun2.png",
        "pet_yun2",
        0.4
    },
    {
        "yun3.png",
        "pet_yun3",
        0.6
    }
};

bool CloudLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    for (int i=0; i<3; i++) {
        CCSprite* yun=CCSprite::createWithSpriteFrameName(clouds[i].pic.c_str());
        yun->setPosition(S_RM->getPositionWithName(clouds[i].startPoint.c_str()));
        yun->setTag(i);
        this->addChild(yun);
    }
    
    this->schedule(schedule_selector(CloudLayer::timer));
    return true;
}

void CloudLayer::timer(float t){
    for (int i=0; i<3; i++) {
        CCSprite* yun=(CCSprite*)this->getChildByTag(i);
        float x=yun->getPosition().x;
        if (x<-yun->getContentSize().width/2) {
            x=W_SIZE.width+yun->getContentSize().width/2;
        }else{
            x-=clouds[i].moveX;
        }
        yun->setPositionX(x);
    }
}

