//
//  TestMic.h
//  Blackcat
//
//  Created by haojunhua on 13-12-16.
//
//

#ifndef Blackcat_TestMic_h
#define Blackcat_TestMic_h

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


#import "AQPlayer.h"
#import "AQRecorder.h"
#import "MeterTable.h"

#include "MicRecordPlayer.h"
#include "MicConfig.h"



@interface MicRecordController : NSObject {
    MicRecordPlayerDelegate *micRecordPlayerDelegate;
    
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
    
}

@property (readonly)			AQPlayer			*player;
@property (readonly)			AQRecorder			*recorder;
@property						BOOL				playbackWasInterrupted;

@property				AudioQueueRef aq; // The AudioQueue object
@property				CGFloat refreshHz; // How many times per second to redraw

    
- (id)initWithScene:(MicRecordPlayerDelegate*)scene;

- (void)changeMode:(MicDelegateMode)mode;

- (void)dealloc;

@end

#endif
