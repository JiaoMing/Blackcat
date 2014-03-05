//
//  ActionFlowManager.h
//  Blackcat
//
//  Created by haojunhua on 14-1-13.
//
//

#ifndef Blackcat_ActionFlowManager_h
#define Blackcat_ActionFlowManager_h

#include "cocos2d.h"
using namespace cocos2d;
#include "AudioManager.h"

class ActionFlow:public CCObject{
public:
    ActionFlow(){
        m_actionArray=CCArray::create();
        m_actionArray->retain();
        m_audioArray=CCArray::create();
        m_audioArray->retain();
    }
    ~ActionFlow(){CC_SAFE_RELEASE(m_actionArray);CC_SAFE_RELEASE(m_audioArray);}
    
    CC_SYNTHESIZE(CCArray*, m_actionArray, ActionArray);
    CC_SYNTHESIZE(CCArray*, m_audioArray, AudioArray);
};

class ActionFlowManager
{
public:
    static ActionFlowManager* sharedActionFlowManager();
    ActionFlowManager();
    ~ActionFlowManager();
public:
    ActionFlow* getActionFlowWithKey(const char* key);
private:
    CCDictionary* m_audioFlowDict;
};


#endif
