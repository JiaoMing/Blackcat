//
//  MessageLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-9-5.
//
//

#include "MessageLayer.h"
#include "CCWebView.h"

MessageLayer::MessageLayer(){
    
}

MessageLayer::~MessageLayer(){
    CCTextureCache::sharedTextureCache()->removeTextureForKey("xinzhi.png");
}

MessageLayer* MessageLayer::create(Message *message,CCTableView* tableView){
    MessageLayer* messageLayer=new MessageLayer();
    messageLayer->m_message=message;
    messageLayer->m_tableView=tableView;
    messageLayer->autorelease();
    messageLayer->init();
    return messageLayer;
}

bool MessageLayer::init(){
    if (!CoverLayer::init()) {
        return false;
    }
    
    m_xinzhi=CCSprite::create("xinzhi.png");
    m_xinzhi->setPosition(S_RM->getJpgBgPosition());
    this->addChild(m_xinzhi);
    
    CCPoint point=S_RM->getPositionWithName("message_webview");
    CCSize size=S_RM->getSizeWithName("message_webview");
    
    //添加WEBVIEW
    CCSize win_size=S_DR->getWinSize();
    CCRect rect=CCRectMake(point.x,win_size.height-point.y, size.width, size.height);//UI坐标,从左上角计算
    
    CCWebView* webview=CCWebView::create(m_message->geturl().c_str(), rect);
    webview->setDelegate(NULL);
    this->addChild(webview);
    
    return true;
}

void MessageLayer::onEnter(){
    CoverLayer::onEnter();
    if (m_message->getisReaded()!=1) {
        CCString* sql=CCString::createWithFormat("update message set isReaded=1 where id=%d",m_message->getid());
        S_DM->executeSql(sql->getCString());
        m_message->setIntisReaded(1);
        m_tableView->reloadData();
    }
}

void MessageLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCSize size=m_xinzhi->getContentSize();
    CCPoint point=m_xinzhi->convertToNodeSpace(pTouch->getLocation());
    if(point.x<0||point.x>size.width||point.y<0||point.y>size.height){
        this->removeFromParent();
    }
}

