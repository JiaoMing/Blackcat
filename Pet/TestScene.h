//
//  TestScene.h
//  Blackcat
//
//  Created by haojunhua on 13-12-2.
//
//

#ifndef Blackcat_TestScene_h
#define Blackcat_TestScene_h

#include "cocos2d.h"
using namespace cocos2d;

class TestScene:public CCLayer{
public:
    TestScene();
    ~TestScene();
    
    static cocos2d::CCScene* scene();
    virtual bool init();
    
    CREATE_FUNC(TestScene);
    
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
private:
    CCLayer* m_layer1;
    CCLayer* m_layer2;
    CCLayer* m_layer3;
};

#endif
