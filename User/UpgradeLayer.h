//
//  UpgradeLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-6-27.
//
//

#ifndef __Blackcat__UpgradeLayer__
#define __Blackcat__UpgradeLayer__

#include "resource.h"
#include "CoverLayer.h"

class UpgradeLayer :public CoverLayer{
public:
    UpgradeLayer();
    ~UpgradeLayer();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(UpgradeLayer);
    
    void scheduleToRemove();
    void xingxingAnimate();
private:
    CCSprite* m_shengji;
    bool m_webVisible;
};

#endif /* defined(__Blackcat__UpgradeLayer__) */
