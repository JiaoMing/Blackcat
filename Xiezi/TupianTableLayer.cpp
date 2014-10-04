#include "TupianTableLayer.h"
#include "ResManager.h"
#include "resource.h"
#include "DBManager.h"
#include "XieziScene.h"
#include "TupianCategory.h"

enum{
    kCellTupianSprite,
    kCellWenhao,
    kCellLabelLayer
};

TupianTableLayer::TupianTableLayer(){
    m_tupianVector=new vector<Tupian*>();
}

TupianTableLayer::~TupianTableLayer(){
    for (vector<Tupian *>::iterator it = m_tupianVector->begin(); it != m_tupianVector->end(); it ++)
        if (NULL != *it)
        {
            //            string path=CCFileUtils::sharedFileUtils()->getWritablePath()+(*it)->getPath();
            //            CCTextureCache::sharedTextureCache()->removeTextureForKey(path.c_str());
            delete *it;
            *it = NULL;
        }
    m_tupianVector->clear();
    CC_SAFE_DELETE(m_tupianVector);
}

TupianTableLayer* TupianTableLayer::create(TupianTableDelegate* tupianTableDelegate,KapianDataMode mode,Kapian* kapian,TupianTablePosition tupianTablePosition){
    TupianTableLayer* layer=new TupianTableLayer();
    layer->m_kapian=kapian;
    layer->m_tupianTableDelegate=tupianTableDelegate;
    layer->m_mode=mode;
    layer->m_tupianTablePosition=tupianTablePosition;
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


bool TupianTableLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    this->initDataFromDB();
    
    int count=(int)m_tupianVector->size();
    //数据加载完后回调，修改上下箭头
    m_tupianTableDelegate->tupianLoadCallBack(count);
    
    if (count>0) {
        m_pTableView = CCTableView::create(this, m_tupianTablePosition.getTableSize());
        m_pTableView->setPosition(m_tupianTablePosition.getTablePoint());
        
        int cellWidth=m_tupianTablePosition.getCellSize().width;
        int tableWidth=m_tupianTablePosition.getTableSize().width;
        int cellHeight=m_tupianTablePosition.getCellSize().height;
        int tableHeight=m_tupianTablePosition.getTableSize().height;
        int scrollCount=0;
        
        m_pTableView->setDirection(m_tupianTablePosition.getDirection());
        if (m_tupianTablePosition.getDirection()==kCCScrollViewDirectionVertical) {
            m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
            scrollCount=tableHeight/cellHeight;
        }else{
            scrollCount=tableWidth/cellWidth;
            if (count<scrollCount) {
                float left=(tableWidth-count*cellWidth)/2;
                //                m_pTableView->setPosition(ccp(-left, m_tupianTablePosition.getTablePoint().y));
                m_pTableView->setViewSize(CCSizeMake(count*cellWidth, tableHeight));
                m_pTableView->setPosition(ccp(left,m_tupianTablePosition.getTablePoint().y));
            }
        }
        
        if(count<scrollCount){
            m_pTableView->setBounceable(false);
        }
        m_pTableView->setDelegate(this);
        
        this->addChild(m_pTableView);
        m_pTableView->reloadData();
    }
    
    return true;
}

void TupianTableLayer::initDataFromDB(){
    CLAUSE_INIT;
    switch (m_mode) {
        case kHanzi:{
            whereClause.push_back(TUPIAN_VERIFY_PASS);
            string like="ci like '%"+m_kapian->getText()+"%'";
            whereClause.push_back(like.c_str());
            S_DM->findScrollData(m_tupianVector,"*",whereClause, orderbyClause, groupByClause);
        }
            break;
            
        case kTupian:{
            CCString* where=CCString::createWithFormat("catid in (select catid from ciyu_cat where cid=%d)",m_kapian->getid());
            whereClause.push_back(where->getCString());
            vector<TupianCategory*>* m_tupianCatVector=new vector<TupianCategory*>();
            S_DM->findScrollData(m_tupianCatVector,"*",whereClause, orderbyClause, groupByClause);
            
            if (m_tupianCatVector->size()>0) {
                char ids[1000]="";
                int i=0;
                for (vector<TupianCategory *>::iterator it = m_tupianCatVector->begin(); it != m_tupianCatVector->end(); it ++){
                    if (i==0) {
                        sprintf(ids, "%d",(*it)->getcid());
                    }
                    sprintf(ids, "%s,%d",ids,(*it)->getcid());
                    i++;
                }
                
                whereClause.clear();
                whereClause.push_back(TUPIAN_VERIFY_PASS);
                CCString* where2=CCString::createWithFormat("id in (%s)",ids);
                whereClause.push_back(where2->getCString());
                S_DM->findScrollData(m_tupianVector,"*",whereClause, orderbyClause, groupByClause);
                
            }
            DELETE_MODEL_VECTOR(TupianCategory, m_tupianCatVector);
            
        }
            break;
        default:
            break;
    }
}

void TupianTableLayer::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    if(cell!=NULL){
        Tupian* tupian=(*m_tupianVector)[cell->getIdx()];
        bool isOverLimit=false;
        if (!tupian->getisCollected()) {
            int count=S_DM->getCount("ciyu","isCollected>0");
            if (count>=COLLECT_LIMIT) {
                isOverLimit=true;
            }else{
                CCNode* label=cell->getChildByTag(kCellLabelLayer);
                if(label)label->setVisible(true);
                CCNode* wenhao=cell->getChildByTag(kCellWenhao);
                CCSprite* tupianSprite=(CCSprite*)cell->getChildByTag(kCellTupianSprite);
                tupianSprite->setVisible(false);
                wenhao->runAction(CCSequence::create(CCOrbitCamera::create(0.3, 1, 0, 0, -90, 0, 0),CCHide::create(),NULL));
                tupianSprite->runAction(CCSequence::create(CCDelayTime::create(0.3),CCShow::create(),CCOrbitCamera::create(0.3, 1, 0, 90, -90, 0, 0),NULL));
            }
        }
        m_tupianTableDelegate->tupianTouchCallBack(tupian,isOverLimit);
    }
}

CCSize TupianTableLayer::cellSizeForTable(CCTableView *table)
{
    return m_tupianTablePosition.getCellSize();
}

CCTableViewCell* TupianTableLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell *cell = table->dequeueCell();
    
    if (true) {
        
        cell = new CCTableViewCell();
        cell->autorelease();
        
        Tupian* tupian=(*m_tupianVector)[idx];
        
        if (tupian!=NULL) {
            CCPoint cardPoint=S_RM->getRelativePosition("xiezi_Tupiankuang_in_cell",m_tupianTablePosition.getCellSize().height);
            
            CCSprite* kuang=CCSprite::createWithSpriteFrameName("lvkuang.png");
            kuang->setPosition(cardPoint);
            cell->addChild(kuang,2);
            
            string path=CCFileUtils::sharedFileUtils()->getWritablePath()+tupian->getPath();
            
            CCSize cellSize=m_tupianTablePosition.getCellThumbSize();
            CCSprite* tupianSprite=CCSprite::create(path.c_str());
            CCSize size=tupianSprite->getContentSize();
            tupianSprite->setScale(cellSize.width/size.width);
            
            CCPoint thumbPoint=S_RM->getRelativePosition("xiezi_Tupianthumb_in_cell",m_tupianTablePosition.getCellSize().height);
            tupianSprite->setPosition(thumbPoint);
            cell->addChild(tupianSprite,1,kCellTupianSprite);
            
            CCSprite* wenhao=CCSprite::createWithSpriteFrameName("lvkuang_3.png");
            wenhao->setPosition(thumbPoint);
            wenhao->setScale(0.95);
            cell->addChild(wenhao,1,kCellWenhao);
            
            CCSprite* bg=CCSprite::createWithSpriteFrameName("lvkuang_2.png");
            bg->setScale(0.95);
            bg->setPosition(thumbPoint);
            cell->addChild(bg,0);
            
            
            if (m_mode==kHanzi) {
                string ciyu=tupian->getci();
                const char* zi=ciyu.c_str();
                char p[ciyu.length()/3][4];
                for (int i = 0,j=0; i < ciyu.length(); i=i+3,j++)
                {
                    char end='\0';
                    sprintf(p[j], "%c%c%c%c",zi[i],zi[i+1],zi[i+2],end);
                }
                CCLayer* labelLayer=CCLayer::create();
                for (int i=0; i< ciyu.length()/3;i++) {
                    CCLabelTTF *txtLabel = CCLabelTTF::create(p[i], "KaiTi.ttf", 30.0);
                    txtLabel->setColor(ccc3(255,255,255));
                    txtLabel->enableShadow(CCSizeMake(2, -2), 2, 0.5);
                    if (m_mode==kHanzi&&p[i]==m_kapian->getText()) {
                        txtLabel->setColor(ccc3(0,255,0));
                    }
                    txtLabel->setPosition(ccp(i*30,0));
                    labelLayer->addChild(txtLabel);
                }
                labelLayer->setPosition(ccp(78-(ciyu.length()/3-1)*15,36));
                cell->addChild(labelLayer,2,kCellLabelLayer);
                
                if (tupian->getisCollected()<=0) {
                    labelLayer->setVisible(false);
                }
                
            }
            
            if (tupian->getisCollected()>0) {
                wenhao->setVisible(false);
            }
            
            
        }
        
    }
    else
    {
        
    }
    
    
    return cell;
}

unsigned int TupianTableLayer::numberOfCellsInTableView(CCTableView *table)
{
    return m_tupianVector->size();
}

/**
 * 暂未考虑数据增前的情况，只用于刷新收藏
 */
void TupianTableLayer::reloadData(){
    CCPoint point=m_pTableView->getContentOffset();
    m_pTableView->reloadData();
    m_pTableView->setContentOffset(point);
    
}

void TupianTableLayer::setHandlerPriority(int newPriority)
{
    if(m_tupianVector->size()>0){
        CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
        pDispatcher->setPriority(newPriority, m_pTableView);
    }
}

void TupianTableLayer::scrollViewDidScroll(cocos2d::extension::CCScrollView *view){
    float y=view->getContentOffset().y;
    float topY=m_tupianTablePosition.getTableSize().height-m_tupianTablePosition.getCellSize().height*m_tupianVector->size();
    ScrollState state;
    if (y==0) {
        state=kScrollStateBottom;
    }else if(y==topY){
        state=kScrollStateTop;
    }else{
        state=kScrollStatemiddle;
    }
    m_tupianTableDelegate->tupianTableScrollCallBack(state,m_tupianVector->size());
}

void TupianTableLayer::scroll(ScrollAction action){
    CCPoint point=m_pTableView->getContentOffset();
    float y=point.y-m_tupianTablePosition.getTableSize().height;
    int number=((int)y)/((int)(m_tupianTablePosition.getCellSize().height));
    switch (action) {
        case kScrollActionDown:
            if(number<0 )number+=1;
            break;
        case kScrollActionUp:
            if(number>-m_tupianVector->size())number-=1;
            break;
    }
    
    m_pTableView->setContentOffset(ccp(point.x,number*m_tupianTablePosition.getCellSize().height+m_tupianTablePosition.getTableSize().height),true);
}
