//
//  CardShelfScene.cpp
//  ecards
//
//  Created by 杨飞 on 13-10-9.
//

#include "CardShelfScene.h"
#include "CardManager.h"
#include "ResManager.h"
#include "resource.h"
#include "LoadingScene.h"
#include "HomeScene.h"
#include "CardListScene.h"
#include "GushiScene.h"
#include "Zi800Scene.h"

enum {
    kTagMenuItemShelfExit,
    kTagMenu
};

USING_NS_CC;
USING_NS_CC_EXT;

CardShelfScene::CardShelfScene()
{
    S_RM->addSceneRes("CardSets", "ecards");
    S_UD->setIntegerForKey("EVENT_OPEN_CARDSET", 1);
}

CardShelfScene::~CardShelfScene()
{
    
}

CCScene* CardShelfScene::scene()
{
    CCScene *scene = CCScene::create();
    CardShelfScene *layer = CardShelfScene::create();
    scene->addChild(layer);
    return scene;
}

bool CardShelfScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSprite *ShelfTile = CCSprite::createWithSpriteFrameName("Shelf_bg.png");
    ShelfTile->setPosition(S_RM->getJpgBgPosition());
    this->addChild(ShelfTile, 1);
    
    CCMenu* pMenu = CCMenu::create(NULL);
    pMenu->setPosition( CCPointZero );
    CCSprite *spriteShelfExitUnpressed=CCSprite::createWithSpriteFrameName("fanhui.png");
    CCMenuItemSprite *menuItem_exit = CCMenuItemSprite::create(
                                                               spriteShelfExitUnpressed,
                                                               NULL,
                                                               this,
                                                               menu_selector(CardShelfScene::menuCallback) );
    
    menuItem_exit->setPosition(S_RM->getPositionWithName("global_back"));
    pMenu->addChild(menuItem_exit, 1, kTagMenuItemShelfExit);
    this->addChild(pMenu, 3,kTagMenu);
    
    m_pTableView = KEGridView::create(this,S_RM->getSizeWithName("CardShelf_size"),this,S_RM->getSizeWithName("CardShelf_gridSize"));
    m_pTableView->setDirection(kCCScrollViewDirectionVertical);
    m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    m_pTableView->setPosition(S_RM->getPositionWithName("CardShelf_position"));
    
//    if (this->numberOfCellsInTableView(m_pTableView)<3) {
//        CCPoint tablePoint=S_RM->getPositionWithName("CardShelf_position");
//        float y=(S_RM->getSizeWithName("CardShelf_size").height-this->numberOfCellsInTableView(m_pTableView)*S_RM->getSizeWithName("CardShelf_cellSize").height)/2;
//        m_pTableView->setPosition(ccp(tablePoint.x,  -y));
//        m_pTableView->setBounceable(false);
//    }
    
    this->addChild(m_pTableView, 2);
    
    
    //打开android按键响应
    this->setKeypadEnabled(true);
    
    return true;
}

void CardShelfScene::onEnter(){
    CCLayer::onEnter();
    BaiduStat::onStatEvent(kBaiduOneEventStart,"SceneRetain","CardShelfScene");
}

void CardShelfScene::onExit(){
    CCLayer::onExit();
    BaiduStat::onStatEvent(kBaiduOneEventEnd,"SceneRetain","CardShelfScene");
}

void CardShelfScene::gridCellTouched(CCTableView* table, CCTableViewCell* cell,int indexInIdx)
{
    int idx=cell->getIdx();
    int index=idx*3+indexInIdx;
    if (index<S_CM->getECardSets()->size()) {
        S_CM->setCurrentECardSet(index);
        if (CURRENT_ECARDSET->getisLock()==0) {
            switch (CURRENT_ECARDSET->gettype()) {
                case kECardSetType0:
                    S_DR->replaceScene(CardListScene::scene());
                    break;
                case kECardSetType1:
                    S_DR->replaceScene(Zi800Scene::scene());
                    break;
                case kECardSetType2:
                    S_DR->replaceScene(GushiScene::scene());
                    break;
                    
                default:
                    break;
            }
            S_AE->playEffect("renwu.mp3");
        }else{
            S_AE->playEffect("default.mp3");
        }
    }
}

CCSize CardShelfScene::cellSizeForTable(CCTableView *table)
{
    return S_RM->getSizeWithName("CardShelf_cellSize");
}

CCTableViewCell* CardShelfScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell *cell = table->dequeueCell();
    if (true){
        //  if (!cell){
        
        cell = new CCTableViewCell();
        cell->autorelease();
        CCSprite *pEBookCell = CCSprite::create();
        pEBookCell->setPosition( CCPointZero );
        CCSprite *pShelfBody = CCSprite::createWithSpriteFrameName("Shelf_ban.png");
        pShelfBody->setPosition(S_RM->getRelativePosition("CardShelf_bodyPosition", S_RM->getSizeWithName("CardShelf_cellSize").height));
        cell->addChild(pShelfBody, 0);
        
        int cellsNum=(int)S_CM->getECardSets()->size()/3;
        int separatedCellItemsNum=S_CM->getECardSets()->size()%3;
        if (separatedCellItemsNum!=0) {
            cellsNum++;
        }
        if (idx<cellsNum) {
            int gridCellCount=0;
            if (idx < cellsNum-1||separatedCellItemsNum==0) {
                gridCellCount=3;
            }else{
                gridCellCount=separatedCellItemsNum;
            }
            
            for (int i=0; i<=gridCellCount; i++) {
                int index=idx*3+i;
                if (S_CM->getECardSets()->size()>index) {
                    ECardSet* ecardset=(*S_CM->getECardSets())[index];
                    CCSprite *spriteTuce = CCSprite::create(CCString::createWithFormat("%secards/shu_%d.png", CCFileUtils::sharedFileUtils()->getWritablePath().c_str(),ecardset->getid())->getCString());
                    spriteTuce->setAnchorPoint( ccp(0.5,0) );
                    spriteTuce->setPosition(ccp(S_RM->getSizeWithName("CardShelf_gridSize").width/2+S_RM->getSizeWithName("CardShelf_gridSize").width*i, S_RM->getRelativePosition("CardShelf_bodyPosition", S_RM->getSizeWithName("CardShelf_gridSize").height*1.02).y));//1.02卡片略微靠后
                    
                    pEBookCell->addChild(spriteTuce);
                }
            }
            cell->addChild(pEBookCell);
        }
    }
    
    return cell;
}

unsigned int CardShelfScene::numberOfCellsInTableView(CCTableView *table)
{
    int cellsNum=(int)S_CM->getECardSets()->size()/3;
    int separatedCellItemsNum=S_CM->getECardSets()->size()%3;
    if (separatedCellItemsNum!=0) {
        cellsNum++;
    }
//    return (cellsNum<3)?3:cellsNum;
    return cellsNum;
}

void CardShelfScene::menuCallback(CCObject* pSender)
{
    CCMenuItem *menuItem = (CCMenuItem *)pSender;
    if (menuItem->getTag()==kTagMenuItemShelfExit) {
        S_RM->removeSceneRes("CardSets");
        S_DR->replaceScene(HomeScene::scene());
//        S_DR->replaceScene(LoadingScene::scene("HomeScene",true));
    }
}

void CardShelfScene::keyBackClicked(){
    CCMenu* menu=(CCMenu*)this->getChildByTag(kTagMenu);
    this->menuCallback(menu->getChildByTag(kTagMenuItemShelfExit));
}

void CardShelfScene::keyMenuClicked(){
    
}