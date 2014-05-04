//
//  CocosJsonParser.cpp
//  YidiPlayerHD
//
//  Created by shuang on 12-7-4.
//  Editd by haojunhua on 14-5-4
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "CocosJsonParser.h"

CCDictionary * CocosJsonParser::parse(const char *pszJsonData)
{
    JSONNODE *n = json_parse(pszJsonData);
    if (n) {
        m_pJsonDict = CCDictionary::create();
        
        m_oNodeStack.push(m_pJsonDict);
        parseNode(n);
    } else {
        m_pJsonDict = NULL;
    }
    return m_pJsonDict;
}

void CocosJsonParser::parseNode(void *n)
{
    if (n == NULL) {
        CCLog("parseJson n NULL");
        return;
    }
    
    CCDictionary *dict = m_oNodeStack.top();
    
    JSONNODE_ITERATOR iter = json_begin(n);
    while (iter != json_end(n)) {
        if (*iter == NULL) {
            CCLog("invalid json node");
            break;
        }
        
        if (json_type(*iter) == JSON_ARRAY) {
            json_char *key = json_name(*iter);
//            CCLog("array=%s", key);
            
            CCArray *arr = CCArray::create();
            dict->setObject(arr, std::string(key));
            m_oArrayStack.push(arr);
            
            json_free(key);
            
            parseArray(*iter);
        } else if (json_type(*iter) == JSON_NODE) {
            json_char *key = json_name(*iter);
//            CCLog("node=%s", key);
            
            CCDictionary *childDict = CCDictionary::create();
            dict->setObject(childDict, std::string(key));
            
            json_free(key);
            
            m_oNodeStack.push(childDict);
            parseNode(*iter);
        } else {
            json_char *key = json_name(*iter);
            json_char *value = json_as_string(*iter);
//            CCLog("%s, %s", key, value);
            
            CCString *str = CCString::create(value);
            dict->setObject(str, std::string(key));
            
            json_free(key);
            json_free(value);
        }
        
        iter++;
    }
    
    m_oNodeStack.pop();
}

void CocosJsonParser::parseArray(void *n)
{
    if (n == NULL) {
        CCLog("parseJson n NULL");
        return;
    }
    
    CCArray *arr = m_oArrayStack.top();
    
    JSONNODE_ITERATOR iter = json_begin(n);
    while (iter != json_end(n)) {
        if (*iter == NULL) {
            CCLog("invalid json node");
            break;
        }
        
        if (json_type(*iter) == JSON_ARRAY) {
            CCLog("json node error");
        } else if (json_type(*iter) == JSON_NODE) {
            json_char *key = json_name(*iter);
//            CCLog("node=%s", key);
            
            CCDictionary *dict = CCDictionary::create();
            arr->addObject(dict);
            
            json_free(key);
            
            m_oNodeStack.push(dict);
            parseNode(*iter);
        } else {
            CCLog("json node error");
        }
        
        iter++;
    }
    
    m_oArrayStack.pop();
}