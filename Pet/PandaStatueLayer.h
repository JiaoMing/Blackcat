//
//  PandaStatueLayer.h
//  Blackcat
//
//  Created by wanghongjun on 13-10-30.
// Modified by haojunhua on 13-11-20
//
//

#ifndef __Blackcat__PandaStatueLayer__
#define __Blackcat__PandaStatueLayer__

#include "cocos2d.h"
USING_NS_CC;

typedef enum{
    kPetStatusPrgressInit=0,
    kPetStatusPrgressSchedule,
    kPetStatusPrgressManual
}PetStatusPrgress;

#define kMenu_Menu 10

class PandaStatueLayer : public CCLayer
{
public:
    
    CREATE_FUNC(PandaStatueLayer);
    
    virtual bool init();
    
public:
    void updateProgress(PetStatusPrgress origin=kPetStatusPrgressSchedule,float progressXinQing=0.0f,float progressJiE=0.0f);
    
private:
    CCSprite *m_Head;
    CCProgressTimer* m_ProgressXinQing;
    CCProgressTimer* m_ProgressJiE;
};

#endif /* defined(__Blackcat__PandaStatueLayer__) */
