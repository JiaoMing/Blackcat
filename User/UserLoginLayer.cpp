//
//  UserLoginLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-4-3.
//
//

#include "UserLoginLayer.h"
#include "UserRegLayer.h"
#include "SettingsLayer.h"
#include "ParentLayer.h"
#include "DownloadManager.h"

enum
{
    kTagLogin = 0,
    kTagReg
};

bool UserLoginLayer::init(){
    if (!UserBaseLayer::init()) {
        return false;
    }
    
    this->setTitle("用户登陆");
    
    CCSize editBoxSize = CCSizeMake(300, 51);
    
    Button9* label1=Button9::create("手机号码");
    label1->setPosition(S_RM->getPositionWithName("user_phoneLabel"));
    this->addChild(label1);
    
    // 手机号码
    m_editPhone = CCEditBox::create(editBoxSize, CCScale9Sprite::createWithSpriteFrameName("dialog_input.png"));
    m_editPhone->setFontSize(25);
    m_editPhone->setFontColor(ccBLACK);
    m_editPhone->setPlaceHolder("手机号码");
    m_editPhone->setPlaceholderFontColor(ccGRAY);
    m_editPhone->setMaxLength(11);
    m_editPhone->setInputMode(kEditBoxInputModePhoneNumber);
    m_editPhone->setReturnType(kKeyboardReturnTypeDone);
    m_editPhone->setDelegate(this);
    m_editPhone->cocos2d::CCNode::setPosition(S_RM->getPositionWithName("user_phone"));
    this->addChild(m_editPhone);
    
    Button9* label2=Button9::create("密    码 ");
    label2->setPosition(S_RM->getPositionWithName("user_pswdLabel"));
    this->addChild(label2);
    
    // 密码
    m_editPswd = CCEditBox::create(editBoxSize, CCScale9Sprite::createWithSpriteFrameName("dialog_input.png"));
    m_editPswd->setFontSize(25);
    m_editPswd->setFontColor(ccBLACK);
    m_editPswd->setPlaceHolder("密码");
    m_editPswd->setPlaceholderFontColor(ccGRAY);
    m_editPswd->setMaxLength(16);
    m_editPswd->setInputFlag(kEditBoxInputFlagPassword);
    m_editPswd->setReturnType(kKeyboardReturnTypeDone);
    m_editPswd->setDelegate(this);
    m_editPswd->cocos2d::CCNode::setPosition(S_RM->getPositionWithName("user_pswd"));
    this->addChild(m_editPswd);
    
    Button9* reg=Button9::create("注  册");
    CCMenuItemSprite* regItem=CCMenuItemSprite::create(reg, reg, this, menu_selector(UserLoginLayer::menuCallback));
    regItem->setPosition(S_RM->getPositionWithName("user_login_regButton"));
    regItem->setTag(kTagReg);
    this->addMenuItem(regItem);
    
    Button9* login=Button9::create("登  陆");
    CCMenuItemSprite* loginItem=CCMenuItemSprite::create(login, login, this, menu_selector(UserLoginLayer::menuCallback));
    loginItem->setPosition(S_RM->getPositionWithName("user_login_loginButton"));
    loginItem->setTag(kTagLogin);
    this->addMenuItem(loginItem);
    
    return true;
}


void UserLoginLayer::enableTouch(){
    UserBaseLayer::enableTouch();
    m_editPhone->setTouchPriority(this->getDelegate()->topHandlerPriority()-1);
    m_editPswd->setTouchPriority(this->getDelegate()->topHandlerPriority()-1);
}

void UserLoginLayer::menuCallback(CCObject *obj){
    CCMenuItem* node=(CCMenuItem*)obj;
    if(!this->isCloseItem(obj)){
        UserBaseLayer::menuCallback(obj);
        switch (node->getTag()) {
            case kTagLogin:{
                const char* mobilePhone=m_editPhone->getText();
                const char* password=m_editPswd->getText();
                CCString* postData=CCString::createWithFormat("loginname=%s&password=%s",mobilePhone,password);
                ApiStruct apiStruct;
                apiStruct.url="user_login_loginUser";
                apiStruct.isBlackcat=false;
                apiStruct.target=this;
                apiStruct.sel_response=apiresponse_selector(UserLoginLayer::onJsonCompleted);
                apiStruct.data=postData->getCString();
                Api* api=Api::create(apiStruct);
                api->send();
            }
                break;
            case kTagReg:
                this->replaceDialog(UserRegLayer::create(kUserRegStep1));
                break;
                
            default:
                break;
        }
    }else{
        if (isFromSetting) {
            this->replaceDialog(SettingsLayer::create());
        }else{
            UserBaseLayer::menuCallback(obj);
        }
    }
}

void UserLoginLayer::onJsonCompleted(CCDictionary* root){
    CCDictionary* loginUser=(CCDictionary*)root->objectForKey("loginUser");
    CCDictionary* user=(CCDictionary*)loginUser->objectForKey("user");
    const CCString* username=user->valueForKey("username");
    const CCString* token=loginUser->valueForKey("token");
    const CCString* avatarKey=user->valueForKey("avatar");
    const CCString* uid=user->valueForKey("uid");
    S_UD->setStringForKey(UDKEY_USER_TOKEN, token->getCString());
    S_UD->setStringForKey(UDKEY_USER_USERNAME, username->getCString());
    S_UD->setIntegerForKey(UDKEY_USER_UID, uid->intValue());
    
    CCString* fd=CCString::createWithFormat("欢迎您，%s",username->getCString());
    S_TT->makeText(fd->getCString());
    this->popDialog();
    
    UserBarLayer* userBarLayer=S_LM->getDelegate();
    if (userBarLayer!=NULL) {
        userBarLayer->setVisible(true);
        userBarLayer->fresh();
        
        string str=S_UD->getStringForKey(UDKEY_USER_AVATAR_KEY);
        if (str!=avatarKey->getCString()) {
            DownloadObject* dlObj=DownloadObject::create();
            CCString* avatar=CCString::createWithFormat("http://www.kidsedu.com/userdata/%d/avatar.jpg",user->valueForKey("uid")->intValue());
            dlObj->setUrl(avatar->getCString());
            dlObj->setExpectFilename("avatar.jpg");
            dlObj->setFileTotalSize(1000000000);
            dlObj->setTarget(userBarLayer);
            dlObj->setSelector(callfuncO_selector(UserBarLayer::downloadAvatar));
            DownloadManager::sharedDownloadManager()->download(dlObj);
            S_UD->setStringForKey(UDKEY_USER_AVATAR_KEY, avatarKey->getCString());
        }
    }
    S_UD->flush();
}