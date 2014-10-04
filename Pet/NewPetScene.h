//
//  NewPetScene.h
//  Blackcat
//
//  Created by haojunhua on 13-12-15.
//
//

#ifndef Blackcat_NewPetScene_h
#define Blackcat_NewPetScene_h

//#include "MicRecordPlayer.h"
#include "resource.h"
#include "cocos2d.h"
#include "Cartoon.h"
#include "GuideBaseLayer.h"
#include "RankingBarLayer.h"
#include "GameLayer.h"
USING_NS_CC;

typedef enum{
    kPetEnterNormal=0,
    kPetEnterGame
}PetEnterMode;

class NewPetScene : public GuideBaseLayer,public GameLayerDelegate
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
    
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void keyBackClicked();
    virtual void keyMenuClicked();
    /**
     *  赠送星星数量
     *
     *  @return
     */
    int xingxingCount();
    
    //菜单回调
    void menuCallBack(CCObject* pSender);
    //点击熊猫回调
    void pressXiaoboCallBack(CCObject *object, const char *pszBoxName);
    
    void zouluEnd();
    
    virtual void gameEnd(GamePlayer winner);
    
    //对话框
    virtual int topHandlerPriority(){return kCCMenuHandlerPriority-1;}
    virtual void dialogCallBack(GuideDialogCMD cmd);
    
//    virtual void micCallBack(MicCallBackCMD cmd);
//    virtual MicDelegateMode getMicDelegateMode(){return m_micDelegateMode;}
private:
    Cartoon* m_xiaobo;//小熊猫
    CCMenuItemToggle* m_modeToggle;
//    MicRecordPlayer* m_micRecordPlayer;
//    MicDelegateMode m_micDelegateMode;
    GuideDialogLayer* m_gudieDialogLayer;
    RankingBarLayer* m_rankingBarLayer;
    
    GameLayer* m_gameLayer;
    CCLayer* m_playLayer;
};

#endif
