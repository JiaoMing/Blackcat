//
//  GraySprite.h
//  Blackcat
//
//  Created by haojunhua on 13-12-11.
//
//

#ifndef __Blackcat__GraySprite__
#define __Blackcat__GraySprite__


#include "cocoa/CCGeometry.h"
#include "cocos2d.h"
USING_NS_CC;

class GraySprite : public CCSprite{
    
public:
    GraySprite();
    virtual ~GraySprite();
    static GraySprite* create(const char* pszFileName);
    static GraySprite* createWithSpriteFrameName(const char *pszSpriteFrameName);
    bool initWithTexture(CCTexture2D* pTexture, const CCRect& tRect);
};



#endif /* defined(__Blackcat__GraySprite__) */
