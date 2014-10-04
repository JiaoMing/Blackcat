//
//  LoadscapeScene.h
//  Blackcat
//
//  Created by haojunhua on 14-8-18.
//
//

#ifndef __Blackcat__LoadscapeScene__
#define __Blackcat__LoadscapeScene__

#include "cocos2d.h"
USING_NS_CC;

class LoadscapeScene : public CCLayer
{
public:
    ~LoadscapeScene();
    static CCScene* scene();
public:
    virtual bool init();
    CREATE_FUNC(LoadscapeScene);
    
    void gotoWelcomeScene(float t);
};


#endif /* defined(__Blackcat__LoadscapeScene__) */
