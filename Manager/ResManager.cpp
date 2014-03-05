//
//  ResManager.cpp
//  iFrameExtractor
//
//  Created by haojunhua on 12-10-9.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "ResManager.h"

#define VISIBLE_SIZE CCDirector::sharedDirector()->getVisibleSize()

static ResManager * g_pResManager = NULL;


ResManager * ResManager::sharedResManager()
{
    if (g_pResManager == NULL) {
        g_pResManager = new ResManager();
    }
    return g_pResManager;
}

ResManager::ResManager()
{
    m_pScenePlistMap = new CCDictionary();
}

ResManager::~ResManager()
{
    CC_SAFE_RELEASE_NULL(m_pScenePlistMap);
    if (g_pResManager) {
        delete g_pResManager;
        g_pResManager = NULL;
    }
    CC_SAFE_RELEASE_NULL(m_pPositionDict);
}

void ResManager::init(const char *pszPosName,float ratio){
    this->init(pszPosName, ratio, NULL, NULL);
}

void ResManager::init(const char *pszPosName,float ratio,const char *pszSceneName, const char *pszPlistData){
    if(pszSceneName!=NULL)this->addSceneRes(pszSceneName, pszPlistData);
    if(pszPosName!=NULL)this->loadPosFile(pszPosName);
    this->setRatio(ratio);
}

void ResManager::setRatio(float ratio)
{
    this->m_fRatio = ratio;
}

void ResManager::addSceneRes(const char *pszSceneName, const char *pszPlistData)
{
    CCArray * pScenePlist = (CCArray *)m_pScenePlistMap->objectForKey(std::string(pszSceneName));
    if (pScenePlist) return;
    
    char szPlistData[1024] = {0};
    sprintf(szPlistData, "%s", pszPlistData);
    
    pScenePlist = new CCArray();
    
    char *pValue = NULL;
    char *pLastValue = szPlistData;
    while ( pLastValue[0] && (pValue = strstr(pLastValue, ","))) {
        pValue[0] = 0;
        if (strlen(pLastValue)) {
            // 非空字符串
            CCString *psStr = new CCString(pLastValue);
            pScenePlist->addObject(psStr);
            psStr->release();
        }
        pLastValue = pValue + 1;
    }
    if (strlen(pLastValue)) {
        CCString *psStr = new CCString(pLastValue);
        pScenePlist->addObject(psStr);
        psStr->release();
    }
    
    m_pScenePlistMap->setObject(pScenePlist, std::string(pszSceneName));
    pScenePlist->release();
    
    loadPlistFile(pszSceneName);
}

void ResManager::removeSceneRes(const char *pszSceneName)
{
    CCArray * pScenePlist = (CCArray *)m_pScenePlistMap->objectForKey(std::string(pszSceneName));
    if (pScenePlist == NULL) return;
    
    char szFileName[128] = {0};
    for (int i = 0; i < pScenePlist->count(); i++) {
        CCString *psStr = (CCString *)pScenePlist->objectAtIndex(i);
        
        sprintf(szFileName, "%s.plist", psStr->m_sString.c_str());
        
        // 删除与plist文件相关的图片和内存资源
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(szFileName);
        
        // 获取对应的图片文件名
        const char *pszPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(szFileName).c_str();
        CCDictionary *dict = CCDictionary::createWithContentsOfFile(pszPath);
        
        std::string texturePath("");
        
        CCDictionary* metadataDict = (CCDictionary*)dict->objectForKey("metadata");
        if (metadataDict)
        {
            // try to read  texture file name from meta data
            texturePath = metadataDict->valueForKey("textureFileName")->getCString();
        }
        if (!texturePath.empty()) {
            // 获取到了，就删除缓存的纹理
            CCTextureCache::sharedTextureCache()->removeTextureForKey(texturePath.c_str());
        }
    }
    
    //从map中删除
    m_pScenePlistMap->removeObjectForKey(std::string(pszSceneName));
}


void ResManager::loadPlistFile(const char *pszSceneName)
{
    if (strlen(pszSceneName) == 0) {
        CCLog("ResMananger::loadPlistFile errer: scenename is null. ");
        return;
    }
    
    char szFileName[128] = {0};
    
    CCArray * pScenePlist = (CCArray *)m_pScenePlistMap->objectForKey(std::string(pszSceneName));
    if (pScenePlist == NULL) return;
    
    for (int i = 0; i < pScenePlist->count(); i++) {
        CCString *psStr = (CCString *)pScenePlist->objectAtIndex(i);
        sprintf(szFileName, "%s.plist", psStr->m_sString.c_str());
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(szFileName);
    }
}

void ResManager::loadPosFile(const char *pszFileName)
{
    char szFileName[128] = {0};
    sprintf(szFileName, "%s.plist", pszFileName);
    m_pPositionDict=CCDictionary::createWithContentsOfFile(szFileName);
    m_pPositionDict->retain();
}

CCSize ResManager::getSizeWithName(const char *pszName)
{
    CCDictionary* pPositionDict=(CCDictionary*)m_pPositionDict->objectForKey(pszName);
    if(pPositionDict!=NULL){
        const CCString *width=pPositionDict->valueForKey("width");
        const CCString *height=pPositionDict->valueForKey("height");
        return CCSizeMake(width->intValue()*m_fRatio, height->intValue()*m_fRatio);
    }else {
        return CCSizeZero;
    }
}

CCPoint ResManager::getPositionWithName(const char *pszName)
{
    CCDictionary* pPositionDict=(CCDictionary*)m_pPositionDict->objectForKey(pszName);
    if(pPositionDict!=NULL){
        const CCString *x=pPositionDict->valueForKey("x");
        const CCString *y=pPositionDict->valueForKey("y");
        return ccp(x->intValue()*m_fRatio,VISIBLE_SIZE.height-y->intValue()*m_fRatio);
    }else {
        return CCPointZero;
    }
}

float ResManager::getScaleWithName(const char *pszName)
{
    CCDictionary* pPositionDict=(CCDictionary*)m_pPositionDict->objectForKey(pszName);
    if(pPositionDict!=NULL){
        const CCString *scale=pPositionDict->valueForKey("scale");
        return scale->floatValue();
    }else {
        return 1;
    }
}

CCPoint ResManager::getRelativePosition(const char *pszName, float nHeightParent)
{
    CCDictionary* pPositionDict=(CCDictionary*)m_pPositionDict->objectForKey(pszName);
    if(pPositionDict!=NULL){
        const CCString *x=pPositionDict->valueForKey("x");
        const CCString *y=pPositionDict->valueForKey("y");
        return ccp(x->intValue()*m_fRatio,nHeightParent-y->intValue()*m_fRatio);
    }else {
        return CCPointZero;
    }
}

CCPoint ResManager::getJpgBgPosition()
{
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    return CCPoint(size.width / 2, size.height / 2);
}

