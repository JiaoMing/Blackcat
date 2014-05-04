//
//  BaiduStat.cpp
//  Blackcat
//
//  Created by haojunhua on 14-3-10.
//
//

#include "BaiduMobStat.h"
#include "BaiduStat.h"

void BaiduStat::onStatEvent(BaiduStatEvent event, const char *event_id, const char *label, int shareData){
    BaiduMobStat* statTracker = [BaiduMobStat defaultStat];
    NSString* eventId=[NSString stringWithUTF8String:event_id];
    NSString* eventLabel=[NSString stringWithUTF8String:label];
    switch (event) {
        case kBaiduOneEvent:
        [statTracker logEvent:eventId eventLabel:eventLabel];
        break;
        case kBaiduOneEventDuration:
        [statTracker logEventWithDurationTime:eventId eventLabel:eventLabel durationTime:shareData];
        break;
        case kBaiduOneEventStart:
        [statTracker eventStart:eventId eventLabel:eventLabel];
        break;
        case kBaiduOneEventEnd:
        [statTracker eventEnd:eventId eventLabel:eventLabel];
        break;
        default:
        break;
    }
}
