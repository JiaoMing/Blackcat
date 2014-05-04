//
//  Button9.h
//  Blackcat
//
//  Created by haojunhua on 14-4-3.
//
//

#ifndef __Blackcat__Button9__
#define __Blackcat__Button9__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
#include <string>
using namespace std;

class Button9 :public CCScale9Sprite{
public:
    static Button9* create(string title,CCSize size=CCSizeMake(120, 56),int fontSize=25);
    void setString(const char* str,CCSize size=CCSizeMake(120, 56));
};

#endif /* defined(__Blackcat__Button9__) */
