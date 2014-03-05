//
//  Heimao.h
//  Blackcat
//
//  Created by haojunhua on 14-1-12.
//
//

#ifndef Blackcat_Heimao_h
#define Blackcat_Heimao_h
#include "Cartoon.h"

typedef enum {
    kHeimaoActionStand = 0,
    kHeimaoActionWrite,
    kHeimaoActionRight,
    kHeimaoActionWrong,
    kHeimaoActionYaobai,
    kHeimaoActionXiangshang,
    kHeimaoActionShuohua1,
    kHeimaoActionShuohua2,
    kHeimaoActionShuohua3,
    kHeimaoActionShuohua4,
    //eWuXiongStatue
}HeimaoAction;

typedef enum {
    kHeimaoAudioNone=-1,
    kHeimaoAudioRenwujieshao,
    kHeimaoAudioRenwukaishi,
    kHeimaoAudioRenwutishi,
    kHeimaoAudioRenwuRight,
    kHeimaoAudioRenwuWrong,
    kHeimaoAudioXieziTixing,
    kHeimaoAudioXiezikaishi,
    kHeimaoAudioXieziRight,
    kHeimaoAudioXieziWrong,
    kHeimaoAudioXieziWrite,
    kHeimaoAudioXieziOK1,
    kHeimaoAudioXieziOK2,
    kHeimaoAudioXieziOK3
}HeimaoAudio;

class Heimao : public Cartoon
{
public:
    static Heimao* create();
    
    
    float action(const char* key,bool random=true,int actionIndex=0,int audioIndex=0);
private:
    
    bool doJieshao2;
    bool doKaishi2;
    bool doXiezi2;
};

#endif
