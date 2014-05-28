//
//  XieziCoverLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-5-27.
//
//

#ifndef __Blackcat__XieziCoverLayer__
#define __Blackcat__XieziCoverLayer__

#include "XieziLayer.h"
#include "CoverLayer.h"

class XieziCoverLayer: public CoverLayer{
public:
    static XieziCoverLayer* create(int hid);
public:
    XieziCoverLayer(int hid);
    ~XieziCoverLayer();
    virtual bool init();
    virtual void onEnter();
    // touch event
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    XieziLayer* getXieziLayer(){return m_xieziLayer;};
private:
    XieziLayer* m_xieziLayer;
    Hanzi* m_hanzi;
};

#endif /* defined(__Blackcat__XieziCoverLayer__) */
