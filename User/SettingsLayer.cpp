//
//  SettingsLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-4-3.
//
//

#include "SettingsLayer.h"
#include "resource.h"
#include "Button9.h"
#include "UserLoginLayer.h"
#include "HomeScene.h"
#include "ParentLayer.h"
#include "KechengController.h"
#include "ChildLayer.h"
#include "WeixinLayer.h"

enum
{
    kTagLoginin=0,
    kTagLoginout,
    kTagWeixin,
    kTagChild,
    kTagClear,
    kTagTime,
    kTagMenu,
    kTagMenuClose,
    kTagVolume
};

bool SettingsLayer::init(){
    if (!DialogLayer::init()) {
        return false;
    }
    
    this->setTitle("设  置");
    
    string token=S_UD->getStringForKey("USER_TOKEN", "");
    string username=S_UD->getStringForKey("USER_USERNAME", "");
    CCLabelTTF* acountLabel=CCLabelTTF::create("账  号:", "", 20);
    acountLabel->setPosition(S_RM->getPositionWithName("setting_acountLabel"));
    acountLabel->setColor(ccc3(0, 0, 0));
    this->addChild(acountLabel);
    int loginTag=kTagLoginin;
    if (username=="") {
        username="登  陆";
        loginTag=kTagLoginin;
    }else{
        loginTag=kTagLoginout;
    }
    Button9* userLabel=Button9::create(username,CCSizeMake(400, 50),20);
    CCMenuItemSprite* userItem=CCMenuItemSprite::create(userLabel, userLabel, this, menu_selector(SettingsLayer::menuCallback));
    userItem->setPosition(S_RM->getPositionWithName("setting_acountItem"));
    userItem->setAnchorPoint(ccp(0,0.5));
    userItem->setTag(loginTag);
    this->addMenuItem(userItem);
    
    CCLabelTTF* childLabel=CCLabelTTF::create("儿  童:", "", 20);
    childLabel->setPosition(S_RM->getPositionWithName("setting_childLabel"));
    childLabel->setColor(ccc3(0, 0, 0));
    this->addChild(childLabel);
    Button9* child=Button9::create("设  置",CCSizeMake(400, 50),20);
    CCMenuItemSprite* childItem=CCMenuItemSprite::create(child, child, this, menu_selector(SettingsLayer::menuCallback));
    childItem->setPosition(S_RM->getPositionWithName("setting_childItem"));
    childItem->setAnchorPoint(ccp(0,0.5));
    childItem->setTag(kTagChild);
    this->addMenuItem(childItem);
    
    CCLabelTTF* resetLabel=CCLabelTTF::create("学习进度: ", "", 20);
    resetLabel->setPosition(S_RM->getPositionWithName("setting_resetLabel"));
    resetLabel->setColor(ccc3(0, 0, 0));
    this->addChild(resetLabel);
    Button9* reset=Button9::create("重  置",CCSizeMake(150, 50),20);
    CCMenuItemSprite* resetItem=CCMenuItemSprite::create(reset, reset, this, menu_selector(SettingsLayer::menuCallback));
    resetItem->setPosition(S_RM->getPositionWithName("setting_resetItem"));
    resetItem->setAnchorPoint(ccp(0,0.5));
    resetItem->setTag(kTagClear);
    this->addMenuItem(resetItem);
    
    CCLabelTTF* timeLabel=CCLabelTTF::create("时间控制: ", "", 20);
    timeLabel->setPosition(S_RM->getPositionWithName("setting_timeLabel"));
    timeLabel->setColor(ccc3(0, 0, 0));
    this->addChild(timeLabel);
    Button9* time=Button9::create("设  置",CCSizeMake(150, 50),20);
    CCMenuItemSprite* timeItem=CCMenuItemSprite::create(time, time, this, menu_selector(SettingsLayer::menuCallback));
    timeItem->setPosition(S_RM->getPositionWithName("setting_timeItem"));
    timeItem->setAnchorPoint(ccp(0,0.5));
    timeItem->setTag(kTagTime);
    this->addMenuItem(timeItem);
    
    CCLabelTTF* weixinLabel=CCLabelTTF::create("微信绑定:", "", 20);
    weixinLabel->setPosition(S_RM->getPositionWithName("setting_weixinLabel"));
    weixinLabel->setColor(ccc3(0, 0, 0));
    this->addChild(weixinLabel);
    Button9* weixin=Button9::create("绑  定",CCSizeMake(150, 50),20);
    CCMenuItemSprite* weixinItem=CCMenuItemSprite::create(weixin, weixin, this, menu_selector(SettingsLayer::menuCallback));
    weixinItem->setPosition(S_RM->getPositionWithName("setting_weixinItem"));
    weixinItem->setAnchorPoint(ccp(0,0.5));
    weixinItem->setTag(kTagWeixin);
    this->addMenuItem(weixinItem);
    
    CCLabelTTF* dhLabel=CCLabelTTF::create("片头动画: ", "", 20);
    dhLabel->setPosition(S_RM->getPositionWithName("setting_dhLabel"));
    dhLabel->setColor(ccc3(0, 0, 0));
    this->addChild(dhLabel);
    CCSprite* fk01 = CCSprite::createWithSpriteFrameName("fangkuang01.png");
    CCMenuItem* fk01Item1=CCMenuItemSprite::create(fk01, fk01);
    CCSprite* fk02 = CCSprite::createWithSpriteFrameName("fangkuang02.png");
    CCMenuItem* fk01Item2=CCMenuItemSprite::create(fk02, fk02);
    CCMenuItemToggle* toggle=CCMenuItemToggle::createWithTarget(this, menu_selector(SettingsLayer::menuCallback), fk01Item1,fk01Item2,NULL);
    toggle->setPosition(S_RM->getPositionWithName("setting_dhToggle"));
    toggle->setAnchorPoint(ccp(0,0.5));
    this->addMenuItem(toggle);
    
    //音量开关
    CCLabelTTF* volumeLabel=CCLabelTTF::create("音  量:", "", 20);
    volumeLabel->setPosition(S_RM->getPositionWithName("setting_volumeLabel"));
    volumeLabel->setColor(ccc3(0, 0, 0));
    this->addChild(volumeLabel);
    CCControlSlider* slider = CCControlSlider::create(CCSprite::createWithSpriteFrameName("volume_3.png"), CCSprite::createWithSpriteFrameName("volume_2.png"), CCSprite::createWithSpriteFrameName("volume_1.png"));
    slider->setMinimumValue(0.1f); // Sets the min value of range
    slider->setMaximumValue(1.0f); // Sets the max value of range
    slider->setPosition(S_RM->getPositionWithName("setting_volumeSlider"));
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(SettingsLayer::volumeValueChanged), CCControlEventValueChanged);
    slider->setTag(kTagVolume);
    slider->setAnchorPoint(ccp(0,0.5));
    this->addChild(slider);
    
    float volume=S_AE->getBackgroundMusicVolume();
    slider->setValue(volume);
    
    return true;
}

void SettingsLayer::enableTouch(){
    DialogLayer::enableTouch();
    CCControlSlider* slider=(CCControlSlider*)this->getChildByTag(kTagVolume);
    slider->setTouchPriority(this->getDelegate()->topHandlerPriority()-1);
}

void SettingsLayer::menuCallback(CCObject *obj){
    CCMenuItem* node=(CCMenuItem*)obj;
    if (!this->isCloseItem(node)) {
        DialogLayer::menuCallback(obj);
        switch (node->getTag()) {
            case kTagLoginin:{
                UserLoginLayer* layer=UserLoginLayer::create();
                layer->setIsFromSetting(true);
                this->replaceDialog(layer);
            }
                break;
            case kTagLoginout:{
                S_UD->setStringForKey(UDKEY_USER_TOKEN, "");
                S_UD->setStringForKey(UDKEY_USER_USERNAME, "");
                node->setTag(kTagLoginin);
                Button9* button=(Button9*)node->getChildren()->objectAtIndex(0);
                button->setString("登  陆",CCSizeMake(400, 50));
                
                S_LM->getDelegate()->fresh();
            }
                break;
            case kTagMenuClose:{
                this->replaceDialog(ParentLayer::create());
            }
                break;
            case kTagClear:{
                S_DM->executeSql("update hanzi set writeCount=0,lastAnswer=0,isCollected=0,isReward=0;update ciyu set isCollected=0,viewCount=0;update hanzi set isCollected=1 where zi in('生','火','男','木','白','菜','手','大','下','一','十','百','天','日','花');update ciyu set isCollected=1 where ci in('哭笑','杨树','飞机','书包','大象','啄木鸟','黄瓜','苹果','洗手','爸爸','妈妈','红色','铅笔','水盆','大海','老师');");
                S_UD->setIntegerForKey(COLLECT_HANZI_COUNT, 15);
                S_UD->setIntegerForKey(COLLECT_TUPIAN_COUNT, 15);
                S_UD->setIntegerForKey(LAST_KECHENG_ID, 0);
                S_UD->setIntegerForKey(LAST_RENWU_DAY_COUNT, 0);
                S_UD->setIntegerForKey(COLLECT_XINGXING_COUNT, 0);
                S_UD->setIntegerForKey(OVER_XINGXING_COUNT, 0);
                
                S_UD->setIntegerForKey(UDKEY_ACHIEVE_XZDW, 0);
                S_UD->setIntegerForKey(UDKEY_ACHIEVE_ZKSCJ, 0);
                S_UD->setIntegerForKey(UDKEY_ACHIEVE_TKSCJ, 0);
                S_UD->setIntegerForKey(UDKEY_ACHIEVE_CGYS, 0);
                S_UD->setIntegerForKey(UDKEY_ACHIEVE_CSJJ, 0);
                
                
                static_uploadRanking();
                
                KechengController::purgeInstance();
                
                this->replaceDialog(ParentLayer::create());
                S_TT->makeText("数据已重置。");
                
            }
                break;
            case kTagTime:
                S_TT->makeText("测试版，暂未实现。");
                break;
            case kTagChild:{
                if (S_UD->getStringForKey(UDKEY_USER_TOKEN)=="") {
                    UserLoginLayer* layer=UserLoginLayer::create();
                    layer->setIsFromSetting(true);
                    this->replaceDialog(layer);
                }else{
                    ChildLayer* layer=ChildLayer::create();
                    layer->setIsFromSetting(true);
                    this->replaceDialog(layer);
                }
            }
                break;
            case kTagWeixin:{
                WeixinLayer* layer=WeixinLayer::create();
                this->replaceDialog(layer);
            }
                break;
            default:
                break;
        }
    }else{
        this->replaceDialog(ParentLayer::create());
    }
}

void SettingsLayer::volumeValueChanged(CCObject *sender, CCControlEvent controlEvent)
{
    CCControlSlider* pSlider = (CCControlSlider*)sender;
    S_AE->setBackgroundMusicVolume(pSlider->getValue());
}

