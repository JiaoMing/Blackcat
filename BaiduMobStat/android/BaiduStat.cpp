//
//  BaiduStat.cpp
//  Blackcat
//
//  Created by haojunhua on 14-3-27.
//
//

#include "BaiduStat.h"
#include "cocos2d.h"
USING_NS_CC;
#include "Java_com_kidsedu_KEHelper.h"

void BaiduStat::onStatEvent(BaiduStatEvent event, const char *event_id, const char *label, int shareData){
    onStatEventJNI(event,event_id,label,shareData);
}