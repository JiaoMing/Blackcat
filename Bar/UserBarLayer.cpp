//
//  UserBarLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-5-9.
//
//

#include "UserBarLayer.h"
#include "resource.h"
#include "UserLoginLayer.h"
#include "ChildLayer.h"
#include "ObjectManager.h"

#define TAG_XINGXING_FLOAT 10

UserBarLayer::UserBarLayer(){
    m_levelObject=(LevelObject*)(S_LM->getLevelObject())->copy();
    m_xingxingCallbackCount=0;
}

UserBarLayer::~UserBarLayer(){
    m_levelObject->release();
}

UserBarLayer* UserBarLayer::create()
{
    UserBarLayer* layer = new UserBarLayer;
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

void UserBarLayer::onEnter(){
    CCLayer::onEnter();
    S_LM->setDelegate(this);
    this->fresh();
    S_DR->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

void UserBarLayer::onExit(){
    CCLayer::onExit();
    S_LM->setDelegate(NULL);
    S_DR->getTouchDispatcher()->removeDelegate(this);
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
    m_mingzi9Sprite->addChild(m_username);
    m_mingzi9Sprite->setContentSize(size);
//    m_username->setAnchorPoint(ccp(0, 0.5));
    m_username->setPosition(S_RM->getRelativePosition("user_mingzi_left", relativeHeight));
//    this->addChild(m_username);
    
    this->addChild(m_mingzi9Sprite);
    
    CCPoint jinduPoint=S_RM->getRelativePosition("user_jindu", relativeHeight);
    m_progressGuang = CCSprite::createWithSpriteFrameName("touxiang_jindu_3.png");
    m_progressGuang->setPosition(jinduPoint);
    m_progressGuang->setOpacity(0);
    this->addChild(m_progressGuang);
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
    m_expLabel->setPosition(jinduPoint);
    this->addChild(m_expLabel);
    
    m_xingDisplayPoint=jinduPoint;
    
//    if (S_UD->getStringForKey(UDKEY_USER_USERNAME).length()==0) {
//        this->setVisible(false);
//    }
    
    return true;
}

bool UserBarLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCPoint point=pTouch->getLocation();
    point=this->convertToNodeSpace(point);
    if (CCRectMake(0, 0, 220, 70).containsPoint(point)) {
        
#pragma message "暂时取消点击"
//        return true;
    }
    return false;
}

void UserBarLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCPoint point=pTouch->getLocation();
    point=this->convertToNodeSpace(point);
    if (CCRectMake(0, 0, 220, 70).containsPoint(point)) {
        if (S_UD->getStringForKey("USER_USERNAME", "")=="") {
            UserLoginLayer* layer=UserLoginLayer::create();
            layer->setDelegate(this);
            layer->setPosition(this->convertToNodeSpace(CCPointZero));
            this->addChild(layer,INT_MAX);
        }else{
            ChildLayer* layer=ChildLayer::create();
            layer->setDelegate(this);
            layer->setPosition(this->convertToNodeSpace(CCPointZero));
            this->addChild(layer,INT_MAX);
        }
    }
}

void UserBarLayer::fresh(){
    m_levelObject->release();
    m_levelObject=(LevelObject*)(S_LM->getLevelObject())->copy();
    
    string username=S_UD->getStringForKey(UDKEY_USER_USERNAME);
    if (username.length()==0) {
        username="未登陆";
    }
    m_username->setString(username.c_str());
    CCSize size=CCSizeMake(m_username->getContentSize().width+20, 37);
    if (size.width>150) {
        m_username->setScale(150/size.width);
        size.width=150;
    }
    m_username->setPosition(ccp(size.width/2,size.height/2));
    m_mingzi9Sprite->setContentSize(size);
    
    
    CCString* level=CCString::createWithFormat("%d",m_levelObject->getLevel());
    m_levelLabel->setString(level->getCString());
    CCString* exp=CCString::createWithFormat("%d/%d",m_levelObject->getExp(),m_levelObject->getExpNextLevel());
    m_expLabel->setString(exp->getCString());
    
    m_progress->setPercentage(m_levelObject->getProcess()*100);
}

void UserBarLayer::gainExp(CCPoint worldPoint,CCPointArray* array){
    m_zOrder=this->getZOrder();
    this->setZOrder(INT_MAX);
    
    m_xingxingCallbackCount=0;
    m_xingxingCount=0;
    
    //测试用例
    if (worldPoint.x==-1) {
        this->xingxingCallback(NULL);
        return;
    }
    
    if (array==NULL) {
        array=CCPointArray::create(1);
        array->addControlPoint(worldPoint);
    }
    
    m_xingxingCount=array->count();
    
    for (int i=0; i<m_xingxingCount; i++) {
        CCPoint point=this->convertToNodeSpace(array->getControlPointAtIndex(i));
        
        CCParticleSun* emitter = CCParticleSun::create();
        emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.png"));
        emitter->setPosition(point);
        this->addChild(emitter);
        
        CCAnimation* animation=CCAnimation::create();
        animation->addSpriteFrame(S_SF->spriteFrameByName("xing_xuanzhuang_1.png"));
        animation->addSpriteFrame(S_SF->spriteFrameByName("xing_xuanzhuang_2.png"));
        animation->addSpriteFrame(S_SF->spriteFrameByName("xing_xuanzhuang_3.png"));
        animation->addSpriteFrame(S_SF->spriteFrameByName("xing_xuanzhuang_4.png"));
        animation->setDelayPerUnit(0.1);
        animation->setRestoreOriginalFrame(true);
        CCAnimate* animate=CCAnimate::create(animation);
        CCSprite* xing=CCSprite::create();
//        xing->setPosition(point);
        emitter->addChild(xing);
        xing->setScale(0.4);
        xing->runAction(CCSequence::create(CCRepeat::create(animate, 5),NULL) );
        
        CCFiniteTimeAction* delay=CCDelayTime::create(i*(0.6/array->count()));
        
        CCFiniteTimeAction* moveTo=CCMoveTo::create(1.5, m_xingDisplayPoint);
        CCFiniteTimeAction* scaleTo=CCScaleTo::create(1.5, 0.4);
        CCFiniteTimeAction* spawn=CCSpawn::create(moveTo,scaleTo,NULL);
        CCFiniteTimeAction* fadeOut=CCFadeOut::create(0.5);
        CCFiniteTimeAction* callFunc=CCCallFuncN::create(this, callfuncN_selector(UserBarLayer::xingxingCallback));
        
        CCArray* actionArray=CCArray::create();
        actionArray->addObject(delay);
        actionArray->addObject(spawn);
        actionArray->addObject(fadeOut);
        actionArray->addObject(callFunc);
        emitter->runAction(CCSequence::create(actionArray));
    }
}

void UserBarLayer::xingxingCallback(CCNode* node){
    if (node) {
        this->removeChild(node);
        m_xingxingCallbackCount++;
        
    }
    if (m_xingxingCallbackCount==m_xingxingCount) {
        float delayToReorder=0;
        m_progressGuang->runAction(CCSequence::create(CCFadeIn::create(0.5),CCFadeOut::create(0.5),NULL));
        
        LevelObject* levelObject=S_LM->getLevelObject();
        CCAction* action;
        if (m_levelObject->getLevel()==levelObject->getLevel()) {
            delayToReorder=0.5;
            action= CCProgressFromTo::create(0.5, m_levelObject->getProcess()*100, levelObject->getProcess()*100);
        }else{
            delayToReorder=1.0;
            CCProgressFromTo* progressFromTo1 = CCProgressFromTo::create(0.5, m_levelObject->getProcess()*100, 100);
            CCProgressFromTo* progressFromTo2 = CCProgressFromTo::create(0.5, 0, levelObject->getProcess()*100);
            action=CCSequence::create(progressFromTo1,CCCallFunc::create(this, callfunc_selector(UserBarLayer::levelUp)),progressFromTo2,NULL);
        }
        m_progress->runAction(action);
        
        m_levelObject->release();
        m_levelObject=(LevelObject*)levelObject->copy();
        
        CCString* expNextLevel=CCString::createWithFormat("%d/%d",m_levelObject->getExp(),m_levelObject->getExpNextLevel());
        m_expLabel->setString(expNextLevel->getCString());
        
        this->scheduleOnce(schedule_selector(UserBarLayer::scheduleReOrder), delayToReorder);
    }
}

void UserBarLayer::levelUp(){
    S_AE->playEffect("mario1.mp3");
    CCString* level=CCString::createWithFormat("%d",m_levelObject->getLevel());
    m_levelLabel->setString(level->getCString());
}

void UserBarLayer::scheduleReOrder(float t){
    //为解决在线程过程中被修改了zorder的情况，增加了INT_MAX判断
    if (this->getZOrder()!=INT_MAX) {
        this->setZOrder(ORDER_USERBAR);
    }else{
        this->setZOrder(m_zOrder);
    }
}