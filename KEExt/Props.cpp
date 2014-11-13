//
//  Props.cpp
//  Blackcat
//
//  Created by haojunhua on 13-12-5.
//
//

#include "Props.h"
#include "resource.h"

Props::Props(string key){
    m_animates=CCArray::create();
    m_animates->retain();
    m_isImmediate=false;
    m_isShowStopPosAfterARun=true;
    m_touchRect=CCRectMake(0, 0, 0, 0);
    
    m_prop=new Prop();
    m_prop->setIntid(0);
    S_DM->getByProperty(m_prop, "key", key);
    if (m_prop->getid()==0) {
        m_prop->setkey(key.c_str());
        m_prop->setday("");
        CCString* sql=CCString::createWithFormat("insert into prop(key,day) values('%s','')",m_prop->getkey().c_str());
        S_DM->executeSql(sql->getCString());
    }
}

Props::~Props(){
    m_animates->release();
    CC_SAFE_DELETE(m_prop);
}

void Props::showStopPose(){
    if(this->numberOfRunningActions()>0){
        this->stopAllActions();
    }
    this->setDisplayFrame(m_stopPos);
}

void Props::runRandomAnimate(unsigned int animateTime,bool willPlayMusic,bool willCallback){
    int count=m_animates->count();
    int random=(int) (CCRANDOM_0_1()*count);
    this->runAnimate(random,animateTime,0,willCallback,willPlayMusic);
}

void Props::runAnimate(int animateIndex,unsigned int animateTime,unsigned int animateInterval,bool willCallback,bool willPlayMusic){
    if(this->numberOfRunningActions()>0){
        this->stopAllActions();
    }
    
    AnimateAndAudio* aaa=(AnimateAndAudio*)(m_animates->objectAtIndex(animateIndex));
    if (animateTime==0){
        if (animateInterval==0) {
            this->runAction(CCRepeatForever::create(aaa->getAnimate()));
        }else{
            this->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(aaa->getAnimate(), CCDelayTime::create(animateInterval))));
        }
    }else{
        CCArray* array=CCArray::create();
        if(animateTime==1){
            array->addObject(aaa->getAnimate());
        }else if(animateTime>1){
            CCActionInterval* animateAction;
            if (animateInterval==0) {
                animateAction=aaa->getAnimate();
            }else{
                animateAction=CCSequence::createWithTwoActions(aaa->getAnimate(),CCDelayTime::create(animateInterval));
            }
            array->addObject(CCRepeat::create(animateAction, animateTime));
        }
        if (willCallback) {
            array->addObject(CCCallFunc::create(this, callfunc_selector(Props::runCallBack)));
        }else if(animateTime>0){
            array->addObject(CCCallFunc::create(this, callfunc_selector(Props::showStopPose)));
        }
        CCAction *pAction=CCSequence::create(array);
        this->runAction(pAction);
    }
    
    if (willPlayMusic) {
        S_AE->playEffect(aaa->getAudioName()->getCString());
    }
}

void Props::runCallBack(){
    if (this->getHasXingxing()&&TimeUtils::getYYYYMMDD()!=m_prop->getday()) {
        S_LM->gain(1, this->getPosition());
        m_prop->setday(TimeUtils::getYYYYMMDD().c_str());
        
        CCString* sql=CCString::createWithFormat("update prop set day='%s' where id=%d",m_prop->getday().c_str(),m_prop->getid());
        S_DM->executeSql(sql->getCString());
    }
    
    if(m_isShowStopPosAfterARun){
        this->showStopPose();
    }
    if (m_pTarget && m_pSelector) {
        (m_pTarget->*m_pSelector)(this);
    }
}

void Props::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
    //*** 其实只需要修改父类clicksprite 的touchableRect，待修改
    //不过需要注意坐标锚点，此处和clicksprite锚点不一致
    
    CCPoint pt = pTouch->getLocation();
    pt = convertToNodeSpace(pt);
    CCRect rect=m_touchRect;
    if (rect.equals(CCRectZero)) {
        rect=CCRect(0, 0, getContentSize().width, getContentSize().height);
    }
    if (!m_bIsMoved&&rect.containsPoint(pt) && m_bVisible) {
        if (!m_isImmediate) {
            this->runAnimate();
        }else{
            this->runCallBack();
        }
    }
}

void Props::setHandlerPriority(int newPriority)
{
    CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    pDispatcher->setPriority(newPriority, this);
}
