//
//  Api.cpp
//  Blackcat
//
//  Created by haojunhua on 14-4-15.
//
//

#include "Api.h"
#include "httpdata.h"
#include "CocosJsonParser.h"
#include "resource.h"
#include <iostream>

Api::Api(){
    m_feedbackDict=S_PTM->getPlistDictionary("feedback.plist");
}

Api* Api::create(ApiStruct apiStruct){
    Api* api=new Api();
    api->autorelease();
    api->m_apiStruct=apiStruct;
    return api;
}

void Api::send(){
    CCHttpRequest* request = new CCHttpRequest();
    string url=s_getFullPath(m_apiStruct.url.c_str(),m_apiStruct.isBlackcat);
    switch (m_apiStruct.requestType) {
        case CCHttpRequest::kHttpGet:{
            url.append("?").append(m_apiStruct.data);
        }
        case CCHttpRequest::kHttpPost:{
            request->setRequestData(m_apiStruct.data.c_str(),(int)strlen(m_apiStruct.data.c_str()));
        }
            break;
        default:
            break;
    }
    request->setUrl(url.c_str());
    
    request->setRequestType(m_apiStruct.requestType);
    request->setResponseCallback(this, httpresponse_selector(Api::onHttpRequestCompleted));
    
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void Api::onHttpRequestCompleted(CCHttpClient *sender, CCHttpResponse *response){
    if (!response->isSucceed())
    {
        if (DEBUG_OPEN) {
            for (vector<string>::iterator it = response->getHttpRequest()->getHeaders().begin(); it != response->getHttpRequest()->getHeaders().end(); it ++){
                std::cout<<*it;
            }
        }
        return;
        S_TT->makeText("连不上服务器，请检查网络。");
        if (DEBUG_OPEN) {
            for (vector<char>::iterator it = response->getResponseData()->begin(); it != response->getResponseData()->end(); it ++){
                std::cout<<*it;
            }
        }
        return;
    }
    
    std::vector<char> *buffer = response->getResponseData();
    std::string responseDataStr(buffer->begin(),buffer->end());
    
    //处理接口数据
    CocosJsonParser jsonParser;
    CCDictionary* root=jsonParser.parse(responseDataStr.c_str());
    if(root!=NULL){
        const CCString* feedback=root->valueForKey("feedback");
        if (feedback->intValue()!=100) {
            CCString* feedbackStr=(CCString*)m_feedbackDict->objectForKey(feedback->getCString());
            S_TT->makeText(feedbackStr->getCString());
            if (feedback->intValue()==-401||feedback->intValue()==-102) {
                //未登录处理
                if(S_LM->getDelegate())S_LM->getDelegate()->logout();
            }
        }else{
            if(m_apiStruct.target)(m_apiStruct.target->*m_apiStruct.sel_response)(root);
        }
    }else{
        S_TT->makeText("数据请求失败。");
    }
}