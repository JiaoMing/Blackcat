//
//  Heimao.cpp
//  Blackcat
//
//  Created by haojunhua on 14-1-12.
//
//

#include "Heimao.h"
#include "resource.h"


Heimao* Heimao::create(){
    Heimao* heimao = new Heimao();
    if (heimao && heimao->initWithName("heimao")) {
        heimao->autorelease();
        return heimao;
    }
    CC_SAFE_DELETE(heimao);
    return NULL;
}


float Heimao::action(const char* key,bool random,int actionIndex,int audioIndex){
    float time=0;
    ActionFlow* actionFlow=S_AFM->getActionFlowWithKey(key);
    if (actionFlow!=NULL) {
        CCArray* actionArray=actionFlow->getActionArray();
        CCArray* audioArray=actionFlow->getAudioArray();
        int _actionIndex=actionIndex,_audioIndex=audioIndex;
        if (random) {
            _actionIndex=(int)(CCRANDOM_0_1()*actionArray->count());
            _audioIndex=(int)(CCRANDOM_0_1()*audioArray->count());
        }
        this->doAction((CCArray*)actionArray->objectAtIndex(_actionIndex));
        
        //拼音数组允许为空
        if (audioArray->count()>0) {
            CCArray* audioList=(CCArray*)audioArray->objectAtIndex(_audioIndex);
            if (audioList->count()>0) {
                S_ALP->play(audioList);
                CCObject* audioObj = NULL;
                CCARRAY_FOREACH(audioList, audioObj){
                    Audio* audioItem=(Audio*)audioObj;
                    if(audioItem!=NULL){
                        time+=audioItem->getDuration();
                    }
                }
            }
        }
    }
    return time;
}