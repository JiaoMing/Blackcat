//
//  AsyncLoadPlist.cpp
//  AsynLoadPlist
//
//  Created by shuang on 12-11-20.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "AsyncLoadPlist.h"
#include <queue>
#include <list>
#include "pthread.h"
//#include "CCThread.h"
#include "semaphore.h"
#include <errno.h>

static AsyncLoadPlist *g_pInstance = NULL;

typedef struct _LoadStruct
{
    std::string texturePath;
    std::string plistFileName;
    CCObject    *target;
    SEL_CallFuncO        selector;
} LoadStruct;

typedef struct _ImageInfo
{
    LoadStruct *loadStruct;
    CCImage        *image;
    CCImage::EImageFormat imageType;
} ImageInfo;

static pthread_t s_loadingThread;

static pthread_mutex_t      s_loadStructQueueMutex;
static pthread_mutex_t      s_ImageInfoMutex;

static sem_t* s_pSem = NULL;
static unsigned long s_nAsyncRefCount = 0;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#define CC_ASYNC_TEXTURE_CACHE_USE_NAMED_SEMAPHORE 1
#else
#define CC_ASYNC_TEXTURE_CACHE_USE_NAMED_SEMAPHORE 0
#endif


#if CC_ASYNC_TEXTURE_CACHE_USE_NAMED_SEMAPHORE
#define CC_ASYNC_TEXTURE_CACHE_SEMAPHORE "myAsync"
#else
static sem_t s_sem;
#endif


static bool need_quit = false;

static std::queue<LoadStruct*>* s_pLoadStructQueue = NULL;
static std::queue<ImageInfo*>*   s_pImageQueue = NULL;

static CCImage::EImageFormat computeImageFormatType(std::string& filename)
{
    CCImage::EImageFormat ret = CCImage::kFmtUnKnown;
    
    if ((std::string::npos != filename.find(".jpg")) || (std::string::npos != filename.find(".jpeg")))
    {
        ret = CCImage::kFmtJpg;
    }
    else if ((std::string::npos != filename.find(".png")) || (std::string::npos != filename.find(".PNG")))
    {
        ret = CCImage::kFmtPng;
    }
    else if ((std::string::npos != filename.find(".tiff")) || (std::string::npos != filename.find(".TIFF")))
    {
        ret = CCImage::kFmtTiff;
    }
    
    return ret;
}

static void* loadImage(void* data)
{
    // create autorelease pool for iOS
    CCThread thread;
    thread.createAutoreleasePool();
    
    LoadStruct *pLoadStruct = NULL;
    
    while (true)
    {
        // wait for rendering thread to ask for loading if s_pAsyncStructQueue is empty
        int semWaitRet = sem_wait(s_pSem);
        if( semWaitRet < 0 )
        {
            CCLOG( "CCTextureCache async thread semaphore error: %s\n", strerror( errno ) );
            break;
        }
        
        std::queue<LoadStruct*> *pQueue = s_pLoadStructQueue;
        
        pthread_mutex_lock(&s_loadStructQueueMutex);// get async struct from queue
        if (pQueue->empty())
        {
            pthread_mutex_unlock(&s_loadStructQueueMutex);
            if (need_quit)
                break;
            else
                continue;
        }
        else
        {
            pLoadStruct = pQueue->front();
            pQueue->pop();
            pthread_mutex_unlock(&s_loadStructQueueMutex);
        }        
        
        const char *filename = pLoadStruct->texturePath.c_str();
        
        // compute image type
        CCImage::EImageFormat imageType = computeImageFormatType(pLoadStruct->texturePath);
        if (imageType == CCImage::kFmtUnKnown)
        {
            CCLOG("unsupported format %s",filename);
            delete pLoadStruct;
            
            continue;
        }
        
        // generate image            
        CCImage *pImage = new CCImage();
        if (! pImage->initWithImageFileThreadSafe(filename, imageType))
        {
            delete pImage;
            CCLOG("can not load %s", filename);
            continue;
        }
        
        // generate image info
        ImageInfo *pImageInfo = new ImageInfo();
        pImageInfo->loadStruct = pLoadStruct;
        pImageInfo->image = pImage;
        pImageInfo->imageType = imageType;
        
        // put the image info into the queue
        pthread_mutex_lock(&s_ImageInfoMutex);
        s_pImageQueue->push(pImageInfo);
        pthread_mutex_unlock(&s_ImageInfoMutex);    
    }
    
    if( s_pSem != NULL )
    {
#if CC_ASYNC_TEXTURE_CACHE_USE_NAMED_SEMAPHORE
        sem_unlink(CC_ASYNC_TEXTURE_CACHE_SEMAPHORE);
        sem_close(s_pSem);
#else
        sem_destroy(s_pSem);
#endif
        s_pSem = NULL;
        delete s_pLoadStructQueue;
        delete s_pImageQueue;
    }
    
    return 0;
}

AsyncLoadPlist * AsyncLoadPlist::sharedAsyncLoadPlist()
{
    if (g_pInstance == NULL) {
        g_pInstance = new AsyncLoadPlist();
    }
    return g_pInstance;
}

AsyncLoadPlist::AsyncLoadPlist()
{}

AsyncLoadPlist::~AsyncLoadPlist()
{
    need_quit = true;
    if (s_pSem != NULL)
    {
        sem_post(s_pSem);
    }
}

void AsyncLoadPlist::purgeAsyncLoadPlist()
{
    CC_SAFE_RELEASE_NULL(g_pInstance);
}

void AsyncLoadPlist::loadPlistAsync(const char *pszPlist, cocos2d::CCObject *target, SEL_CallFuncO selector)
{
    CCAssert(pszPlist, "plist filename should not be NULL");
    
    // 读取plist数据，获得图片路径
    const char *pszPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pszPlist).c_str();
    CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(pszPath);
    
    std::string texturePath("");
    
    CCDictionary* metadataDict = (CCDictionary*)dict->objectForKey("metadata");
    if (metadataDict)
    {
        // try to read  texture file name from meta data
        texturePath = metadataDict->valueForKey("textureFileName")->getCString();
    }
    
	// CCDictionary::createWithContentsOfFileThreadSafe 需要释放dict
	dict->release();

    if (! texturePath.empty())
    {
        // build texture path relative to plist file
        texturePath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(texturePath.c_str(), pszPath);
    }
    else
    {
        CCAssert(0, "在plist文件中没有找到图片文件");
        return;
    }
    
    // 从texturePath中取文件名 如/../../filename.png 则filename.png
    std::string textureFileName = texturePath.substr(texturePath.rfind('/') + 1);
    
    // 从TextureCache中找到纹理就直接返回
    CCTexture2D *texture = CCTextureCache::sharedTextureCache()->textureForKey(textureFileName.c_str());
    if (texture != NULL)
    {
        if (target && selector)
        {
            (target->*selector)(CCString::create(pszPlist));
        }
        
        return;
    }
    
    // lazy init
    if (s_pSem == NULL)
    {             
#if CC_ASYNC_TEXTURE_CACHE_USE_NAMED_SEMAPHORE
        s_pSem = sem_open(CC_ASYNC_TEXTURE_CACHE_SEMAPHORE, O_CREAT, 0644, 0);
        if( s_pSem == SEM_FAILED )
        {
            CCLOG( "CCTextureCache async thread semaphore init error: %s\n", strerror( errno ) );
            s_pSem = NULL;
            return;
        }
#else
        int semInitRet = sem_init(&s_sem, 0, 0);
        if( semInitRet < 0 )
        {
            CCLOG( "CCTextureCache async thread semaphore init error: %s\n", strerror( errno ) );
            return;
        }
        s_pSem = &s_sem;
#endif
        s_pLoadStructQueue = new std::queue<LoadStruct*>();
        s_pImageQueue = new std::queue<ImageInfo*>();        
        
        pthread_mutex_init(&s_loadStructQueueMutex, NULL);
        pthread_mutex_init(&s_ImageInfoMutex, NULL);
        pthread_create(&s_loadingThread, NULL, loadImage, NULL);
        
        need_quit = false;
    }
    
    if (0 == s_nAsyncRefCount)
    {
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(AsyncLoadPlist::loadPlistAsyncCallBack), this, 0, false);
    }
    
    ++s_nAsyncRefCount;
    
    if (target)
    {
        target->retain();
    }
    
    // generate async struct
    LoadStruct *data = new LoadStruct();
    data->texturePath = texturePath.c_str();
    data->plistFileName = pszPlist;
    data->target = target;
    data->selector = selector;
    
    // add async struct into queue
    pthread_mutex_lock(&s_loadStructQueueMutex);
    s_pLoadStructQueue->push(data);
    pthread_mutex_unlock(&s_loadStructQueueMutex);
    
    sem_post(s_pSem);
}

void AsyncLoadPlist::loadPlistAsyncCallBack(float dt)
{
    // the image is generated in loading thread
    std::queue<ImageInfo*> *imagesQueue = s_pImageQueue;
    
    pthread_mutex_lock(&s_ImageInfoMutex);
    if (imagesQueue->empty())
    {
        pthread_mutex_unlock(&s_ImageInfoMutex);
    }
    else
    {
        ImageInfo *pImageInfo = imagesQueue->front();
        imagesQueue->pop();
        pthread_mutex_unlock(&s_ImageInfoMutex);
        
        LoadStruct *pLoadStruct = pImageInfo->loadStruct;
        CCImage *pImage = pImageInfo->image;
        
        CCObject *target = pLoadStruct->target;
        SEL_CallFuncO selector = pLoadStruct->selector;
        
        std::string texturePath = pLoadStruct->texturePath;
        // 从texturePath中取文件名 如/../../filename.png 则filename.png
        std::string textureFileName = texturePath.substr(texturePath.rfind('/') + 1);
        
        CCTextureCache::sharedTextureCache()->addUIImage(pImage, textureFileName.c_str());
        
        if (target && selector)
        {
            (target->*selector)(CCString::create(pLoadStruct->plistFileName));
            target->release();
        }
        
        pImage->release();
        delete pLoadStruct;
        delete pImageInfo;
        
        --s_nAsyncRefCount;
        if (0 == s_nAsyncRefCount)
        {
            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(AsyncLoadPlist::loadPlistAsyncCallBack), this);
        }
    }
}
