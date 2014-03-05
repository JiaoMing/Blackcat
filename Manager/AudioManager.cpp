//
//  AudioManager.cpp
//  Blackcat
//
//  Created by haojunhua on 14-2-24.
//
//

#include "AudioManager.h"
#include "resource.h"

static AudioManager *g_instance = NULL;

AudioManager::AudioManager()
{
   m_audioDict=S_PTM->getPlistDictionary("audio.plist");
}

AudioManager::~AudioManager()
{
	CC_SAFE_DELETE(g_instance);
}

AudioManager* AudioManager::sharedAudioManager()
{
	if (g_instance == NULL)
	{
		g_instance = new AudioManager();
	}
	return g_instance;
}

Audio* AudioManager::getAudioWithKey(const char *key){
    //不在plist列表中的话，直接duration为0
    float duration=0.0f;
    CCDictionary* animateDict=(CCDictionary*)m_audioDict->objectForKey(key);
    if(animateDict!=NULL){
        const CCString* _duration=animateDict->valueForKey("duration");
        duration=_duration->floatValue();
    }
    Audio* audio=new Audio();
    audio->autorelease();
    audio->setAudioName(key);
    audio->setDuration(duration);
    return audio;
}
