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
#include "Cartoon.h"

class GuideDialogDelegate;

class GuideDialogLayer: public DialogLayer{
public:
    GuideDialogLayer(){m_guideDialog=NULL;m_cartoon=NULL;}
    ~GuideDialogLayer(){CC_SAFE_RELEASE(m_guideDialog);};
    static GuideDialogLayer* create(GuideDialogType guideDialogType=kDialogBtuOnly,GuideDialogWithTextButtonPosiztion dialogButtonPosiztion=kDialogWithTextButtonPosiztionRight);
    virtual bool init();
    
    virtual void setDelegate(GuideDialogDelegate* guideDialogDelegate);
//    virtual GuideDialogDelegate* getDelegate(){return m_guideDialogDelegate;}
    
    //用于处理空白处点击事件
//    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    void setGuideDialogData(GuideDialog* guideDialog);
    void setCartoon(Cartoon* cartoon){m_cartoon=cartoon;};
    
    virtual void menuCallback(CCObject* object);
    void sayFinished();
    
protected:
    GuideDialogDelegate* m_guideDialogDelegate;
    GuideDialog* m_guideDialog;
    GuideDialogType m_guideDialogType;
    GuideDialogWithTextButtonPosiztion m_dialogButtonPosiztion;
    
    /**
     * 准备将cartoon处理从GuideBaseLayer中转移至GuideDialogLayer
     */
    Cartoon* m_cartoon;
};

class GuideDialogDelegate :public DialogLayerDelegate{
public:
    /**
     * 对话框按钮事件回调
     */
    virtual void dialogCallBack(GuideDialogCMD cmd)=0;
};

#endif
