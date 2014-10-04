//
//  MicRecordPlayer.h
//  PointPen
//
//  Created by wanghongjun on 13-9-22.
//
//

#ifndef __PointPen__MicRecordPlayer__
#define __PointPen__MicRecordPlayer__

#include "cocos2d.h"
USING_NS_CC;

typedef enum{
    kPlayResumed=0,
    kPlayStopped,
    kPeakPowerLow,
    kPeakPowerMiddle,
    kPeakPowerHigh
}MicCallBackCMD;

typedef enum{
    kStop=0,
    kMotion,
    kTalk
}MicDelegateMode;

class MicRecordPlayerDelegate{
public:
    virtual void micCallBack(MicCallBackCMD cmd)=0;
    virtual MicDelegateMode getMicDelegateMode()=0;
};

class MicRecordPlayer:public CCObject
{
public:
    void changeMode(MicDelegateMode mode);
public:
    MicRecordPlayer(MicRecordPlayerDelegate *indexScene);
    ~MicRecordPlayer();
};



#endif /* defined(__PointPen__MicRecordPlayer__) */
