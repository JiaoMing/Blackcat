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
public:
    CC_SYNTHESIZE(string, url, Url);
    CC_SYNTHESIZE(string, expectFilename, ExpectFilename);
    CC_SYNTHESIZE(long, fileTotalSize, FileTotalSize);
    CC_SYNTHESIZE(long, LastdownloadedSize, LastDownloadedSize);
    CC_SYNTHESIZE(CCObject*, target, Target);
    CC_SYNTHESIZE(SEL_CallFuncO, selector, Selector);
    
    CC_SYNTHESIZE(bool, isMerged, IsMerged);
    CC_SYNTHESIZE(string, realFilename, RealFilename);
    CC_SYNTHESIZE(int, tempFileNum, TempFileNum);
    CC_SYNTHESIZE(FILE*, currentFile, CurrentFile);
    
    /*百分比*/
    CC_SYNTHESIZE(int, percent, Percent);
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
};


#endif /* defined(__Blackcat__DownloadManager__) */
