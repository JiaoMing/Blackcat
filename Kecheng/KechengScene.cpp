//
//  KechengScene.cpp
//  Blackcat
//
//  Created by haojunhua on 13-10-8.
//
//

#include "KechengScene.h"
#include "resource.h"
#include "XieziScene.h"
#include "KechengListScene.h"
#include "Kecheng.h"
#include "Utils.h"
#include "HomeScene.h"

#define TAG_EXAM -2
#define TAG_BACK -3
#define TAG_MENU -4
#define TAG_KUANGZI -5
enum TAG {
    kTagBadge=0,
    kTagCaidai,
    kTagBiankuang,
    kTagJiangpai
    
};

KechengScene::KechengScene(int kcid,KechengSceneDelegate* kechengSceneDelegate){
    m_kcid=kcid;
    m_hanziVector=new vector<Hanzi*>();
    m_kechengSceneDelegate=kechengSceneDelegate;
    m_hids=vector<int>();
}

KechengScene::~KechengScene(){
    DELETE_MODEL_VECTOR(Hanzi, m_hanziVector);
}


CCScene* KechengScene::scene(int kcid,KechengSceneDelegate* kechengSceneDelegate)
{
    CCScene *scene = CCScene::create();
    KechengScene *layer = new KechengScene(kcid,kechengSceneDelegate);
    if (layer && layer->init())
    {
        layer->autorelease();
    }
    else
    {
        delete layer;
        layer = NULL;
    }
    
    scene->addChild(layer);
    return scene;
}

bool KechengScene::init()
{
    if ( !KechengBaseLayer::init() )
    {
        return false;
    }
    
    
    CCMenu* menu=CCMenu::create();
    menu->setPosition(CCPointZero);
    menu->setTag(TAG_MENU);
    
    
    //任务框
    CCSprite* pTableBg = CCSprite::createWithSpriteFrameName("renwu_renwukuang_2.png");
    pTableBg->setPosition(S_RM->getPositionWithName("renwu_list_bg"));
    this->addChild(pTableBg);
    
    //返回按钮
    CCSprite* pFanhui_1=CCSprite::createWithSpriteFrameName("fanhui.png");
    CCMenuItemSprite* pbackItem=CCMenuItemSprite::create(pFanhui_1, pFanhui_1, this, menu_selector(KechengScene::menuCallback));;
    pbackItem->setPosition(S_RM->getPositionWithName("global_back"));
    pbackItem->setTag(TAG_BACK);
    menu->addChild(pbackItem);
    
    CLAUSE_INIT;
    CCString* where=CCString::createWithFormat("kcid=%d",m_kcid);
    whereClause.push_back(where->getCString());
    S_DM->findScrollData(m_hanziVector,"id,zi,lastAnswer,cnAudioPath",whereClause, orderbyClause, groupByClause);
    
    //汉字背景框层
    CCLayer* kuangziLayer=CCLayer::create();
    kuangziLayer->setPosition(CCPointZero);
    kuangziLayer->setTag(TAG_KUANGZI);
    this->addChild(kuangziLayer);
    
    int i=0;
    int lastKechengId=S_UD->getIntegerForKey(LAST_KECHENG_ID);
    float delay=0.2f;
    for (vector<Hanzi *>::iterator it = m_hanziVector->begin(); it != m_hanziVector->end(); it ++){
        if (NULL != *it){
            i++;
            CCSprite* kuangzi=CCSprite::create();
            if (m_kcid<=lastKechengId) {
                kuangzi->setDisplayFrame(S_SF->spriteFrameByName("renwu_lankuangzi.png"));
                CCSprite* badge=CCSprite::createWithSpriteFrameName("renwu_badge.png");
                CCSize size=S_RM->getSizeWithName("renwu_zi_kuangzi_badge_offset");
                badge->setPosition(ccp(size.width, size.height));
                badge->setScale(0.9);
                badge->setTag(kTagBadge);
                kuangzi->addChild(badge);
            }else{
                kuangzi->setDisplayFrame(S_SF->spriteFrameByName("renwu_huangkuang.png"));
            }
            CCString* ziPointStr=CCString::createWithFormat("renwu_zi_%d",i);
            CCPoint ziPoint=S_RM->getPositionWithName(ziPointStr->getCString());
            kuangzi->setPosition(ziPoint);
            kuangzi->setTag((*it)->getid());
            
            kuangziLayer->addChild(kuangzi);
            
            CCLabelTTF *txtLabel = CCLabelTTF::create((*it)->getText().c_str(), "KaiTi.ttf", 80.0);
            txtLabel->setColor(ccc3(0, 0, 0));
            CCMenuItemLabel* labelItem=CCMenuItemLabel::create(txtLabel, this,menu_selector(KechengScene::menuCallback));
            labelItem->setPosition(ziPoint);
            labelItem->setTag((*it)->getid());
            menu->addChild(labelItem);
            
            if (m_kcid>lastKechengId){
                labelItem->runAction(CCSequence::create(CCDelayTime::create(delay),CCScaleTo::create(0.3, 1.5),CCScaleTo::create(0.3, 1),NULL));
                delay+=0.5;
            }
        }
    }
    CCString* title=CCString::createWithFormat("学习%lu个汉字",m_hanziVector->size());
    
    //任务描述
    CCLabelTTF *txtLabel = CCLabelTTF::create(title->getCString(), "KaiTi.ttf", 40, S_RM->getSizeWithName("renwu_miaoshu_size"),kCCTextAlignmentLeft);
    txtLabel->setColor(ccc3(82,0,0));
    txtLabel->setPosition(S_RM->getPositionWithName("renwu_miaoshu"));
    this->addChild(txtLabel);
    
    //测试按钮
    CCSprite* pexamNormal=CCSprite::createWithSpriteFrameName("renwu_kaishiceshi.png");
    CCSprite* pexamDisable=CCSprite::createWithSpriteFrameName("renwu_kaishiceshi_disable.png");
    CCMenuItemSprite* pexamItem=CCMenuItemSprite::create(pexamNormal, pexamNormal,pexamDisable, this, menu_selector(KechengScene::menuCallback));;
    pexamItem->setPosition(S_RM->getPositionWithName("renwu_ceshi_btn"));
    pexamItem->setTag(TAG_EXAM);
//    if(m_kcid>lastKechengId)pexamItem->setEnabled(false);
    menu->addChild(pexamItem);
    
    this->addChild(menu);
    
    
    //打开android按键响应
    this->setKeypadEnabled(true);
    
    
    return true;
}

void KechengScene::menuCallback(CCObject* object){
    CCNode* node=(CCNode*)object;
    int tag=node->getTag();
    if (tag>0) {
        m_heimao->action("heimao_stand");
//        S_AE->playEffect("renwu_hanzi.mp3");
        CCDirector::sharedDirector()->pushScene(XieziScene::scene(node->getTag()));
        
        CCNode* kuangziLayer=this->getChildByTag(TAG_KUANGZI);
        CCSprite* kuangzi=(CCSprite*)kuangziLayer->getChildByTag(tag);
        kuangzi->setDisplayFrame(S_SF->spriteFrameByName("renwu_lankuangzi.png"));
        
        if (m_hids.size()<6){
            int i=0;
            for (; i<m_hids.size(); i++) {
                if(m_hids[i]==tag)break;
            }
            if (i==m_hids.size()) {
                m_hids.push_back(tag);
                
            }
        }
        if (m_hids.size()>=6) {
            CCNode* menu=(CCNode*)this->getChildByTag(TAG_MENU);
            CCMenuItemSprite* pexamItem=(CCMenuItemSprite*)menu->getChildByTag(TAG_EXAM);
            pexamItem->setEnabled(true);
        }
        
        
    }else{
        m_heimao->action("heimao_stand");
        if (tag==TAG_EXAM) {
            CCDirector::sharedDirector()->pushScene(KechengExamScene::scene(this,m_hanziVector));
            
        }else if(tag==TAG_BACK){
            CCDirector::sharedDirector()->popScene();
        }
    }
}

void KechengScene::examAllRightCallback(){
    
    CCNode* kuangziLayer=this->getChildByTag(TAG_KUANGZI);
    
    //更新汉字背景框及数据库
    for (vector<Hanzi *>::iterator it = m_hanziVector->begin(); it != m_hanziVector->end(); it ++){
        if (NULL != *it){
            CCSprite* kuangzi=(CCSprite*)kuangziLayer->getChildByTag((*it)->getid());
//            kuangzi->setDisplayFrame(S_SF->spriteFrameByName("renwu_lankuangzi.png"));
//            CCSprite* badge=CCSprite::createWithSpriteFrameName("renwu_badge.png");
//            badge->setPosition(ccp(96, 96));
//            badge->setScale(0.9);
//            kuangzi->addChild(badge);
            CCNode* node=this->getChildByTag(kTagBadge);
            if (node!=NULL) {
                kuangzi->removeChild(node, false);
            }
            CCString* sql=CCString::createWithFormat("update hanzi set isReward=1,isCollected=1 where id=%d",(*it)->getid());
            S_DM->executeSql(sql->getCString());
        }
    }
    
    //回调列表页面
    m_kechengSceneDelegate->examAllRightCallback(m_kcid);
    
    //更新课程数据库
//    Kecheng* kecheng=new Kecheng();
//    kecheng->setIntid(0);
//    S_DM->getByKey(kecheng, m_kcid);
//    CCString* sql;
//    if (kecheng->getid()>0) {
//        sql=CCString::createWithFormat("update kecheng set star=%d where id=%d;",star,m_kcid);
//    }else{
//        sql=CCString::createWithFormat("insert into kecheng(star,id) values(%d,%d)",star,m_kcid);
//    }
//    S_DM->executeSql(sql->getCString());
    
    this->runRewardAnimate();
}

void KechengScene::runRewardAnimate(){
    float delay=0.0f;
    m_badgeCallBackCount=0;
    for (int i=0; i<6; i++) {
        CCSprite* jiangli=CCSprite::create();
        
        CCSprite* caidai=CCSprite::createWithSpriteFrameName("renwu_caidai.png");
        caidai->setTag(kTagCaidai);
        caidai->setOpacity(0);
        caidai->setPosition(ccp(-8, -8));
        jiangli->addChild(caidai);
        
        CCSprite* biankuang=CCSprite::createWithSpriteFrameName("renwu_jiangpai_biankuang.png");
        biankuang->setTag(kTagBiankuang);
        biankuang->setScale(0.1f);
        biankuang->setVisible(false);
        jiangli->addChild(biankuang);
        
        CCSprite* jiangpai=CCSprite::createWithSpriteFrameName("renwu_jiangpai.png");
        jiangpai->setTag(kTagJiangpai);
        jiangpai->setScale(1.0);
        jiangpai->setOpacity(0);
        jiangli->addChild(jiangpai);
        
        CCString* pointStr=CCString::createWithFormat("renwu_zi_%d",i+1);
        CCPoint point=S_RM->getPositionWithName(pointStr->getCString());
        CCSize offset=S_RM->getSizeWithName("renwu_zi_badge_offset");
        jiangli->setPosition(ccp(point.x+offset.width, point.y+offset.height));
        this->addChild(jiangli);
        
        CCArray* actionArray=CCArray::create();
        actionArray->addObject(CCDelayTime::create(delay));
        delay+=0.5f;
        actionArray->addObject(CCFadeIn::create(0.2));
        actionArray->addObject(CCSpawn::create(CCScaleTo::create(0.3, 0.08),CCRotateBy::create(0.3, 315),NULL));
        actionArray->addObject(CCCallFuncN::create(this, callfuncN_selector(KechengScene::runRewardAnimateCallBack)));
        CCFiniteTimeAction* sequence=CCSequence::create(actionArray);
        jiangpai->runAction(sequence);
    }
    
}

void KechengScene::runRewardAnimateCallBack(CCObject* obj){
    CCNode* node=(CCNode*)obj;
    CCSprite* jiangli=(CCSprite*)node->getParent();
    CCSprite* caidai=(CCSprite*)jiangli->getChildByTag(kTagCaidai);
    caidai->runAction(CCFadeIn::create(0.7));
    CCSprite* biankuang=(CCSprite*)jiangli->getChildByTag(kTagBiankuang);
    biankuang->setVisible(true);
    biankuang->runAction(CCSpawn::create(CCFadeOut::create(0.7),CCScaleTo::create(0.7, 1),NULL));
    S_AE->playEffect("renwu_gaizhang.mp3");
    m_badgeCallBackCount++;
    if (m_badgeCallBackCount==6) {
        GuideDialog* guideDialog=new GuideDialog();
        guideDialog->autorelease();
        guideDialog->setText("继续挑战?");
        guideDialog->setMode(kGuideDialogYesOrNo);
        m_gudieDialogLayer=GuideDialogLayer::create(kDialogWithText);
        m_gudieDialogLayer->setDelegate(this);
        this->addChild(m_gudieDialogLayer);
        m_gudieDialogLayer->setGuideDialogData(guideDialog);
    }
}

void KechengScene::keyBackClicked(){
    CCDirector::sharedDirector()->popScene();
}

void KechengScene::keyMenuClicked(){
    
}

void KechengScene::dialogCallBack(GuideDialogCMD cmd){
    m_gudieDialogLayer->removeFromParentAndCleanup(true);
    switch (cmd) {
        case kDialogCMDYes:{
            S_DR->popScene();
        }
            break;
        case kDialogCMDNo:{
            CCDirector::sharedDirector()->replaceScene(HomeScene::scene());
        }
            break;
        default:
            break;
    }
}