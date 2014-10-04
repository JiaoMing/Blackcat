//
//  TupianTableLayer.h
//  blackcat
//
//  Created by haojunhua on 13-8-26.
//
//

#ifndef blackcat_TupianTableLayer_h
#define blackcat_TupianTableLayer_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Tupian.h"
#include "resource.h"
#include <iostream>
using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;

class CC_DLL TupianTablePosition{
public:
    TupianTablePosition(){};
    TupianTablePosition(const char* tablePoint,const char* tableSize,const char* cellSize,const char* cellThumbSize,const char* kuangInCell,const char* thumbInCell,CCScrollViewDirection direction){
        setTablePoint(S_RM->getPositionWithName(tablePoint));
        setTableSize(S_RM->getSizeWithName(tableSize));
        setCellSize(S_RM->getSizeWithName(cellSize));
        setCellThumbSize(S_RM->getSizeWithName(cellThumbSize));
        setKuangInCell(S_RM->getPositionWithName(kuangInCell));
        setThumbInCell(S_RM->getPositionWithName(thumbInCell));
        setDirection(direction);
    }
    
    CC_SYNTHESIZE(CCPoint, tablePoint, TablePoint);
    CC_SYNTHESIZE(CCSize, tableSize, TableSize);
    CC_SYNTHESIZE(CCSize, cellSize, CellSize);
    CC_SYNTHESIZE(CCSize, cellThumbSize, CellThumbSize);
    CC_SYNTHESIZE(CCPoint, kuangInCell, KuangInCell);
    CC_SYNTHESIZE(CCPoint, thumbInCell, ThumbInCell);
    CC_SYNTHESIZE(CCScrollViewDirection, direction, Direction);
};

class TupianTableDelegate;


typedef enum{
    kScrollStateTop=0,
    kScrollStatemiddle,
    kScrollStateBottom
}ScrollState;

typedef enum{
    kScrollActionDown=0,
    kScrollActionUp
}ScrollAction;

class TupianTableLayer : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    TupianTableLayer();
    ~TupianTableLayer();
    
    virtual bool init();
    static TupianTableLayer* create(TupianTableDelegate* tupianTableDelegate,KapianDataMode mode,Kapian* kapian,TupianTablePosition tupianTablePosition);
    
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view) {};
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
    void reloadData();
    
    void scroll(ScrollAction action);
    
    void setHandlerPriority(int newPriority);
    
private:
    void initDataFromDB();
    
private:
    CCTableView* m_pTableView;
    vector<Tupian*>* m_tupianVector;
    string m_zi;//对应的汉字
    Kapian* m_kapian;
    KapianDataMode m_mode;
    TupianTableDelegate* m_tupianTableDelegate;
    TupianTablePosition m_tupianTablePosition;
};

class TupianTableDelegate{
public:
    virtual void tupianLoadCallBack(int count)=0;
    virtual void tupianTouchCallBack(Tupian* tupian,bool isOverLimit=false)=0;
    virtual void tupianTableScrollCallBack(ScrollState state,int cellCount)=0;
};


#endif
