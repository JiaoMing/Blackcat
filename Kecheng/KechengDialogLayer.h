//
//  KechengDialogLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-5-22.
//
//

#ifndef __Blackcat__KechengDialogLayer__
#define __Blackcat__KechengDialogLayer__

#include "cocos2d.h"
#include "GuideDialogLayer.h"
#include "Hanzi.h"
#include "KechengDialogTableLayer.h"
#include "KechengExamScene.h"
#include "Heimao.h"
USING_NS_CC;

class KechengDialogLayer : public GuideDialogLayer,public KechengDialogTableLayerDelegate,public KechengExamSceneDelegate{
public:
    static KechengDialogLayer* create(Heimao* heimao);
    KechengDialogLayer();
    ~KechengDialogLayer();
    
public:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual void menuCallback(CCObject* object);
    virtual void enableTouch();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void examAllRightCallback();
    
    void heimaoAction();
    
    void runRewardAnimate();
    void runRewardAnimateCallBack(CCObject* obj);
    
    void showOrHideTable();
    
    void ceShiTiXing(float t);
    void dingShiTiXing(float t);
    
public:
    void setKecheng(int kcid);
protected:
    KechengDialogTableLayer* m_tableLayer;
    CCMenu* m_hanziLabelMenu;
    CCLabelTTF* m_kechengLabel;
    CCMenuItem* m_ceshiItem;
    Heimao* m_heimao;
    int m_badgeCallBackCount;
    CCArray* m_jiangpaiArray;
};

#endif /* defined(__Blackcat__KechengFloatLayer__) */
