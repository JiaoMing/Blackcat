//
//  CCWebViewImplAndroid.cpp
//  Blackcat
//
//  Created by haojunhua on 13-10-22.
//
//

#include "CCWebViewImplAndroid.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "CCWebView.h"
#include "Java_com_kidsedu_KEHelper.h"

static std::string s_cmd;

static void webCallBack(std::string cmd, void* ctx)
{
    CCWebViewImplAndroid* thiz = (CCWebViewImplAndroid*)ctx;
	
    if (thiz->getDelegate() != NULL)
    {
        s_cmd=cmd;
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(CCWebViewImplAndroid::scheduleCallback),thiz, 0.0f,0, 0,false);
    }
}

CCWebViewImpl* __createSystemWebView(CCWebView* pWebView)
{
    return new CCWebViewImplAndroid(pWebView);
}

CCWebViewImplAndroid::CCWebViewImplAndroid(CCWebView* pWebView):CCWebViewImpl(pWebView)
{
    
}

CCWebViewImplAndroid::~CCWebViewImplAndroid(){
    removeWebViewJNI();
}

bool CCWebViewImplAndroid::initWithRectAndHtmlFilename(const CCRect& uiRect,const char* htmlFilename)
{
    do
    {
        addWebViewJNI(htmlFilename,uiRect.origin.x,uiRect.origin.y,uiRect.size.width,uiRect.size.height,webCallBack,(void*)this);
        return true;
    }while (0);
    
    return false;
}


void CCWebViewImplAndroid::callWebWithJs(const char* js){
    callWebWithJsJNI(js);
}

void CCWebViewImplAndroid::setVisible(bool var){
    setVisibleJNI(var);
}

void CCWebViewImplAndroid::scheduleCallback(float t){
    this->getDelegate()->webCallBack(s_cmd);
}

#endif
