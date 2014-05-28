//
//  UserBarLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-5-9.
//
//

#include "UserBarLayer.h"
#include "resource.h"

#define TAG_XINGXING_FLOAT 10

UserBarLayer::UserBarLayer(){
    m_levelObject=(LevelObject*)(S_LM->getLevelObject())->copy();
    S_LM->setDelegate(this);
}

UserBarLayer::~UserBarLayer(){
    m_levelObject->release();
    S_LM->setDelegate(NULL);
}

void UserBarLayer::onEnter(){
    CCLayer::onEnter();
}

void UserBarLayer::onExit(){
    CCLayer::onExit();
}


bool UserBarLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    CCSprite* touxiangBg=CCSprite::createWithSpriteFrameName("touxiang_xiangkuang.png");
    float relativeHeight=touxiangBg->getContentSize().height;
    touxiangBg->setPosition(S_RM->getRelativePosition("user_touxiang", relativeHeight));
    
    CCSprite* touxiang=CCSprite::createWithSpriteFrameName("default_avatar2.png");
    touxiang->setPosition(S_RM->getRelativePosition("user_touxiang", relativeHeight));
    this->addChild(touxiang);
    
    this->addChild(touxiangBg);
    
    m_username=CCLabelTTF::create(S_UD->getStringForKey(UDKEY_USER_USERNAME).c_str(), "KaiTi.ttf", 25);
    m_username->setColor(ccc3(0, 0, 0));
    m_mingzi9Sprite=CCScale9Sprite::createWithSpriteFrameName("touxiang_mingzikuang.png", CCRectMake(10, 10, 17, 17));
    m_mingzi9Sprite->setAnchorPoint(ccp(0, 0.5));
    m_mingzi9Sprite->setPosition(S_RM->getRelativePosition("user_mingzi_left", relativeHeight));
    CCSize size=CCSizeMake(m_username->getContentSize().width+20, 37);
    if (size.width>150) {
        m_username->setScale(150/size.width);
        size.width=150;
    }
    m_username->setPosition(ccp(size.width/2,size.height/2));
//    m_mingzi9Sprite->addChild(m_username);
    m_mingzi9Sprite->setContentSize(size);
    m_username->setAnchorPoint(ccp(0, 0.5));
    m_username->setPosition(S_RM->getRelativePosition("user_mingzi_left", relativeHeight));
    this->addChild(m_username);
    
    
    
//    this->addChild(m_mingzi9Sprite);
    
    CCPoint jinduPoint=S_RM->getRelativePosition("user_jindu", relativeHeight);
    CCSprite* progressBg = CCSprite::createWithSpriteFrameName("touxiang_jindu_1.png");
    progressBg->setPosition(jinduPoint);
    this->addChild(progressBg);
    CCSprite* progressFg = CCSprite::createWithSpriteFrameName("touxiang_jindu_2.png");
    m_progress = CCProgressTimer::create(progressFg);
    m_progress->setType(kCCProgressTimerTypeBar);
    m_progress->setMidpoint(ccp(0, 0));
    m_progress->setBarChangeRate(ccp(1, 0));
    m_progress->setPercentage(m_levelObject->getProcess()*100);
    m_progress->setPosition(jinduPoint);
    this->addChild(m_progress);
    
    CCPoint xingxingPoint=S_RM->getRelativePosition("user_xingxing", relativeHeight);
    CCSprite* xingxing=CCSprite::createWithSpriteFrameName("touxiang_xing.png");
    xingxing->setPosition(xingxingPoint);
    this->addChild(xingxing);
    
    CCString* level=CCString::createWithFormat("%d",m_levelObject->getLevel());
    m_levelLabel=CCLabelTTF::create(level->getCString(), "", 17);
    m_levelLabel->setColor(ccc3(0, 0, 0));
    m_levelLabel->setPosition(S_RM->getRelativePosition("user_xingxing", relativeHeight));
    this->addChild(m_levelLabel);
    
    CCString* exp=CCString::createWithFormat("%d/%d",m_levelObject->getExp(),m_levelObject->getExpNextLevel());
    m_expLabel=CCLabelTTF::create(exp->getCString(), "", 17);
    m_expLabel->setColor(ccc3(0, 0, 0));
    m_expLabel->setPosition(S_RM->getRelativePosition("user_jindu", relativeHeight));
    this->addChild(m_expLabel);
    
    m_xingDisplayPoint=xingxingPoint;
    
    if (S_UD->getStringForKey(UDKEY_USER_USERNAME).length()==0) {
        this->setVisible(false);
    }
    
    return true;
}

void UserBarLayer::fresh(){
    m_levelObject->release();
    m_levelObject=(LevelObject*)(S_LM->getLevelObject())->copy();
    
    m_username->setString(S_UD->getStringForKey(UDKEY_USER_USERNAME).c_str());
    CCSize size=CCSizeMake(m_username->getContentSize().width+20, 37);
    if (size.width>150) {
        m_username->setScale(150/size.width);
        size.width=150;
    }
    m_username->setPosition(ccp(size.width/2,size.height/2));
//    m_mingzi9Sprite->setContentSize(size);
    
    
    CCString* level=CCString::createWithFormat("%d",m_levelObject->getLevel());
    m_levelLabel->setString(level->getCString());
    CCString* exp=CCString::createWithFormat("%d/%d",m_levelObject->getExp(),m_levelObject->getExpNextLevel());
    m_expLabel->setString(exp->getCString());
    
    m_progress->setPercentage(m_levelObject->getProcess()*100);
}

void UserBarLayer::gainExp(CCPoint worldPoint,CCPointArray* array){
    m_zOrder=this->getZOrder();
    this->setZOrder(INT_MAX);
    
    //测试用例
    if (worldPoint.x==-1) {
        this->expUp();
        return;
    }
    
    if (array==NULL) {
        array=CCPointArray::create(1);
        array->addControlPoint(worldPoint);
    }
    
    for (int i=0; i<array->count(); i++) {
        CCPoint point=this->convertToNodeSpace(array->getControlPointAtIndex(i));
        
        CCSprite* xingguang1=CCSprite::createWithSpriteFrameName("xingguang.png");
        xingguang1->runAction(CCRepeatForever::create(CCRotateBy::create(5, 360)));
        CCSprite* xingguang2=CCSprite::createWithSpriteFrameName("xingguang.png");
        xingguang2->runAction(CCRepeatForever::create(CCRotateBy::create(5, -360)));
        
        CCSprite* xingxing=CCSprite::createWithSpriteFrameName("xing_float.png");
        xingxing->setTag(TAG_XINGXING_FLOAT);
        xingxing->setPosition(point);
        CCPoint midPoint=CCPoint(xingxing->getContentSize().width/2, xingxing->getContentSize().height/2);
        xingguang1->setPosition(midPoint);
        xingguang2->setPosition(midPoint);
        xingxing->addChild(xingguang1,-1);
        xingxing->addChild(xingguang2,-2);
        this->addChild(xingxing);
        
        CCFiniteTimeAction* delay=CCDelayTime::create(i*(0.6/array->count()));
        
        CCFiniteTimeAction* moveTo=CCMoveTo::create(1.5, m_xingDisplayPoint);
        CCFiniteTimeAction* scaleTo=CCScaleTo::create(1.5, 0.2);
        CCFiniteTimeAction* spawn=CCSpawn::create(moveTo,scaleTo,NULL);
        CCFiniteTimeAction* fadeOut=CCFadeOut::create(0.5);
        CCFiniteTimeAction* callFunc=CCCallFunc::create(this, callfunc_selector(UserBarLayer::expUp));
        
        CCArray* actionArray=CCArray::create();
        actionArray->addObject(delay);
        actionArray->addObject(spawn);
        actionArray->addObject(fadeOut);
        if (i==array->count()-1) {
            actionArray->addObject(callFunc);
        }
        xingxing->runAction(CCSequence::create(actionArray));
        
        CCFiniteTimeAction* guangAction=CCSequence::create(delay,CCFadeOut::create(2),NULL);
        xingguang1->runAction(guangAction);
        xingguang2->runAction((CCFiniteTimeAction*)guangAction->copy());
    }
}

void UserBarLayer::expUp(){
    this->removeChildByTag(TAG_XINGXING_FLOAT);
    LevelObject* levelObject=S_LM->getLevelObject();
    CCAction* action;
    if (m_levelObject->getLevel()==levelObject->getLevel()) {
        action= CCProgressFromTo::create(0.5, m_levelObject->getProcess()*100, levelObject->getProcess()*100);
    }else{
        CCProgressFromTo* progressFromTo1 = CCProgressFromTo::create(0.5, m_levelObject->getProcess()*100, 100);
        CCProgressFromTo* progressFromTo2 = CCProgressFromTo::create(0.5, 0, levelObject->getProcess()*100);
        action=CCSequence::create(progressFromTo1,CCCallFunc::create(this, callfunc_selector(UserBarLayer::levelUp)),progressFromTo2,NULL);
    }
    m_progress->runAction(action);
    
    m_levelObject->release();
    m_levelObject=(LevelObject*)(S_LM->getLevelObject())->copy();
    
    CCString* expNextLevel=CCString::createWithFormat("%d/%d",m_levelObject->getExp(),m_levelObject->getExpNextLevel());
    m_expLabel->setString(expNextLevel->getCString());
    
    this->setZOrder(m_zOrder);
}

void UserBarLayer::levelUp(){
    CCString* level=CCString::createWithFormat("%d",m_levelObject->getLevel());
    m_levelLabel->setString(level->getCString());
}