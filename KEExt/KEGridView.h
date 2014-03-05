//
//  KapianTableView.h
//  blackcat
//
//  Created by haojunhua on 13-9-7.
//
//

#ifndef blackcat_KapianTableView_h
#define blackcat_KapianTableView_h

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
class KEGridViewDelegate;

class KEGridView : public CCTableView{
public:
    static KEGridView* create(CCTableViewDataSource* dataSource, CCSize size,KEGridViewDelegate* gridViewDelegate,CCSize gridSize);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
protected:
    virtual int _gridIndexFromOffset(CCPoint offset);
private:
    KEGridViewDelegate* m_gridViewDelegate;
    CCSize m_gridSize;
};

class KEGridViewDelegate
{
public:
    /**
     * Delegate to respond touch event
     *
     * @param table table contains the given cell
     * @param cell  cell that is touched
     */
    virtual void gridCellTouched(CCTableView* table, CCTableViewCell* cell,int indexInIdx) = 0;
};

#endif
