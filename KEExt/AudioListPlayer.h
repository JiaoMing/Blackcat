//
//  AudioListPlayer.h
//  Blackcat
//
//  Created by haojunhua on 14-1-12.
//
//

#ifndef Blackcat_AudioListPlayer_h
#define Blackcat_AudioListPlayer_h
#include <string>
using namespace std;

#include "cocos2d.h"
USING_NS_CC;

typedef struct
{
    string audioName;
    float duration;
}AudioAndDuration;

class AudioListPlayer:public CCObject{
public:
    static AudioListPlayer* sharedAudioListPlayer();
    void play(CCArray* array);
    void play(const char * audio1, ...);//注意传入参数
    void stop();
    void next();
    //用于next中间缓冲
    void huanchong();
public:
    ~AudioListPlayer();
private:
    AudioListPlayer();
private:
    vector<AudioAndDuration> m_audioVector;
    int m_index;
    int m_soundId;
};



#endif
