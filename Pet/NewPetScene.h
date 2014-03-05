//
//  NewPetScene.h
//  Blackcat
//
//  Created by haojunhua on 13-12-15.
//
//

#ifndef Blackcat_NewPetScene_h
#define Blackcat_NewPetScene_h

#include "resource.h"
#include "cocos2d.h"
#include "Cartoon.h"
//#include "MicRecordPlayer.h"
#include "GuideDialogLayer.h"
#include "RankingBarLayer.h"
USING_NS_CC;


//class NewPetScene : public CCLayer,public MicRecordPlayerDelegate,public GuideDialogDelegate
class NewPetScene : public CCLayer,public GuideDialogDelegate
{
public:
    static CCScene* scene();
public:
    NewPetScene();
    ~NewPetScene();
private:
    virtual bool init();
    CREATE_FUNC(NewPetScene);
    
    virtual void onEnter();
    virtual void onExit();
    
    //菜单回调
    void menuCallBack(CCObject* pSender);
    //点击熊猫回调
    void pressPandaCallBack(CCObject *object, const char *pszBoxName);
    
    //对话框
    virtual int topHandlerPriority(){return kCCMenuHandlerPriority-1;}
    virtual void dialogCallBack(GuideDialogCMD cmd);
    
//    virtual void micCallBack(MicCallBackCMD cmd);
//    virtual MicDelegateMode getMicDelegateMode(){return m_micDelegateMode;}
private:
    Cartoon* m_panda;//小熊猫
    CCMenuItemToggle* m_modeToggle;
//    MicRecordPlayer* m_micRecordPlayer;
//    MicDelegateMode m_micDelegateMode;
    GuideDialogLayer* m_gudieDialogLayer;
    RankingBarLayer* m_rankingBarLayer;
};

#endif
