//
//  rewardLayer.cpp
//  Blackcat
//
//  Created by yangfei on 13-11-22.
//
//

#include "KechengRewardLayer.h"
#include "resource.h"

enum TAG {
    kTagCaidai = 0,
    kTagBiankuang,
    kTagJiangpai
};

KechengRewardLayer* KechengRewardLayer::create(vector<Hanzi*>* hanziVector)
{
    KechengRewardLayer* layer = new KechengRewardLayer;
    layer->m_hanziVector=hanziVector;
    if (layer && layer->init())
    {
        layer->autorelease();
    }
    else
    {
        delete layer;
        layer = NULL;
    }
    return layer;
}



bool KechengRewardLayer::init()
{
    if(! CCLayer::init())
        return false;
    
    CCSprite* bg=CCSprite::createWithSpriteFrameName("renwu_jianglikuang.png");
    bg->setPosition(S_RM->getPositionWithName("renwu_jiangli_bg"));
    this->addChild(bg);
    
    for (int i=0; i<6; i++) {
        CCLabelTTF* label=CCLabelTTF::create((*m_hanziVector)[i]->getzi().c_str(), "KaiTi.ttf", 80.0);
        CCString* point=CCString::createWithFormat("renwu_jiangli_%d",i+1);
        label->setPosition(S_RM->getPositionWithName(point->getCString()));
        label->setColor(ccc3(0, 0, 0));
        this->addChild(label);
    }
    
    
    return true;
}

void KechengRewardLayer::onEnter(){
    CCLayer::onEnter();
    this->runRewardAnimate();
    
    S_AE->playEffect("renwu_huanhu.mp3");
}

void KechengRewardLayer::runRewardAnimate(){
    float delay=0.0f;
    for (int i=0; i<6; i++) {
        CCSprite* jiangli=CCSprite::create();
        
        CCSprite* caidai=CCSprite::createWithSpriteFrameName("renwu_caidai.png");
        caidai->setTag(kTagCaidai);
        caidai->setOpacity(0);
        caidai->setPosition(ccp(-8, -8));
        jiangli->addChild(caidai);
        
        CCSprite* biankuang=CCSprite::createWithSpriteFrameName("renwu_jiangpai_biankuang.png");
        biankuang->setTag(kTagBiankuang);
        biankuang->setScale(0.1f);
        biankuang->setVisible(false);
        jiangli->addChild(biankuang);
        
        CCSprite* jiangpai=CCSprite::createWithSpriteFrameName("renwu_jiangpai.png");
        jiangpai->setTag(kTagJiangpai);
        jiangpai->setScale(1.0);
        jiangpai->setOpacity(0);
        jiangli->addChild(jiangpai);
        
        CCString* pointStr=CCString::createWithFormat("renwu_jiangli_%d",i+1);
        CCPoint point=S_RM->getPositionWithName(pointStr->getCString());
        jiangli->setPosition(ccp(point.x+40, point.y+40));
        this->addChild(jiangli);
        
        CCArray* actionArray=CCArray::create();
        actionArray->addObject(CCDelayTime::create(delay));
        delay+=0.3f;
        actionArray->addObject(CCFadeIn::create(0.1));
        actionArray->addObject(CCSpawn::create(CCScaleTo::create(0.2, 0.08),CCRotateBy::create(0.2, 315),NULL));
        actionArray->addObject(CCCallFuncN::create(this, callfuncN_selector(KechengRewardLayer::runRewardAnimateCallBack)));
        CCFiniteTimeAction* sequence=CCSequence::create(actionArray);
        jiangpai->runAction(sequence);
    }
}

void KechengRewardLayer::runRewardAnimateCallBack(CCObject* obj){
    CCNode* node=(CCNode*)obj;
    CCSprite* jiangli=(CCSprite*)node->getParent();
    CCSprite* caidai=(CCSprite*)jiangli->getChildByTag(kTagCaidai);
    caidai->runAction(CCFadeIn::create(0.3));
    CCSprite* biankuang=(CCSprite*)jiangli->getChildByTag(kTagBiankuang);
    biankuang->setVisible(true);
    biankuang->runAction(CCSpawn::create(CCFadeOut::create(0.3),CCScaleTo::create(0.3, 1),NULL));
    S_AE->playEffect("renwu_gaizhang.mp3");
}
