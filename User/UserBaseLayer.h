//
//  UserBaseLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-4-3.
//
//

#ifndef __Blackcat__UserBaseLayer__
#define __Blackcat__UserBaseLayer__

#include "DialogLayer.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "CocosJsonParser.h"
#include "httpdata.h"
#include "resource.h"
#include "Button9.h"
USING_NS_CC;
USING_NS_CC_EXT;

class UserBaseLayer : public DialogLayer, public CCEditBoxDelegate
{
public:
    virtual bool init();
    
    virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox);
    virtual void editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox);
    
};

#endif /* defined(__Blackcat__UserBaseLayer__) */
