//
//  UserBaseLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-4-3.
//
//

#include "UserBaseLayer.h"

bool UserBaseLayer::init(){
    if (!DialogLayer::init()) {
        return false;
    }
    
    return true;
}

void UserBaseLayer::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
}

void UserBaseLayer::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
}

void UserBaseLayer::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
    
}

void UserBaseLayer::editBoxReturn(cocos2d::extension::CCEditBox* editBox)
{
    
}