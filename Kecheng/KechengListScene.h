//
//  ListLayer.h
//  Blackcat
//
//  Created by haojunhua on 13-10-7.
//
//

#ifndef Blackcat_ListScene_h
#define Blackcat_ListScene_h
#include "KechengBaseLayer.h"
#include "KechengTableLayer.h"

class KechengListScene : public KechengBaseLayer,public KechengTableDelegate{
public:
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(KechengListScene);
    
    virtual void onEnter();
    virtual void onExit();
    virtual void keyBackClicked();
    virtual void keyMenuClicked();
    
    void heimaoAction();
    virtual void heimaoCallBack(CCObject* node, const char* buwei);
    virtual void kechengLoadCallBack();
    virtual void kechengTouchCallBack();
    
    void menuCallback(CCObject* pSender);
};

#endif
