//
//  RankingLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-3-2.
//
//

#include "RankingLayer.h"
#include "httpdata.h"
#include "CocosJsonParser.h"
#include "UserLoginLayer.h"

RankingLayer::RankingLayer(){
    m_rankingVector=new vector<Ranking*>();
}
RankingLayer::~RankingLayer(){
    DELETE_MODEL_VECTOR(Ranking, m_rankingVector);
}

RankingLayer* RankingLayer::create(Rank rank){
    RankingLayer* layer=new RankingLayer();
    if (layer&&layer->init(rank)) {
        layer->autorelease();
    }else{
        CC_SAFE_DELETE(layer);
    }
    return layer;
}

bool RankingLayer::init(Rank rank){
    if (DialogLayer::init()) {
        this->m_rank=rank;
        this->setTitle("排行榜");
        m_pTableView = CCTableView::create(this,CCSizeMake(583, 305));
        m_pTableView->setDirection(kCCScrollViewDirectionVertical);
        m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_pTableView->setPosition(ccp(222, 210));
        this->addChild(m_pTableView);
        
        return true;
    }
    return false;
}

void RankingLayer::onEnter(){
    DialogLayer::onEnter();
    
    string userToken=S_UD->getStringForKey(UDKEY_USER_TOKEN);
    CCString* postData=CCString::createWithFormat("token=%s&sort=%s&order=desc",userToken.c_str(),rankStruts[m_rank].column.c_str());
    
    ApiStruct apiStruct;
    apiStruct.url="user_rankingList_rankings";
    apiStruct.target=this;
    apiStruct.sel_response=apiresponse_selector(RankingLayer::onJsonCompleted);
    apiStruct.data=postData->getCString();
    Api* api=Api::create(apiStruct);
    api->send();
}

void RankingLayer::onExit(){
    DialogLayer::onExit();
}

void RankingLayer::enableTouch(){
    DialogLayer::enableTouch();
    CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    pDispatcher->setPriority(this->getDelegate()->topHandlerPriority()-1, m_pTableView);
}

void RankingLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
}

CCSize RankingLayer::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(583, 78);
}

CCTableViewCell* RankingLayer::tableCellAtIndex(CCTableView *table, unsigned int idx){
    CCTableViewCell *cell = table->dequeueCell();
    if (true){
        cell = new CCTableViewCell();
        cell->autorelease();
        
        if (m_rankingVector->size()==0) {
            CCSprite* jiazai=CCSprite::createWithSpriteFrameName("jiazai.png");
            jiazai->setPosition(ccp(291, 39));
            jiazai->runAction(CCRepeatForever::create(CCRotateBy::create(2, 360)));
            cell->addChild(jiazai);
        }else{
            Ranking* ranking=(*m_rankingVector)[idx];
            
            CCSprite* rank=CCSprite::createWithSpriteFrameName("rank_bg.png");
            rank->setPosition(ccp(291, 39));
            
            if (idx<3) {
                CCString* num=CCString::createWithFormat("n%d.png",idx+1);
                CCSprite* number=CCSprite::createWithSpriteFrameName(num->getCString());
                number->setPosition(ccp(50, 40));
                rank->addChild(number);
            }else{
                CCString* countString=CCString::createWithFormat("%d",idx+1);
                CCLabelBMFont* numFont = CCLabelBMFont::create(countString->getCString(), "font_2.fnt",70,kCCTextAlignmentLeft);
                if (numFont->getContentSize().width>70) {
                    numFont->setScale(70/numFont->getContentSize().width);
                }
                numFont->setPosition(ccp(50, 40));
                rank->addChild(numFont);
            }
            
            CCLabelTTF* username=CCLabelTTF::create(ranking->getusername().c_str(), "KaiTi.ttf", 30,CCSizeMake(100, 78),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
            username->setPosition(ccp(150, 40));
            username->enableShadow(CCSizeMake(2, 2), 1, 1);
            rank->addChild(username);
            
            CCSprite* icon=CCSprite::createWithSpriteFrameName(rankStruts[m_rank].rankIcon.c_str());
            icon->setPosition(ccp(280, 39));
            rank->addChild(icon);
            
            CCSprite* x=CCSprite::createWithSpriteFrameName("rank_x.png");
            x->setPosition(ccp(330, 39));
            rank->addChild(x);
            
            int count=0;
            switch (m_rank) {
                case kRankZi:
                    count=ranking->getzika();
                    break;
                case kRankKa:
                    count=ranking->gettuka();
                    break;
                case kRankXunzhang:
                    count=ranking->getxunzhang();
                    break;
                case kRankXingxing:
                    count=ranking->getxingxing();
                    break;
                default:
                    break;
            }
            
            CCString* countString=CCString::createWithFormat("%d",count);
            CCLabelBMFont* numFont = CCLabelBMFont::create(countString->getCString(), "font_1.fnt",150,kCCTextAlignmentLeft);
            if (numFont->getContentSize().width>150) {
                numFont->setScale(150/numFont->getContentSize().width);
            }
            numFont->setPosition(ccp(350, 39));
            numFont->setAnchorPoint(ccp(0, 0.5));
            rank->addChild(numFont);
            
            CCSprite* xinfeng=CCSprite::createWithSpriteFrameName("xinfeng.png");
            xinfeng->setPosition(ccp(530, 39));
            rank->addChild(xinfeng);
            
            cell->addChild(rank);
        }
    }
    return cell;
}

unsigned int RankingLayer::numberOfCellsInTableView(CCTableView *table){
    return (m_rankingVector->size()==0)?1:(int)m_rankingVector->size();
}

void RankingLayer::onJsonCompleted(CCDictionary* root){
    CCDictionary* rankDict = (CCDictionary*)root->objectForKey("rankings");
    CCArray *rankings = (CCArray*)rankDict->objectForKey("rows");
    CCObject *pObject = NULL;
    CCARRAY_FOREACH(rankings, pObject)
    {
        CCDictionary* rank = (CCDictionary *)pObject;
        Ranking* ranking=new Ranking();
        ranking->setusername(((CCString*)rank->objectForKey("username"))->getCString());
        ranking->setInttuka(((CCString*)rank->objectForKey("tuka"))->intValue());
        ranking->setIntzika(((CCString*)rank->objectForKey("zika"))->intValue());
        ranking->setIntxingxing(((CCString*)rank->objectForKey("xingxing"))->intValue());
        ranking->setIntxunzhang(((CCString*)rank->objectForKey("xunzhang"))->intValue());
        m_rankingVector->push_back(ranking);
    }
    m_pTableView->reloadData();
}