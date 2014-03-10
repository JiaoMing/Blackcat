//
//  BaiduStat.cpp
//  Blackcat
//
//  Created by haojunhua on 14-3-10.
//
//

#include "BaiduStat.h"
#include "resource.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "Java_com_kidsedu_KEHelper.h"
#endif

void BaiduStat::onStatEvent(BaiduStatEvent event, const char *event_id, const char *label, int shareData){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    onStatEvent(event,event_id,label,shareData);
#endif
}
