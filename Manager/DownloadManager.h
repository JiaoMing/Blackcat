//
//  DownloadManager.h
//  Blackcat
//
//  Created by haojunhua on 14-3-18.
//
//

#ifndef __Blackcat__DownloadManager__
#define __Blackcat__DownloadManager__

#include "cocos2d.h"
using namespace cocos2d;
using namespace std;

class DownloadObject :public CCObject{
private:
    DownloadObject(){
        isMerged=false;
        isMerging=false;
        tempFileNum=0;
        target=NULL;
        selector=NULL;
    }
    ~DownloadObject(){
        CC_SAFE_RELEASE(target);
    }
public:
    static DownloadObject* create(){
        DownloadObject* dlObj=new DownloadObject();
        dlObj->autorelease();
        return dlObj;
    }
public:
    CC_SYNTHESIZE(string, url, Url);
    CC_SYNTHESIZE(string, expectFilename, ExpectFilename);
    CC_SYNTHESIZE(long, fileTotalSize, FileTotalSize);
    CC_SYNTHESIZE(long, LastdownloadedSize, LastDownloadedSize);
    CC_SYNTHESIZE(SEL_CallFuncO, selector, Selector);
    
    CC_SYNTHESIZE(bool, isMerged, IsMerged);
    CC_SYNTHESIZE(bool, isMerging, IsMerging);
    CC_SYNTHESIZE(string, realFilename, RealFilename);
    CC_SYNTHESIZE(int, tempFileNum, TempFileNum);
    CC_SYNTHESIZE(FILE*, currentFile, CurrentFile);
    
    /*百分比*/
    CC_SYNTHESIZE(int, percent, Percent);
    
//    CC_SYNTHESIZE(CCObject*, target, Target);
    CC_SYNTHESIZE_RETAIN(CCObject*, target, Target);//注意retian和析构的release对应
};

class DownloadManager:public CCObject{
public:
    static DownloadManager* sharedDownloadManager();
    void download(DownloadObject* obj);
    void downloadCallBack(float t);
public:
    ~DownloadManager();
private:
    DownloadManager();
private:
    std::vector<DownloadObject*>* m_dlObjVector;
};


#endif /* defined(__Blackcat__DownloadManager__) */
