//
//  KapianTableView.cpp
//  blackcat
//
//  Created by haojunhua on 13-9-7.
//
//

#include "KEGridView.h"
#include "Utils.h"

KEGridView* KEGridView::create(CCTableViewDataSource* dataSource, CCSize size,KEGridViewDelegate* gridViewDelegate,CCSize gridSize){
    KEGridView *table = new KEGridView();
    table->initWithViewSize(size, NULL);
    table->autorelease();
    table->setDataSource(dataSource);
    table->_updateCellPositions();
    table->_updateContentSize();
    
    table->m_gridViewDelegate=gridViewDelegate;
    table->m_gridSize=gridSize;
    return table;
}

void KEGridView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    if (!this->isVisible()) {
        return;
    }
    if (m_pTouchedCell){
		CCRect bb = this->boundingBox();
		bb.origin = m_pParent->convertToWorldSpace(bb.origin);
        
		if (bb.containsPoint(pTouch->getLocation()) && m_gridViewDelegate != NULL)
        {
            CCPoint point = this->getContainer()->convertTouchToNodeSpace(pTouch);
            m_gridViewDelegate->gridCellTouched(this,m_pTouchedCell,this->_gridIndexFromOffset(point));
        }
    }
    CCScrollView::ccTouchEnded(pTouch, pEvent);
}

int KEGridView::_gridIndexFromOffset(CCPoint offset)
{
    int  index = 0;
    CCSize     gridSize= this->m_gridSize;
    
    switch (this->getDirection()) {
        case kCCScrollViewDirectionHorizontal:
            index = offset.y/gridSize.height;
            break;
        default:
            index = offset.x/gridSize.width;
            break;
    }
    
    return index;
}
