//
//  KapianTableLayer.h
//  blackcat
//
//  Created by haojunhua on 13-9-2.
//
//

#ifndef blackcat_KapianTableLayer_h
#define blackcat_KapianTableLayer_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "resource.h"
#include "Kapian.h"
#include "DBManager.h"
#include "resource.h"
#include "KEGridView.h"
#include "Hanzi.h"
#include "Tupian.h"
#include "XieziScene.h"
#include "TupianBrowserLayer.h"
USING_NS_CC;
USING_NS_CC_EXT;

class KapianTableLayer : public CCLayer, public CCTableViewDataSource,public KEGridViewDelegate,public XieziSceneDelegate,public TupianBrowserDelegate
{
public:
    static KapianTableLayer* create(KapianDataMode m_mode,KapianShowMode showMode);
    void changeShowMode(KapianShowMode mode);
    
public:
    KapianTableLayer(KapianDataMode m_mode,KapianShowMode showMode);
    ~KapianTableLayer();
    virtual bool init();
    
    virtual void scrollViewDidScroll(CCScrollView* view) {};
    virtual void scrollViewDidZoom(CCScrollView* view) {};
    virtual void gridCellTouched(CCTableView* table, CCTableViewCell* cell,int indexInIdx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
    virtual void xieziCallback(int index,float progress);
    
private:
    void initDataFromDB();
private:
    CCTableView* m_pTableView;
    vector<Kapian*>* m_kapianVector;//用于统一处理，目前只有size有效，***MODE强制类型转换无效(由于卡片本身没有成员变量)
    vector<Kapian*>* m_allVector;
    vector<Kapian*>* m_cltVector;
    KapianDataMode m_dataMode;
    CCPoint m_Offset;
    CCSize m_gridSize;
    CCSize m_cellSize;
};

#endif
