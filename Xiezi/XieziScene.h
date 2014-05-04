//
//  ShiziScene.h
//  blackcat
//
//  Created by haojunhua on 13-8-26.
//
//

#ifndef blackcat_XieziScene_h
#define blackcat_XieziScene_h

#include "cocos2d.h"
#include "TupianTableLayer.h"
#include "Hanzi.h"
#include "resource.h"
#include "CCWebView.h"
#include "Heimao.h"
#include "TupianBrowserLayer.h"

class XieziSceneDelegate;

class XieziScene : public cocos2d::CCLayerColor,public TupianTableDelegate,public CCWebViewDelegate,public TupianBrowserDelegate,public GuideDialogDelegate
{
public:
    static CCScene* scene(int hid);
    static CCScene* scene(int hid,XieziSceneDelegate* xieziSceneDelegate,int indexInKapianTable);
public:
    XieziScene(int hid,XieziSceneDelegate* xieziSceneDelegate,int indexInKapianTable);
    ~XieziScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual void keyBackClicked();
    virtual void keyMenuClicked();
    
    void menuCallback(CCObject* pSender);
    
    void callWeb(CCObject* pSender);
    
    /**
     * 回调需注意所有的音效必须preload，否则会死锁
     */
    void wenziCallback(CCObject* pSender);
    
    /**
     定时提醒用户操作
     */
    void dingShiTiXing();
    
    void setStar(bool isInit=false);
    void xingxingAnimate();
    
    void pressHeimaoCallback();
    
    virtual void webCallBack(CCWebView* webview,std::string cmd);
    
    virtual void tupianLoadCallBack(int count);
    virtual void tupianTouchCallBack(Tupian* tupian);
    
    virtual void hideBrowserCallBack();
    virtual bool isPushToXieziScene(){return false;}
    
    
    virtual int topHandlerPriority(){return kCCMenuHandlerPriority-1;}
    virtual void dialogCallBack(GuideDialogCMD cmd);
    
private:
    TupianTableLayer* m_tupianTabel;
    Hanzi* m_hanzi;
    CCWebView* m_webView;
    CCWebView* m_webViewTest;
    
    Heimao* m_heimao;
    
    //用于回调
    XieziSceneDelegate* m_xieziSceneDelegate;
    
    int m_indexInKapianTable;
    
    //闪星效果
    CCParticleSun* m_emitter;
    
    //是否写完整个汉字标记，用于笔图标作用
    bool isWriteFinished;
    
    
    GuideDialogLayer* m_gudieDialogLayer;
    
    int m_collectedCount;//已收集的汉字数量
    int m_writeCount;
};

class XieziSceneDelegate{
public:
    virtual void xieziCallback(int index,float progress)=0;
};



#endif
