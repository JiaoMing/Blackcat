//
//  SettingsLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-4-3.
//
//

#ifndef __Blackcat__SettingsLayer__
#define __Blackcat__SettingsLayer__

#include "DialogLayer.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

class SettingsLayer : public DialogLayer
{
public:
    virtual bool init();
    CREATE_FUNC(SettingsLayer);
    
    virtual void enableTouch();
    
    virtual void menuCallback(CCObject* obj);
    void volumeValueChanged(CCObject *sender, CCControlEvent controlEvent);
};

#endif /* defined(__Blackcat__SettingsLayer__) */
