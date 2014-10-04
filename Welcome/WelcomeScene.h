//
//  WelcomeScene.h
//  Blackcat
//
//  Created by haojunhua on 14-8-1.
//
//

#ifndef __Blackcat__WelcomeScene__
#define __Blackcat__WelcomeScene__

#include "cocos2d.h"
USING_NS_CC;
#include "CoverLayer.h"

class WelcomeScene : public CCLayer,public CoverLayerDelegate
{
public:
    static CCScene* scene();
public:
    WelcomeScene();
    ~WelcomeScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(WelcomeScene);
    
    virtual void keyBackClicked();
    virtual void keyMenuClicked();
    
    virtual int topHandlerPriority(){return kCCMenuHandlerPriority-1;};
    
    // touch event
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    void playBgAduio(float t);
    
private:
    void menuCallback(CCObject* object);
};


#endif /* defined(__Blackcat__WelcomeScene__) */
