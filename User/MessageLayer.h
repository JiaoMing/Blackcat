//
//  MessageLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-9-5.
//
//

#ifndef __Blackcat__MessageLayer__
#define __Blackcat__MessageLayer__

#include "CoverLayer.h"
#include "resource.h"
#include "Message.h"

class MessageLayer :public CoverLayer{
public:
    MessageLayer();
    ~MessageLayer();
    static MessageLayer* create(Message* message,CCTableView* tableView);
    virtual bool init();
    virtual void onEnter();
    
    void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    Message* m_message;
    CCSprite* m_xinzhi;
    CCTableView* m_tableView;
};

#endif /* defined(__Blackcat__MessageLayer__) */
