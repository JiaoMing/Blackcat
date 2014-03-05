//
//  RankingLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-3-2.
//
//

#include "RankingLayer.h"
#include "resource.h"

RankingLayer* RankingLayer::create(Rank rank){
    RankingLayer* layer=new RankingLayer();
    if (layer&&layer->init(rank)) {
        layer->autorelease();
    }else{
        CC_SAFE_DELETE(layer);
    }
    return layer;
}

bool RankingLayer::init(Rank rank){
    if (CoverLayer::initWithColor(ccc4(0, 0, 0, 127))) {
        CCSprite* sprite=CCSprite::createWithSpriteFrameName(rankStruts[rank].pic.c_str());
        sprite->setPosition(S_RM->getJpgBgPosition());
        this->addChild(sprite);
        
        return true;
    }
    return false;
}