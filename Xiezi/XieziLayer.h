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
#include "Heimao.h"
#include "resource.h"
#include "CCWebView.h"
#include "GuideDialogLayer.h"
#include "Content.h"

class XieziLayerDelegate;
class XieziLayer : public CCLayer,public CCWebViewDelegate,public GuideDialogDelegate
{
public:
    static XieziLayer* create(Hanzi* hanzi,bool isShowX=true,Heimao* heimao=NULL,string pyyd="");
public:
    XieziLayer();
    ~XieziLayer();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void menuCallback(CCObject* pSender);
    
    /**
     *  生成webview快照
     */
    CCSprite* generateSnapshot();
    
    void detailChangeCallback();

    /**
     定时提醒用户操作
     */
    void dingShiTiXing();
    
    void xingxingAnimateEnd();
    
    void callWeb(CCObject* pSender);
    
    virtual void webCallBack(CCWebView* webview,std::string cmd);
    
    void apiCallBack(CCDictionary* root);
    
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
    Content* m_mContent;//主汉字音频资源
    CCWebView* m_webView;
    
    
    bool m_isWriteFinished;//是否写完整个汉字标记，用于笔图标作用
    
    bool m_isShowX;//是否显示关闭按钮
    
    int m_collectedCount;//已收集的汉字数量
    int m_writedHanziCount;//被写郭的汉字个数
    
    GuideDialogLayer* m_gudieDialogLayer;
    
    CCSprite* m_xingxing;
    CCLabelTTF* m_count;
    
    Heimao* m_heimao;//黑猫引用，不在当前layer中create
    
    int m_topHandlerPriority;
    
    /**
     *  是否显示详情标记
     */
    bool m_isShowDetail;
    
    int m_xieziLogId;//写字LOG生成的ID
    int m_todayXieziCount;//今天当前汉字书写次数
};

class XieziLayerDelegate{
public:
    virtual void webCallBack(WebCallBackCMD cmd)=0;
};

#endif /* defined(__Blackcat__XieziLayer__) */
