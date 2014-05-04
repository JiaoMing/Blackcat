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

//说明
//CocosJsonParser利用libjson把json数据以CCDictionary形式返回
//parse返回的CCDictonary是autorelease后的
//value都以CCString类来保存的，如果需要int或者float数据，可以调用CCString类的intValue，floatValue等方法
//事例代码
//CocosJsonParser parser;
//CCDictionary *json = parser.parse(ms.data);
//m_nMovieCount = ((CCString *)json->objectForKey(std::string("appCount")))->intValue();
//CCArray *apps = (CCArray *)json->objectForKey(std::string("apps"));
//CCObject *pObject = NULL;
//CCARRAY_FOREACH(apps, pObject)
//{
//    CCDictionary *app = (CCDictionary *)pObject;
//    Movie *m = new Movie();
//    m->setMovieId(((CCString *)app->objectForKey(std::string("id")))->intValue());
//    m->setName(((CCString *)app->objectForKey(std::string("name")))->getCString());
//    m->setIcon(CCString::stringWithFormat("http://res.kidsedu.com/%s", ((CCString *)app->objectForKey(std::string("iconUrl")))->getCString())->getCString());
//    m->setCover(CCString::stringWithFormat("http://res.kidsedu.com/%s", ((CCString *)app->objectForKey(std::string("cover")))->getCString())->getCString());
//    m->setDesc(((CCString *)app->objectForKey(std::string("mobiledesc")))->getCString());
//    m->setScore(((CCString *)app->objectForKey(std::string("commentGrade")))->intValue());
//    
//    m_pMovies->addObject(m);
//    m->release(), m = NULL;
//}

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
