//
//  RankingLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-3-2.
//
//

#ifndef Blackcat_MessageTableLayer_h
#define Blackcat_MessageTableLayer_h

#include "DialogLayer.h"
#include "resource.h"
#include "Message.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

class MessageTableLayer : public DialogLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    MessageTableLayer();
    ~MessageTableLayer();
    virtual bool init();
    CREATE_FUNC(MessageTableLayer);
    
    
    virtual void enableTouch();
    virtual void onEnter();
    virtual void onExit();
    virtual void menuCallback(CCObject *obj);
    
    virtual void scrollViewDidScroll(CCScrollView* view) {};
    virtual void scrollViewDidZoom(CCScrollView* view) {};
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
private:
    CCTableView* m_pTableView;
    vector<Message*>* m_messageVector;
};


#endif
