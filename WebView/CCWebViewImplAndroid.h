//
//  CCWebViewImplAndroid.h
//  Blackcat
//
//  Created by haojunhua on 13-10-22.
//
//

#ifndef Blackcat_CCWebViewImplAndroid_h
#define Blackcat_CCWebViewImplAndroid_h

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "CCWebViewImpl.h"


class CCWebView;

class CCWebViewImplAndroid : public CCWebViewImpl
{
public:
    CCWebViewImplAndroid(CCWebView* pWebView);
    virtual ~CCWebViewImplAndroid();
    
public:
    virtual bool initWithRectAndHtmlFilename(const CCRect& ccRect,const char* htmlFilename);
    virtual void callWebWithJs(const char* js);
    virtual void setVisible(bool var);
    virtual void scheduleCallback(float t);
};


#endif /* #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) */

#endif
