//
//  GushiScene.h
//  Blackcat
//
//  Created by haojunhua on 14-7-23.
//
//

#ifndef __Blackcat__GushiScene__
#define __Blackcat__GushiScene__

#include "cocos2d.h"
#include "CoverLayer.h"
using namespace cocos2d;

class GushiScene : public CCLayer,public CoverLayerDelegate
{
public:
    static CCScene* scene();
    
    virtual bool init();
    CREATE_FUNC(GushiScene);
    
    virtual int topHandlerPriority(){return kCCMenuHandlerPriority-1;};
    void menuCallback(CCObject* sender);
};


#endif /* defined(__Blackcat__TestLayer__) */
