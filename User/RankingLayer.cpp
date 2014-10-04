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
#include "KapianScene.h"

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
        m_pTableView = CCTableView::create(this,S_RM->getSizeWithName("dialog_table_size"));
        m_pTableView->setDirection(kCCScrollViewDirectionVertical);
        m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_pTableView->setPosition(S_RM->getPositionWithName("dialog_table"));
        this->addChild(m_pTableView);
        
//        if (m_rank==kRankZi||m_rank==kRankKa) {
//            //按钮
//            CCString* str=CCString::createWithFormat("rank_my_%s.png",rankStruts[m_rank].column.c_str());
//            CCSprite* anniu=CCSprite::createWithSpriteFrameName(str->getCString());
//            CCMenuItemSprite* item=CCMenuItemSprite::create(anniu, anniu,anniu, this, menu_selector(RankingLayer::menuCallback));;
//            item->setPosition(S_RM->getPositionWithName("rank_my"));
//            this->addMenuItem(item);
//        }
        
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
    apiStruct.isBlackcat=true;
    apiStruct.sel_response=apiresponse_selector(RankingLayer::onJsonCompleted);
    apiStruct.data=postData->getCString();
    Api* api=Api::create(apiStruct);
    api->send();
    
    if (userToken=="") {
        S_ALP->play("heimao054_1.mp3:5","heimao054_2.mp3:4",NULL);
    }
}

void RankingLayer::onExit(){
    DialogLayer::onExit();
}

void RankingLayer::enableTouch(){
    DialogLayer::enableTouch();
    CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    pDispatcher->setPriority(this->getDelegate()->topHandlerPriority()-1, m_pTableView);
}

void RankingLayer::menuCallback(CCObject *obj){
    DialogLayer::menuCallback(obj);
    if(!this->isCloseItem(obj)){
        switch (m_rank) {
            case kRankZi:
                S_DR->replaceScene(KapianHanziScene::scene());
                break;
            case kRankKa:
                S_DR->replaceScene(KapianTupianScene::scene());
                break;
            default:
                break;
        }
    }
}

void RankingLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
}

CCSize RankingLayer::cellSizeForTable(CCTableView *table)
{
    return S_RM->getSizeWithName("dialog_table_cell_size");
}

CCTableViewCell* RankingLayer::tableCellAtIndex(CCTableView *table, unsigned int idx){
    CCTableViewCell *cell = table->dequeueCell();
    if (true){
        cell = new CCTableViewCell();
        cell->autorelease();
        
        float cellHeight=S_RM->getSizeWithName("dialog_table_cell_size").height;
        CCPoint middlePoint=S_RM->getRelativePosition("dialog_table_middle", cellHeight);
        if (m_rankingVector->size()==0) {
            CCSprite* jiazai=CCSprite::createWithSpriteFrameName("jiazai.png");
            jiazai->setPosition(middlePoint);
            jiazai->runAction(CCRepeatForever::create(CCRotateBy::create(2, 360)));
            cell->addChild(jiazai);
        }else{
            Ranking* ranking=(*m_rankingVector)[idx];
            
            CCSprite* rank=CCSprite::createWithSpriteFrameName("rank_bg.png");
            rank->setPosition(middlePoint);
            
            if (idx<3) {
                CCString* num=CCString::createWithFormat("n%d.png",idx+1);
                CCSprite* number=CCSprite::createWithSpriteFrameName(num->getCString());
                number->setPosition(S_RM->getRelativePosition("dialog_table_number", cellHeight));
                rank->addChild(number);
            }else{
                CCString* countString=CCString::createWithFormat("%d",idx+1);
                CCLabelBMFont* numFont = CCLabelBMFont::create(countString->getCString(), "font_2.fnt",70,kCCTextAlignmentLeft);
                float maxWidth=S_RM->getSizeWithName("dialog_table_number_size").width;
                if (numFont->getContentSize().width>maxWidth) {
                    numFont->setScale(maxWidth/numFont->getContentSize().width);
                }
                numFont->setPosition(S_RM->getRelativePosition("dialog_table_number", cellHeight));
                rank->addChild(numFont);
            }
            
            CCLabelTTF* username=CCLabelTTF::create(ranking->getusername().c_str(), "KaiTi.ttf",30);
            ccFontDefinition* def=new ccFontDefinition();
            def->m_fontSize=30;
            def->m_fontName="KaiTi.ttf";
            def->m_alignment=kCCTextAlignmentLeft;
            def->m_vertAlignment=kCCVerticalTextAlignmentCenter;
            username->setTextDefinition(def);
            
            float maxUsernameWidth=S_RM->getSizeWithName("dialog_table_username_size").width;
            if (username->getContentSize().width>maxUsernameWidth) {
                username->setScale(maxUsernameWidth/username->getContentSize().width);
            }
            username->setPosition(S_RM->getRelativePosition("dialog_table_username", cellHeight));
            username->enableShadow(CCSizeMake(2, 2), 1, 1);
            rank->addChild(username);
            
            CCSprite* icon=CCSprite::createWithSpriteFrameName(rankStruts[m_rank].rankIcon.c_str());
            icon->setPosition(S_RM->getRelativePosition("dialog_table_icon", cellHeight));
            rank->addChild(icon);
            
            if (m_rank!=kRankLevel) {
                CCSprite* x=CCSprite::createWithSpriteFrameName("rank_x.png");
                x->setPosition(S_RM->getRelativePosition("dialog_table_x", cellHeight));
                rank->addChild(x);
            }
            
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
                case kRankLevel:{
                    int jingyan=ranking->getjingyan();
                    CCLog("%d",jingyan);
                    count=S_LM->getLevelByExpAndRule(jingyan);
                }
                    break;
                default:
                    break;
            }
            
            CCString* countString=CCString::createWithFormat("%d",count);
            CCLabelBMFont* numFont = CCLabelBMFont::create(countString->getCString(), "font_1.fnt",150,kCCTextAlignmentLeft);
            float maxCountWidth=S_RM->getSizeWithName("dialog_table_count_size").width;
            if (numFont->getContentSize().width>maxCountWidth) {
                numFont->setScale(maxCountWidth/numFont->getContentSize().width);
            }
            numFont->setPosition(S_RM->getRelativePosition("dialog_table_count", cellHeight));
            numFont->setAnchorPoint(ccp(0, 0.5));
            rank->addChild(numFont);
            
            CCSprite* xinfeng=CCSprite::createWithSpriteFrameName("xinfeng.png");
            xinfeng->setPosition(S_RM->getRelativePosition("dialog_table_xinfeng", cellHeight));
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
        ranking->setIntjingyan(((CCString*)rank->objectForKey("jingyan"))->intValue());
        m_rankingVector->push_back(ranking);
    }
    m_pTableView->reloadData();
}