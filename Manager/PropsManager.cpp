//
//  PropsManager.h
//  Blackcat
//
//  Created by 俊华 郝 on 12-8-9.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "PropsManager.h"
#include "resource.h"
using namespace std;

static PropsManager* s_pPropsManager = NULL;

static vector<int> aniListToVector(string sAniList){
    vector<int> aniVector;
    string sOrder = sAniList;
    int begin = 0, end = 0;
    while (end != -1) {
        end = (int)sOrder.find("-", begin);
        string tmp = "";
        if (end == -1) {
            tmp = sOrder.substr(begin, sOrder.length() - begin);
        } else {
            tmp = sOrder.substr(begin, end - begin);
        }
        aniVector.push_back(atoi(tmp.c_str()));
        begin = end + strlen("-");
    }
    return aniVector;
}


PropsManager* PropsManager::sharedPropsManager()
{
    if (s_pPropsManager == NULL) {
        s_pPropsManager = new PropsManager();
    }
    return s_pPropsManager;
}

PropsManager::PropsManager(){
    m_dictRoot = CCDictionary::createWithContentsOfFileThreadSafe("props.plist");
}

PropsManager::~PropsManager(){
    CC_SAFE_RELEASE(m_dictRoot);
}

Props* PropsManager::getPropsWithName(const char* name)
{
    Props* props=new Props(name);
    CCDictionary* animateDict=(CCDictionary*)m_dictRoot->objectForKey(name);
    const CCString* stopPosName=animateDict->valueForKey("stopPos");
    CCSpriteFrame* stopPos=S_SF->spriteFrameByName(stopPosName->getCString());
    props->initWithSpriteFrame(stopPos);
    
    const CCString* hasXingxing=animateDict->valueForKey("hasXingxing");
    if (hasXingxing->length()>0) {
       props->setHasXingxing(hasXingxing->boolValue());
    }else{
        props->setHasXingxing(false);
    }
    
    const CCString* _touchRect=animateDict->valueForKey("touchableRect");
    if (_touchRect) {
//        props->setTouchableRect(S_RM->getGLRectWithName(_touchRect->getCString()));
        props->setTouchRect(S_RM->getGLRectWithName(_touchRect->getCString()));
    }
    //变量中以‘_’开头表示“从dict中读出的值”
    CCArray* _animates=(CCArray*)animateDict->objectForKey("animates");
    CCObject *animateObj = NULL;
    CCARRAY_FOREACH(_animates, animateObj){
        CCDictionary* _animateItem=(CCDictionary*)animateObj;
        const CCString* _timer=_animateItem->valueForKey("timer");
        float timer=_timer->floatValue();
        const CCString* _animate=_animateItem->valueForKey("animate");
        const CCString* _prefix=_animateItem->valueForKey("prefix");
        if (_prefix->length()==0) {
            _prefix=CCString::createWithFormat("%s_",name);
        }
        const CCString* _audio=_animateItem->valueForKey("audio");
        CCString* audioName;
        if (_audio->length()==0) {
            audioName=CCString::createWithFormat("%s.mp3",name);
        }else{
            audioName=CCString::createWithFormat("%s",_audio->getCString());
        }
        
        vector<int> aniVector=aniListToVector(_animate->m_sString);
        vector<int>::iterator it;
        CCAnimation* animation=CCAnimation::create();
        for (it=aniVector.begin(); it!=aniVector.end(); it++) {
            CCString* fileName=CCString::createWithFormat("%s%d.png",_prefix->getCString(),*it);
            animation->addSpriteFrame(S_SF->spriteFrameByName(fileName->getCString()));
        }
        animation->setDelayPerUnit(timer);
        animation->setRestoreOriginalFrame(true);
        CCAnimate* animate=CCAnimate::create(animation);
        
        AnimateAndAudio* aaa=new AnimateAndAudio();
        aaa->autorelease();
        aaa->setAndRetainAnimate(animate);
        aaa->setAndRetainAudioName(audioName);
        
        props->getAnimates()->addObject(aaa);
    }
    
    props->setStopPos(stopPos);
    props->autorelease();
    return props;
}