//
//  GuideManager.h
//  Blackcat
//
//  Created by haojunhua on 13-12-26.
//
//

#ifndef Blackcat_GuideManager_h
#define Blackcat_GuideManager_h


#include "GuideModel.h"
#define S_GM GuideManager::sharedGuideManager()

class GuideManager
{
public:
    static GuideManager* sharedGuideManager();
    GuideManager();
    ~GuideManager();
public:
    Guide* getGuideDialogWithQueueKeyAndStepKey(const char* queueKey,const char* stepKey);
private:
    CCDictionary* m_dictRoot;
};



#endif
