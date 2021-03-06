//
//  Guide.h
//  Blackcat
//
//  Created by haojunhua on 13-12-26.
//
//

#ifndef Blackcat_Guide_h
#define Blackcat_Guide_h

#include "cocos2d.h"
USING_NS_CC;
#include <string.h>
#include "Cartoon.h"
using namespace std;

typedef enum{
    kGuideDialog=0,
    kGuideEvent
}GuideType;

typedef enum{
    kGuideEventDefault=-1,
    kGuideEventOpen=0,
    kGuideEventPass=1
}GuideEventStatus;

typedef enum{
    kGuideDialogOk=0,
    kGuideDialogYesOrNo,
    kGuideDialogHidden,
    kGuideDialogCustomized
}GuideDialogMode;

typedef enum{
    kDialogBtuOnly=0,
    kDialogWithText
}GuideDialogType;

typedef enum{
    kDialogWithTextButtonPosiztionLeft=0,
    kDialogWithTextButtonPosiztionRight
}GuideDialogWithTextButtonPosiztion;

typedef enum{
    kDialogCMDOk=0,
    kDialogCMDYes,
    kDialogCMDNo,
    kDialogCMDBlank,
    kDialogCMDClose,
}GuideDialogCMD;

class Guide : public CCObject{
public:
    virtual ~Guide(){};
    
    //引导类型
    CC_SYNTHESIZE(int, m_type, Type);
    //下一个引导Dict对应的key，用于ok事件完成引导
    CC_SYNTHESIZE(string, m_toGuideKey, ToGuideKey);
};

class GuideDialog :public Guide{
public:
    GuideDialog(){
        m_type=kGuideDialog;
        m_tag=0;
        m_duration=0;
        m_text="";
        m_isAutoStep=false;
        m_isAutoPlayAudio=false;
        m_isDelayShow=true;
        m_mode=kGuideDialogOk;
        m_dialogType=kDialogBtuOnly;
        m_actionArray=CCArray::create();
        m_actionArray->retain();
        m_audioArray=CCArray::create();
        m_audioArray->retain();
        m_cartoonPoint="";
        m_cartoon=NULL;
    };
    
    static GuideDialog* create(){
        GuideDialog* guideDialog=new GuideDialog();
        guideDialog->autorelease();
        return guideDialog;
    }
    
    ~GuideDialog(){CC_SAFE_RELEASE(m_actionArray);CC_SAFE_RELEASE(m_audioArray);}
    
    CC_SYNTHESIZE(int, m_tag, Tag);
    
    CC_SYNTHESIZE(float, m_duration, Duration);
    
    CC_SYNTHESIZE(string, m_text, Text);
    
    CC_SYNTHESIZE(bool, m_isAutoStep, IsAutoStep);
    
    CC_SYNTHESIZE(bool, m_isAutoPlayAudio, IsAutoPlayAudio);
    
    CC_SYNTHESIZE(bool, m_isDelayShow, IsDelayShow);
    
    //对话框模式，OK，yesorno，隐藏模式
    CC_SYNTHESIZE(GuideDialogMode, m_mode, Mode);
    
    //对话框类型，显示对话框和按钮、仅按钮
    CC_SYNTHESIZE(GuideDialogType, m_dialogType, DialogType);
    
    CC_SYNTHESIZE(string, m_cartoonPoint, CartoonPoint);
    
    CC_SYNTHESIZE(CCArray*, m_actionArray, ActionArray);
    
    CC_SYNTHESIZE(CCArray*, m_audioArray, AudioArray);
    
    CC_SYNTHESIZE(Cartoon*, m_cartoon, Cartoon);
};

class GuideEvent :public Guide{
public:
    GuideEvent(){m_type=kGuideEvent;};
    ~GuideEvent(){}
    CC_SYNTHESIZE(string, m_key, Key);
    CC_SYNTHESIZE(int, m_tag, Tag);
};

#endif
