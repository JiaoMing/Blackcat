//
//  Props.h
//  Blackcat
//
//  Created by haojunhua on 13-12-5.
//
//

#ifndef Blackcat_Props_h
#define Blackcat_Props_h

#include "cocos2d.h"
#include "ClickableSprite.h"
#include "Prop.h"
using namespace cocos2d;
using namespace std;


class AnimateAndAudio :public CCObject{
public:
    ~AnimateAndAudio(){m_audioName->release();m_animate->release();}
    void setAndRetainAudioName(CCString* audioName){m_audioName=audioName;m_audioName->retain();};
    void setAndRetainAnimate(CCAnimate* animate){m_animate=animate;m_animate->retain();};
    
    CC_SYNTHESIZE(CCString*, m_audioName, AudioName);
    CC_SYNTHESIZE(CCAnimate*, m_animate, Animate);
};

class Props : public ClickableSprite
{
public:
    Props(string key);
    ~Props();
public:
    /**
     * 展示静止动画
     */
    void showStopPose();
    /**
     * 运行一个随机的动画
     * animateTime:次数
     */
    void runRandomAnimate(unsigned int animateTime=1,bool willPlayMusic=true,bool willCallback=true);
    /**
     * 运行一个动画
     * animateIndex:动画序号
     */
    void runAnimate(int animateIndex=0,unsigned int animateTime=1,unsigned int animateInterval=0,bool willCallback=true,bool willPlayMusic=true);
    
    //运行回调
    void runCallBack();
    
    //设置点击优先级
    void setHandlerPriority(int newPriority);
public:
    //是否立即响应操作
    CC_SYNTHESIZE(bool, m_isImmediate, IsImmediate);
    //每次结束动作后是否立即显示stoppos
    CC_SYNTHESIZE(bool, m_isShowStopPosAfterARun, IsShowStopPosAfterARun);
    CC_SYNTHESIZE(bool, m_isClickable, IsClickAble);
    CC_SYNTHESIZE(bool, m_hasXingxing, HasXingxing);
    CC_SYNTHESIZE(CCSpriteFrame*, m_stopPos, StopPos);
    CC_SYNTHESIZE(CCArray*, m_animates, Animates);
    CC_SYNTHESIZE(CCRect, m_touchRect, TouchRect);
    CC_SYNTHESIZE(Prop*, m_prop, Prop);
private:
    CCPoint m_oPointBegan;
public:
    
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
};


#endif
