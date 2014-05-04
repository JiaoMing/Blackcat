//
//  Button9.cpp
//  Blackcat
//
//  Created by haojunhua on 14-4-3.
//
//

#include "Button9.h"

#define LABEL INT_MAX

Button9* Button9::create(string title,CCSize size,int fontSize){
    Button9* pReturn = new Button9();
    if ( pReturn && pReturn->initWithSpriteFrameName("dialog_button.png",CCRectMake(25, 15, 30, 13)) )
    {
        pReturn->autorelease();
        pReturn->setContentSize(size);
        CCLabelTTF* label=CCLabelTTF::create(title.c_str(), "", fontSize);
        label->setTag(LABEL);
        label->setColor(ccc3(0, 0, 0));
        label->setPosition(ccp(size.width/2,size.height/2));
        pReturn->addChild(label);
        return pReturn;
    }
    CC_SAFE_DELETE(pReturn);
    return NULL;
}

void Button9::setString(const char *str,CCSize size){
    CCLabelTTF* label=(CCLabelTTF*)this->getChildByTag(LABEL);
    label->setString(str);
    this->setContentSize(size);
    label->setPosition(ccp(size.width/2,size.height/2));
}