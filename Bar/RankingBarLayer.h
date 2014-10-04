//
//  RankingBarLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-1-26.
//
//

#ifndef Blackcat_RankingBarLayer_h
#define Blackcat_RankingBarLayer_h
#include "cocos2d.h"
#include "ClickableSprite.h"
#include "RankingLayer.h"
USING_NS_CC;

class RankingBarLayer : public CCLayer
{
public:
    static RankingBarLayer* create(bool isFromHome=false);
    virtual bool init();
    
    
    virtual void onEnter();
    virtual void onExit();
    
    CCSprite* dataItem(const char* typeName,CCPoint point, int data, int data2=0,int tag=0);
    CCSprite* createSpriteWithRankStruct(int index);
    
    void subXingxing(int count=1);
    
    void fresh(float t);
    
    void logoCallBack(CCObject* object);
    void messageCallBack(CCObject* object);
    void rankCallBack(CCObject* object);
    
    void logoFreshCallBack(CCNode* node);
    
    void setDelegate(DialogLayerDelegate* delegate){m_delegate=delegate;};
    
    int getMessageCount();
    
    void freshRecord();
private:
    CCLayer* m_rankingDataLayer;
    CCMenu* m_logoMenu;
    bool m_isLogoShow;//用于logo和排行榜切换
    DialogLayerDelegate* m_delegate;
    bool m_isFromHome;
};

#endif
