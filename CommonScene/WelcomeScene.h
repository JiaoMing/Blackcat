//
//  WelcomeScene.h
//  Blackcat
//
//  Created by haojunhua on 14-3-2.
//
//

#ifndef Blackcat_WelComeScene_h
#define Blackcat_WelComeScene_h
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
#include <string>
using namespace std;

class WelcomeScene : public CCLayerColor, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    WelcomeScene();
    ~WelcomeScene();
    static CCScene* scene();
public:
    virtual bool init();
    CREATE_FUNC(WelcomeScene);
    
    void menuCallBack(CCObject* obj);
    
    virtual void scrollViewDidScroll(CCScrollView* view) {};
    virtual void scrollViewDidZoom(CCScrollView* view) {};
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    void scrollText(float t);
    
    virtual void keyBackClicked();
    virtual void keyMenuClicked();
    
private:
    CCTableView* m_tableView;
};


#endif
