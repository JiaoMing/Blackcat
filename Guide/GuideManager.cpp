//
//  GuideManager.cpp
//  Blackcat
//
//  Created by haojunhua on 13-12-27.
//
//

#include "GuideManager.h"
#include "resource.h"

static GuideManager* s_pGuideManager = NULL;

GuideManager* GuideManager::sharedGuideManager()
{
    if (s_pGuideManager == NULL) {
        s_pGuideManager = new GuideManager();
    }
    return s_pGuideManager;
}

GuideManager::GuideManager(){
    m_dictRoot = S_PTM->getPlistDictionary("guide.plist");
}

GuideManager::~GuideManager(){
}

Guide* GuideManager::getGuideDialogWithQueueKeyAndStepKey(const char* queueKey,const char* stepKey){
    CCDictionary* allSceneGuideDict=(CCDictionary*)m_dictRoot->objectForKey(queueKey);
    CCDictionary* guideDict=(CCDictionary*)allSceneGuideDict->objectForKey(stepKey);
    if (guideDict==NULL) {
        return NULL;
    }
    const CCString* type=guideDict->valueForKey("type");
    const CCString* to=guideDict->valueForKey("to");
    
    Guide* guide;
    switch (type->intValue()) {
        case kGuideDialog:{
            CCDictionary* dialogDict=(CCDictionary*)guideDict->objectForKey("dialog");
            const CCString* tag=dialogDict->valueForKey("tag");
            const CCString* duration=dialogDict->valueForKey("duration");
            const CCString* audio=dialogDict->valueForKey("audio");
            const CCString* text=dialogDict->valueForKey("text");
            const CCString* autoStep=dialogDict->valueForKey("autoStep");
            const CCString* mode=dialogDict->valueForKey("mode");
            const CCString* type=dialogDict->valueForKey("type");
            const CCString* action=dialogDict->valueForKey("action");
            const CCString* cartoonPoint=dialogDict->valueForKey("cartoonPoint");
            
            GuideDialog* guideDialog=new GuideDialog();
            guideDialog->setTag(tag->intValue());
            guideDialog->setDuration(duration->floatValue());
            guideDialog->setText(text->getCString());
            guideDialog->setCartoonPoint(cartoonPoint->getCString());
            if (autoStep->length()>0) {
                guideDialog->setIsAutoStep(autoStep->boolValue());
            }
            guideDialog->setMode((GuideDialogMode)mode->intValue());
            if (type->length()>0) {
                guideDialog->setDialogType(kDialogWithText);
            }
            
            vector<string> rs;
            StringUtils::split(action->getCString(), ",", rs);
            for (int i=0; i<rs.size(); i++) {
                CCString* action=CCString::createWithFormat("%s",rs[i].c_str());
                guideDialog->getActionArray()->addObject(action);
            }
            rs.clear();
            
            StringUtils::split(audio->getCString(), ",", rs);
            for (int i=0; i<rs.size(); i++) {
                guideDialog->getAudioArray()->addObject(S_AOM->getAudioWithKey(rs[i].c_str()));
            }
            
            guide=guideDialog;
            
        }
            break;
        case kGuideEvent:{
            CCDictionary* eventDict=(CCDictionary*)guideDict->objectForKey("event");
            const CCString* key=eventDict->valueForKey("key");
            const CCString* tag=eventDict->valueForKey("tag");
            GuideEvent* guideEvent=new GuideEvent();
            guideEvent->setKey(key->getCString());
            guideEvent->setTag(tag->intValue());
            guide=guideEvent;
        }
            break;
    }
    guide->setToGuideKey(to->getCString());
    guide->autorelease();
    return guide;
}