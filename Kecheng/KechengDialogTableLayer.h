//
//  KechengDialogTableLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-5-22.
//
//

#ifndef __Blackcat__KechengDialogTableLayer__
#define __Blackcat__KechengDialogTableLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Kecheng.h"
#include "Hanzi.h"
USING_NS_CC;
USING_NS_CC_EXT;

class KechengDialogTableLayerDelegate{
public:
    virtual void freshKecheng(bool isInit=false)=0;
};

class KechengDialogTableLayer : public CCLayer, public CCTableViewDataSource,public CCTableViewDelegate{
public:
    static KechengDialogTableLayer* create(KechengDialogTableLayerDelegate* delegate);
public:
    KechengDialogTableLayer();
    ~KechengDialogTableLayer();
    
    virtual bool init();
    
public:
    virtual void scrollViewDidScroll(CCScrollView* view) {};
    virtual void scrollViewDidZoom(CCScrollView* view) {};
    
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
    void setHandlerPriority(int newPriority);
    void middleOffset();
    void openNewXingxing();
private:
    CCTableView* m_tableView;
    KechengDialogTableLayerDelegate* m_delegate;
};

#endif /* defined(__Blackcat__KechengDialogTableLayer__) */
