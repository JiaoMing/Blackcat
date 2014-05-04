//
//  CocosJsonParser.h
//  YidiPlayerHD
//
//  Created by shuang on 12-7-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef YidiPlayerHD_CocosJsonParser_h
#define YidiPlayerHD_CocosJsonParser_h

#include "cocos2d.h"
using namespace cocos2d;

#include "libjson.h"
#include <stack>


class CocosJsonParser
{
public:
    CCDictionary * parse(const char *pszJsonData);
    
private:
    void parseNode(JSONNODE *n);
    void parseArray(JSONNODE *n);
    
private:
    CCDictionary * m_pJsonDict;
    std::stack<CCDictionary *> m_oNodeStack;
    std::stack<CCArray *> m_oArrayStack;
};

#endif
