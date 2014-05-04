//
//  ResInstall.h
//  Blackcat
//
//  Created by haojunhua on 14-3-19.
//
//

#ifndef Blackcat_ResInstall_h
#define Blackcat_ResInstall_h

#include "cocos2d.h"
using namespace cocos2d;


class ResInstall : public CCObject
{
public:
    ResInstall();
    ~ResInstall();
    static ResInstall* sharedResInstall();
    void purgeResInstall();
    void install(const char* filename,CCObject *target, SEL_CallFunc selector);
    
    void installCallback(float t);
};

#endif
