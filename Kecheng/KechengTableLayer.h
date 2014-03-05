//
//  ListTableLayer.h
//  Blackcat
//
//  需要保证课程从1开始编号！
//
//  Created by haojunhua on 13-10-7.
//
//

#ifndef Blackcat_ListTableLayer_h
#define Blackcat_ListTableLayer_h
#include "cocos2d.h"
#include "cocos-ext.h"
#include "KEGridView.h"
#include "Kecheng.h"
#include "KechengScene.h"
USING_NS_CC;
USING_NS_CC_EXT;

class KechengTableDelegate{
public:
    virtual void kechengLoadCallBack()=0;
    virtual void kechengTouchCallBack()=0;
};

class KechengTableLayer : public CCLayer, public CCTableViewDataSource,public KEGridViewDelegate,public KechengSceneDelegate{
public:
    KechengTableLayer();
    ~KechengTableLayer();
    
    virtual void onEnter();
    virtual bool init();
    static KechengTableLayer* create(KechengTableDelegate* kechengTableDelegate);
public:
    virtual void scrollViewDidScroll(CCScrollView* view) {};
    virtual void scrollViewDidZoom(CCScrollView* view) {};
    virtual void gridCellTouched(CCTableView* table, CCTableViewCell* cell,int indexInIdx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
    virtual void examAllRightCallback(int kcid);
private:
    int m_lastKechengId;
    int m_dayRenwuCount;
    int m_kcCount;//最大课程编号，也就是课程数量，所以必须保证课程从1开始编号！
    KechengTableDelegate* m_kechengTableDelegate;
};

#endif
