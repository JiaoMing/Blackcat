//
//  TupianBrowserLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-1-2.
//
//

#ifndef Blackcat_TupianBrowserLayer_h
#define Blackcat_TupianBrowserLayer_h
#include "cocos2d.h"
USING_NS_CC;
#include "Tupian.h"
#include "TupianTableLayer.h"
#include "GuideDialogLayer.h"

class TupianBrowserDelegate;

class TupianBrowserLayer : public CCLayerColor,public TupianTableDelegate,public GuideDialogDelegate
{
public:
    TupianBrowserLayer();
    ~TupianBrowserLayer();
    static TupianBrowserLayer* create(TupianBrowserDelegate* TupianBrowserDelegate,Tupian *tupian);
    virtual bool init();
    
    // register touch
    virtual void onEnter();
    virtual void onExit();
    
    virtual void keyBackClicked();
    virtual void keyMenuClicked();
    // touch event
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void tupianLoadCallBack(int count);
    virtual void tupianTouchCallBack(Tupian* tupian);
    
    virtual int topHandlerPriority(){return kCCMenuHandlerPriority-1;}
    virtual void dialogCallBack(GuideDialogCMD cmd);
    
    void setMainTupianSprite();
    void enableTouch();
    void menuCallback(CCObject* pSender);
    
    void wenziCallback(CCObject* pSender);
    
    
public:
    CC_SYNTHESIZE(Tupian*, m_tupian, Tupian);
private:
    TupianBrowserDelegate* m_tupianBrowserDelegate;
    TupianTableLayer* m_tupianTabelLayer;
    CCMenu* m_wenziMenu;
    bool m_isTableShow;
    float m_scale;
    int m_hanziID;
    GuideDialogLayer* m_gudieDialogLayer;
    cc_timeval m_enterTime;//用于定时启动触屏事件(防止点击汉字关闭层)
    float m_delay;//用于点击事件延迟(等待读完词语)
};


class TupianBrowserDelegate{
public:
    virtual void hideBrowserCallBack(){};
    virtual bool isPushToXieziScene(){return true;}

};


#endif
