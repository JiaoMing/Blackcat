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
static std::map<int, void*> s_webviewImplMap;
static int map_key=0;

static void webCallBack(int key,std::string cmd)
{
    CCWebViewImplAndroid* thiz = (CCWebViewImplAndroid*)(s_webviewImplMap.at(key));
    thiz->setCmd(cmd);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(CCWebViewImplAndroid::scheduleCallback),thiz, 0.0f,0, 0,false);
}

CCWebViewImpl* __createSystemWebView(CCWebView* pWebView)
{
    return new CCWebViewImplAndroid(pWebView);
}

CCWebViewImplAndroid::CCWebViewImplAndroid(CCWebView* pWebView):CCWebViewImpl(pWebView)
{
    
}

CCWebViewImplAndroid::~CCWebViewImplAndroid(){
    s_webviewImplMap.erase(m_key);
    if (s_webviewImplMap.size()==0) {
        map_key=0;
    }
    removeWebViewJNI(m_key);
}

bool CCWebViewImplAndroid::initWithRectAndHtmlFilename(const CCRect& uiRect,const char* htmlFilename)
{
    do
    {
        m_key=map_key++;
        s_webviewImplMap.insert(std::pair<int, void*>(m_key,this));
        addWebViewJNI(m_key,htmlFilename,uiRect.origin.x,uiRect.origin.y,uiRect.size.width,uiRect.size.height,webCallBack,(void*)this);
        return true;
    }while (0);
    return false;
}

const char* CCWebViewImplAndroid::callWebWithJs(const char* js){
    callWebWithJsJNI(m_key,js);
    return "";
}

void CCWebViewImplAndroid::setVisible(bool var){
    setVisibleJNI(m_key,var);
}

void CCWebViewImplAndroid::scheduleCallback(float t){
    if(this->getDelegate())this->getDelegate()->webCallBack(this->getCCWebView(),this->getCmd());
}

#endif
