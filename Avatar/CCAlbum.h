//
//  CCAlbum.h
//  Blackcat
//
//  Created by haojunhua on 14-7-2.
//
//

#ifndef __Blackcat__CCAlbum__
#define __Blackcat__CCAlbum__

#include "cocos2d.h"
USING_NS_CC;

class CCAlbumPickDelegate{
public:
    virtual ~CCAlbumPickDelegate() {};
    virtual void pickCallback(CCTexture2D *texture,float width,float height)=0;
};

class CCAlbum:public CCLayer{
public:
    CREATE_FUNC(CCAlbum);
private:
    bool init();
public:
    CC_SYNTHESIZE(CCAlbumPickDelegate*, m_delegate, Delegate);
};

#endif /* defined(__Blackcat__CCAlbum__) */
