//
//  KechengDialogTableLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-5-22.
//
//

#include "KechengDialogTableLayer.h"
#include "resource.h"
#include "KechengController.h"

KechengDialogTableLayer::KechengDialogTableLayer(){
}

KechengDialogTableLayer::~KechengDialogTableLayer(){
}

KechengDialogTableLayer* KechengDialogTableLayer::create(KechengDialogTableLayerDelegate* delegate){
    KechengDialogTableLayer* layer=new KechengDialogTableLayer();
    layer->m_delegate=delegate;
    if (layer&&layer->init()) {
        layer->autorelease();
    }else{
        CC_SAFE_DELETE(layer);
    }
    return layer;
}
bool KechengDialogTableLayer::init(){
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    m_tableView = CCTableView::create(this,S_RM->getSizeWithName("renwu_dialog_table_size"));
    m_tableView->setDirection(kCCScrollViewDirectionHorizontal);
	m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tableView->setPosition(S_RM->getPositionWithName("renwu_dialog_table"));
    m_tableView->setClippingToBounds(true);
    m_tableView->setDelegate(this);
    this->addChild(m_tableView);
    
    return true;
}


CCTableViewCell* KechengDialogTableLayer::tableCellAtIndex(CCTableView *table, unsigned int idx){
    CCTableViewCell* cell = table->dequeueCell();
    if (true){
        //  if (!cell){
        cell = new CCTableViewCell();
        cell->autorelease();
        
        
        CCSprite* gridCell=CCSprite::create();
        
        CCLabelTTF *txtLabel = CCLabelTTF::create(CCString::createWithFormat("%d",idx+1)->getCString(), "KaiTi.ttf", 30.0);
        txtLabel->setPosition(ccp(50, 50));
        txtLabel->setColor(ccc3(252,111,25));
        
        CCSprite* xingxing=CCSprite::create();
        xingxing->setPosition(ccp(50, 50));
        if (idx<=S_KC->getLastKechengId()) {
            xingxing->setDisplayFrame(S_SF->spriteFrameByName("xingxing_2.png"));
        }else{
            xingxing->setDisplayFrame(S_SF->spriteFrameByName("xingxing_1.png"));
        }
        
        gridCell->addChild(xingxing);
        gridCell->addChild(txtLabel);
        cell->addChild(gridCell);
    }
    
    return cell;
}

void KechengDialogTableLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
    if(cell!=NULL&&this->isVisible()){
        int idx=cell->getIdx();
        
        if (idx+1<=S_KC->getLastKechengId()||(idx==S_KC->getLastKechengId())) {
            m_delegate->setKecheng(idx+1);
            S_AE->playEffect("renwu.mp3");
            
            this->setVisible(false);
        }else{
            S_AE->playEffect("renwu_suo.mp3");
        }
        
    }
}

CCSize KechengDialogTableLayer::cellSizeForTable(CCTableView *table){
    return S_RM->getSizeWithName("renwu_dialog_table_cell_size");
}

unsigned int KechengDialogTableLayer::numberOfCellsInTableView(CCTableView *table){
    return S_KC->getKcCount();
}

void KechengDialogTableLayer::setHandlerPriority(int newPriority)
{
    if(S_KC->getKcCount()>0){
        CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
        pDispatcher->setPriority(newPriority, m_tableView);
    }
}

void KechengDialogTableLayer::examAllRightCallback(int kcid){
    if (kcid==S_KC->getLastKechengId()+1){
        m_tableView->reloadData();
    }
}

