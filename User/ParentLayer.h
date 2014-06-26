//
//  ParentLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-4-3.
//
//

#ifndef __Blackcat__ParentLayer__
#define __Blackcat__ParentLayer__

#include "DialogLayer.h"

class ParentLayer : public DialogLayer
{
public:
    virtual bool init();
    CREATE_FUNC(ParentLayer);
    
    // touch event
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void menuCallback(CCObject* obj);
    void timingReplace(float t);
    
    
    void showOrHide(bool show);
private:
    CCSprite* m_toggle;
    bool m_isTouchToggle;//began是否touch关开
    bool m_showParent;//当前是否显示家长面板
    int m_tag;//用于定时
    
    float m_fTouchMoveDistance;//总距离,绝对值只和
    float m_fTouchDirectionDistance;//记录touchend触发前move最后一个方向touchmove的距离
    bool m_bDirection;//记录方向false为向左移(m_pThumbLayer坐标值变小),true反之
    cc_timeval m_cctimeStart;//记录最后一个方向的初始时间
};

#endif /* defined(__Blackcat__ParentLayer__) */
