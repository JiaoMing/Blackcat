//
//  GameLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-3-30.
//
//

#ifndef __Blackcat__GameLayer__
#define __Blackcat__GameLayer__


#include "resource.h"
#include "CCWebView.h"
#include "Hanzi.h"
#include "Content.h"
#include "XiaoboWriteLayer.h"


class GameLayerDelegate{
public:
    virtual void gameEnd(GamePlayer winner)=0;
};

class GameLayer:public CCLayer,public CCWebViewDelegate,public XiaoboWriteDelegate{
public:
    virtual bool init();
    CREATE_FUNC(GameLayer);
    
    // register touch
    virtual void onEnter();
    virtual void onExit();
    // touch event
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void webCallBack(CCWebView* webview,std::string cmd);
    
    virtual void xiaoboEndCallBack();
    
    void startGame();
    void gameEnd(GamePlayer winner);
    void gameEndAnimateEnd();
    virtual void setDelegate(GameLayerDelegate* delegate){m_delegate=delegate;}
    
    //主循环
    void mainloop(float t);
private:
    CCWebView* m_webView;
    int userStrokeIndex;
    Hanzi* m_hanzi;
    Content* m_content;
    XiaoboWriteLayer* m_xiaoboWriteLayer;
    XiaoboWriteLayer* m_woWriteLayer;
    GameLayerDelegate* m_delegate;
    GamePlayer m_winner;
    CCSprite* m_guanzhong;
    bool m_isEnd;
    bool m_isStart;
};

#endif /* defined(__Blackcat__GameLayer__) */
