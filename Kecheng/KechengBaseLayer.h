//
//  KechengBaseLayer.h
//  Blackcat
//
//  Created by haojunhua on 13-12-19.
//
//

#ifndef Blackcat_KechengBaseLayer_h
#define Blackcat_KechengBaseLayer_h
#include "cocos2d.h"
#include "Heimao.h"
USING_NS_CC;

class KechengBaseLayer : public CCLayer{
public:
    virtual bool init();
    
    virtual void onExit();
protected:
    Heimao* m_heimao;
};

#endif
