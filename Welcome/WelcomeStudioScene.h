//
//  WelcomeStudioScene.h
//  Blackcat
//
//  Created by haojunhua on 14-8-21.
//
//

#ifndef __Blackcat__WelcomeStudioScene__
#define __Blackcat__WelcomeStudioScene__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class WelcomeStudioScene : public CCLayer
{
public:
    static CCScene* scene();
public:
    WelcomeStudioScene();
    ~WelcomeStudioScene();
    
    bool init();
    CREATE_FUNC(WelcomeStudioScene);
    
    
    void touchEvent(CCObject* sender,TouchEventType type);
private:
    CCNode* m_rootNode;
};


#endif /* defined(__Blackcat__WelcomeStudioScene__) */
