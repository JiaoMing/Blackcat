//
//  Cartoon.cpp
//  yidiengine
//
//  Created by shuang on 12-5-2.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.

// ratio需要修改
//

#include "Cartoon.h"
#include "cocoa/CCNS.h"
#include "ResManager.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
#include "resource.h"

#define kCartoonActionTag 10

Cartoon::~Cartoon()
{
    CC_SAFE_RELEASE(m_pEye);
    CC_SAFE_RELEASE(m_pMouth);
    CC_SAFE_RELEASE_NULL(m_pAnimationConfigs);
    CC_SAFE_RELEASE_NULL(m_pClipConfigs);
    CC_SAFE_RELEASE_NULL(m_pBoxConfigs);
    CC_SAFE_RELEASE_NULL(m_pActionsDoing);
}

Cartoon * Cartoon::cartoonWithName(const char *pszName)
{
    Cartoon *cartoon = new Cartoon();
    if (cartoon && cartoon->initWithName(pszName)) {
        cartoon->autorelease();
        return cartoon;
    }
    CC_SAFE_DELETE(cartoon);
    return NULL;
}

bool Cartoon::initWithName(const char *pszName)
{
    if ( CCSprite::init() ) {
        m_strName = pszName;
        
        m_pEye = new CCSprite();
        m_pEye->init();
        this->addChild(m_pEye);
        
        m_pMouth = new CCSprite();
        m_pMouth->init();
        this->addChild(m_pMouth);
        
        m_pAnimationConfigs = new CCDictionary();
        m_pClipConfigs = new CCDictionary();
        m_pBoxConfigs = new CCDictionary();
        
        return parseConfigFile2();
    }
    return false;
}

// touch event 

void Cartoon::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    
    CCNode::onEnter();
}

void Cartoon::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    CCNode::onExit();
}

bool Cartoon::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    CCPoint touchLocation = touch->getLocationInView();
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    touchLocation = convertToNodeSpace(touchLocation);
    
    // from left-bottom point to from left-top point
    touchLocation.y = getContentSize().height - touchLocation.y;
    
    if (m_pListener && m_pfnSelector) {
        CCDictElement* pElement = NULL;
        CCDICT_FOREACH(m_pBoxConfigs, pElement)
        {
            CCString *psBox = (CCString*)pElement->getObject();
            CCRect rectOfTouched = CCRectFromString(psBox->m_sString.c_str());
            if (rectOfTouched.containsPoint(touchLocation) && getBoxEnabled(pElement->getStrKey())) {
                (m_pListener->*m_pfnSelector)(this, pElement->getStrKey());
            }
        }
    }
    
    return true;
}

// public function

void Cartoon::setCallback(cocos2d::CCObject *pTarget, SEL_CartoonHandler pSelector)
{
    m_pListener = pTarget;
    m_pfnSelector = pSelector;
}

void Cartoon::setBoxEnabled(const char *pszBoxName, bool enabled)
{
    m_mapBoxEnabled.insert(std::pair<std::string, bool>(std::string(pszBoxName), enabled));
}

bool Cartoon::getBoxEnabled(const char *pszBoxName)
{
    std::map<std::string, bool>::iterator iter;
    iter = m_mapBoxEnabled.find(std::string(pszBoxName));
    if (iter != m_mapBoxEnabled.end()) {
        return iter->second;
    } else {
        // default disabled
        return false;
    }
}

void Cartoon::doAction(CCArray *pActions)
{
    m_nIndexOfAction = 0;
    if (m_pActionsDoing) {
        CC_SAFE_RELEASE_NULL(m_pActionsDoing);
    }
    m_pActionsDoing = (CCArray *)pActions->copy();
    if (m_nIndexOfAction < m_pActionsDoing->count()) {
        doSingleAction(((CCString *)m_pActionsDoing->objectAtIndex(m_nIndexOfAction))->m_sString.c_str());
    }
}

void Cartoon::doAction(const char * pszAction1, ...)
{
    m_nIndexOfAction = 0;
    if (m_pActionsDoing) {
        m_pActionsDoing->removeAllObjects();
    } else {
        m_pActionsDoing = new CCArray();
    }
    
    va_list params;
	va_start(params, pszAction1);
    
    const char *pszAction = pszAction1;
    while (pszAction) {
        CCString *psAction = new CCString(pszAction);
        m_pActionsDoing->addObject(psAction);
        psAction->release();
        
        pszAction = va_arg(params, const char *);
    }
    
    va_end(params);
    
	if (m_nIndexOfAction < m_pActionsDoing->count()) {
        doSingleAction(((CCString *)m_pActionsDoing->objectAtIndex(m_nIndexOfAction))->m_sString.c_str());
    }
}

void Cartoon::saySomething(const char *pszFileName, float duration)
{
    S_ALP->stop();
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
    SimpleAudioEngine::sharedEngine()->playEffect(pszFileName);
    
    MotionClip *clip = (MotionClip *)m_pClipConfigs->objectForKey(m_strClipName);
    if (clip && strlen(clip->getMouthAnimation())) {
        std::string strMouthName = clip->getMouthAnimation();
        AnimationConfig *animationConfig = (AnimationConfig *)m_pAnimationConfigs->objectForKey(strMouthName);
        int frames = animationConfig->getOrderArray()->count();
        // 大概估算嘴巴动作执行的次数
#define kMouthTimer 0.15f
        int times = (int)ceilf(duration / frames / kMouthTimer);
        
        m_pMouth->stopActionByTag(kCartoonActionTag);
		char fullname[100] = {0};
		sprintf(fullname, "%s_%s.png", strMouthName.c_str(), "1");
        m_pMouth->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fullname));
        
        m_pMouth->setPosition(ccp(animationConfig->getOffset().x, getContentSize().height - animationConfig->getOffset().y));
        m_pMouth->setVisible(true);
        
        CCAnimation *animation = CCAnimation::create();
        CCObject *pObject = NULL;
        CCARRAY_FOREACH(animationConfig->getOrderArray(), pObject)
        {
            CCString *psOrder = (CCString *)pObject;
			sprintf(fullname, "%s_%s.png", strMouthName.c_str(), psOrder->m_sString.c_str());
            animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fullname));
        }
        animation->setDelayPerUnit(kMouthTimer);
        animation->setRestoreOriginalFrame(true);
        
        CCAction *action = CCRepeat::create(CCAnimate::create(animation), times);
        action->setTag(kCartoonActionTag);
        m_pMouth->runAction(action);
    }
}

void Cartoon::doCallFuncOAction(cocos2d::CCString *pAction)
{
    // 清空动作
    m_nIndexOfAction = 0;
    if (m_pActionsDoing) {
        CC_SAFE_RELEASE_NULL(m_pActionsDoing);
    }
    
    doSingleAction(pAction->getCString());
}

void Cartoon::updateMouthFrame(short peak)
{
    MotionClip *clip = (MotionClip *)m_pClipConfigs->objectForKey(m_strClipName);
    if (!clip || strlen(clip->getMouthAnimation()) == 0) return;
    float peakPower;
    if (peak <= 0) peakPower = -160.f;
    else peakPower = 20.f*log10f((float)peak / 32768.f);
    
//    int index = 1;
//    if (peakPower < -30.f) {
//        index = 1;
//    } else if (peakPower >= -30.f && peakPower < -20.f) {
//        index = 2;
//    } else if (peakPower >= -20.f && peakPower < -15.f) {
//        index = 3;
//    } else if (peakPower >= -15.f && peakPower < -10.f) {
//        index = 4;
//    } else {
//        index = 5;
//    }
    AnimationConfig *animationConfig = (AnimationConfig *)m_pAnimationConfigs->objectForKey(clip->getMouthAnimation());
    int total = animationConfig->getOrderArray()->count();
    int index = 1;
    float step = 30.0f / (total - 1);
    for (int i = 0; i <= total - 1; i++) {
        if (peakPower < -30.0f + i * step) {
            index = 1 + i;
            break;
        }
    }
    
    if(m_pMouth->isVisible()) {
		char fullname[100] = {0};
		sprintf(fullname, "%s_%d.png", clip->getMouthAnimation(), index);
		CCSpriteFrame * spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fullname);
		if (spriteFrame && !m_pMouth->isFrameDisplayed(spriteFrame)) {
			m_pMouth->setDisplayFrame(spriteFrame);
		}
    }
}

CCAction * Cartoon::runFadeAction(cocos2d::CCAction *action)
{
    runAction(action);
    m_pEye->runAction((CCAction *)action->copy()->autorelease());
    m_pMouth->runAction((CCAction *)action->copy()->autorelease());
    return action;
}

// private function

void Cartoon::doSingleAction(const char *pszAction)
{
    // pszAction format: "actionname:actiontimes" or "actionname"
    char szActionName[32] = {0};
    int times = 1;
    strcpy(szActionName, pszAction);
    char *pszTimes = strstr(szActionName, ":");
    if (pszTimes) {
        pszTimes[0] = NULL;
        times = atoi(++pszTimes);
    }
    
    MotionClip *clip = (MotionClip *)m_pClipConfigs->objectForKey(std::string(szActionName));
    if (clip == NULL) {
        CCLog("%s clip not found, please check %s-anis.ani", szActionName, m_strName.c_str());
        return;
    }
    
    // 记录clip名字
    m_strClipName = szActionName;
    
    CCSpriteFrameCache *spriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    if (strlen(clip->getBodyAnimation())) {
        std::string sBodyAnimation = clip->getBodyAnimation();
        AnimationConfig *animationConfig = (AnimationConfig *)m_pAnimationConfigs->objectForKey(sBodyAnimation);
        stopActionByTag(kCartoonActionTag);
		char fullname[100] = {0};
		sprintf(fullname, "%s_%s.png", sBodyAnimation.c_str(), "1");
        setDisplayFrame(spriteFrameCache->spriteFrameByName(fullname));
        
        CCAnimation *animation = CCAnimation::create();
        CCObject *pObject = NULL;
        CCARRAY_FOREACH(animationConfig->getOrderArray(), pObject)
        {
            CCString *psOrder = (CCString *)pObject;
			sprintf(fullname, "%s_%s.png", sBodyAnimation.c_str(), psOrder->m_sString.c_str());
            animation->addSpriteFrame(spriteFrameCache->spriteFrameByName(fullname));
        }
        animation->setDelayPerUnit(animationConfig->getTimer());
        animation->setRestoreOriginalFrame(true);
        
        if (times == 0) {
            CCAction *action = CCRepeatForever::create(CCAnimate::create(animation));
            action->setTag(kCartoonActionTag);
            runAction(action);
        } else {
            CCAction *action = CCSequence::create(CCRepeat::create(CCAnimate::create(animation), times), CCCallFunc::create(this, callfunc_selector(Cartoon::doNextAction)), NULL);
            action->setTag(kCartoonActionTag);
            runAction(action);
        }
    }
    if (strlen(clip->getEyeAnimation())) {
        std::string sEyeAnimation = clip->getEyeAnimation();
        AnimationConfig *animationConfig = (AnimationConfig *)m_pAnimationConfigs->objectForKey(sEyeAnimation);
        m_pEye->stopActionByTag(kCartoonActionTag);
		char fullname[100] = {0};
		sprintf(fullname, "%s_%s.png", sEyeAnimation.c_str(), "1");
        m_pEye->setDisplayFrame(spriteFrameCache->spriteFrameByName(fullname));
        
        m_pEye->setPosition(ccp(animationConfig->getOffset().x, getContentSize().height - animationConfig->getOffset().y));
        m_pEye->setVisible(true);
        
        CCAnimation *animation = CCAnimation::create();
        CCObject *pObject = NULL;
        CCARRAY_FOREACH(animationConfig->getOrderArray(), pObject)
        {
            CCString *psOrder = (CCString *)pObject;
			sprintf(fullname, "%s_%s.png", sEyeAnimation.c_str(), psOrder->m_sString.c_str());
            animation->addSpriteFrame(spriteFrameCache->spriteFrameByName(fullname));
        }
        animation->setDelayPerUnit(animationConfig->getTimer());
        animation->setRestoreOriginalFrame(true);
        
        CCAction *action = CCSequence::create(CCDelayTime::create(rand() % 5 + 4), CCAnimate::create(animation), CCCallFuncO::create(this, callfuncO_selector(Cartoon::doNextBlink), animationConfig), NULL);
        action->setTag(kCartoonActionTag);
        m_pEye->runAction(action);
    } else {
        m_pEye->stopActionByTag(kCartoonActionTag);
        m_pEye->setVisible(false);
    }
    if (strlen(clip->getMouthAnimation())) {
        std::string sMouthAnimation = clip->getMouthAnimation();
        AnimationConfig *animationConfig = (AnimationConfig *)m_pAnimationConfigs->objectForKey(sMouthAnimation);
        m_pMouth->stopActionByTag(kCartoonActionTag);
		char fullname[100] = {0};
		sprintf(fullname, "%s_%s.png", sMouthAnimation.c_str(), "1");
        m_pMouth->setDisplayFrame(spriteFrameCache->spriteFrameByName(fullname));
        
        m_pMouth->setPosition(ccp(animationConfig->getOffset().x, getContentSize().height - animationConfig->getOffset().y));
        m_pMouth->setVisible(true);
    } else {
        m_pMouth->setVisible(false);
    }
}

void Cartoon::doNextAction()
{
    m_nIndexOfAction++;
    if (m_nIndexOfAction < m_pActionsDoing->count()) {
        doSingleAction(((CCString *)m_pActionsDoing->objectAtIndex(m_nIndexOfAction))->m_sString.c_str());
    } else {
        //CCLog("no action");
    }
}

void Cartoon::doNextBlink(cocos2d::CCObject *object)
{
    AnimationConfig *animationConfig = (AnimationConfig *)object;
    CCSpriteFrameCache *spriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
    CCAnimation *animation = CCAnimation::create();
    CCObject *pObject = NULL;
    CCARRAY_FOREACH(animationConfig->getOrderArray(), pObject)
    {
        CCString *psOrder = (CCString *)pObject;
        char fullname[100] = {0};
        sprintf(fullname, "%s_%s.png", animationConfig->getName(), psOrder->m_sString.c_str());
        animation->addSpriteFrame(spriteFrameCache->spriteFrameByName(fullname));
    }
    animation->setDelayPerUnit(animationConfig->getTimer());
    animation->setRestoreOriginalFrame(true);
    
    CCAction *action = CCSequence::create(CCDelayTime::create(rand() % 5 + 4), CCAnimate::create(animation), CCCallFuncO::create(this, callfuncO_selector(Cartoon::doNextBlink), animationConfig), NULL);
    action->setTag(kCartoonActionTag);
    m_pEye->runAction(action);
}

bool Cartoon::parseConfigFile2()
{
//    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    char szFileName[128] = {0};

    sprintf(szFileName, "%s-anis.ani", m_strName.c_str());
    
    unsigned long fileSize = 0;
    unsigned char *pszFileData = CCFileUtils::sharedFileUtils()->getFileData(CCFileUtils::sharedFileUtils()->fullPathForFilename(szFileName).c_str(), "r", &fileSize);
    if (pszFileData == NULL) return false;
    
    bool inPlistConfig = false, inRatioConfig = false, inAnimationConfig = false, inBoxConfig = false, inClipConfig = false;
    AnimationConfig *pAnimationConfig = NULL;
    MotionClip *pMotionClip = NULL;
    char *pValue = NULL;
    
    //目前写死，需要做修改！！！！！！
    float ratio=1;
    
    char szLineData[1024];
    unsigned long pos = 0, lastpos = 0;
    for (pos = 0; pos <= fileSize; pos++) {
        if (pszFileData[pos] != '\n' && pos != fileSize) continue;
        
        memcpy(szLineData, &pszFileData[lastpos], pos - lastpos);
        szLineData[pos - lastpos] = 0;
        lastpos = pos + 1;
        
        //parse line data
        if (strstr(szLineData, "plist start")) {
            inPlistConfig = true;
            continue;
        } else if (strstr(szLineData, "plist end")) {
            inPlistConfig = false;
            continue;
        } else if (strstr(szLineData, "ratio start")) {
            inPlistConfig = false;
            inRatioConfig = true;
            continue;
        } else if (strstr(szLineData, "ratio end")) {
            inRatioConfig = false;
            continue;
        } else if (strstr(szLineData, "cfg start")) {
            inRatioConfig = false;
            inAnimationConfig = true;
            continue;
        } else if (strstr(szLineData, "cfg end")) {
            if (pAnimationConfig) {
                // save to m_pAnimationConfigs
                if (pAnimationConfig->getOrderArray() == NULL) {
                    CCArray *pOrderArray = new CCArray();
                    
                    int i = 1;
                    char szSpriteName[100] = {0};
                    for (i = 2; ; i++) {
                        sprintf(szSpriteName, "%s_%d.png", pAnimationConfig->getName(), i);
                        CCSpriteFrame *pSpriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(szSpriteName);
                        if (pSpriteFrame == NULL) break;
                        else {
                            sprintf(szSpriteName, "%d", i);
                            CCString *psOrder = new CCString(szSpriteName);
                            pOrderArray->addObject(psOrder);
                            psOrder->release();
                        }
                    }
                    if (i != 1) {
                        sprintf(szSpriteName, "%d", 1);
                        CCString *psOrder = new CCString(szSpriteName);
                        pOrderArray->addObject(psOrder);
                        psOrder->release();
                    }
                    
                    pAnimationConfig->setOrderArray(pOrderArray);
                }
                
                m_pAnimationConfigs->setObject(pAnimationConfig, std::string(pAnimationConfig->getName()));
                pAnimationConfig->release();
                pAnimationConfig = NULL;
            }
            inAnimationConfig = false;
            continue;
        } else if (strstr(szLineData, "box start")) {
            inAnimationConfig = false;
            inBoxConfig = true;
            continue;
        } else if (strstr(szLineData, "box end")) {
            inBoxConfig = false;
            inClipConfig = true;
            continue;
        } else if (strstr(szLineData, "//")) {
            continue;
        }
        
        // erase special char
        do {
            char szTmp[1024] = {0};
            int j = 0;
            for (int i = 0; szLineData[i] != 0; i++) {
                if (szLineData[i] > ' ') {
                    szTmp[j++] = szLineData[i];
                }
            }
            memcpy(szLineData, szTmp, j + 1);
        } while (0);
        
        // blank line
        if (!strlen(szLineData)) continue;
        
        if (inPlistConfig) {
            pValue = strstr(szLineData, ":");
            if (pValue) {
                char szPlistFullName[32] = {0};
                char *pLastValue = pValue + 1;
                while ( pLastValue[0] && (pValue = strstr(pLastValue, ","))) {
                    pValue[0] = 0;
                    if (strlen(pLastValue)) {
                        sprintf(szPlistFullName, "%s.plist", pLastValue);
                        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(szPlistFullName);
                        pLastValue = pValue + 1;
                    }
                }
                    if(strlen(pLastValue)){
                        sprintf(szPlistFullName, "%s.plist", pLastValue);
                        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(szPlistFullName);
                    }
                
            } else {
                CCLog("plist %s : not found", szLineData);
            }
        } else if (inRatioConfig) {
            pValue = strstr(szLineData, ":");
            if (pValue) {
                float sourcew = atof(++pValue);
                pValue = strstr(pValue, ",");
                //float sourceh = atof(++pValue);
                ++pValue;//补充上一行
                     ++pValue;
                pValue = strstr(pValue, ",");
                float destw = atof(++pValue);
                pValue = strstr(pValue, ",");
                //float desth = atof(++pValue);
                ++pValue;//补充上一行
                    ++pValue;
                ratio = ratio * (destw / sourcew);
            } else {
                CCLog("ratio %s : not found", szLineData);
            }
        } else if (inAnimationConfig) {
            if (pAnimationConfig == NULL) {
                pAnimationConfig = new AnimationConfig();
                pAnimationConfig->setName(szLineData);
                continue;
            }
            
            // offset, timer, order key
            if (strstr(szLineData, "offset")) {
                pValue = strstr(szLineData, ":");
                if (pValue) {
                    float numx = atof(++pValue);
                    pValue = strstr(pValue, ",");
                    float numy = atof(++pValue);
                    pAnimationConfig->setOffset(CCPointMake(numx * ratio, numy * ratio));
                } else {
                    CCLog("%s %s : not found", pAnimationConfig->getName(), szLineData);
                }
            } else if (strstr(szLineData, "timer")) {
                pValue = strstr(szLineData, ":");
                if (pValue) {
                    pAnimationConfig->setTimer(atof(pValue + 1));
                } else {
                    CCLog("%s %s : not found", pAnimationConfig->getName(), szLineData);
                }
            } else if (strstr(szLineData, "order")) {
                pValue = strstr(szLineData, ":");
                if (pValue) {
                    CCArray *pOrderArray = new CCArray();
                    std::string sOrder = pValue;
                    int begin = 0, end = 0;
                    while (end != -1) {
                        end = (int)sOrder.find("-", begin);
                        std::string tmp = "";
                        if (end == -1) {
                            tmp = sOrder.substr(begin, sOrder.length() - begin);
                        } else {
                            tmp = sOrder.substr(begin, end - begin);
                        }
                        if (begin > 0) {
                            // note: 1-2-3-4-3-2-1 in file, but in exec animation code should be 2-3-4-3-2-1
                            CCString *psOrder = new CCString(tmp.c_str());
                            pOrderArray->addObject(psOrder);
                            psOrder->release();
                        }
                        begin = end + strlen("-");
                    }
                    pAnimationConfig->setOrderArray(pOrderArray);
                } else {
                    CCLog("%s %s : not found", pAnimationConfig->getName(), szLineData);
                }
            } else {
                if (pAnimationConfig->getOrderArray() == NULL) {
                    CCArray *pOrderArray = new CCArray();
                    
                    int i = 1;
                    char szSpriteName[100] = {0};
                    for (i = 2; ; i++) {
                        sprintf(szSpriteName, "%s_%d.png", pAnimationConfig->getName(), i);
                        CCSpriteFrame *pSpriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(szSpriteName);
                        if (pSpriteFrame == NULL) break;
                        else {
                            sprintf(szSpriteName, "%d", i);
                            CCString *psOrder = new CCString(szSpriteName);
                            pOrderArray->addObject(psOrder);
                            psOrder->release();
                        }
                    }
                    if (i != 1) {
                        sprintf(szSpriteName, "%d", 1);
                        CCString *psOrder = new CCString(szSpriteName);
                        pOrderArray->addObject(psOrder);
                        psOrder->release();
                    }
                    
                    pAnimationConfig->setOrderArray(pOrderArray);
                }
                
                m_pAnimationConfigs->setObject(pAnimationConfig, std::string(pAnimationConfig->getName()));
                pAnimationConfig->release();
                pAnimationConfig = NULL;
                pAnimationConfig = new AnimationConfig();
                pAnimationConfig->setName(szLineData);
            }
        } else if (inBoxConfig) {
            pValue = strstr(szLineData, ":");
            pValue[0] = 0;
            float numx = atof(++pValue);
            pValue = strstr(pValue, ",");
            float numy = atof(++pValue);
            pValue = strstr(pValue, ",");
            float numwidth = atof(++pValue);
            pValue = strstr(pValue, ",");
            float numheight = atof(++pValue);
            
            // convert to dest device, and construct CCRectFromString's string
            char szBox[64] = {0};
            
            ratio=1.0f;//修改！！！！！，此处有问题！
            
            sprintf(szBox, "{{%f, %f}, {%f, %f}}", numx * ratio, numy * ratio, numwidth * ratio, numheight * ratio);
            
            CCString *psBox = new CCString(szBox);
            m_pBoxConfigs->setObject(psBox, std::string(szLineData));
            psBox->release();
        } else if (inClipConfig) {
            if (pMotionClip == NULL) {
                pMotionClip = new MotionClip();
                pMotionClip->setName(szLineData);
                continue;
            }
            
            // action,body,eye,mouth,times key 
            if (strstr(szLineData, "body")){
                pValue = strstr(szLineData, ":");
                if (pValue) {
                    pMotionClip->setBodyAnimation(pValue + 1);
                } else {
                    CCLog("%s %s : not found", pMotionClip->getName(), szLineData);
                }
            } else if (strstr(szLineData, "eye")) {
                pValue = strstr(szLineData, ":");
                if (pValue) {
                    // not equal null
                    if (strcmp(pValue + 1, "null")) pMotionClip->setEyeAnimation(pValue + 1);
                } else {
                    CCLog("%s %s : not found", pMotionClip->getName(), szLineData);
                }
            } else if (strstr(szLineData, "mouth")) {
                pValue = strstr(szLineData, ":");
                if (pValue) {
                    // not equal null
                    if (strcmp(pValue + 1, "null")) pMotionClip->setMouthAnimation(pValue + 1);
                } else {
                    CCLog("%s %s : not found", pMotionClip->getName(), szLineData);
                }
            } else if (strstr(szLineData, "times")) {
                // default 1
                pValue = strstr(szLineData, ":");
                if (pValue) {
                    pMotionClip->setTimes(atoi(pValue + 1));
                } else {
                    CCLog("%s %s : not found", pMotionClip->getName(), szLineData);
                }
            } else {
                // end and start another motionclip
                m_pClipConfigs->setObject(pMotionClip, std::string(pMotionClip->getName()));
                pMotionClip->release();
                pMotionClip = NULL;
                
                // start a new motionclip
                pMotionClip = new MotionClip();
                pMotionClip->setName(szLineData);
            }
        }
    }
    // end of file, save pClips
    if (pMotionClip) {
        m_pClipConfigs->setObject(pMotionClip, std::string(pMotionClip->getName()));
        pMotionClip->release();
        pMotionClip = NULL;
    }
    
    free(pszFileData);
    pszFileData = NULL;
    
    return true;
}

// 调试动作

void Cartoon::debugFirstAction()
{
    debugActionIndex = 0;
    CCArray * keys = m_pClipConfigs->allKeys();
    char szStr[64] = {0};
    sprintf(szStr, "%s:0", ((CCString *)keys->objectAtIndex(debugActionIndex))->getCString());
    CCLog("debuging %s", szStr);
    doAction(szStr, NULL);
}

void Cartoon::debugNextAction()
{
    debugActionIndex++;
    CCArray * keys = m_pClipConfigs->allKeys();
    if (debugActionIndex >= keys->count()) debugActionIndex = 0;
    char szStr[64] = {0};
    sprintf(szStr, "%s:0", ((CCString *)keys->objectAtIndex(debugActionIndex))->getCString());
    CCLog("debuging %s", szStr);
    doAction(szStr, NULL);
}

void Cartoon::debugLastAction()
{
    debugActionIndex--;
    CCArray * keys = m_pClipConfigs->allKeys();
    if (debugActionIndex < 0) debugActionIndex = keys->count() - 1;
    char szStr[64] = {0};
    sprintf(szStr, "%s:0", ((CCString *)keys->objectAtIndex(debugActionIndex))->getCString());
    CCLog("debuging %s", szStr);
    doAction(szStr, NULL);
}

const char * Cartoon::getDebugingAction()
{
    CCArray * keys = m_pClipConfigs->allKeys();
    return ((CCString *)keys->objectAtIndex(debugActionIndex))->getCString();
}

float Cartoon::action(const char* key,bool random,int actionIndex,int audioIndex){
    float time=0;
    ActionFlow* actionFlow=S_AFM->getActionFlowWithKey(key);
    if (actionFlow!=NULL) {
        CCArray* actionArray=actionFlow->getActionArray();
        CCArray* audioArray=actionFlow->getAudioArray();
        int _actionIndex=actionIndex,_audioIndex=audioIndex;
        if (random) {
            _actionIndex=(int)(CCRANDOM_0_1()*actionArray->count());
            _audioIndex=(int)(CCRANDOM_0_1()*audioArray->count());
        }
        this->doAction((CCArray*)actionArray->objectAtIndex(_actionIndex));
        
        //拼音数组允许为空
        if (audioArray->count()>0) {
            CCArray* audioList=(CCArray*)audioArray->objectAtIndex(_audioIndex);
            if (audioList->count()>0) {
                S_ALP->play(audioList);
                CCObject* audioObj = NULL;
                CCARRAY_FOREACH(audioList, audioObj){
                    Audio* audioItem=(Audio*)audioObj;
                    if(audioItem!=NULL){
                        time+=audioItem->getDuration();
                    }
                }
            }
        }
    }
    return time;
}