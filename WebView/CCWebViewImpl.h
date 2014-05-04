//
//  CCWebViewImpl.h
//  Blackcat
//
//  Created by haojunhua on 13-10-15.
//
//

#ifndef Blackcat_CCWebViewImpl_h
#define Blackcat_CCWebViewImpl_h

#include "cocos2d.h"
#include "CCWebView.h"

class CCWebViewImpl:public CCObject{
public:
    CCWebViewImpl(CCWebView* pWebView):m_pWebView(pWebView), m_pWebViewDelegate(NULL)  {}
    virtual ~CCWebViewImpl() {}
    
    void setDelegate(CCWebViewDelegate* pDelegate) { m_pWebViewDelegate = pDelegate; };
    CCWebViewDelegate* getDelegate() { return m_pWebViewDelegate; };
    CCWebView* getCCWebView() { return m_pWebView; };
public:
    virtual bool initWithRectAndHtmlFilename(const CCRect& rect,const char* htmlFilename) = 0;
    virtual void callWebWithJs(const char* js)=0;
    virtual void setVisible(bool var)=0;
    
protected:
    CCWebView* m_pWebView;
    CCWebViewDelegate* m_pWebViewDelegate;
};

// This method must be implemented at each subclass of CCWebViewImpl.
extern CCWebViewImpl* __createSystemWebView(CCWebView* pWebView);

#endif
