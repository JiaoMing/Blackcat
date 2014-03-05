//
//  KEMicPlayer.h
//  Blackcat
//
//  Created by haojunhua on 14-1-22.
//
//

#ifndef Blackcat_MicPlayer_h
#define Blackcat_MicPlayer_h

#include "cocos2d.h"
USING_NS_CC;

typedef enum{
    kPlayResumed=0,
    kPlayStopped,
    kPeakPowerLow,
    kPeakPowerMiddle,
    kPeakPowerHigh
}KEMicCallBackCMD;

typedef enum{
    kStop=0,
    kMotion,
    kTalk
}KEMicDelegateMode;

class KEMicPlayer;
class KEMicPlayerImpl;

class KEMicPlayerDelegate{
public:
    virtual void micCallBack(KEMicCallBackCMD cmd)=0;
    virtual KEMicDelegateMode getMicDelegateMode()=0;
};

class KEMicPlayer :public CCObject{
public:
    static KEMicPlayer* create();
    
    //回调委托
    void setDelegate(KEMicPlayerDelegate* pDelegate);
    
    void changeMode(KEMicDelegateMode mode);
    
public:
    KEMicPlayer();
    ~KEMicPlayer();
    
private:
    bool init();
    
protected:
    KEMicPlayerImpl* m_pMicPlayerImpl;
    KEMicPlayerDelegate* m_pMicPlayerDelegate;
};




#endif
