//
//  DownloadManager.cpp
//  Blackcat
//
//  Created by haojunhua on 14-3-18.
//
//

#include "DownloadManager.h"
#include "stdio.h"
#include "stdlib.h"
#include <memory.h>
#include "curl/curl.h"
#include <sys/stat.h> 
#include "Utils.h"
#include <queue>
#include <errno.h>

static DownloadManager* s_pDownloadManager = NULL;
static const char *postfix = ".tmp";

static long totalSize=0;
static long speed=0;

static CURLM *s_multiHandle = NULL;
static pthread_mutex_t  s_curlQueueMutex;
static std::queue<CURL*>* s_curlQueue = NULL;

static pthread_mutex_t  s_dlObjQueueMutex;

static std::queue<DownloadObject*>* s_dlObjQueue = NULL;

static pthread_t s_downloadThread;

static long getDownloadedSize(DownloadObject* dlObj){
    long dlSize=0;
    
	FILE *file = NULL;
	int index = 0;
    char fileName[255] = {0};
    
    do{
        sprintf(fileName, "%s%s_%d%s", CCFileUtils::sharedFileUtils()->getWritablePath().c_str(), dlObj->getRealFilename().c_str(), index++, postfix);
        file = fopen(fileName, "rb");
        if (file==NULL) {
            break;
        }else{
            fseek(file, 0L, SEEK_END);
            dlSize+= ftell(file);
            dlObj->setTempFileNum(dlObj->getTempFileNum()+1);
        }
    }while (true);
    dlObj->setLastDownloadedSize(dlSize);
    int percent=(int)(dlSize/(dlObj->getFileTotalSize())*100);
    dlObj->setPercent(percent);
    return dlSize;
}

static void mergeFile(DownloadObject* dlObj)
{
    dlObj->setIsMerged(true);
    
    string rootPath=CCFileUtils::sharedFileUtils()->getWritablePath();
    char buffer[8];
    int len;
    
    FILE *file = NULL;
    FILE *wtFile = fopen((rootPath + dlObj->getRealFilename()).c_str(), "wb");
    if(!wtFile)
    {
        CCLOG("can't create file!");
        return;
    }
    int index = 0;
    char fileName[255] = {0};
    do{
        sprintf(fileName, "%s%s_%d%s", rootPath.c_str(), dlObj->getRealFilename().c_str(), index++, postfix);
        file = fopen(fileName, "rb");
        if (file==NULL) {
            break;
        }else{
            while ((len = fread(buffer,1,8,file)) > 0) {
                fwrite(buffer,1,len,wtFile);
            }
            fclose(file);
            FileUtils::removeFile(fileName);
        }
    }while (true);
    
    fclose(wtFile);
}

static size_t writeFunc(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    DownloadObject* dlObj=(DownloadObject*)userdata;
    FILE *fp = dlObj->getCurrentFile();
    size_t written = fwrite(ptr, size, nmemb, fp);
    return written;
}

static int progressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
    DownloadObject* dlObj=(DownloadObject*)ptr;
    
    pthread_mutex_lock(&s_dlObjQueueMutex);
    s_dlObjQueue->push(dlObj);
    pthread_mutex_unlock(&s_dlObjQueueMutex);
    
    if (totalToDownload>0) {
        int downloadedSize=dlObj->getLastDownloadedSize()+nowDownloaded;
        int percent=(int)(downloadedSize/(dlObj->getLastDownloadedSize()+totalToDownload)*100);
        if (percent>0) {
            dlObj->setPercent(percent);
            if (percent==100&&!dlObj->getIsMerged()) {
                fclose(dlObj->getCurrentFile());
                mergeFile(dlObj);
            }
        }
    }
    return 0;
}

static CURL* curl_easy_handler(DownloadObject* dlObj)
{
    dlObj->setRealFilename(dlObj->getExpectFilename());//需要修改
    
    long fromIndex = getDownloadedSize(dlObj);
    
    //生成文件下载范围
    char rangeInfo[50];
    sprintf(rangeInfo, "%ld-%ld", fromIndex, dlObj->getFileTotalSize());
    
    //生成当前的临时文件的文件名
    char fileName[255] = {0};
    sprintf(fileName, "%s%s_%d%s", CCFileUtils::sharedFileUtils()->getWritablePath().c_str(), dlObj->getRealFilename().c_str(), dlObj->getTempFileNum(), postfix);
    
    FILE* file = fopen(fileName, "wb");
    if (!file)
    {
        CCLOG("can not create file");
        return NULL;
    }
    
    dlObj->setCurrentFile(file);
    CURL * http_handle = curl_easy_init();
    curl_easy_setopt(http_handle, CURLOPT_URL, dlObj->getUrl().c_str());
    curl_easy_setopt(http_handle, CURLOPT_RANGE, rangeInfo);
    curl_easy_setopt(http_handle, CURLOPT_WRITEFUNCTION, writeFunc);
    curl_easy_setopt(http_handle, CURLOPT_WRITEDATA, dlObj);
    curl_easy_setopt(http_handle, CURLOPT_NOPROGRESS, false);
    curl_easy_setopt(http_handle, CURLOPT_PROGRESSFUNCTION, progressFunc);
    curl_easy_setopt(http_handle, CURLOPT_PROGRESSDATA, dlObj);
    curl_easy_getinfo(http_handle, CURLINFO_SPEED_DOWNLOAD, &speed);
    curl_easy_getinfo(http_handle, CURLINFO_SIZE_DOWNLOAD, &totalSize);
    return http_handle;
}

static void* downloadThread(void *data){
    
    /* keep number of running handles */
    int still_running;
    
    /* we start some action by calling perform right away */
    curl_multi_perform(s_multiHandle, &still_running);
    
    do {
        struct timeval timeout;
        int rc; /* select() return code */
        
        fd_set fdread;
        fd_set fdwrite;
        fd_set fdexcep;
        int maxfd = -1;
        
        long curl_timeo = -1;
        
        FD_ZERO(&fdread);
        FD_ZERO(&fdwrite);
        FD_ZERO(&fdexcep);
        
        /* set a suitable timeout to play around with */
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        
        curl_multi_timeout(s_multiHandle, &curl_timeo);
        if(curl_timeo >= 0) {
            timeout.tv_sec = curl_timeo / 1000;
            if(timeout.tv_sec > 1)
                timeout.tv_sec = 1;
            else
                timeout.tv_usec = (curl_timeo % 1000) * 1000;
        }
        
        /* get file descriptors from the transfers */
        curl_multi_fdset(s_multiHandle, &fdread, &fdwrite, &fdexcep, &maxfd);
        
        /* In a real-world program you OF COURSE check the return code of the
         function calls.  On success, the value of maxfd is guaranteed to be
         greater or equal than -1.  We call select(maxfd + 1, ...), specially in
         case of (maxfd == -1), we call select(0, ...), which is basically equal
         to sleep. */
        
        rc = select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout);
        
        switch(rc) {
            case -1:
                /* select error */
                still_running = 0;
                printf("select() returns error, this is badness\n");
                break;
            case 0:
            default:
                /* timeout or readable/writable sockets */
                curl_multi_perform(s_multiHandle, &still_running);
                break;
        }
        
        // resume dispatcher selector
        CCDirector::sharedDirector()->getScheduler()->resumeTarget(DownloadManager::sharedDownloadManager());
        
        
    } while(still_running);
    
    
    if (s_curlQueue!=NULL) {
        pthread_mutex_lock(&s_curlQueueMutex);
        while (!s_curlQueue->empty())  {
            CURL* curl=s_curlQueue->front();
            curl_multi_remove_handle(s_multiHandle, curl);
            curl_easy_cleanup(curl);
            s_curlQueue->pop();
        }
        pthread_mutex_unlock(&s_curlQueueMutex);
        
        CC_SAFE_DELETE(s_curlQueue);
        CC_SAFE_DELETE(s_dlObjQueue);
        pthread_mutex_destroy(&s_curlQueueMutex);
        pthread_mutex_destroy(&s_dlObjQueueMutex);
    }
    
    
    curl_multi_cleanup(s_multiHandle);
    
    curl_global_cleanup();
    
    
    return NULL;
}


void DownloadManager::download(DownloadObject* dlObj){
    
    
    if (s_multiHandle==NULL) {
        
        s_curlQueue=new queue<CURL*>();
        s_dlObjQueue=new queue<DownloadObject*>();
        
        curl_global_init(CURL_GLOBAL_DEFAULT);
        
        /* init a multi stack */
        s_multiHandle = curl_multi_init();
        
        pthread_mutex_init(&s_curlQueueMutex, NULL);
        pthread_mutex_init(&s_dlObjQueueMutex, NULL);
        pthread_create(&s_downloadThread, NULL, downloadThread, NULL);
        
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(DownloadManager::downloadCallBack), this, 0, false);
    }
    
    CURL* curl=curl_easy_handler(dlObj);
    
    /* add the individual transfers */
    curl_multi_add_handle(s_multiHandle, curl);
    
    pthread_mutex_lock(&s_curlQueueMutex);
    s_curlQueue->push(curl);
    pthread_mutex_unlock(&s_curlQueueMutex);
    
}


void DownloadManager::downloadCallBack(float t){
    pthread_mutex_lock(&s_dlObjQueueMutex);
    
    if (s_dlObjQueue->empty())
    {
        pthread_mutex_unlock(&s_dlObjQueueMutex);
    }
    else
    {
        DownloadObject* dlObj= s_dlObjQueue->front();
        s_dlObjQueue->pop();
        pthread_mutex_unlock(&s_dlObjQueueMutex);
        
        CCObject *target = dlObj->getTarget();
        SEL_CallFuncO selector = dlObj->getSelector();
        if (target && selector)
        {
            (target->*selector)(dlObj);
        }
    }
    
}


DownloadManager* DownloadManager::sharedDownloadManager()
{
    if (s_pDownloadManager == NULL) {
        s_pDownloadManager = new DownloadManager();
    }
    return s_pDownloadManager;
}

DownloadManager::DownloadManager(){
}

DownloadManager::~DownloadManager(){
    s_pDownloadManager=NULL;
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(DownloadManager::downloadCallBack), s_pDownloadManager);
}
