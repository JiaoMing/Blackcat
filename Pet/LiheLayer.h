//
//  LiheLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-6-3.
//
//

#ifndef __Blackcat__LiheLayer__
#define __Blackcat__LiheLayer__

#include "cocos2d.h"
#include "CoverLayer.h"
USING_NS_CC;

class LiheLayer :public CoverLayer{
public:
    LiheLayer();
    ~LiheLayer();
    CREATE_FUNC(LiheLayer);
private:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    int xingxingCount();
    void delayGainExp(float t);
private:
    CCSprite* m_lihe;
    int m_xingxingCount;
    CCSprite* m_guang1;
    CCSprite* m_guang2;
};

#endif /* defined(__Blackcat__LiheLayer__) */
