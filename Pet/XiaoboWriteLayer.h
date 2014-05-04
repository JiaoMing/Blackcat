//
//  XiaoboWriteLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-4-1.
//
//

#ifndef __Blackcat__XiaoboWriteLayer__
#define __Blackcat__XiaoboWriteLayer__

#include "resource.h"
#include "Hanzi.h"
#include "ParseredHanzi.h"

class XiaoboWriteDelegate{
public:
    virtual void xiaoboEndCallBack()=0;
};

class XiaoboWriteLayer:public CCLayer{
public:
    static XiaoboWriteLayer* create(Hanzi* hanzi,GamePlayer gamePlayer);
    XiaoboWriteLayer(Hanzi* hanzi);
    ~XiaoboWriteLayer();
    virtual bool init();
    
    void runHanziLabelAnimate(float delay=0);
    void startGame();
    void loop(float t);
    
//    void reGame();
    virtual void setDelegate(XiaoboWriteDelegate* delegate){m_delegate=delegate;}
    ParseredHanzi* getParseredHanzi(){return m_parseredHanzi;}
private:
    Hanzi* m_hanzi;
    CCDrawNode* m_draw;
    ParseredHanzi* m_parseredHanzi;
    CCSprite* m_touxiang;
    XiaoboWriteDelegate* m_delegate;
    GamePlayer m_gamePlayer;
};

#endif /* defined(__Blackcat__XiaoboWriteLayer__) */
