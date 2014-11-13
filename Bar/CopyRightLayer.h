//
//  CopyRightLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-1-26.
//
//

#ifndef Blackcat_CopyRightLayer_h
#define Blackcat_CopyRightLayer_h
#include "cocos2d.h"
USING_NS_CC;
#include "CoverLayer.h"

class CopyRightLayer : public CoverLayer
{
public:
    virtual bool init();
    CREATE_FUNC(CopyRightLayer);
    
    // touch event
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

};

#endif
