//
//  PlatformAction.cpp
//  Blackcat
//
//  Created by haojunhua on 14-8-11.
//
//

#include "PlatformAction.h"


#include "cocos2d.h"
USING_NS_CC;

#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
#include <jni.h>
#include "../cocos2dx/platform/android/jni/JniHelper.h"
#include <android/log.h>

void PlatformAction::playMovie()
{
    //Android播放本地视频
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/kidsedu/Blackcat/BlackcatActivity","playMovie", "()V");
    if (isHave)
    {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}

void PlatformAction::avatar()
{
    //Android播放本地视频
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/kidsedu/Blackcat/BlackcatActivity","avatar", "()V");
    if (isHave)
    {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}
#endif
