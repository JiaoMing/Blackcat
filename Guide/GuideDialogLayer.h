//
//  GuideDelegate.h
//  Blackcat
//
//  Created by haojunhua on 13-12-27.
//
//

#ifndef Blackcat_GuideDelegate_h
#define Blackcat_GuideDelegate_h

#include "GuideManager.h"
#include "GuideCoverLayer.h"

typedef enum{
    kDialogCMDOk=0,
    kDialogCMDYes,
    kDialogCMDNo,
    kDialogCMDBlank
}GuideDialogCMD;

typedef enum{
    kDialogBtuOnly=0,
    kDialogWithText
}GuideDialogType;

class GuideDialogDelegate;

class GuideDialogLayer: public GuideCoverLayer{
public:
    GuideDialogLayer(){}
    static GuideDialogLayer* create(GuideDialogType guideDialogType=kDialogBtuOnly);
    virtual bool init();
    
    // register touch
    virtual void onEnter();
    virtual void onExit();
    
    virtual void setDelegate(GuideDialogDelegate* guideDialogDelegate);
    
    void enableTouch();
    
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    void setGuideDialogData(GuideDialog* guideDialog);
    void menuCallBack(CCObject* object);
    void sayFinished();
    
private:
    GuideDialogDelegate* m_guideDialogDelegate;
    GuideDialog* m_guideDialog;
    GuideDialogType m_guideDialogType;
};

class GuideDialogDelegate :public GuideCoverDelegate{
public:
    /**
     * 对话框按钮事件回调
     */
    virtual void dialogCallBack(GuideDialogCMD cmd)=0;
};

#endif
