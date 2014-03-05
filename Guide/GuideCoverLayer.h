//
//  GuideCoverLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-1-8.
//
//

#ifndef Blackcat_GuideEventLayer_h
#define Blackcat_GuideEventLayer_h


#include "GuideManager.h"

class GuideCoverDelegate{
public:
    /**
     * 对话框优先级反馈，也等于当前层的最高级
     * 用于禁止下层事件相应
     */
    virtual int topHandlerPriority()=0;
};

class GuideCoverLayer: public CCLayerColor{
public:
    virtual bool init();
    CREATE_FUNC(GuideCoverLayer);
    
    // register touch
    virtual void onEnter();
    virtual void onExit();
    // touch event
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void setDelegate(GuideCoverDelegate* delegate);
protected:
    GuideCoverDelegate* m_guideCoverDelegate;
};

#endif
