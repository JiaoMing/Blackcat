//
//  PropsManager.h
//  Blackcat
//
//  道具管理类，用于从道具配置文件读取数据建立相应道具
//  道具配置文件:props.plist
//
//  Created by 俊华 郝 on 12-8-9.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef YidiPlayerHD_Props_h
#define YidiPlayerHD_Props_h

#include "cocos2d.h"
#include "Props.h"
using namespace cocos2d;

class PropsManager
{
public:
    static PropsManager* sharedPropsManager();
    PropsManager();
    ~PropsManager();
public:
    Props* getPropsWithName(const char* name);
    
private:
    
    CCDictionary* m_dictRoot;
};


#endif
