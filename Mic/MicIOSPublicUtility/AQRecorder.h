/*
 
    File: AQRecorder.h
Abstract: Helper class for recording audio files via the AudioQueue
 Version: 2.5



 
*/

#include <AudioToolbox/AudioToolbox.h>
#include <Foundation/Foundation.h>
#include <libkern/OSAtomic.h>

#include "CAStreamBasicDescription.h"
#include "CAXException.h"

#define kNumberRecordBuffers	3


#include "MicConfig.h"

#include "SoundTouch.h"
#include "BPMDetect.h"
using namespace soundtouch;

class AQRecorder 
	{
	public:
		AQRecorder();
		~AQRecorder();
		
		UInt32						GetNumberChannels() const	{ return mRecordFormat.NumberChannels(); }
		CFStringRef					GetFileName() const			{ return mFileName; }
		AudioQueueRef				Queue() const				{ return mQueue; }
		CAStreamBasicDescription	DataFormat() const			{ return mRecordFormat; }
		
		void			StartRecord(CFStringRef inRecordFile);
		void			StopRecord();		
		Boolean			IsRunning() const			{ return mIsRunning; }
		
		UInt64			startTime;
        
		SoundTouch* GetSoundTouch() { return &mSoundTouch; }
		
		SoundTouch mSoundTouch;
				
	private:
		CFStringRef					mFileName;
		AudioQueueRef				mQueue;
		AudioQueueBufferRef			mBuffers[kNumberRecordBuffers];
		AudioFileID					mRecordFile;
		SInt64						mRecordPacket; // current packet number in record file
		CAStreamBasicDescription	mRecordFormat;
		Boolean						mIsRunning;

		void			CopyEncoderCookieToFile();
		void			SetupAudioFormat(UInt32 inFormatID);
		int				ComputeRecordBufferSize(const AudioStreamBasicDescription *format, float seconds);

		static void MyInputBufferHandler(	void *								inUserData,
											AudioQueueRef						inAQ,
											AudioQueueBufferRef					inBuffer,
											const AudioTimeStamp *				inStartTime,
											UInt32								inNumPackets,
											const AudioStreamPacketDescription*	inPacketDesc);
	};