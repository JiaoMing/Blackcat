//
//  ChildLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-6-20.
//
//

#include "ChildLayer.h"
#include "SettingsLayer.h"
#include "UserLoginLayer.h"

enum{
    kChildName=0,
    kAge,
    kGender,
    kCity,
    kKindergarten,
    kTagSetting,
    kTagMenu
};

typedef struct _InputStruct
{
    int tag;
    string label;
    string placeHolder;
    int MaxLength;
    EditBoxInputMode mode;
    KeyboardReturnType returnType;
    float top;
}InputStruct;

// 在这里配置每个场景要加载的资源
static InputStruct inputs[] = {
    {
        kChildName,
        "姓名",
        "姓名",
        16,
        kEditBoxInputModeAny,
        kKeyboardReturnTypeDone,
        0
    },
    {
        kAge,
        "年龄",
        "年龄",
        2,
        kEditBoxInputModeNumeric,
        kKeyboardReturnTypeDone,
        50
    },
    {
        kGender,
        "性别",
        "性别",
        16,
        kEditBoxInputModeAny,
        kKeyboardReturnTypeDone,
        100
    },
    {
        kCity,
        "城市",
        "城市",
        16,
        kEditBoxInputModeAny,
        kKeyboardReturnTypeDone,
        150
    },
    {
        kKindergarten,
        "幼儿园",
        "幼儿园",
        16,
        kEditBoxInputModeAny,
        kKeyboardReturnTypeDone,
        200
    }
};

bool ChildLayer::init(){
    if (!UserBaseLayer::init()) {
        return false;
    }
    
    this->setTitle("儿童信息");
    
    CCPoint labelMiddlePont=ccp(190,330);
    CCPoint inputLeftPont=ccp(200,330);
    for (int i=kChildName; i<=kKindergarten; i++) {
        CCString* str=CCString::createWithFormat("%s：",inputs[i].label.c_str());
        CCLabelTTF* childLabel=CCLabelTTF::create(str->getCString(), "", 25);
        childLabel->setPosition(ccpSub(labelMiddlePont, ccp(0,inputs[i].top)));
        childLabel->setColor(ccc3(0, 0, 0));
        childLabel->setAnchorPoint(ccp(1, 0.5));
        m_contentLayer->addChild(childLabel);
        
        CCControl* control;
        if (i==kGender) {
            CCLabelTTF* label1=CCLabelTTF::create("男", "", 16);
            label1->setColor(ccc3(0, 0, 0));
            CCLabelTTF* label2=CCLabelTTF::create("女", "", 16);
            label2->setColor(ccc3(0, 0, 0));
            
            CCControlSwitch *switchControl = CCControlSwitch::create
            (
             CCSprite::create("switch-mask.png"),
             CCSprite::create("switch-on.png"),
             CCSprite::create("switch-off.png"),
             CCSprite::create("switch-thumb.png"),label1,label2
             );
            control=switchControl;
            
        }else{
            CCEditBox* editbox = CCEditBox::create(CCSizeMake(350, 40), CCScale9Sprite::createWithSpriteFrameName("dialog_input.png"));
            editbox->setFontSize(20);
            editbox->setFontColor(ccBLACK);
            editbox->setPlaceHolder(inputs[i].placeHolder.c_str());
            editbox->setPlaceholderFontColor(ccGRAY);
            editbox->setMaxLength(inputs[i].MaxLength);
            editbox->setInputMode(inputs[i].mode);
            editbox->setReturnType(inputs[i].returnType);
            editbox->setDelegate(this);
            control=editbox;
        }
        control->setAnchorPoint(ccp(0, 0.5));
        control->setPosition(ccpSub(inputLeftPont, ccp(0,inputs[i].top)));
        control->setTag(inputs[i].tag);
        m_contentLayer->addChild(control);
    }
    
    CCMenu* menu=CCMenu::create();
    menu->setPosition(CCPointZero);
    m_contentLayer->addChild(menu);
    Button9* setting=Button9::create("设  置",CCSizeMake(120,40),20);
    CCMenuItemSprite* settingItem=CCMenuItemSprite::create(setting, setting, this, menu_selector(ChildLayer::menuCallback));
    settingItem->setAnchorPoint(ccp(0, 0.5));
    settingItem->setPosition(ccp(200,75));
    settingItem->setTag(kTagSetting);
    menu->addChild(settingItem);
    menu->setTag(kTagMenu);
    
    return true;
}

void ChildLayer::enableTouch(){
    UserBaseLayer::enableTouch();
    for (int i=kChildName; i<=kKindergarten; i++) {
        CCControl* control=( CCControl*) m_contentLayer->getChildByTag(i);
        control->setTouchPriority(this->getDelegate()->topHandlerPriority()-1);
    }
    CCMenu* menu=( CCMenu*) m_contentLayer->getChildByTag(kTagMenu);
    menu->setTouchPriority(this->getDelegate()->topHandlerPriority()-1);
}

void ChildLayer::onEnter(){
    UserBaseLayer::onEnter();
    
    string userToken=S_UD->getStringForKey(UDKEY_USER_TOKEN);
    if (userToken=="") {
        this->replaceDialog(UserLoginLayer::create());
    }else{
        CCString* postData=CCString::createWithFormat("token=%s",userToken.c_str());
        
        ApiStruct apiStruct;
        apiStruct.url="user_userDetail_user";
        apiStruct.target=this;
        apiStruct.isBlackcat=false;
        apiStruct.sel_response=apiresponse_selector(ChildLayer::onJsonCompleted);
        apiStruct.data=postData->getCString();
        Api* api=Api::create(apiStruct);
        api->send();
        m_step=kApiStepInit;
    }
}

void ChildLayer::menuCallback(cocos2d::CCObject *obj){
    
    CCMenuItem* node=(CCMenuItem*)obj;
    if(!this->isCloseItem(obj)){
        UserBaseLayer::menuCallback(obj);
        
        if (node->getTag()==kTagSetting) {
            CCEditBox* nameBox=( CCEditBox*) m_contentLayer->getChildByTag(kChildName);
            const char* name=nameBox->getText();
            
            CCEditBox* ageBox=( CCEditBox*) m_contentLayer->getChildByTag(kAge);
            CCString* age=CCString::createWithFormat("%s",ageBox->getText());
            cc_timeval time=TimeUtils::millisecondNow();
            struct tm* locationTime= TimeUtils::getLocaltime(time.tv_sec);
            int thisYear=locationTime->tm_year+1900;
            int year=thisYear-age->intValue();
            
            CCControlSwitch* genderSwitch=( CCControlSwitch*) m_contentLayer->getChildByTag(kGender);
            int gender=genderSwitch->isOn();
            
            CCEditBox* cityBox=( CCEditBox*) m_contentLayer->getChildByTag(kCity);
            const char* city=cityBox->getText();
            
            CCEditBox* kindergartenBox=( CCEditBox*) m_contentLayer->getChildByTag(kKindergarten);
            const char* kindergarten=kindergartenBox->getText();
            
            if (age->intValue()>20) {
                CCString* str=CCString::createWithFormat("你确定宝宝已经%d岁啦？",age->intValue());
                S_TT->makeText(str->getCString());
            }
            
            string userToken=S_UD->getStringForKey(UDKEY_USER_TOKEN);
            CCString* postData=CCString::createWithFormat("token=%s&user.userProfile.babyRealname=%s&user.userProfile.babyBirthyear=%d&user.userProfile.babyGender=%d&user.userProfile.city=%s&user.userProfile.kindergarten=%s",userToken.c_str(),name,year,gender,city,kindergarten);
            
            ApiStruct apiStruct;
            apiStruct.url="user_editUserProfile_feedback";
            apiStruct.target=this;
            apiStruct.isBlackcat=false;
            apiStruct.sel_response=apiresponse_selector(ChildLayer::onJsonCompleted);
            apiStruct.data=postData->getCString();
            Api* api=Api::create(apiStruct);
            api->send();
            m_step=kApiStepUpdate;
        }
    }else{
        if (isFromSetting) {
            this->replaceDialog(SettingsLayer::create());
        }else{
            UserBaseLayer::menuCallback(obj);
        }
    }
    
}

void ChildLayer::onJsonCompleted(CCDictionary* root){
    switch (m_step) {
        case kApiStepInit:{
            CCDictionary* user=(CCDictionary*)root->objectForKey("user");
            CCDictionary* userProfile=(CCDictionary*)user->objectForKey("userProfile");
            if (userProfile!=NULL) {
                CCEditBox* chilidName=( CCEditBox*) m_contentLayer->getChildByTag(kChildName);
                chilidName->setText(userProfile->valueForKey("babyRealname")->getCString());
                
                const CCString* babyBirthyear=userProfile->valueForKey("babyBirthyear");
                int year=babyBirthyear->intValue();
                cc_timeval time=TimeUtils::millisecondNow();
                struct tm* locationTime= TimeUtils::getLocaltime(time.tv_sec);
                int thisYear=locationTime->tm_year+1900;
                CCString* str=CCString::createWithFormat("%d",thisYear-year);
                CCEditBox* age=( CCEditBox*) m_contentLayer->getChildByTag(kAge);
                age->setText(str->getCString());
                
                int gender=userProfile->valueForKey("babyGender")->intValue();
                CCControlSwitch* genderSwitch=( CCControlSwitch*) m_contentLayer->getChildByTag(kGender);
                genderSwitch->setOn(gender);
                
                CCEditBox* city=( CCEditBox*) m_contentLayer->getChildByTag(kCity);
                city->setText(userProfile->valueForKey("city")->getCString());
                
                CCEditBox* kindergartenBox=( CCEditBox*) m_contentLayer->getChildByTag(kKindergarten);
                kindergartenBox->setText(userProfile->valueForKey("kindergarten")->getCString());
                
            }
        }
            
            break;
        case kApiStepUpdate:{
            S_TT->makeText("更新成功");
        }
    }
    
}