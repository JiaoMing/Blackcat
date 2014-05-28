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
#include "DialogLayer.h"

class GuideDialogDelegate;

class GuideDialogLayer: public DialogLayer{
public:
    GuideDialogLayer(){}
    ~GuideDialogLayer(){m_guideDialog->release();};
    static GuideDialogLayer* create(GuideDialogType guideDialogType=kDialogBtuOnly,GuideDialogWithTextButtonPosiztion dialogButtonPosiztion=kDialogWithTextButtonPosiztionRight);
    virtual bool init();
    
    virtual void setDelegate(GuideDialogDelegate* guideDialogDelegate);
//    virtual GuideDialogDelegate* getDelegate(){return m_guideDialogDelegate;}
    
    //用于处理空白处点击事件
//    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    void setGuideDialogData(GuideDialog* guideDialog);
    virtual void menuCallback(CCObject* object);
    void sayFinished();
    
private:
    GuideDialogDelegate* m_guideDialogDelegate;
    GuideDialog* m_guideDialog;
    GuideDialogType m_guideDialogType;
    GuideDialogWithTextButtonPosiztion m_dialogButtonPosiztion;
};

class GuideDialogDelegate :public CoverLayerDelegate{
public:
    /**
     * 对话框按钮事件回调
     */
    virtual void dialogCallBack(GuideDialogCMD cmd)=0;
};

#endif
