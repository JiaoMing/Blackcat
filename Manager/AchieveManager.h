//
//  AchieveManager.h
//  Blackcat
//
//  Created by haojunhua on 14-6-16.
//
//

#ifndef __Blackcat__AchieveManager__
#define __Blackcat__AchieveManager__

#include "cocos2d.h"
USING_NS_CC;
#include "udkey.h"
using namespace std;

typedef enum{
    kAchieveXZDW=0,
    kAchieveTKSCJ,
    kAchieveZKSCJ,
    kAchieveCGYS,
    kAchieveCSJJ
}AchieveKey;

typedef struct _AchieveStruct
{
    string text;
    string icon;
	string key;
	string UD_key;
}AchieveStruct;

static AchieveStruct achieveStruts[] = {
	{
		"xiezidawang",
		"xiezidawang",
        "XZDW",
        UDKEY_ACHIEVE_XZDW
	},
	{
		"tukashoucangjia",
		"kapianshoucangjia",
        "TKSCJ",
        UDKEY_ACHIEVE_TKSCJ
	},
	{
		"zikashoucangjia",
		"kapianshoucangjia",
        "ZKSCJ",
        UDKEY_ACHIEVE_ZKSCJ
	},
	{
		"chuangguanyongshi",
		"chuangguanyongshi",
        "CGYS",
        UDKEY_ACHIEVE_CGYS
	},
	{
		"changshengjiangjun",
		"changshengjiangjun",
        "CSJJ",
        UDKEY_ACHIEVE_CSJJ
	}
};

class AchieveManager :public CCObject{
public:
    static AchieveManager* sharedAchieveManager();
public:
    AchieveManager();
    ~AchieveManager();
    
    /**
     *  根据KEY获得成就等级
     *
     *  @param AchieveKey 例：kAchieveXZDW
     *
     *  @return 返回对应的成就等级
     */
    int achieveLevel(AchieveKey key);
    
    /**
     *  成就分数提升
     *
     *  @param AchieveKey 例：kAchieveXZDW
     *  @param upValue 提升的分数
     *  @param target 传递给rewardlayer用于回调
     *  @param selector 传递给rewardlayer用于回调
     *
     */
    void achieveUp(AchieveKey key,int upValue=1,CCObject* target=NULL,SEL_CallFunc selector=NULL);
    
private:
    CCDictionary* m_medalDict;
};


#endif /* defined(__Blackcat__AchieveManager__) */
