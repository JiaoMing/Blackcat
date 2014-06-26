//
//  UserLoginLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-4-3.
//
//

#ifndef __Blackcat__UserLoginLayer__
#define __Blackcat__UserLoginLayer__

#include "UserBaseLayer.h"

class UserLoginLayer : public UserBaseLayer
{
public:
    UserLoginLayer(){};
    virtual bool init();
    CREATE_FUNC(UserLoginLayer);
    virtual void menuCallback(CCObject* obj);
    
    virtual void enableTouch();
    virtual void onJsonCompleted(CCDictionary* root);
private:
    CCEditBox* m_editPhone;
    CCEditBox* m_editPswd;
};

#endif /* defined(__Blackcat__UserLoginLayer__) */
