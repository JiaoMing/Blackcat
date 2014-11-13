//
//  ImageEditLayer.h
//  Blackcat
//
//  Created by haojunhua on 14/10/11.
//
//

#ifndef __Blackcat__ImageEditLayer__
#define __Blackcat__ImageEditLayer__

#include "cocos2d.h"
#include "DialogLayer.h"
USING_NS_CC;

class ImageEditLayer : public DialogLayer
{
public:
    ImageEditLayer();
    ~ImageEditLayer();
    static ImageEditLayer* create(CCImage* image);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    
    void moveAndEnd();
    
    virtual void menuCallback(CCObject* obj);
    
    virtual void callbackFresh();
    virtual CCArray* getAssets(){return _assets;}
private:
    bool touchMenu(CCNode*item,CCPoint point);
private:
    CCImage* _image;
    CCArray* _assets;
    CCSprite* _sprite;
    CCClippingNode* _clip;
    CCNode* _stencil;
    CCNode* _mask;
    CCRect _rect;
    
    CCArray* _allTargetedHandlers;
    
    CCPoint _initPoint;//图片初始位置
    double _distance;    //两个触摸点之间的距离
    double _deltaX;    //目标x轴的改变值
    double _deltaY;    //目标y轴的改变值
    double _scale;   //缩放比例
    CCArray* _touchs;
};


#endif /* defined(__Blackcat__ImageEditLayer__) */
