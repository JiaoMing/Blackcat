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
#include "DialogLayer.h"
#include "Hanzi.h"
#include "KechengDialogTableLayer.h"
#include "KechengExamScene.h"
USING_NS_CC;

class KechengDialogLayer : public DialogLayer,public CoverLayerDelegate,public KechengDialogTableLayerDelegate,public KechengExamSceneDelegate{
public:
    KechengDialogLayer();
    ~KechengDialogLayer();
    
public:
    CREATE_FUNC(KechengDialogLayer);
    virtual bool init();
    virtual void menuCallback(CCObject* object);
    virtual void enableTouch();
    
    virtual int topHandlerPriority(){return this->getDelegate()->topHandlerPriority()-1;}
    
    virtual void examAllRightCallback();
    
    void showOrHideTable();
    
public:
    void setKecheng(int kcid);
    
protected:
    int m_kcid;
    vector<Hanzi*>* m_hanziVector;
    KechengDialogTableLayer* m_tableLayer;
    CCMenu* m_menu;
};

#endif /* defined(__Blackcat__KechengFloatLayer__) */
