//
//  CCWebViewImplIOS.h
//  Blackcat
//
//  Created by haojunhua on 13-10-15.
//
//

#ifndef Blackcat_CCWebViewImplIOS_h
#define Blackcat_CCWebViewImplIOS_h

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "cocos2d.h"
#include "CCWebView.h"
#include "CCWebViewImpl.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface WebViewImplIOS : NSObject<UIWebViewDelegate>{
    UIWebView*  m_uiWebview;
    void* m_ccWebview;
}

@property(nonatomic, retain) UIWebView*  uiWebview;
@property(nonatomic, assign) void*  ccWebview;

-(id) initWithFrame: (CGRect) frameRect webView:(void*) webView htmlFilename: (const char*) htmlFilename;
-(const char*)callWebWithJs:(const char*)js;
-(void)setVisible:(BOOL)var;
@end


class CCWebViewImplIOS : public CCWebViewImpl{
public:
    CCWebViewImplIOS(CCWebView* pWebView);
    virtual ~CCWebViewImplIOS();
public:
    virtual bool initWithRectAndHtmlFilename(const CCRect& ccRect,const char* htmlFilename);
    virtual const char* callWebWithJs(const char* js);
    virtual void setVisible(bool var);
protected:
    WebViewImplIOS* m_systemControl;
};


#endif /* #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) */

#endif
