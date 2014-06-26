//
//  CCWebView.cpp
//  Blackcat
//
//  Created by haojunhua on 13-10-15.
//
//

#include "CCWebView.h"
#include "CCWebViewImpl.h"
#include "ObjectManager.h"

CCWebView::CCWebView(){
    
}

CCWebView::~CCWebView(){
    CC_SAFE_DELETE(m_pWebViewImpl);
}

CCWebView* CCWebView::create(const char* htmlFilename,CCRect rectOfScreen){
    CCWebView *pRet = new CCWebView();
    if (pRet && pRet->init(htmlFilename,rectOfScreen))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

void CCWebView::onEnter(){
    CCLayer::onEnter();
    ObjectManager::sharedObjectManager()->setWebView(this);
}

void CCWebView::onExit(){
    CCLayer::onExit();
    ObjectManager::sharedObjectManager()->setWebView(NULL);
}

bool CCWebView::init(const char* htmlFilename,CCRect rectOfScreen){
    m_pWebViewImpl = __createSystemWebView(this);
    m_pWebViewImpl->initWithRectAndHtmlFilename(this->rectByResolutionPolicy(rectOfScreen), htmlFilename);
    return true;
}

CCRect CCWebView::rectByResolutionPolicy(CCRect rect){
    float scaleX=CCEGLView::sharedOpenGLView()->getScaleX();
    float scaleY=CCEGLView::sharedOpenGLView()->getScaleY();
    CCRect viewPortRect=CCEGLView::sharedOpenGLView()->getViewPortRect();
    return CCRectMake(rect.origin.x*scaleX+viewPortRect.origin.x, rect.origin.y*scaleY+viewPortRect.origin.y, rect.size.width*scaleX, rect.size.height*scaleY);
}

void CCWebView::setDelegate(CCWebViewDelegate* pDelegate)
{
    m_pWebViewDelegate = pDelegate;
    if (m_pWebViewImpl != NULL)
    {
        m_pWebViewImpl->setDelegate(pDelegate);
    }
}

void CCWebView::callWebWithJs(const char* js){
    if (m_pWebViewImpl != NULL)
    {
        m_pWebViewImpl->callWebWithJs(js);
    }
}

void CCWebView::setVisible(bool var){
    if (m_pWebViewImpl != NULL)
    {
        m_pWebViewImpl->setVisible(var);
    }
}
