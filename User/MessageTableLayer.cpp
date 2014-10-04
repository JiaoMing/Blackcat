//
//  MessageTableLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-8-11.
//
//

#include "MessageTableLayer.h"
#include "MessageLayer.h"
#include "RecordManager.h"

#define kTagWebview 100

MessageTableLayer::MessageTableLayer(){
    m_messageVector=new vector<Message*>();
}
MessageTableLayer::~MessageTableLayer(){
    DELETE_MODEL_VECTOR(Message, m_messageVector);
}

bool MessageTableLayer::init(){
    if (DialogLayer::init()) {
        this->setTitle("消息");
        
        CLAUSE_INIT;
        S_DM->findScrollData(m_messageVector,"*",whereClause, orderbyClause, groupByClause);
        
        m_pTableView = CCTableView::create(this,S_RM->getSizeWithName("dialog_table_size"));
        m_pTableView->setDirection(kCCScrollViewDirectionVertical);
        m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_pTableView->setPosition(S_RM->getPositionWithName("dialog_table"));
        m_pTableView->setDelegate(this);
        this->addChild(m_pTableView);
        
        return true;
    }
    return false;
}

void MessageTableLayer::onEnter(){
    DialogLayer::onEnter();
    
    string userToken=S_UD->getStringForKey(UDKEY_USER_TOKEN);
    if (userToken=="") {
        
    }else{
        
    }
}

void MessageTableLayer::onExit(){
    DialogLayer::onExit();
    RankingBarLayer* bar=S_RDM->getDelegate();
    if (bar!=NULL) {
        bar->freshRecord();
    }
}

void MessageTableLayer::enableTouch(){
    DialogLayer::enableTouch();
    CCTouchDispatcher* pDispatcher = S_DR->getTouchDispatcher();
    pDispatcher->setPriority(this->getDelegate()->topHandlerPriority()-1, m_pTableView);
}

void MessageTableLayer::menuCallback(CCObject *obj){
    DialogLayer::menuCallback(obj);
}

void MessageTableLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    Message* message=(*m_messageVector)[cell->getIdx()];
    MessageLayer* layer=MessageLayer::create(message,m_pTableView);
    layer->setDelegate(this);
    this->addChild(layer);
}

CCSize MessageTableLayer::cellSizeForTable(CCTableView *table)
{
    return S_RM->getSizeWithName("dialog_table_cell_size");
}

CCTableViewCell* MessageTableLayer::tableCellAtIndex(CCTableView *table, unsigned int idx){
    CCTableViewCell *cell = table->dequeueCell();
    if (true){
        cell = new CCTableViewCell();
        cell->autorelease();
        
        float cellHeight=S_RM->getSizeWithName("dialog_table_cell_size").height;
        CCPoint middlePoint=S_RM->getRelativePosition("dialog_table_middle", cellHeight);
        if (m_messageVector->size()==0) {
            CCSprite* jiazai=CCSprite::createWithSpriteFrameName("jiazai.png");
            jiazai->setPosition(middlePoint);
            jiazai->runAction(CCRepeatForever::create(CCRotateBy::create(2, 360)));
            cell->addChild(jiazai);
        }else{
            Message* message=(*m_messageVector)[idx];
            
            float cellHeight=S_RM->getSizeWithName("dialog_table_cell_size").height;
            
            CCSprite* bg=CCSprite::createWithSpriteFrameName("rank_bg.png");
            bg->setPosition(middlePoint);
            
            CCLabelTTF* title=CCLabelTTF::create(message->gettitle().c_str(), "KaiTi.ttf",25,S_RM->getSizeWithName("dialog_table_message_title"),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
            if (message->getisReaded()==1) {
                title->setColor(ccGRAY);
            }else{
                title->setColor(ccBLUE);
                title->setFontSize(27);
                title->enableShadow(CCSizeMake(2, 2), 1, 1);
            }
            
            title->setPosition(S_RM->getRelativePosition("dialog_table_message_title", cellHeight));
            bg->addChild(title);
            
            cell->addChild(bg);
        }
    }
    return cell;
}

unsigned int MessageTableLayer::numberOfCellsInTableView(CCTableView *table){
    return (m_messageVector->size()==0)?1:(int)m_messageVector->size();
}
