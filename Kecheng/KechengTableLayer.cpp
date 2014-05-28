//
//  ListTableLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 13-10-8.
//
//

#include "KechengTableLayer.h"
#include "resource.h"
#include "ResManager.h"
#include "DBManager.h"
#include "SimpleAudioEngine.h"

#define COLUMN_NUMBER 3
#define TAG_TABLE 1
#define TAG_KUANG 2
#define TAG_LABEL 3

KechengTableLayer::KechengTableLayer(){
}

KechengTableLayer::~KechengTableLayer(){
}

KechengTableLayer* KechengTableLayer::create(KechengTableDelegate* kechengTableDelegate){
    KechengTableLayer* layer=new KechengTableLayer();
    layer->m_kechengTableDelegate=kechengTableDelegate;
    if (layer && layer->init())
    {
        layer->autorelease();
    }
    else
    {
        delete layer;
        layer = NULL;
    }
    return layer;
}


bool KechengTableLayer::init(){
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

    KEGridView* tableView = KEGridView::create(this,S_RM->getSizeWithName("renwu_table_size"),this,S_RM->getSizeWithName("renwu_grid_cell_size"));
    tableView->setDirection(kCCScrollViewDirectionVertical);
	tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    tableView->setPosition(S_RM->getPositionWithName("renwu_table"));
    tableView->setClippingToBounds(true);
    tableView->setTag(TAG_TABLE);
    this->addChild(tableView);
    
    m_kechengTableDelegate->kechengLoadCallBack();
    
    return true;
}

void KechengTableLayer::onEnter(){
    CCLayer::onEnter();
}

CCTableViewCell* KechengTableLayer::tableCellAtIndex(CCTableView *table, unsigned int idx){
    CCTableViewCell *cell = table->dequeueCell();
    if (true){
        //  if (!cell){
        cell = new CCTableViewCell();
        cell->autorelease();
        
        for (int i=0; i<COLUMN_NUMBER; i++) {
            int index=idx*COLUMN_NUMBER+i;
            if (index>=m_kcCount) {
                break;//最后一行判断
            }
            
//            Kecheng* kecheng=(*m_kechengVector)[index];
            
            CCSprite* gridCell=CCSprite::create();
            
            CCPoint renwuPoint=S_RM->getRelativePosition("renwu_kuang_in_cell",S_RM->getSizeWithName("renwu_table_cell_size").height);
            
            CCSprite* kuang=CCSprite::create();
            kuang->setPosition(renwuPoint);
            CCLabelTTF *txtLabel = CCLabelTTF::create(CCString::createWithFormat("%d",index+1)->getCString(), "KaiTi.ttf", 60.0);
            txtLabel->setTag(TAG_LABEL);
            txtLabel->setPosition(renwuPoint);
            txtLabel->setColor(ccc3(252,111,25));
            gridCell->addChild(txtLabel,3);
            
            if (index+1<=m_lastKechengId) {
                kuang->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("renwu_lvkuang.png"));
                CCSprite* badge=CCSprite::createWithSpriteFrameName("renwu_badge.png");
                CCPoint badgePoint=S_RM->getRelativePosition("renwu_badge",S_RM->getSizeWithName("renwu_table_cell_size").height);
                badge->setPosition(badgePoint);
                gridCell->addChild(badge,3);
            }else if(index==m_lastKechengId&&m_dayRenwuCount<2){
                kuang->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("renwu_lvkuang.png"));
            }else{
                txtLabel->setVisible(false);
                kuang->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("renwu_huikuang.png"));
            }
            
            kuang->setTag(TAG_KUANG);
            gridCell->addChild(kuang,2);
            
            gridCell->setPosition(ccp(i*S_RM->getSizeWithName("renwu_grid_cell_size").width,0));
            
            cell->addChild(gridCell);
        }
        
    }
    
    return cell;
}

void KechengTableLayer::gridCellTouched(CCTableView* table, CCTableViewCell* cell,int indexInIdx){
    //计算元素序号
    int row=cell->getIdx();
    int column=indexInIdx;
    int index=COLUMN_NUMBER*row+column;
    if (index<=m_lastKechengId) {
        m_kechengTableDelegate->kechengTouchCallBack();
        CCDirector::sharedDirector()->pushScene(KechengScene::scene(index+1,this));
        S_ALP->play("renwu.mp3",NULL);
    }else{
        S_ALP->play("renwu_suo.mp3",NULL);
    }
}

CCSize KechengTableLayer::cellSizeForTable(CCTableView *table){
    return S_RM->getSizeWithName("renwu_table_cell_size");
}

unsigned int KechengTableLayer::numberOfCellsInTableView(CCTableView *table){
    return (m_kcCount%COLUMN_NUMBER)==0?m_kcCount/COLUMN_NUMBER:(m_kcCount/COLUMN_NUMBER+1);
}

void KechengTableLayer::examAllRightCallback(int kcid){
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
        
        CCTableView* tableView=(CCTableView*)this->getChildByTag(TAG_TABLE);
        tableView->reloadData();
    }
}