//
//  AchieveLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-5-30.
//
//

#ifndef __Blackcat__AchieveLayer__
#define __Blackcat__AchieveLayer__

#include "DialogLayer.h"
#include "resource.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;


class AchieveLayer : public DialogLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    virtual bool init();
    CREATE_FUNC(AchieveLayer);
    
    virtual void enableTouch();
    
    virtual void scrollViewDidScroll(CCScrollView* view) {};
    virtual void scrollViewDidZoom(CCScrollView* view) {};
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
private:
    CCTableView* m_pTableView;
};


#endif /* defined(__Blackcat__AchieveLayer__) */
