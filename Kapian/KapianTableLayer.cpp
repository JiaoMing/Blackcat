//
//  KapianTableLayer.cpp
//  blackcat
//
//  Created by haojunhua on 13-9-2.
//
//

#include "KapianTableLayer.h"
#include "ResManager.h"
#include "resource.h"
#include "DBManager.h"
#include "KapianScene.h"
#include "GraySprite.h"

#define COLUMN_NUMBER 6
#define CARD_CONTENT_TAG 10
#define CARD_PROGRESS_TAG 10

static void getProcess(const char* model,const char* column,int* count,int* sumCount){
    map<string, string>* data=new map<string, string>();
    map<string, string> expClause=map<string, string>();
    map<string, string>::iterator it;
    CCString* exp=CCString::createWithFormat("count(%s)",column);
    expClause.insert(pair<string, string>("count",exp->getCString()));
    exp=CCString::createWithFormat("Sum(case when %s>0 then 1 else 0 end)",column);
    expClause.insert(pair<string, string>("sumCount",exp->getCString()));
    S_DM->statistic(data, model, expClause);
    for(it=data->begin();it!=data->end();++it){
        if(strcmp(it->first.c_str(), "count")){
            *count=atoi(it->second.c_str());
        }else if(strcmp(it->first.c_str(), "sumCount")){
            *sumCount=atoi(it->second.c_str());
        }
        data->erase(it);
    }
    CC_SAFE_DELETE(data);
}


KapianTableLayer::KapianTableLayer(KapianDataMode dataMode){
    this->m_dataMode=dataMode;
    m_allVector=new vector<Kapian*>();
    m_cltVector=new vector<Kapian*>();
    m_kapianVector=m_cltVector;
    m_Offset=ccp(0, 1);//用于切换检测，offsetY应未非正数
}

KapianTableLayer::~KapianTableLayer(){
    if (m_dataMode==kTupian) {
        //移除缩略图纹理
//        for (vector<Kapian*>::iterator it = m_allVector->begin(); it != m_allVector->end(); it ++){
//            string path=CCFileUtils::sharedFileUtils()->getWritablePath()+(*it)->getPath();
//            CCTextureCache::sharedTextureCache()->removeTextureForKey(path.c_str());
//        }
    }
    RELEASE_MODEL_VECTOR(Kapian, m_allVector);
    CC_SAFE_DELETE(m_cltVector);//由于只添加了引用，只需要删除自身
    
    m_kapianVector=NULL;
}

KapianTableLayer* KapianTableLayer::create(KapianDataMode dataMode){
    KapianTableLayer* layer=new KapianTableLayer(dataMode);
    if (layer&&layer->init()) {
        layer->autorelease();
    }else{
        CC_SAFE_DELETE(layer);
    }
    return layer;
}

bool KapianTableLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    //初始化数据库
    this->initDataFromDB();
    
    CCSize tableSize=S_RM->getSizeWithName("kapian_table_size");
    CCPoint tableMiddlePoint=S_RM->getPositionWithName("kapian_table");
    CCPoint left_bottom=ccp(tableMiddlePoint.x-tableSize.width/2,tableMiddlePoint.y-tableSize.height/2);
    
    switch (m_dataMode) {
        case kHanzi:{
            m_gridSize=S_RM->getSizeWithName("kapian_hanzi_grid_cell_size");
            m_cellSize=S_RM->getSizeWithName("kapian_hanzi_table_cell_size");
        }
            break;
        case kTupian:{
            m_gridSize=S_RM->getSizeWithName("kapian_tupian_grid_cell_size");
            m_cellSize=S_RM->getSizeWithName("kapian_tupian_table_cell_size");
        }
            break;
    }
    
    m_pTableView = KEGridView::create(this,tableSize,this,m_gridSize);
    m_pTableView->setDirection(kCCScrollViewDirectionVertical);
	m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_pTableView->setPosition(left_bottom);
    m_pTableView->setClippingToBounds(true);
    this->addChild(m_pTableView);
    
    return true;
}

void KapianTableLayer::initDataFromDB(){
    CLAUSE_INIT;
    switch (m_dataMode) {
        case kHanzi:{
            whereClause.push_back(HANZI_VERIFY_PASS);
            orderbyClause.insert(pair<const char*, const char*>("bihuashu","asc"));
            S_DM->findScrollData((vector<Hanzi*>*)m_allVector,"id,zi,writeCount,isCollected,lastAnswer,isReward",whereClause, orderbyClause, groupByClause);
        }
            break;
        case kTupian:{
            whereClause.push_back(TUPIAN_VERIFY_PASS);
            orderbyClause.clear();
            orderbyClause.insert(pair<const char*, const char*>("id","desc"));
            S_DM->findScrollData((vector<Tupian*>*)m_allVector,"*",whereClause, orderbyClause, groupByClause);
        }
            break;
    }
    for (vector<Kapian*>::iterator it = m_allVector->begin(); it != m_allVector->end(); it ++){
        if ((*it)->getisCollected()>0)
        {
            m_cltVector->push_back((*it));
        }
    }
}


void KapianTableLayer::gridCellTouched(CCTableView* table, CCTableViewCell* cell,int indexInIdx)
{
    //j计算元素序号
    int row=cell->getIdx();
    int column=indexInIdx;
    int index=COLUMN_NUMBER*row+column;
    
    Kapian* kapian=(*m_kapianVector)[index];
    if(kapian->getisCollected()==1){
        
        switch (m_dataMode) {
            case kHanzi:{
                CCDirector::sharedDirector()->pushScene(XieziScene::scene(kapian->getid(),this,index));
            }
                break;
            case kTupian:{
                //图片浏览层
                TupianBrowserLayer* m_tupianBrowser=TupianBrowserLayer::create(this,(Tupian*)kapian);
                m_tupianBrowser->setPosition(CCPointZero);
                this->addChild(m_tupianBrowser,100);
            }break;
        }
    }else{
        S_ALP->play("heimao034.mp3",NULL);
    }
}

CCSize KapianTableLayer::cellSizeForTable(CCTableView *table)
{
    return m_cellSize;
}

CCTableViewCell* KapianTableLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    if (m_kapianVector->size()==0) {
        //没有元素
        CCTableViewCell* cell = new CCTableViewCell();
        cell->autorelease();
        CCLabelTTF* txtLabel = CCLabelTTF::create("还没有收集到卡片哦～", "KaiTi.ttf", 40.0);
        txtLabel->setColor(ccc3(0, 0, 0));
        txtLabel->setPosition(ccp(500,0));
        cell->addChild(txtLabel);
        return cell;
    }
    
    CCTableViewCell *cell = table->dequeueCell();
    if (true){
//  if (!cell){
        cell = new CCTableViewCell();
        cell->autorelease();
        
        for (int i=0; i<COLUMN_NUMBER; i++) {
            int index=idx*COLUMN_NUMBER+i;
            if (index>=m_kapianVector->size()) {
                break;//最后一行判断
            }
            
            CCSprite* gridCell=CCSprite::create();
            
            //设置卡片
            CCPoint cardPoint=S_RM->getRelativePosition("kapiankuang_in_cell",m_gridSize.height);
            
            Kapian* kapian=(*m_kapianVector)[index];
            switch (m_dataMode) {
                case kHanzi:{
                    CCSprite* wenzikuang=CCSprite::createWithSpriteFrameName("wenzikuang_1.png");
                    wenzikuang->setPosition(cardPoint);//相对于cell设置坐标
                    gridCell->addChild(wenzikuang);
                    
                    CCLabelTTF* txtLabel = CCLabelTTF::create(kapian->getText().c_str(), "KaiTi.ttf", 80.0);
                    if (kapian->getisCollected()) {
                        txtLabel->setColor(ccc3(0, 0, 0));
                    }else{
                        txtLabel->setColor(ccc3(200, 200, 200));
                    }
                    txtLabel->setPosition(cardPoint);
                    txtLabel->setTag(CARD_CONTENT_TAG);
                    gridCell->addChild(txtLabel);
                    
                    CCPoint midPoint=S_RM->getRelativePosition("kapian_jindu_in_cell",m_gridSize.height);
                    Hanzi* hanzi=(Hanzi*)kapian;
                    int starCount=hanzi->getwriteCount();
                    if (starCount>3) {
                        CCSprite* star=CCSprite::createWithSpriteFrameName("kapian_xingxing_1.png");
                        star->setPosition(ccpAdd(midPoint, ccp(-40, 0)));
                        gridCell->addChild(star);
                        
                        CCLabelTTF* x = CCLabelTTF::create("X", "Helvetica", 20.0);
                        x->setColor(ccc3(255,165,0));
                        x->setPosition(midPoint);
                        gridCell->addChild(x);
                        
                        CCLabelTTF *label = CCLabelTTF::create(CCString::createWithFormat("%d",starCount)->getCString(), "Helvetica", 30.0);
                        label->setColor(ccc3(255,100,0));
                        label->setPosition(ccpAdd(midPoint, ccp(40, 0)));
                        gridCell->addChild(label);
                        
                    }else{
                        for (int i=0; i<3; i++) {
                            CCSprite* star;
                            if (i+1<=starCount) {
                                star=CCSprite::createWithSpriteFrameName("kapian_xingxing_1.png");
                            }else{
                                star=CCSprite::createWithSpriteFrameName("kapian_xingxing_2.png");
                            }
                            star->setPosition(ccpAdd(midPoint, ccp((i-1)*40, 0)));
                            star->setScale(0.9);
                            gridCell->addChild(star);
                        }
                    }
                    
                    
                    if (hanzi->getisReward()>0) {
                        CCSprite* badge=CCSprite::createWithSpriteFrameName("badge.png");
                        badge->setPosition(S_RM->getRelativePosition("kapian_badge_in_gridcell",m_gridSize.height));
                        gridCell->addChild(badge);
                    }
                    
                    //设置进度
//                    CCSprite* pProgressBg1=CCSprite::createWithSpriteFrameName("card-learnedprogressbar-bg.png");
//                    CCPoint processPoint=S_RM->getRelativePosition("kapianjindu_in_cell",S_RM->getSizeWithName("kapian_grid_cell_size").height);
//                    pProgressBg1->setPosition(processPoint);
//                    gridCell->addChild(pProgressBg1);
//                    CCSprite* pProgressBg2=CCSprite::createWithSpriteFrameName("card-learnedprogressbar.png");
//                    CCProgressTimer* progressTimer = CCProgressTimer::create(pProgressBg2);
//                    progressTimer->setType(kCCProgressTimerTypeBar);
//                    progressTimer->setMidpoint(ccp(0, 0));
//                    progressTimer->setBarChangeRate(ccp(1, 0));
//                    progressTimer->setPercentage(kapian->getProgress()*100);
//                    progressTimer->setPosition(processPoint);
//                    progressTimer->setTag(CARD_PROGRESS_TAG);
//                    gridCell->addChild(progressTimer);
                }
                    break;
                    
                case kTupian:{
                    string path=CCFileUtils::sharedFileUtils()->getWritablePath()+kapian->getPath();
                    CCSize cellSize=S_RM->getSizeWithName("kapian_table_cell_thumb_size");
                 
                    CCSprite* tupianSprite;
                    if (kapian->getisCollected()) {
                        tupianSprite=CCSprite::create(path.c_str());
                    }else{
                        tupianSprite=GraySprite::create(path.c_str());
                    }
                    
                    if (tupianSprite!=NULL) {
                        CCSize size=tupianSprite->getContentSize();
                        tupianSprite->setScale(cellSize.width/size.width);//缩放
                        
                        CCPoint thumbPoint=S_RM->getRelativePosition("kapian_in_cell",m_gridSize.height);
                        tupianSprite->setPosition(thumbPoint);
                        tupianSprite->setTag(CARD_CONTENT_TAG);
                        gridCell->addChild(tupianSprite);
                        
                        CCSprite* tupiankuangzi_1=CCSprite::createWithSpriteFrameName("tupiankuang_1.png");
                        tupiankuangzi_1->setPosition(cardPoint);
                        gridCell->addChild(tupiankuangzi_1);
                    }
                    
                }
                    
                    break;
            }
            
            //添加gridcell，设置坐标
            gridCell->setPosition(ccp(i*m_gridSize.width,0));
            cell->addChild(gridCell);
        }
    }
    else{
//        CCArray* gridCells=cell->getChildren();
//        for (int i=0; i<gridCells->count(); i++) {
//            int index=idx*COLUMN_NUMBER+i;
//            CCSprite* gridCellSprite=(CCSprite*)gridCells->objectAtIndex(i);
//            
//            Kapian* kapian=(*m_kapianVector)[index];
//            switch (m_dataMode) {
//                case kHanzi:{
//                    CCLabelTTF *label = (CCLabelTTF*)gridCellSprite->getChildByTag(CARD_CONTENT_TAG);
//                    label->setString(kapian->getText().c_str());
//                }
//                    break;
//                    
//                case kTupian:{
//                    string path=CCFileUtils::sharedFileUtils()->getWritablePath()+kapian->getPath();
//                    CCSprite* tupian=(CCSprite*)gridCellSprite->getChildByTag(CARD_CONTENT_TAG);
//                    CCSize cellSize=S_RM->getSizeWithName("Tupian_table_cell_thumb_size");
//                    tupian->setDisplayFrame(CCSpriteFrame::create(path.c_str(),CCRectMake(0, 0, cellSize.width, cellSize.height)));
//                }
//                    
//                    break;
//            }
//        }
    }
    return cell;
}

unsigned int KapianTableLayer::numberOfCellsInTableView(CCTableView *table)
{
    int count=(int)m_kapianVector->size();
    if (count==0) {
        //没有元素，显示提示
        return 1;
    }
    return (count%COLUMN_NUMBER)==0?count/COLUMN_NUMBER:(count/COLUMN_NUMBER+1);
}

void KapianTableLayer::xieziCallback(int index,float progress){
    (*m_kapianVector)[index]->setProgress(progress);
    m_pTableView->reloadData();
}

void KapianTableLayer::changeShowMode(KapianShowMode showMode){
    switch (showMode) {
        case kCollect:
            m_kapianVector=m_cltVector;
            break;
        case kAll:
            m_kapianVector=m_allVector;
            break;
        default:
            break;
    }
    
    CCPoint lastOffset=m_Offset;
    m_Offset=m_pTableView->getContentOffset();
    m_pTableView->reloadData();
    if(lastOffset.y<=0)m_pTableView->setContentOffset(lastOffset,false);
    else m_pTableView->setContentOffset(m_pTableView->minContainerOffset(),false);
    m_pTableView->unscheduleAllSelectors();
    
}