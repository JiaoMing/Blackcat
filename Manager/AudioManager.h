//
//  AudioManager.h
//  Blackcat
//
//  Created by haojunhua on 14-2-24.
//
//

#ifndef Blackcat_AudioManager_h
#define Blackcat_AudioManager_h

#include "cocos2d.h"
USING_NS_CC;
#include <string>
using namespace std;

class Audio:public CCObject{
public:
    Audio(){};
    ~Audio(){};
    
    CC_SYNTHESIZE(string, m_audioName, AudioName);
    CC_SYNTHESIZE(float, m_duration, Duration);
};

class AudioManager:public CCObject{
public:
    static AudioManager* sharedAudioManager();
    Audio* getAudioWithKey(const char* key);
public:
    ~AudioManager();
private:
    AudioManager();
private:
    CCDictionary* m_audioDict;
};


#endif
