//
//  UserRegLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-4-3.
//
//

#include "UserRegLayer.h"
#include "UserLoginLayer.h"

typedef struct _RegStruct
{
    string inputTitle;
	string inputPlaceHolder;
    int inputMaxLength;
	string buttonTitle;
    EditBoxInputMode inputMode;
}RegStruct;

static RegStruct s_regStruts[] = {
	{
		"手机号码",
		"请输入11位手机号码",
		11,
        " 下 一 步 ",
        kEditBoxInputModePhoneNumber
	},
	{
		"验证码",
		"请输入6位数字验证码",
		6,
        " 下 一 步 ",
        kEditBoxInputModeDecimal
	},
	{
		" 密   码 ",
		"请输入6-16位密码，建议字母和数字组合",
		16,
        " 下 一 步 ",
        kEditBoxInputModeSingleLine
	},
	{
		"昵称",
		"取一个个性的昵称吧～",
		10,
        " 确  定 ",
        kEditBoxInputModeSingleLine
	}
};

UserRegLayer* UserRegLayer::create(UserRegStep step){
    UserRegLayer* pReturn = new UserRegLayer();
    pReturn->m_step=step;
    if ( pReturn && pReturn->init() )
    {
        pReturn->autorelease();
        return pReturn;
    }
    CC_SAFE_DELETE(pReturn);
    return NULL;
}

bool UserRegLayer::init(){
    if (!UserBaseLayer::init()) {
        return false;
    }
    
    this->setTitle("用户注册");
    
    CCSize editBoxSize = CCSizeMake(300, 51);
    
    Button9* label1=Button9::create(s_regStruts[m_step].inputTitle);
    label1->setPosition(300, 450);
    this->addChild(label1);
    
    // 手机号码
    m_editBox = CCEditBox::create(editBoxSize, CCScale9Sprite::createWithSpriteFrameName("dialog_input.png"));
    m_editBox->setFontSize(25);
    m_editBox->setFontColor(ccBLACK);
    m_editBox->setPlaceHolder(s_regStruts[m_step].inputTitle.c_str());
    m_editBox->setPlaceholderFontColor(ccGRAY);
    m_editBox->setMaxLength(s_regStruts[m_step].inputMaxLength);
    m_editBox->setInputMode(s_regStruts[m_step].inputMode);
    m_editBox->setReturnType(kKeyboardReturnTypeDone);
    m_editBox->setDelegate(this);
    m_editBox->cocos2d::CCNode::setPosition(550, 450);
    this->addChild(m_editBox);
    
    Button9* button=Button9::create(s_regStruts[m_step].buttonTitle);
    CCMenuItemSprite* buttonItem=CCMenuItemSprite::create(button, button, this, menu_selector(UserRegLayer::menuCallback));
    buttonItem->setPosition(600, 250);
    this->addMenuItem(buttonItem);
    
    return true;
}

void UserRegLayer::menuCallback(CCObject* obj){
    if (!this->isCloseItem(obj)) {
        UserBaseLayer::menuCallback(obj);
        ApiStruct apiStruct;
        CCString* postData;
        switch (m_step) {
            case kUserRegStep1:{
                apiStruct.url="user_phoneRegRequest_feedback";
                const char* mobilePhone=m_editBox->getText();
                postData=CCString::createWithFormat("mobilePhone=%s",mobilePhone);
            }
                break;
            case kUserRegStep2:{
                apiStruct.url="user_phoneRegVerify_activateCode";
                const char* phoneCode=m_editBox->getText();
                postData=CCString::createWithFormat("phoneCode=%s&mobilePhone=%s",phoneCode,S_UD->getStringForKey(UDKEY_USER_REG_MOBILEPHONE).c_str());
            }
                break;
            case kUserRegStep3:{
                apiStruct.url="user_phoneReg_loginUser";
                const char* password=m_editBox->getText();
                postData=CCString::createWithFormat("mobilePhone=%s&password=%s&phoneCode=%s&activateCode=%s&username=%s",S_UD->getStringForKey(UDKEY_USER_REG_MOBILEPHONE).c_str(),password,S_UD->getStringForKey(UDKEY_USER_REG_CODE).c_str(),S_UD->getStringForKey(UDKEY_USER_REG_ACTIVATECODE).c_str(),"");
            }
                break;
            case kUserRegStep4:{
                apiStruct.url="user_editUserBasic_feedback";
                const char* username=m_editBox->getText();
                postData=CCString::createWithFormat("user.username=%s&token=%s",username,S_UD->getStringForKey(UDKEY_USER_TOKEN).c_str());
            }
                break;
        }
        apiStruct.isBlackcat=false;
        apiStruct.target=this;
        apiStruct.sel_response=apiresponse_selector(UserRegLayer::onJsonCompleted);
        apiStruct.data=postData->getCString();
        Api* api=Api::create(apiStruct);
        api->send();
    }else{
        this->replaceDialog(UserLoginLayer::create());
    }
}

void UserRegLayer::enableTouch(){
    UserBaseLayer::enableTouch();
    m_editBox->setTouchPriority(this->getDelegate()->topHandlerPriority()-1);
}

void UserRegLayer::onJsonCompleted(CCDictionary* root){
    switch (m_step) {
        case kUserRegStep1:{
            S_UD->setStringForKey(UDKEY_USER_REG_MOBILEPHONE,m_editBox->getText());
            S_UD->flush();
        }
            break;
        case kUserRegStep2:{
            const CCString* activateCode=root->valueForKey("activateCode");
            S_UD->setStringForKey(UDKEY_USER_REG_ACTIVATECODE, activateCode->getCString());
            S_UD->setStringForKey(UDKEY_USER_REG_CODE,m_editBox->getText());
            S_UD->flush();
        }
            break;
        case kUserRegStep3:{
            S_TT->makeText("注册成功！取一个好听的名字吧～");
            CCDictionary* loginUser=(CCDictionary*)root->objectForKey("loginUser");
            CCDictionary* user=(CCDictionary*)loginUser->objectForKey("user");
            const CCString* username=user->valueForKey("username");
            const CCString* token=loginUser->valueForKey("token");
            S_UD->setStringForKey(UDKEY_USER_TOKEN, token->getCString());
            S_UD->setStringForKey(UDKEY_USER_USERNAME, username->getCString());
            S_UD->flush();
        }
            break;
        case kUserRegStep4:{
            S_UD->setStringForKey(UDKEY_USER_USERNAME, m_editBox->getText());
            this->removeFromParent();
            CCMessageBox("注册完成", "");
        }
            break;
            
        default:
            break;
    }
    
    int step=m_step;
    if (step<kUserRegStep4) {
        this->replaceDialog(UserRegLayer::create((UserRegStep)(++step)));
    }
}