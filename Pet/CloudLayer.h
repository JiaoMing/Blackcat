//
//  CloudLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-6-22.
//
//

#ifndef __Blackcat__CloudLayer__
#define __Blackcat__CloudLayer__

#include "cocos2d.h"
USING_NS_CC;

class CloudLayer: public CCLayer{
public:
    CREATE_FUNC(CloudLayer);
private:
    virtual bool init();
    
    void timer(float t);
private:
    CCSprite* m_yun1;
    CCSprite* m_yun2;
    CCSprite* m_yun3;
};

#endif /* defined(__Blackcat__CloudLayer__) */
