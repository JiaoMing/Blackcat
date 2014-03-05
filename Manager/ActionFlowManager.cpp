//
//  ActionFlowManager.cpp
//  Blackcat
//
//  Created by haojunhua on 14-1-13.
//
//

#include "ActionFlowManager.h"
#include "resource.h"

static ActionFlowManager* s_pActionFlowManager = NULL;

ActionFlowManager* ActionFlowManager::sharedActionFlowManager()
{
    if (s_pActionFlowManager == NULL) {
        s_pActionFlowManager = new ActionFlowManager();
    }
    return s_pActionFlowManager;
}

ActionFlowManager::ActionFlowManager(){
    m_audioFlowDict=S_PTM->getPlistDictionary("action_flow.plist");
}

ActionFlowManager::~ActionFlowManager(){
}

ActionFlow* ActionFlowManager::getActionFlowWithKey(const char *key){
    ActionFlow* actionFlow=new ActionFlow();
    actionFlow->autorelease();
    CCDictionary* actionDict=(CCDictionary*)m_audioFlowDict->objectForKey(key);
    CCArray* actionList=(CCArray*)actionDict->objectForKey("actionList");
    CCObject *actionObj = NULL;
    CCARRAY_FOREACH(actionList, actionObj){
        CCString* actionItem=(CCString*)actionObj;
        vector<string> rs;
        StringUtils::split(actionItem->getCString(), ",", rs);
        CCArray* actions=CCArray::create();
        for (int i=0; i<rs.size(); i++) {
            CCString* action=CCString::createWithFormat("%s",rs[i].c_str());
            actions->addObject(action);
        }
        actionFlow->getActionArray()->addObject(actions);
    }
    
    CCArray* audioList=(CCArray*)actionDict->objectForKey("audioList");
    CCObject *audioObj = NULL;
    CCARRAY_FOREACH(audioList, audioObj){
        CCString* audioItem=(CCString*)audioObj;
        vector<string> rs;
        StringUtils::split(audioItem->getCString(), ",", rs);
        CCArray* audios=CCArray::create();
        for (int i=0; i<rs.size(); i++) {
            audios->addObject(S_AOM->getAudioWithKey(rs[i].c_str()));
        }
        actionFlow->getAudioArray()->addObject(audios);
    }
    
    return actionFlow;
}

