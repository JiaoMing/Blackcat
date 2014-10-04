//
//  CardShelfScene.h
//  ecards
//
//  Created by 杨飞 on 13-10-9.
//
//

#ifndef __ecards__CardShelfScene__
#define __ecards__CardShelfScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "KEGridView.h"
using namespace cocos2d;

class CardShelfScene : public CCLayer, public extension::CCTableViewDataSource,public KEGridViewDelegate
{
public:
    CardShelfScene();
    ~CardShelfScene();
    
    virtual void keyBackClicked();
    virtual void keyMenuClicked();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    static CCScene* scene();
    
    virtual void menuCallback(CCObject* pSender);
    
    virtual void scrollViewDidScroll(extension::CCScrollView* view) {};
    virtual void scrollViewDidZoom(extension::CCScrollView* view) {}
    virtual void gridCellTouched(CCTableView* table, CCTableViewCell* cell,int indexInIdx);
    virtual CCSize cellSizeForTable(extension::CCTableView *table);
    virtual extension::CCTableViewCell* tableCellAtIndex(extension::CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(extension::CCTableView *table);
    
    CREATE_FUNC(CardShelfScene);
    
    
private:
    CCTableView* m_pTableView;
};

#endif /* defined(__ecards__CardShelfScene__) */
