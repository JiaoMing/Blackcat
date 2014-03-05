//
//  RankingLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-3-2.
//
//

#ifndef Blackcat_RankingLayer_h
#define Blackcat_RankingLayer_h

#include "CoverLayer.h"
#include "resource.h"

typedef enum{
    kRankKa=0,
    kRankZi,
    kRankXunzhang,
    kRankXingxing
}Rank;

typedef struct _RankStruct
{
    string rankIcon;
	string position;
	string UD_key;
	int UD_default;
    string pic;
}RankStruct;

static RankStruct rankStruts[] = {
	{
		"data_ka.png",
		"home_ka_diwen",
        COLLECT_TUPIAN_COUNT,
        15,
		"rank_ka.png"
	},
	{
		"data_zi.png",
		"home_zi_diwen",
        COLLECT_HANZI_COUNT,
        15,
		"rank_zi.png"
	},
	{
		"data_xunzhang.png",
		"home_xunzhang_diwen",
        LAST_KECHENG_ID,
        0,
		"rank_xunzhang.png"
	},
	{
		"data_xingxing.png",
		"home_xingxing_diwen",
        COLLECT_XINGXING_COUNT,
        0,
		"rank_xingxing.png"
	}
};


class RankingLayer : public CoverLayer
{
public:
    virtual bool init(Rank rank);
    static RankingLayer* create(Rank rank);
private:
};


#endif
