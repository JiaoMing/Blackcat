//
//  CoverLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-3-2.
//
//

#ifndef Blackcat_CoverLayer_h
#define Blackcat_CoverLayer_h

#include "cocos2d.h"
USING_NS_CC;

class CoverLayer : public CCLayerColor
{
public:
    CREATE_FUNC(CoverLayer);
    
    // register touch
    virtual void onEnter();
    virtual void onExit();
    // touch event
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
};


#endif
