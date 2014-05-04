//
//  UserRegLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-4-3.
//
//

#ifndef __Blackcat__UserRegLayer__
#define __Blackcat__UserRegLayer__

#include "UserBaseLayer.h"

typedef enum{
    kUserRegStep1=0,
    kUserRegStep2,
    kUserRegStep3,
    kUserRegStep4
}UserRegStep;

class UserRegLayer : public UserBaseLayer
{
public:
    static UserRegLayer* create(UserRegStep step);
    virtual bool init();
    virtual void menuCallback(CCObject* obj);
    
    virtual void enableTouch();
    virtual void onJsonCompleted(CCDictionary* root);
private:
    CCEditBox* m_editBox;
    UserRegStep m_step;
};


#endif /* defined(__Blackcat__UserRegLayer__) */
