//
//  AsyncLoadPlist.h
//  AsynLoadPlist
//
//  Created by shuang on 12-11-20.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef AsynLoadPlist_AsyncLoadPlist_h
#define AsynLoadPlist_AsyncLoadPlist_h

#include "cocos2d.h"
using namespace cocos2d;

// 在线程中加载图片纹理，然后在回调的主线程里执行CCSpriteFrameCache::sharedSpriteFrameCache->addSpriteFramesWithFile(pszPlist)
class AsyncLoadPlist : public CCObject
{
public:
    AsyncLoadPlist();
    ~AsyncLoadPlist();
    static AsyncLoadPlist *sharedAsyncLoadPlist();
    // 停止线程和释放资源, 在AppDelegate的析构函数里调用下
    void purgeAsyncLoadPlist();
    void loadPlistAsync(const char *pszPlist, CCObject *target, SEL_CallFuncO selector);
    
private:
    void loadPlistAsyncCallBack(float dt);
};

#endif
