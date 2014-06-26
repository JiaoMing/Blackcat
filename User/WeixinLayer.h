//
//  WeixinLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-6-22.
//
//

#ifndef __Blackcat__WeixinLayer__
#define __Blackcat__WeixinLayer__

#include "DialogLayer.h"
#include "resource.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

class WeixinLayer :public DialogLayer{
public:
    virtual bool init();
    CREATE_FUNC(WeixinLayer);
    
    void menuCallback(cocos2d::CCObject *obj);
};

#endif /* defined(__Blackcat__WeixinLayer__) */
