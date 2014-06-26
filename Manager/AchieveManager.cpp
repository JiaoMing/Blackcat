//
//  AchieveManager.cpp
//  Blackcat
//
//  Created by haojunhua on 14-6-16.
//
//

#include "AchieveManager.h"
#include "AchieveRewardLayer.h"
#include "resource.h"

static AchieveManager *g_instance = NULL;

AchieveManager* AchieveManager::sharedAchieveManager()
{
	if (g_instance == NULL)
	{
		g_instance = new AchieveManager();
	}
	return g_instance;
}

AchieveManager::AchieveManager()
{
    m_medalDict=S_PTM->getPlistDictionary("medal.plist");
}

AchieveManager::~AchieveManager()
{
	CC_SAFE_DELETE(g_instance);
}

int AchieveManager::achieveLevel(AchieveKey key){
    int value=S_UD->getIntegerForKey(achieveStruts[key].UD_key.c_str(), 0);
    int level=0;
    CCArray* array=(CCArray*)m_medalDict->objectForKey(achieveStruts[key].key.c_str());
    CCObject *obj = NULL;
    CCARRAY_FOREACH(array, obj){
        CCString* item=(CCString*)obj;
        int number=item->intValue();
        if (value>number) {
            level++;
        }else{
            break;
        }
    }
    return level;
}

void AchieveManager::achieveUp(AchieveKey key, int upValue,CCObject* target,SEL_CallFunc selector){
    const char* udkey=achieveStruts[key].UD_key.c_str();
    int oldValue=S_UD->getIntegerForKey(achieveStruts[key].UD_key.c_str(), 0);
    int newValue=oldValue+upValue;
    S_UD->setIntegerForKey(udkey, newValue);
    S_UD->flush();
    
    int oldLevel=0,newLevel=0;
    CCArray* array=(CCArray*)m_medalDict->objectForKey(achieveStruts[key].key);
    CCObject *obj = NULL;
    CCARRAY_FOREACH(array, obj){
        CCString* item=(CCString*)obj;
        int number=item->intValue();
        if (newValue>number) {
            newLevel++;
            if (oldValue>number) {
                oldLevel++;
            }
        }else{
            break;
        }
    }
    if (newLevel>oldLevel) {
        CCScene* scene=S_DR->getRunningScene();
        AchieveRewardLayer* layer= AchieveRewardLayer::create(key, newLevel);
        layer->setTarget(target, selector);
        scene->addChild(layer,INT_MAX);
    }
}