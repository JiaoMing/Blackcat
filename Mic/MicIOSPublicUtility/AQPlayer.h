/*
 
    File: AQPlayer.h
Abstract: Helper class for playing audio files via the AudioQueue
 Version: 2.5


 
*/

#include <AudioToolbox/AudioToolbox.h>

#include "CAStreamBasicDescription.h"
#include "CAXException.h"

#define kNumberBuffers 3


#include "MicConfig.h"

class AQPlayer
	{
	public:
		AQPlayer();
		~AQPlayer();

		OSStatus						StartQueue(BOOL inResume);
		OSStatus						StopQueue();
		OSStatus						PauseQueue();
		
		AudioQueueRef					Queue()					{ return mQueue; }
		CAStreamBasicDescription		DataFormat() const		{ return mDataFormat; }		
		Boolean							IsRunning()	const		{ return (mIsRunning) ? true : false; }
		Boolean							IsInitialized()	const	{ return mIsInitialized; }		
		CFStringRef						GetFilePath() const		{ return (mFilePath) ? mFilePath : CFSTR(""); }
		Boolean							IsLooping() const		{ return mIsLooping; }
		
		void SetLooping(Boolean inIsLooping)	{ mIsLooping = inIsLooping; }
		void CreateQueueForFile(CFStringRef inFilePath);
		void DisposeQueue(Boolean inDisposeFile);	
										
	private:
		UInt32							GetNumPacketsToRead()				{ return mNumPacketsToRead; }
		SInt64							GetCurrentPacket()					{ return mCurrentPacket; }
		AudioFileID						GetAudioFileID()					{ return mAudioFile; }
		void							SetCurrentPacket(SInt64 inPacket)	{ mCurrentPacket = inPacket; }
		
		void							SetupNewQueue();
		
		AudioQueueRef					mQueue;
		AudioQueueBufferRef				mBuffers[kNumberBuffers];
		AudioFileID						mAudioFile;
		CFStringRef						mFilePath;
		CAStreamBasicDescription		mDataFormat;
		Boolean							mIsInitialized;
		UInt32							mNumPacketsToRead;
		SInt64							mCurrentPacket;
		UInt32							mIsRunning;
		Boolean							mIsDone;
		Boolean							mIsLooping;
		
		static void isRunningProc(		void *              inUserData,
										AudioQueueRef           inAQ,
										AudioQueuePropertyID    inID);

		static void AQBufferCallback(	void *					inUserData,
										AudioQueueRef			inAQ,
										AudioQueueBufferRef		inCompleteAQBuffer); 

		void CalculateBytesForTime(		CAStreamBasicDescription & inDesc, 
										UInt32 inMaxPacketSize, 
										Float64 inSeconds, 
										UInt32 *outBufferSize, 
										UInt32 *outNumPackets);							  								 			
	};