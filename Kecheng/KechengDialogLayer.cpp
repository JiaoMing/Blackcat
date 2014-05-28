//
//  KechengFloatLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-5-22.
//
//

#include "KechengDialogLayer.h"
#include "resource.h"
#include "KechengExamScene.h"
#include "XieziCoverLayer.h"

#define TAG_EXAM -2
#define TAG_KUANGZI -5
enum TAG {
    kTagBadge=0,
    kTagCaidai,
    kTagBiankuang,
    kTagJiangpai
};

KechengDialogLayer::KechengDialogLayer(){
    m_hanziVector=new vector<Hanzi*>();
}

KechengDialogLayer::~KechengDialogLayer(){
    DELETE_MODEL_VECTOR(Hanzi, m_hanziVector);
}

bool KechengDialogLayer::init(){
    if (!DialogLayer::init(ccc4(0, 0, 0, 180))) {
        return false;
    }
    this->setTitle("任 务");
    
    
    m_bg->setDisplayFrame(S_SF->spriteFrameByName("renwu_kuang.png"));
    m_bg->setPosition(S_RM->getPositionWithName("renwu_kuang"));
    
    m_title->setFontSize(45);
    
    CCLayer* kuangziLayer=CCLayer::create();
    kuangziLayer->setPosition(CCPointZero);
    kuangziLayer->setTag(TAG_KUANGZI);
    this->addChild(kuangziLayer);
    
    
    m_menu=CCMenu::create();
    m_menu->setPosition(CCPointZero);
    this->addChild(m_menu);
    
    //测试按钮
    CCSprite* pexamNormal=CCSprite::createWithSpriteFrameName("renwu_ceshianniu.png");
    CCSprite* pexamDisable=CCSprite::createWithSpriteFrameName("renwu_ceshianniu.png");
    CCMenuItemSprite* pexamItem=CCMenuItemSprite::create(pexamNormal, pexamNormal,pexamDisable, this, menu_selector(KechengDialogLayer::menuCallback));;
    pexamItem->setPosition(S_RM->getPositionWithName("renwu_ceshianniu"));
    pexamItem->setTag(TAG_EXAM);
    this->addMenuItem(pexamItem);
    
    
    m_kcid=S_UD->getIntegerForKey(LAST_KECHENG_ID,0);
    if (m_kcid==0) {
        m_kcid=1;
    }
    this->setKecheng(m_kcid);
    
    CCSprite* xingxing=CCSprite::createWithSpriteFrameName("renwu_xing3.png");
    CCString* str=CCString::createWithFormat("%d",m_kcid);;
    CCMenuItemSprite* item=CCMenuItemSprite::create(xingxing,xingxing, this, menu_selector(KechengDialogLayer::showOrHideTable));
    item->setPosition(S_RM->getPositionWithName("renwu_xing3"));
    this->addMenuItem(item);
    int fontSize=40;
    if (m_kcid>9) {
        fontSize=30;
    }
    CCLabelTTF* kecheng=CCLabelTTF::create(str->getCString(), "", fontSize);
    kecheng->setFontFillColor(ccc3(255, 255, 255));
    kecheng->enableStroke(ccc3(0, 0, 0), 1);
    kecheng->setPosition(S_RM->getPositionWithName("renwu_xing3_zi"));
    this->addChild(kecheng);
    
    m_tableLayer=KechengDialogTableLayer::create(this);
    m_tableLayer->setVisible(false);
    this->addChild(m_tableLayer);
    
    return true;
}

void KechengDialogLayer::enableTouch(){
    DialogLayer::enableTouch();
    m_tableLayer->setHandlerPriority(this->getDelegate()->topHandlerPriority()-1);
    m_menu->setHandlerPriority(this->getDelegate()->topHandlerPriority()-1);
}

void KechengDialogLayer::showOrHideTable(){
    if (m_tableLayer->isVisible()) {
        m_tableLayer->setVisible(false);
    }else{
        m_tableLayer->setVisible(true);
    }
}

void KechengDialogLayer::setKecheng(int kcid){
//    if (kcid==m_kcid) {
//        return;
//    }
    m_kcid=kcid;
    
    
    //可修改为只清空数据
    DELETE_MODEL_VECTOR(Hanzi, m_hanziVector);
    m_hanziVector=new vector<Hanzi*>();
    
    CLAUSE_INIT;
    CCString* where=CCString::createWithFormat("kcid=%d",m_kcid);
    whereClause.push_back(where->getCString());
    
    S_DM->findScrollData(m_hanziVector,"id,zi,lastAnswer,cnAudioPath",whereClause, orderbyClause, groupByClause);
    
    
    CCLayer* kuangziLayer=(CCLayer*)this->getChildByTag(TAG_KUANGZI);
    kuangziLayer->removeAllChildren();
    
    m_menu->removeAllChildren();
    
    int i=0;
    int lastKechengId=S_UD->getIntegerForKey(LAST_KECHENG_ID,0);
    float delay=0.2f;
    for (vector<Hanzi *>::iterator it = m_hanziVector->begin(); it != m_hanziVector->end(); it ++){
        if (NULL != *it){
            i++;
            CCSprite* kuangzi=CCSprite::create();
            kuangzi->setDisplayFrame(S_SF->spriteFrameByName("renwu_zikuang.png"));
            if (m_kcid<=lastKechengId) {
//                CCSprite* badge=CCSprite::createWithSpriteFrameName("renwu_badge.png");
//                CCSize size=S_RM->getSizeWithName("renwu_zi_kuangzi_badge_offset");
//                badge->setPosition(ccp(size.width, size.height));
//                badge->setScale(0.9);
//                badge->setTag(kTagBadge);
//                kuangzi->addChild(badge);
            }
            CCString* ziPointStr=CCString::createWithFormat("renwu_dialog_zi_%d",i);
            CCPoint ziPoint=S_RM->getPositionWithName(ziPointStr->getCString());
            kuangzi->setPosition(ziPoint);
            kuangzi->setTag((*it)->getid());
            
            kuangziLayer->addChild(kuangzi);
            
            CCLabelTTF *txtLabel = CCLabelTTF::create((*it)->getText().c_str(), "KaiTi.ttf", 80.0);
            txtLabel->setColor(ccc3(0, 0, 0));
            CCMenuItemLabel* labelItem=CCMenuItemLabel::create(txtLabel, this,menu_selector(KechengDialogLayer::menuCallback));
            labelItem->setPosition(ziPoint);
            labelItem->setTag((*it)->getid());
            m_menu->addChild(labelItem);
//            this->addMenuItem(labelItem);
            
            if (m_kcid>lastKechengId){
                labelItem->runAction(CCSequence::create(CCDelayTime::create(delay),CCScaleTo::create(0.3, 1.5),CCScaleTo::create(0.3, 1),NULL));
                delay+=0.5;
            }
        }
    }
}

void KechengDialogLayer::menuCallback(CCObject* object){
    if(!this->isCloseItem(object)){
        CCNode* node=(CCNode*)object;
        int tag=node->getTag();
        if (tag>0) {
            S_AE->playEffect("renwu_hanzi.mp3");
            
            m_tableLayer->setVisible(false);
            XieziCoverLayer* xieziCoverLayer=XieziCoverLayer::create(node->getTag());
            xieziCoverLayer->setDelegate(this);
            this->addChild(xieziCoverLayer);
            
            UserBarLayer* userbarLayer=S_LM->getDelegate();
            userbarLayer->setZOrder(INT_MAX);
            
        }else{
            if (tag==TAG_EXAM) {
                S_DR->pushScene(KechengExamScene::scene(this,m_hanziVector));
            }
        }
    }else{
        this->removeFromParent();
    }
    
}


void KechengDialogLayer::examAllRightCallback(){
    
    //星星控制点数组
    CCPointArray* array=CCPointArray::create(m_hanziVector->size());
    
    int i=0;
    //更新汉字背景框及数据库
    for (vector<Hanzi *>::iterator it = m_hanziVector->begin(); it != m_hanziVector->end(); it ++){
        if (NULL != *it){
//            CCSprite* kuangzi=(CCSprite*)kuangziLayer->getChildByTag((*it)->getid());
//            //            kuangzi->setDisplayFrame(S_SF->spriteFrameByName("renwu_lankuangzi.png"));
//            //            CCSprite* badge=CCSprite::createWithSpriteFrameName("renwu_badge.png");
//            //            badge->setPosition(ccp(96, 96));
//            //            badge->setScale(0.9);
//            //            kuangzi->addChild(badge);
//            CCNode* node=this->getChildByTag(kTagBadge);
//            if (node!=NULL) {
//                kuangzi->removeChild(node, false);
//            }
            CCString* sql=CCString::createWithFormat("update hanzi set isReward=1,isCollected=1 where id=%d",(*it)->getid());
            S_DM->executeSql(sql->getCString());
            
            //添加星星控制点
            CCString* point=CCString::createWithFormat("renwu_dialog_zi_%d",++i);
            array->addControlPoint(S_RM->getPositionWithName(point->getCString()));
        }
    }
    S_LM->gain("RENWU",S_RM->getJpgBgPosition(),array);
    
    m_tableLayer->examAllRightCallback(m_kcid);
}

