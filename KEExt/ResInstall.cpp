//
//  ResInstall.cpp
//  Blackcat
//
//  Created by haojunhua on 14-3-19.
//
//

#include "ResInstall.h"
#include "resource.h"
#include <queue>
#include <errno.h>


typedef struct _InstallStruct
{
    std::string fileName;
    std::string srcFulPath;
    std::string descPath;
    CCObject    *target;
    SEL_CallFunc        selector;
} InstallStruct;

static ResInstall *g_instance = NULL;
static queue<InstallStruct*>* s_installStructQueue = NULL;
static pthread_mutex_t      s_installStructQueueMutex;


//！！！！！！未清空
static queue<InstallStruct*>* s_installStructFinishedQueue = NULL;
static pthread_mutex_t      s_installStructFinishedQueueMutex;

static pthread_mutex_t		s_SleepMutex;
static pthread_cond_t		s_SleepCondition;

static pthread_t s_installThread;

static bool need_quit = false;

static void* installThread(void* data){
    // create autorelease pool for iOS
    CCThread thread;
    thread.createAutoreleasePool();
    
    InstallStruct* installStruct;
    
    while (true)
    {
        
        std::queue<InstallStruct*> *pQueue = s_installStructQueue;
        pthread_mutex_lock(&s_installStructQueueMutex);// get async struct from queue
        if (pQueue->empty())
        {
            pthread_mutex_unlock(&s_installStructQueueMutex);
            if (need_quit) {
                break;
            }
            else {
                pthread_cond_wait(&s_SleepCondition, &s_SleepMutex);
                continue;
            }
        }
        else
        {
            installStruct = pQueue->front();
            pQueue->pop();
            pthread_mutex_unlock(&s_installStructQueueMutex);
            
            char extract[50];
            sprintf(extract, "is%sExtracted",installStruct->fileName.c_str());
            char deposit[50];
            sprintf(deposit, "is%sDeposited",installStruct->fileName.c_str());
//            bool isExtracted=S_UD->getBoolForKey(extract,false);
            bool isExtracted=false;
            if (!isExtracted) {
                CCLog("!isExtracted");
                //转存卡片资源
                bool isDeposited=false;
//                bool isDeposited=S_UD->getBoolForKey(deposit,false);
                string descFullPath=installStruct->descPath+installStruct->fileName;
                if(!isDeposited){
                    CCLog("!isDeposited");
                    if(FileUtils::depositFile(installStruct->srcFulPath,descFullPath)) {
//                        S_UD->setBoolForKey(deposit,true);
                        //                S_UD->flush();
                        CCLog("depositFile");
                    }
                }
                //解压图片资源
                if (FileUtils::unzipOpenFile(descFullPath,installStruct->descPath)) {
                    FileUtils::removeFile(descFullPath);
//                    S_UD->setBoolForKey(extract,true);
                    //            S_UD->flush();
                    CCLog("unzipOpenFile");
                }
            }
            
            
            pthread_mutex_lock(&s_installStructFinishedQueueMutex);
            s_installStructFinishedQueue->push(installStruct);
            pthread_mutex_unlock(&s_installStructFinishedQueueMutex);
            
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(ResInstall::installCallback), g_instance, 0, false);
            
        }
    }
    
    if( s_installStructQueue != NULL )
    {
        
        CC_SAFE_DELETE(s_installStructQueue);
        CC_SAFE_DELETE(s_installStructFinishedQueue);
        
        pthread_mutex_destroy(&s_installStructQueueMutex);
        pthread_mutex_destroy(&s_installStructFinishedQueueMutex);
        pthread_mutex_destroy(&s_SleepMutex);
        pthread_cond_destroy(&s_SleepCondition);
    }
    
    return NULL;
}


ResInstall * ResInstall::sharedResInstall()
{
    if (g_instance == NULL) {
        g_instance = new ResInstall();
    }
    return g_instance;
}

ResInstall::ResInstall(){
    
}

ResInstall::~ResInstall(){
    need_quit = true;
    pthread_cond_signal(&s_SleepCondition);
}

void ResInstall::purgeResInstall(){
    CC_SAFE_RELEASE_NULL(g_instance);
}

void ResInstall::install(const char* filename,CCObject *target, SEL_CallFunc selector){
    
    if (s_installStructQueue == NULL)
    {
        s_installStructQueue = new std::queue<InstallStruct*>();
        s_installStructFinishedQueue = new std::queue<InstallStruct*>();
        
        pthread_mutex_init(&s_installStructFinishedQueueMutex, NULL);
        pthread_mutex_init(&s_installStructQueueMutex, NULL);
        pthread_mutex_init(&s_SleepMutex, NULL);
        pthread_cond_init(&s_SleepCondition, NULL);
        
        pthread_create(&s_installThread, NULL, installThread, NULL);
        
        need_quit = false;
    }
    
    InstallStruct* is= new InstallStruct();
    is->target=target;
    is->selector=selector;
    is->fileName=filename;
    is->srcFulPath=CCFileUtils::sharedFileUtils()->fullPathForFilename(filename);
    is->descPath=CCFileUtils::sharedFileUtils()->getWritablePath();
    
    // add async struct into queue
    pthread_mutex_lock(&s_installStructQueueMutex);
    s_installStructQueue->push(is);
    pthread_mutex_unlock(&s_installStructQueueMutex);
    pthread_cond_signal(&s_SleepCondition);
    
}

void ResInstall::installCallback(float t){
    
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(ResInstall::installCallback), g_instance);
    
    InstallStruct* installStruct;
    std::queue<InstallStruct*> *pQueue = s_installStructFinishedQueue;
    pthread_mutex_lock(&s_installStructFinishedQueueMutex);// get async struct from queue
    installStruct = pQueue->front();
    pQueue->pop();
    pthread_mutex_unlock(&s_installStructFinishedQueueMutex);
    
    CCObject *target = installStruct->target;
    SEL_CallFunc selector = installStruct->selector;
    if (target && selector)
    {
        (target->*selector)();
    }
    
    delete installStruct;
}
