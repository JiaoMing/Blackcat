//
//  BDStat.h
//  Blackcat
//
//  Created by haojunhua on 14-3-10.
//
//

#ifndef __Blackcat__BDStat__
#define __Blackcat__BDStat__

typedef enum{
    kBaiduOneEvent=0,
    kBaiduOneEventCount,
    kBaiduOneEventStart,
    kBaiduOneEventEnd,
    kBaiduOneEventDuration
}BaiduStatEvent;

class BaiduStat{
public:
    static void onStatEvent(BaiduStatEvent event, const char* event_id, const char* label, int shareData=0);
};

#endif
