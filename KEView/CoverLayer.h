//
//  CoverLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-4-10.
//
//

#ifndef Blackcat_CoverLayer_h
#define Blackcat_CoverLayer_h


#include "cocos2d.h"
USING_NS_CC;

class CoverLayerDelegate{
public:
    /**
     * 对话框优先级反馈，也等于当前层的最高级
     * 用于禁止下层事件相应
     */
    virtual int topHandlerPriority()=0;
};

class CoverLayer: public CCLayerColor{
public:
    virtual bool init();
    CREATE_FUNC(CoverLayer);
    
    // register touch
    virtual void onEnter();
    virtual void onExit();
    // touch event
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void setDelegate(CoverLayerDelegate* delegate);
    virtual CoverLayerDelegate* getDelegate(){return m_coverLayerDelegate;}
protected:
    CoverLayerDelegate* m_coverLayerDelegate;
};

#endif
