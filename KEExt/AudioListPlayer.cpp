//
//  AudioListPlayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-1-12.
//
//

#include "AudioListPlayer.h"
#include "resource.h"
#include "Utils.h"

static AudioListPlayer *g_instance = NULL;

AudioListPlayer::AudioListPlayer()
{
}

AudioListPlayer::~AudioListPlayer()
{
	CC_SAFE_DELETE(g_instance);
}

AudioListPlayer *AudioListPlayer::sharedAudioListPlayer()
{
	if (g_instance == NULL)
	{
		g_instance = new AudioListPlayer();
	}
	return g_instance;
}

void AudioListPlayer::play(CCArray* array){
    this->stop();
    
    CCObject *obj = NULL;
    CCARRAY_FOREACH(array, obj){
        Audio* audio=(Audio*)obj;
        AudioAndDuration aad;
        aad.audioName=audio->getAudioName();
        aad.duration=audio->getDuration();
        m_audioVector.push_back(aad);
    }
    
    m_index=0;
    if(m_audioVector.size()>0)this->next();
}

void AudioListPlayer::play(const char * audio1, ...){
    this->stop();
    va_list params;
	va_start(params, audio1);
    
    const char* audio = audio1;
    vector<string> audios;
    while (audio) {
        audios.clear();
        StringUtils::split(audio,":",audios);
        if (audios.size()==2||audios.size()==1) {
            AudioAndDuration aad;
            aad.audioName=audios[0];
            if(audios.size()==2)aad.duration=atof(audios[1].c_str());
            else aad.duration=0.0f;
            m_audioVector.push_back(aad);
        }
        audio = va_arg(params, const char *);
    }
    va_end(params);
    
    m_index=0;
    if(m_audioVector.size()>0)this->next();
}

void AudioListPlayer::stop(){
    m_audioVector.clear();
    S_AE->stopEffect(m_soundId);
    S_DR->getScheduler()->unscheduleSelector(schedule_selector(AudioListPlayer::next), this);
}

void AudioListPlayer::next(){
    S_DR->getScheduler()->unscheduleSelector(schedule_selector(AudioListPlayer::next), this);
    int count=(int)m_audioVector.size();
    if(count>0&&m_index<count){
        m_soundId=S_AE->playEffect(m_audioVector[m_index].audioName.c_str());
        if(++m_index<count){
            S_DR->getScheduler()->scheduleSelector(schedule_selector(AudioListPlayer::huanchong),this, 0.0f,0,0.1,false);
        }
    }
}

void AudioListPlayer::huanchong(){
    S_DR->getScheduler()->scheduleSelector(schedule_selector(AudioListPlayer::next),this, 0.0f,0, m_audioVector[m_index-1].duration,false);
}