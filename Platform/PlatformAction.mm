//
//  PlatformAction.cpp
//  Blackcat
//
//  Created by haojunhua on 14-8-7.
//
//

#include "PlatformAction.h"


#include "cocos2d.h"
USING_NS_CC;

#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
#import "CCVideoPlayer.h"

void PlatformAction::playMovie()
{
    //IOS播放本地视频
    [CCVideoPlayer setNoSkip:false];
    [CCVideoPlayer playMovieWithFile:@"global/welcome.mp4"];
}

void PlatformAction::avatar()
{
    
}


#endif
