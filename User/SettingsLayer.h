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
#include "GuideDialogLayer.h"
USING_NS_CC_EXT;

class SettingsLayer : public DialogLayer,public GuideDialogDelegate
{
public:
    virtual bool init();
    CREATE_FUNC(SettingsLayer);
    
    virtual void enableTouch();
    
    virtual int topHandlerPriority(){return kCCMenuHandlerPriority-1;};
    virtual void dialogCallBack(GuideDialogCMD cmd);
    
    virtual void menuCallback(CCObject* obj);
    void volumeValueChanged(CCObject *sender, CCControlEvent controlEvent);
private:
    int m_tag;//用于dialog返回判断
};

#endif /* defined(__Blackcat__SettingsLayer__) */
