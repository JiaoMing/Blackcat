//
//  AchieveLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-5-30.
//
//

#include "AchieveLayer.h"
#include "GraySprite.h"

bool AchieveLayer::init(){
    if (DialogLayer::init()) {
        this->setTitle("荣誉榜");
        
        m_pTableView = CCTableView::create(this,S_RM->getSizeWithName("dialog_table_size"));
        m_pTableView->setDirection(kCCScrollViewDirectionVertical);
        m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
        m_pTableView->setPosition(S_RM->getPositionWithName("dialog_table"));
        this->addChild(m_pTableView);
        
        string userToken=S_UD->getStringForKey(UDKEY_USER_TOKEN);
        if (userToken=="") {
            S_ALP->play("heimao054_1.mp3:5","heimao054_2.mp3:4",NULL);
        }
        return true;
    }
    return false;
}

void AchieveLayer::enableTouch(){
    DialogLayer::enableTouch();
    CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    pDispatcher->setPriority(this->getDelegate()->topHandlerPriority()-1, m_pTableView);
}

void AchieveLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    
}

CCSize AchieveLayer::cellSizeForTable(CCTableView *table)
{
    return S_RM->getSizeWithName("dialog_achieve_table_cell_size");
}

CCTableViewCell* AchieveLayer::tableCellAtIndex(CCTableView *table, unsigned int idx){
    CCTableViewCell *cell = table->dequeueCell();
    if (true){
        cell = new CCTableViewCell();
        cell->autorelease();
        
        CCSprite* bar=CCSprite::createWithSpriteFrameName("achieve_hengtiao2.png");
        bar->setPosition(ccp(291, 53));
        
        float barHeight=bar->getContentSize().height;
        if (idx>=kAchieveXZDW&&idx<=kAchieveCSJJ) {
            CCSprite* huizhang=CCSprite::createWithSpriteFrameName("achieve_huizhang.png");
            huizhang->setPosition(S_RM->getRelativePosition("achieve_huizhang", barHeight));
            bar->addChild(huizhang);
            
            CCString* str=CCString::createWithFormat("%s.png",achieveStruts[idx].text.c_str());
            CCSprite* text=CCSprite::createWithSpriteFrameName(str->getCString());
            text->setPosition(S_RM->getRelativePosition("achieve_text", barHeight));
            bar->addChild(text);
            
            for (int i=1; i<S_AEM->achieveLevel((AchieveKey)idx)+1; i++) {
                CCString* str=CCString::createWithFormat("%s_%d.png",achieveStruts[idx].icon.c_str(),i);
                CCSprite* item=CCSprite::createWithSpriteFrameName(str->getCString());
                CCString* point=CCString::createWithFormat("achieve_item%d",i);
                item->setPosition(S_RM->getRelativePosition(point->getCString(), barHeight));
                bar->addChild(item);
            }
            
        }
        cell->addChild(bar);
    }
    return cell;
}

unsigned int AchieveLayer::numberOfCellsInTableView(CCTableView *table){
    return 5;
}
