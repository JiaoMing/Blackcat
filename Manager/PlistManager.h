//
//  PlistManager.h
//  Blackcat
//
//  Created by haojunhua on 14-2-24.
//
//

#ifndef Blackcat_PlistManager_h
#define Blackcat_PlistManager_h

#include "cocos2d.h"
USING_NS_CC;

class PlistManager:public CCObject{
public:
    static PlistManager* sharedPlistManager();
    CCDictionary* getPlistDictionary(const char* key);
public:
    ~PlistManager();
private:
    PlistManager();
private:
    CCDictionary* m_dict;
};


#endif
