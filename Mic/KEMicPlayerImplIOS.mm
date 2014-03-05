//
//  KEMicPlayerImplIOS.cpp
//  Blackcat
//
//  Created by haojunhua on 14-1-22.
//
//

#include "KEMicPlayerImplIOS.h"
#import <AVFoundation/AVFoundation.h>
#include "EAGLView.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
@implementation MicPlayerImplIOS

@synthesize player;
@synthesize recorder;
@synthesize micPlayerImpl = m_micPlayerImpl;

@synthesize playbackWasInterrupted;

- (id)init:(void*)micPlayerImpl
{
    self = [super init];
    self.micPlayerImpl=micPlayerImpl;
    
    //初始化定时刷新相关内容
    _refreshHz = 1. / 5.;
    _chan_lvls = (AudioQueueLevelMeterState*)malloc(sizeof(AudioQueueLevelMeterState));
    _meterTable = new MeterTable(kMinDBvalue);
    
    //初始化麦克风+播放器
    recorder = new AQRecorder();
	player = new AQPlayer();
    
    OSStatus error = AudioSessionInitialize(NULL, NULL, interruptionListener, self);
	if (error) printf("ERROR INITIALIZING AUDIO SESSION! %d\n", (int)error);
	else
	{
		UInt32 category = kAudioSessionCategory_PlayAndRecord;
		error = AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(category), &category);
		if (error) printf("couldn't set audio category!");
        
		error = AudioSessionAddPropertyListener(kAudioSessionProperty_AudioRouteChange, propListener, self);
		if (error) printf("ERROR ADDING AUDIO SESSION PROP LISTENER! %d\n", (int)error);
		UInt32 inputAvailable = 0;
		UInt32 size = sizeof(inputAvailable);
		
		// we do not want to allow recording if input is not available
		error = AudioSessionGetProperty(kAudioSessionProperty_AudioInputAvailable, &size, &inputAvailable);
		if (error) printf("ERROR GETTING INPUT AVAILABILITY! %d\n", (int)error);
        //		btn_record.enabled = (inputAvailable) ? YES : NO;
		
		// we also need to listen to see if input availability changes
		error = AudioSessionAddPropertyListener(kAudioSessionProperty_AudioInputAvailable, propListener, self);
		if (error) printf("ERROR ADDING AUDIO SESSION PROP LISTENER! %d\n", (int)error);
        
		error = AudioSessionSetActive(true);
		if (error) printf("AudioSessionSetActive (true) failed");
        
	}
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(playbackQueueStopped:) name:@"playbackQueueStopped" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(playbackQueueResumed:) name:@"playbackQueueResumed" object:nil];
    
	playbackWasInterrupted = NO;
	playbackWasPaused = NO;
    
    [self registerForBackgroundNotifications];
    
    return self;
}

- (void)changeMode:(KEMicDelegateMode)mode{
    switch (mode) {
        case kStop:
        [self setAq: nil];
        recorder->StopRecord();
        player->DisposeQueue(true);
        break;
        case kMotion:
        
        break;
        
        case kTalk:
        CCLog("startRecord");
        [self startRecord];
        break;
    }
}

#pragma mark Cleanup
- (void)dealloc
{
    if (_updateTimer) {
        [_updateTimer invalidate];
        _updateTimer=nil;
    }
	
	delete player;
	delete recorder;
    
	delete _meterTable;
    free(_chan_lvls);
    
	[super dealloc];
    
    CCLog("mic dealloc");
}



- (void)startRecord
{
    if (recorder->IsRunning())
	{
        //需要修改未从player修剪，而不是重启recorder
		[self setAq: nil];
        recorder->StopRecord();
        player->DisposeQueue(true);
	}
    recorder->StartRecord(CFSTR("recordedFile.caf"));
    [self setAq: recorder->Queue()];
}

- (void)stopRecord{
    [self setAq: nil];
    recorder->StopRecord();
	player->DisposeQueue(true);
	recordFilePath = (CFStringRef)[NSTemporaryDirectory() stringByAppendingPathComponent: @"recordedFile.caf"];
	player->CreateQueueForFile(recordFilePath);
}

- (void)play
{
    if (player->IsRunning())
	{
		if (playbackWasPaused) {
			OSStatus result = player->StartQueue(true);
			if (result == noErr)
            [[NSNotificationCenter defaultCenter] postNotificationName:@"playbackQueueResumed" object:self];
		}
		else
        [self stopPlayQueue];
	}
	else
	{
		OSStatus result = player->StartQueue(false);
		if (result == noErr)
        [[NSNotificationCenter defaultCenter] postNotificationName:@"playbackQueueResumed" object:self];
	}
}

#pragma mark AudioSession listeners
void interruptionListener(	void *	inClientData,
                          UInt32	inInterruptionState)
{
    CCLog("interruptionListener");
	MicPlayerImplIOS *THIS = (MicPlayerImplIOS*)inClientData;
	if (inInterruptionState == kAudioSessionBeginInterruption)
	{
		if (THIS->recorder->IsRunning()) {
			[THIS stopRecord];
		}
		else if (THIS->player->IsRunning()) {
			//the queue will stop itself on an interruption, we just need to update the UI
			[[NSNotificationCenter defaultCenter] postNotificationName:@"playbackQueueStopped" object:THIS];
			THIS->playbackWasInterrupted = YES;
		}
	}
	else if ((inInterruptionState == kAudioSessionEndInterruption) && THIS->playbackWasInterrupted)
	{
		// we were playing back when we were interrupted, so reset and resume now
		THIS->player->StartQueue(true);
		[[NSNotificationCenter defaultCenter] postNotificationName:@"playbackQueueResumed" object:THIS];
		THIS->playbackWasInterrupted = NO;
	}
}

void propListener(	void *                  inClientData,
                  AudioSessionPropertyID	inID,
                  UInt32                  inDataSize,
                  const void *            inData)
{
	MicPlayerImplIOS *THIS = (MicPlayerImplIOS*)inClientData;
	if (inID == kAudioSessionProperty_AudioRouteChange)
	{
		CFDictionaryRef routeDictionary = (CFDictionaryRef)inData;
		//CFShow(routeDictionary);
		CFNumberRef reason = (CFNumberRef)CFDictionaryGetValue(routeDictionary, CFSTR(kAudioSession_AudioRouteChangeKey_Reason));
		SInt32 reasonVal;
		CFNumberGetValue(reason, kCFNumberSInt32Type, &reasonVal);
		if (reasonVal != kAudioSessionRouteChangeReason_CategoryChange)
		{
			if (reasonVal == kAudioSessionRouteChangeReason_OldDeviceUnavailable)
			{
				if (THIS->player->IsRunning()) {
					[THIS pausePlayQueue];
					[[NSNotificationCenter defaultCenter] postNotificationName:@"playbackQueueStopped" object:THIS];
				}
			}
            
			// stop the queue if we had a non-policy route change
			if (THIS->recorder->IsRunning()) {
				[THIS stopRecord];
			}
		}
	}
	else if (inID == kAudioSessionProperty_AudioInputAvailable)
	{
		if (inDataSize == sizeof(UInt32)) {
            //			UInt32 isAvailable = *(UInt32*)inData;
			// disable recording if input is not available
            //			THIS->btn_record.enabled = (isAvailable > 0) ? YES : NO;
            printf("disable recording if input is not available");
		}
	}
}

- (void)playbackQueueStopped:(NSNotification *)note
{
    printf("playbackQueueStopped\n");
    
    //播放完毕自动开始录音
    [self startRecord];
}

- (void)playbackQueueResumed:(NSNotification *)note
{
    printf("playbackQueueResumed\n");
}

-(void)stopPlayQueue
{
	player->StopQueue();
    //播放完毕自动开始录音
    [self startRecord];
}

-(void)pausePlayQueue
{
	player->PauseQueue();
	playbackWasPaused = YES;
}

#pragma mark 注册前后台切换通知
- (void)registerForBackgroundNotifications
{
	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(resignActive)
												 name:UIApplicationWillResignActiveNotification
											   object:nil];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(enterForeground)
												 name:UIApplicationWillEnterForegroundNotification
											   object:nil];
}

- (void)resignActive
{
    //需要修改
    if (recorder->IsRunning()) [self stopRecord];
    if (player->IsRunning()) [self stopPlayQueue];
}

- (void)enterForeground
{
    OSStatus error = AudioSessionSetActive(true);
    if (error) printf("AudioSessionSetActive (true) failed");
}


#pragma mark 监听波形
- (AudioQueueRef)aq { return _aq; }
- (void)setAq:(AudioQueueRef)v
{
    if(v == NULL){
        if (_updateTimer) {
            [_updateTimer invalidate];
            _updateTimer=nil;
        }
        
    }
	else if ((_aq == NULL) && (v != NULL))
	{
		if (_updateTimer) [_updateTimer invalidate];
		
		_updateTimer = [NSTimer
						scheduledTimerWithTimeInterval:_refreshHz
						target:self
						selector:@selector(_refresh)
						userInfo:nil
						repeats:YES
						];
	}
	
	_aq = v;
	
	if (_aq)
	{
		try {
			UInt32 val = 1;
			XThrowIfError(AudioQueueSetProperty(_aq, kAudioQueueProperty_EnableLevelMetering, &val, sizeof(UInt32)), "couldn't enable metering");
			
			// now check the number of channels in the new queue, we will need to reallocate if this has changed
			CAStreamBasicDescription queueFormat;
			UInt32 data_sz = sizeof(queueFormat);
			XThrowIfError(AudioQueueGetProperty(_aq, kAudioQueueProperty_StreamDescription, &queueFormat, &data_sz), "couldn't get stream description");
            
			_chan_lvls = (AudioQueueLevelMeterState*)realloc(_chan_lvls, queueFormat.NumberChannels() * sizeof(AudioQueueLevelMeterState));
		}
		catch (CAXException e) {
			char buf[256];
			fprintf(stderr, "Error: %s (%s)\n", e.mOperation, e.FormatError(buf));
		}
	} else {
        //		for (LevelMeter *thisMeter in _subLevelMeters) {
        //			[thisMeter setNeedsDisplay];
        //		}
	}
}

- (void)_refresh
{
	BOOL success = NO;
    
	// if we have no queue, but still have levels, gradually bring them down
	if (_aq == NULL)
	{
		_level=0;
        _peakLevel=0;
        _count=0;
	} else {
		
		UInt32 data_sz = sizeof(AudioQueueLevelMeterState);
		OSErr status = AudioQueueGetProperty(_aq, kAudioQueueProperty_CurrentLevelMeterDB, _chan_lvls, &data_sz);
		if (status != noErr) goto bail;
        
        if (_chan_lvls)
        {
            //            float last_level=_level;
            float last_peakLevel=_peakLevel;
            _level = _meterTable->ValueAt((float)(_chan_lvls[0].mAveragePower));
            _peakLevel = _meterTable->ValueAt((float)(_chan_lvls[0].mPeakPower));
            
            //            printf("leve=%f,peaklevel=%f,%d\n",_level,_peakLevel,recorder->IsRunning());
            success = YES;
            _count++;
            
            if (_count>1) {
                if (last_peakLevel>0.2&&_peakLevel<0.2) {
                    _count=0;
                    [self stopRecord];
                    [self play];
                }else if(last_peakLevel<0.2&&_peakLevel>0.2){
                    _count=0;
                    [self startRecord];
                }
            }
            
        }
	}
	
bail:
	if (!success)
	{
		printf("ERROR: metering failed\n");
	}
}

@end



KEMicPlayerImpl* __createSystemMicPlayer(KEMicPlayer* pMicPlayer)
{
    return new KEMicPlayerImplIOS(pMicPlayer);
}


KEMicPlayerImplIOS::KEMicPlayerImplIOS(KEMicPlayer* pMicPlayer):KEMicPlayerImpl(pMicPlayer){
    
}

KEMicPlayerImplIOS::~KEMicPlayerImplIOS(){
    [m_systemControl release];
}

bool KEMicPlayerImplIOS::init(){
    do
    {
        m_systemControl = [[MicPlayerImplIOS alloc] init:this];
        if (!m_systemControl) break;
        return true;
    }while (0);
    
    return false;
}

void KEMicPlayerImplIOS::changeMode(KEMicDelegateMode mode){
    [m_systemControl changeMode:mode];
}

#endif