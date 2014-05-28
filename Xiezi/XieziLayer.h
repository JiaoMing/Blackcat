//
//  XieziLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-5-27.
//
//

#ifndef __Blackcat__XieziLayer__
#define __Blackcat__XieziLayer__


#include "Hanzi.h"
#include "resource.h"
#include "CCWebView.h"
#include "GuideDialogLayer.h"

class XieziLayerDelegate;
class XieziLayer : public CCLayer,public CCWebViewDelegate,public GuideDialogDelegate
{
public:
    static XieziLayer* create(Hanzi* hanzi,bool isShowX=true);
public:
    XieziLayer();
    ~XieziLayer();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void menuCallback(CCObject* pSender);
    
    void xingxingAnimateEnd();
    
    void callWeb(CCObject* pSender);
    
    virtual void webCallBack(CCWebView* webview,std::string cmd);
    
    void setHandlerPriority(int handlerPriority);
    void enableTouch();
    
    virtual int topHandlerPriority(){return m_topHandlerPriority-1;}
    virtual void dialogCallBack(GuideDialogCMD cmd);
    
    CCWebView* getWebView(){return m_webView;};
    
    virtual void setVisible(bool isVisible);
public:
    CC_SYNTHESIZE(XieziLayerDelegate*, m_delegate, Delegate);
private:
    Hanzi* m_hanzi;
    CCWebView* m_webView;
    
    
    bool m_isWriteFinished;//是否写完整个汉字标记，用于笔图标作用
    
    bool m_isShowX;//是否显示关闭按钮
    
    int m_collectedCount;//已收集的汉字数量
    int m_writeCount;
    
    GuideDialogLayer* m_gudieDialogLayer;
    
    CCSprite* m_xingxing;
    CCLabelTTF* m_count;
    
    int m_topHandlerPriority;
    
};

class XieziLayerDelegate{
public:
    virtual void webCallBack(WebCallBackCMD cmd)=0;
};

#endif /* defined(__Blackcat__XieziLayer__) */
