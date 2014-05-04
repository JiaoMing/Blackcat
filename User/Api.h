//
//  Api.h
//  Blackcat
//
//  Created by haojunhua on 14-4-15.
//
//

#ifndef __Blackcat__Api__
#define __Blackcat__Api__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
#include <string>
using namespace std;

typedef void (CCObject::*SEL_ApiResponse)(CCDictionary* root);
#define apiresponse_selector(_SELECTOR) (SEL_ApiResponse)(&_SELECTOR)

typedef struct _ApiStruct
{
    string url;
    bool isBlackcat=true;
	string data;
	CCObject* target=NULL;
	SEL_ApiResponse sel_response;
    CCHttpRequest::HttpRequestType requestType=CCHttpRequest::kHttpGet;
}ApiStruct;

class Api: public CCObject{
public:
    Api();
    static Api* create(ApiStruct apiStruct);
    void send();
    virtual void onHttpRequestCompleted(CCHttpClient *sender, CCHttpResponse *response);
private:
    ApiStruct m_apiStruct;
    CCDictionary* m_feedbackDict;
};

#endif /* defined(__Blackcat__Api__) */
