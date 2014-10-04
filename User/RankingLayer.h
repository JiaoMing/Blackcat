//
//  RankingLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-3-2.
//
//

#ifndef Blackcat_RankingLayer_h
#define Blackcat_RankingLayer_h

#include "DialogLayer.h"
#include "resource.h"
#include "Ranking.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

typedef enum{
    kRankKa=0,
    kRankZi,
    kRankXunzhang,
    kRankXingxing,
    kRankLevel
}Rank;

typedef struct _RankStruct
{
    string rankIcon;
	string position;
	string UD_key;
	int UD_default;
    string pic;
    string column;
}RankStruct;

static RankStruct rankStruts[] = {
	{
		"data_ka.png",
		"home_ka_diwen",
        COLLECT_TUPIAN_COUNT,
        15,
		"rank_ka.png",
        "tuka"
	},
	{
		"data_zi.png",
		"home_zi_diwen",
        COLLECT_HANZI_COUNT,
        15,
		"rank_zi.png",
        "zika"
	},
	{
		"data_xunzhang.png",
		"home_xunzhang_diwen",
        LAST_KECHENG_ID,
        0,
		"rank_xunzhang.png",
        "xunzhang"
	},
	{
		"data_xingxing.png",
		"home_xingxing_diwen",
        COLLECT_XINGXING_COUNT,
        0,
		"rank_xingxing.png",
        "xingxing"
	},
	{
		"data_level.png",
		"",
        USER_EXPERIENCE,
        0,
		"rank_level.png",
        "jingyan"
	}
};


class RankingLayer : public DialogLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    RankingLayer();
    ~RankingLayer();
    virtual bool init(Rank rank);
    static RankingLayer* create(Rank rank);
    
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
    
    virtual void onJsonCompleted(CCDictionary* root);
private:
    CCTableView* m_pTableView;
    vector<Ranking*>* m_rankingVector;
    Rank m_rank;
};


#endif
