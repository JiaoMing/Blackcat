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

#define XINGXING 100

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
    
    this->middleOffset();
    
    return true;
}


CCTableViewCell* KechengDialogTableLayer::tableCellAtIndex(CCTableView *table, unsigned int idx){
    CCTableViewCell* cell = table->dequeueCell();
    if (true){
        //  if (!cell){
        cell = new CCTableViewCell();
        cell->autorelease();
        
        
        CCSprite* sprite=CCSprite::create();
        
        CCLabelTTF *txtLabel = CCLabelTTF::create(CCString::createWithFormat("%d",idx+1)->getCString(), "KaiTi.ttf", 30.0);
        txtLabel->setPosition(ccp(50, 50));
        txtLabel->setColor(ccc3(252,111,25));
        
        CCSprite* xingxing=CCSprite::create();
        xingxing->setPosition(ccp(50, 50));
        xingxing->setTag(XINGXING);
        if (idx<=S_KC->getLastKechengId()) {
            xingxing->setDisplayFrame(S_SF->spriteFrameByName("xingxing_2.png"));
        }else{
            xingxing->setDisplayFrame(S_SF->spriteFrameByName("xingxing_1.png"));
        }
        
        sprite->addChild(xingxing);
        sprite->addChild(txtLabel);
        cell->addChild(sprite);
    }
    
    return cell;
}

void KechengDialogTableLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell){
    if(cell!=NULL&&this->isVisible()){
        int idx=cell->getIdx();
        
        if (idx+1<=S_KC->getLastKechengId()||(idx==S_KC->getLastKechengId())) {
            if (S_KC->getKcid()!=idx+1) {
                S_KC->changeKecheng(idx+1);
                m_delegate->freshKecheng(false);
            }
            S_AE->playEffect("renwu.mp3");
//            this->setVisible(false);
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

void KechengDialogTableLayer::middleOffset(){
    int index=S_KC->getKcid();
    float containerWidth=m_tableView->getViewSize().width;
    float cellWidth=this->cellSizeForTable(m_tableView).width;
    int showCount=(int)(containerWidth/cellWidth);
    if (index>showCount/2&&index<S_KC->getKcCount()-showCount/2-1) {
        m_tableView->setContentOffsetInDuration(ccp(-cellWidth*(index-showCount/2), 0), 0.5);
    }else{
        if (index>=S_KC->getKcCount()-showCount/2-1) {
            m_tableView->setContentOffset(m_tableView->minContainerOffset());
        }
    }
}

void KechengDialogTableLayer::openNewXingxing(){
    CCPoint lastOffset=m_tableView->getContentOffset();
    m_tableView->reloadData();
    m_tableView->setContentOffset(lastOffset,false);
    this->middleOffset();
    CCTableViewCell* cell=m_tableView->cellAtIndex(S_KC->getKcid()-1);
    CCSprite* sprite=(CCSprite*)cell->getChildren()->objectAtIndex(0);
    CCSprite* xingxing=(CCSprite*)sprite->getChildByTag(XINGXING);
    xingxing->runAction(CCSequence::create(CCScaleTo::create(0.3,1.5),CCScaleTo::create(0.3,1),NULL));
}