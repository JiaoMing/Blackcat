//
//  CCWebView.h
//  Blackcat
//
//  Created by haojunhua on 13-10-15.
//
//

#ifndef Blackcat_CCWebView_h
#define Blackcat_CCWebView_h
#include "cocos2d.h"
USING_NS_CC;


class CCWebView;
class CCWebViewImpl;

class CCWebViewDelegate{
public:
    virtual ~CCWebViewDelegate() {};
    virtual void webCallBack(std::string cmd)=0;
};

class CCWebView : public CCLayer{
public:
    static CCWebView* create(const char* htmlFilename,CCRect rectOfScreen);
    
    //回调委托
    void setDelegate(CCWebViewDelegate* pDelegate);
    
    void callWebWithJs(const char* js);
    
    virtual void setVisible(bool var);
    
public:
    CCWebView();
    ~CCWebView();
    
private:
    bool init(const char* htmlFilename,CCRect rectOfScreen);
    CCRect rectByResolutionPolicy(CCRect rect);
    
protected:
    CCWebViewImpl* m_pWebViewImpl;
    CCWebViewDelegate* m_pWebViewDelegate;
};



#endif
