//
//  KechengDialogTableLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-5-22.
//
//

#include "KechengDialogTableLayer.h"
#include "resource.h"


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
    
    map<string, string>* data=new map<string, string>();
    map<string, string> expClause=map<string, string>();
    expClause.insert(pair<string, string>("maxKcid","max(kcid)"));
    S_DM->statistic(data, "hanzi", expClause);
    
    m_kcCount=0;
    map<string, string>::iterator it;
    it=data->find("maxKcid");
    if(it!=data->end()){
        m_kcCount=atoi(it->second.c_str());
    }
    CC_SAFE_DELETE(data);
    
    m_lastKechengId=S_UD->getIntegerForKey(LAST_KECHENG_ID,0);
    
    //用于table查询，避免每次查文件
    m_dayRenwuCount=static_getDayRenwuCount();
    
    m_tableView = CCTableView::create(this,CCSizeMake(1024, 100));
    m_tableView->setDirection(kCCScrollViewDirectionHorizontal);
	m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_tableView->setPosition(ccp(0, 650));
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
        if (idx+1<=m_lastKechengId) {
            xingxing->setDisplayFrame(S_SF->spriteFrameByName("xingxing_2.png"));
        }else if(idx==m_lastKechengId&&m_dayRenwuCount<2){
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
    if(cell!=NULL){
        int idx=cell->getIdx();
        
        if (idx+1<=m_lastKechengId) {
            m_delegate->setKecheng(idx+1);
            S_ALP->play("renwu.mp3",NULL);
            
            this->setVisible(false);
        }else{
            S_ALP->play("renwu_suo.mp3",NULL);
        }
        
    }
}

CCSize KechengDialogTableLayer::cellSizeForTable(CCTableView *table){
    return CCSizeMake(100, 100);
}

unsigned int KechengDialogTableLayer::numberOfCellsInTableView(CCTableView *table){
    return m_kcCount;
}

void KechengDialogTableLayer::setHandlerPriority(int newPriority)
{
    if(m_kcCount>0){
        CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
        pDispatcher->setPriority(newPriority, m_tableView);
    }
}

void KechengDialogTableLayer::examAllRightCallback(int kcid){
    if (kcid==m_lastKechengId+1){
        int count=S_UD->getIntegerForKey(LAST_RENWU_DAY_COUNT);
        string time=TimeUtils::getYYYYMMDD();
        string lastTime=S_UD->getStringForKey(LAST_RENWU_DAY);
        if (time!=lastTime) {
            count=1;
            S_UD->setStringForKey(LAST_RENWU_DAY, time);
            S_UD->setIntegerForKey(LAST_RENWU_DAY_COUNT, 1);
        }else if (count<2) {
            S_UD->setIntegerForKey(LAST_RENWU_DAY_COUNT, ++count);
        }
        m_dayRenwuCount=count;
        S_UD->setIntegerForKey(LAST_KECHENG_ID, ++m_lastKechengId);
        S_UD->flush();
        
        m_tableView->reloadData();
    }
}

