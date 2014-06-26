//
//  ChildLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-6-20.
//
//

#ifndef __Blackcat__ChildLayer__
#define __Blackcat__ChildLayer__

#include "UserBaseLayer.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

typedef enum{
    kApiStepInit=0,
    kApiStepUpdate
}ApiStep;


class ChildLayer : public UserBaseLayer
{
public:
    virtual bool init();
    CREATE_FUNC(ChildLayer);
    
    virtual void enableTouch();
    
    virtual void onEnter();
    
    virtual void menuCallback(CCObject* obj);
    
    virtual void onJsonCompleted(CCDictionary* root);
private:
    ApiStep m_step;
};


#endif /* defined(__Blackcat__ChildLayer__) */
