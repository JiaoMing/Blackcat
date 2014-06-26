//
//  WeixinLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-6-22.
//
//

#include "WeixinLayer.h"
#include "SettingsLayer.h"

bool WeixinLayer::init(){
    if (!DialogLayer::init()) {
        return false;
    }
    
    CCLabelTTF* text=CCLabelTTF::create("请关注易迪乐园的微信公众号，在菜单中选择“关联易迪APP”，输入易迪乐园的账号和密码，就可以完成绑定，通过微信随时了解小朋友的学习情况啦。\n\n    请使用微信扫描以下二维码关注易迪乐园：", "KaiTi.ttf", 25,CCSizeMake(560, 245),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
    text->setColor(ccc3(58,37,4));
    text->setPosition(S_RM->getPositionWithName("dialog_text"));
    this->addChild(text);
    
    CCSprite* qr=CCSprite::create("QR.png");
    qr->setPosition(S_RM->getPositionWithName("dialog_qr"));
    this->addChild(qr);
    
    
    return true;
}

void WeixinLayer::menuCallback(cocos2d::CCObject *obj){
    if(!this->isCloseItem(obj)){
        DialogLayer::menuCallback(obj);
    }else{
        this->replaceDialog(SettingsLayer::create());
    }
}