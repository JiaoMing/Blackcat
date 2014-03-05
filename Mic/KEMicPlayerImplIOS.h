//
//  KEMicPlayerImplIOS.h
//  Blackcat
//
//  Created by haojunhua on 14-1-22.
//
//

#ifndef Blackcat_KEMicPlayerImplIOS_h
#define Blackcat_KEMicPlayerImplIOS_h

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)


#include "KEMicPlayer.h"
#include "KEMicPlayerImpl.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "AQPlayer.h"
#import "AQRecorder.h"
#import "MeterTable.h"
#import "MicConfig.h"

@interface MicPlayerImplIOS : NSObject {
	AQPlayer*					player;
	AQRecorder*					recorder;
	BOOL						playbackWasInterrupted;
	BOOL						playbackWasPaused;
	
	CFStringRef					recordFilePath;
    
	AudioQueueRef				_aq;
	AudioQueueLevelMeterState	*_chan_lvls;
	MeterTable					*_meterTable;
	NSTimer						*_updateTimer;
	CGFloat						_refreshHz;
    float _level;
    float _peakLevel;
    float _count;
    
    void* m_micPlayerImpl;
}

@property(nonatomic, assign) void*  micPlayerImpl;
    
@property (readonly)			AQPlayer			*player;
@property (readonly)			AQRecorder			*recorder;
@property						BOOL				playbackWasInterrupted;

@property				AudioQueueRef aq; // The AudioQueue object
@property				CGFloat refreshHz; // How many times per second to redraw

-(id) init:(void*)micPlayerImpl;
-(void)changeMode:(KEMicDelegateMode)mode;
@end


class KEMicPlayerImplIOS : public KEMicPlayerImpl{
public:
    KEMicPlayerImplIOS(KEMicPlayer* pMicPlayer);
    virtual ~KEMicPlayerImplIOS();
public:
    virtual bool init();
    virtual void changeMode(KEMicDelegateMode mode);
protected:
    MicPlayerImplIOS* m_systemControl;
};

#endif

#endif
