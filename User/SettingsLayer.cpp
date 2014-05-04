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
    
    this->setTitle("设置");
    
    CCLabelTTF* label1=CCLabelTTF::create("账  号:", "", 25);
    label1->setPosition(ccp(280, 480));
    label1->setColor(ccc3(0, 0, 0));
    this->addChild(label1);
    
    string token=S_UD->getStringForKey("USER_TOKEN", "");
    string username=S_UD->getStringForKey("USER_USERNAME", "");
    if (username=="") {
        Button9* label=Button9::create("登  陆");
        CCMenuItemSprite* buttonItem=CCMenuItemSprite::create(label, label, this, menu_selector(SettingsLayer::menuCallback));
        buttonItem->setPosition(350, 480);
        buttonItem->setAnchorPoint(ccp(0,0.5));
        buttonItem->setTag(kTagLoginin);
        this->addMenuItem(buttonItem);
    }else{
        Button9* label=Button9::create(username);
        CCMenuItemSprite* buttonItem=CCMenuItemSprite::create(label, label, this, menu_selector(SettingsLayer::menuCallback));
        buttonItem->setPosition(350, 480);
        buttonItem->setTag(kTagLoginout);
        buttonItem->setAnchorPoint(ccp(0,0.5));
        this->addMenuItem(buttonItem);
    }
    
    CCLabelTTF* label2=CCLabelTTF::create("儿  童:", "", 25);
    label2->setPosition(ccp(280, 410));
    label2->setColor(ccc3(0, 0, 0));
    this->addChild(label2);
    Button9* label=Button9::create("设  置");
    CCMenuItemSprite* buttonItem=CCMenuItemSprite::create(label, label, this, menu_selector(SettingsLayer::menuCallback));
    buttonItem->setPosition(350, 410);
    buttonItem->setAnchorPoint(ccp(0,0.5));
    buttonItem->setTag(kTagChild);
    this->addMenuItem(buttonItem);
    
    CCLabelTTF* weixinLabel=CCLabelTTF::create("微  信:", "", 25);
    weixinLabel->setPosition(ccp(280, 340));
    weixinLabel->setColor(ccc3(0, 0, 0));
    this->addChild(weixinLabel);
    Button9* weixin=Button9::create("绑  定");
    CCMenuItemSprite* weixinItem=CCMenuItemSprite::create(weixin, weixin, this, menu_selector(SettingsLayer::menuCallback));
    weixinItem->setPosition(350, 340);
    weixinItem->setAnchorPoint(ccp(0,0.5));
    weixinItem->setTag(kTagWeixin);
    this->addMenuItem(weixinItem);
    
    //音量开关
    CCLabelTTF* volumeLabel=CCLabelTTF::create("音  量:", "", 25);
    volumeLabel->setPosition(ccp(280, 250));
    volumeLabel->setColor(ccc3(0, 0, 0));
    this->addChild(volumeLabel);
    CCControlSlider* slider = CCControlSlider::create(CCSprite::createWithSpriteFrameName("volume_3.png"), CCSprite::createWithSpriteFrameName("volume_2.png"), CCSprite::createWithSpriteFrameName("volume_1.png"));
    slider->setMinimumValue(0.1f); // Sets the min value of range
    slider->setMaximumValue(1.0f); // Sets the max value of range
    slider->setPosition(ccp(350,250));
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(SettingsLayer::volumeValueChanged), CCControlEventValueChanged);
    slider->setTag(kTagVolume);
    slider->setAnchorPoint(ccp(0,0.5));
    this->addChild(slider);
    
    float volume=S_AE->getBackgroundMusicVolume();
    slider->setValue(volume);
    
    CCLabelTTF* resetLabel=CCLabelTTF::create(" 学习进度: ", "", 25);
    resetLabel->setPosition(ccp(550, 480));
    resetLabel->setColor(ccc3(0, 0, 0));
    this->addChild(resetLabel);
    Button9* reset=Button9::create("重  置");
    CCMenuItemSprite* resetItem=CCMenuItemSprite::create(reset, reset, this, menu_selector(SettingsLayer::menuCallback));
    resetItem->setPosition(640, 480);
    resetItem->setAnchorPoint(ccp(0,0.5));
    resetItem->setTag(kTagClear);
    this->addMenuItem(resetItem);
    
    CCLabelTTF* timeLabel=CCLabelTTF::create(" 时间控制: ", "", 25);
    timeLabel->setPosition(ccp(550, 410));
    timeLabel->setColor(ccc3(0, 0, 0));
    this->addChild(timeLabel);
    Button9* time=Button9::create("设  置");
    CCMenuItemSprite* timeItem=CCMenuItemSprite::create(time, time, this, menu_selector(SettingsLayer::menuCallback));
    timeItem->setPosition(640, 410);
    timeItem->setAnchorPoint(ccp(0,0.5));
    timeItem->setTag(kTagTime);
    this->addMenuItem(timeItem);
    
    CCLabelTTF* dhLabel=CCLabelTTF::create("片头动画: ", "", 25);
    dhLabel->setPosition(ccp(550, 340));
    dhLabel->setColor(ccc3(0, 0, 0));
    this->addChild(dhLabel);
    CCSprite* fk01 = CCSprite::createWithSpriteFrameName("fangkuang01.png");
    CCMenuItem* fk01Item1=CCMenuItemSprite::create(fk01, fk01);
    CCSprite* fk02 = CCSprite::createWithSpriteFrameName("fangkuang02.png");
    CCMenuItem* fk01Item2=CCMenuItemSprite::create(fk02, fk02);
    CCMenuItemToggle* toggle=CCMenuItemToggle::createWithTarget(this, menu_selector(SettingsLayer::menuCallback), fk01Item1,fk01Item2,NULL);
    toggle->setPosition(ccp(640, 340));
    toggle->setAnchorPoint(ccp(0,0.5));
    this->addMenuItem(toggle);
    
    
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
                HomeScene* home=(HomeScene*)this->getParent();
                UserLoginLayer* layer=UserLoginLayer::create();
                layer->setDelegate(home);
                home->addChild(layer,INT_MAX);
                this->removeFromParent();
            }
                break;
            case kTagLoginout:{
                S_UD->setStringForKey(UDKEY_USER_TOKEN, "");
                S_UD->setStringForKey(UDKEY_USER_USERNAME, "");
                node->setTag(kTagLoginin);
                Button9* button=(Button9*)node->getChildren()->objectAtIndex(0);
                button->setString("登  陆");
            }
                break;
            case kTagMenuClose:{
                this->replaceDialog(ParentLayer::create());
            }
                break;
            case kTagClear:{
                S_DM->executeSql("update hanzi set writeCount=0,lastAnswer=0,isCollected=0;update ciyu set isCollected=0,viewCount=0;update hanzi set isCollected=1 where zi in('生','火','男','木','白','菜','手','大','下','一','十','百','天','日','花');update ciyu set isCollected=1 where ci in('哭笑','杨树','飞机','书包','大象','啄木鸟','黄瓜','苹果','洗手','爸爸','妈妈','红色','铅笔','水盆','大海','老师');");
                S_UD->setIntegerForKey(COLLECT_HANZI_COUNT, 15);
                S_UD->setIntegerForKey(COLLECT_TUPIAN_COUNT, 15);
                S_UD->setIntegerForKey(LAST_KECHENG_ID, 0);
                S_UD->setIntegerForKey(COLLECT_XINGXING_COUNT, 0);
                S_UD->setIntegerForKey(OVER_XINGXING_COUNT, 0);
                
                static_uploadRanking();
                
                
                this->replaceDialog(ParentLayer::create());
                S_TT->makeText("数据已重置。");
                
            }
                break;
            case kTagTime:
            case kTagChild:
            case kTagWeixin:{
                S_TT->makeText("敬请期待。");
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

