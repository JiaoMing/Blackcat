//
//  ObjectManager.h
//  Blackcat
//
//  Created by haojunhua on 14-6-26.
//
//

#ifndef __Blackcat__ObjectManager__
#define __Blackcat__ObjectManager__

#include "cocos2d.h"
USING_NS_CC;
#include "CCWebView.h"

class ObjectManager :public CCObject{
public:
    static ObjectManager* sharedObjectManager();
public:
    ObjectManager();
    ~ObjectManager();
    
    void hideNativeView();
    void resumeNativeView();
    
protected:
    CC_SYNTHESIZE(CCWebView*, m_webView, WebView);
    bool m_webVisible;
};

#endif /* defined(__Blackcat__ObjectManager__) */
