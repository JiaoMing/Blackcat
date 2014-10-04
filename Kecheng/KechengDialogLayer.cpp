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
#include "KechengController.h"
#include "HomeScene.h"

#define TAG_EXAM -2
#define TAG_KUANGZI -5
enum TAG {
    kTagBadge=0,
    kTagCaidai,
    kTagBiankuang,
    kTagJiangpai
};

KechengDialogLayer::KechengDialogLayer(){
    m_jiangpaiArray=CCArray::create();
    m_jiangpaiArray->retain();
}

KechengDialogLayer::~KechengDialogLayer(){
    this->unschedule(schedule_selector(KechengDialogLayer::dingShiTiXing));
    m_jiangpaiArray->release();
}

KechengDialogLayer* KechengDialogLayer::create(Heimao* heimao){
    KechengDialogLayer* kechengDialogLayer=new KechengDialogLayer();
    kechengDialogLayer->m_heimao=heimao;
    kechengDialogLayer->init();
    kechengDialogLayer->autorelease();
    return kechengDialogLayer;
}


bool KechengDialogLayer::init(){
    if (!DialogLayer::init(ccc4(0, 0, 0, 180))) {
        return false;
    }
    
    this->setTitle("任  务");
    
    m_bg->setDisplayFrame(S_SF->spriteFrameByName("renwu_kuang.png"));
    m_bg->setPosition(S_RM->getPositionWithName("renwu_kuang"));
    
    m_title->setFontSize(45);
    
    CCLayer* kuangziLayer=CCLayer::create();
    kuangziLayer->setPosition(CCPointZero);
    kuangziLayer->setTag(TAG_KUANGZI);
    this->addChild(kuangziLayer);
    
    m_hanziLabelMenu=CCMenu::create();
    m_hanziLabelMenu->setPosition(CCPointZero);
    this->addChild(m_hanziLabelMenu);
    
    //测试按钮
    CCSprite* pexamNormal=CCSprite::createWithSpriteFrameName("renwu_ceshianniu.png");
    CCSprite* pexamDisable=CCSprite::createWithSpriteFrameName("renwu_ceshianniu.png");
    m_ceshiItem=CCMenuItemSprite::create(pexamNormal, pexamNormal,pexamDisable, this, menu_selector(KechengDialogLayer::menuCallback));;
    m_ceshiItem->setPosition(S_RM->getPositionWithName("renwu_ceshianniu"));
    m_ceshiItem->setTag(TAG_EXAM);
    this->addMenuItem(m_ceshiItem);
    
    CCSprite* xingxing=CCSprite::createWithSpriteFrameName("renwu_xing3.png");
    CCMenuItemSprite* item=CCMenuItemSprite::create(xingxing,xingxing, this, menu_selector(KechengDialogLayer::showOrHideTable));
    item->setPosition(S_RM->getPositionWithName("renwu_xing3"));
    this->addMenuItem(item);
    int fontSize=40;
    if (S_KC->getKcid()>9) {
        fontSize=30;
    }
    
    CCString* str=CCString::createWithFormat("%d",S_KC->getKcid());
    m_kechengLabel=CCLabelTTF::create(str->getCString(), "", fontSize);
    m_kechengLabel->setFontFillColor(ccc3(255, 255, 255));
    m_kechengLabel->enableStroke(ccc3(0, 0, 0), 1);
    m_kechengLabel->setPosition(S_RM->getPositionWithName("renwu_xing3_zi"));
    this->addChild(m_kechengLabel);
    
    m_tableLayer=KechengDialogTableLayer::create(this);
    m_tableLayer->setVisible(false);
    this->addChild(m_tableLayer);
    
    this->setKecheng(S_KC->getKcid());
    
    //定时提醒
    this->scheduleOnce(schedule_selector(KechengDialogLayer::ceShiTiXing), 4);
    
    return true;
}

void KechengDialogLayer::enableTouch(){
    GuideDialogLayer::enableTouch();
    m_tableLayer->setHandlerPriority(this->getDelegate()->topHandlerPriority()-1);
    m_hanziLabelMenu->setHandlerPriority(this->getDelegate()->topHandlerPriority()-1);
}

void KechengDialogLayer::onEnter(){
    GuideDialogLayer::onEnter();
    
    this->heimaoAction();
}

void KechengDialogLayer::onExit(){
    GuideDialogLayer::onExit();
}

bool KechengDialogLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint touchLocation = pTouch->getLocation();
    CCRect tableRect=S_RM->getRectWithName("renwu_dialog_table_rect");
    if (!tableRect.containsPoint(touchLocation)) {
        CCRect rect=S_RM->getRectWithName("renwu_dialog_title_rect");
        if (rect.containsPoint(touchLocation)) {
            m_tableLayer->setVisible(true);
        }else{
            m_tableLayer->setVisible(false);
        }
    }
    return true;
}

void KechengDialogLayer::showOrHideTable(){
    if (m_tableLayer->isVisible()) {
        m_tableLayer->setVisible(false);
    }else{
        m_tableLayer->setVisible(true);
    }
}

void KechengDialogLayer::setKecheng(int kcid){
    //复位
    this->unschedule(schedule_selector(KechengDialogLayer::dingShiTiXing));
    //定时提醒
    if (kcid<S_KC->getLastKechengId()) {
        this->schedule(schedule_selector(KechengDialogLayer::dingShiTiXing), 20, kCCRepeatForever, 10);
    }
    
    //清除奖牌
    CCObject* object;
    CCARRAY_FOREACH(m_jiangpaiArray, object){
        CCNode* node=(CCNode*)object;
        this->removeChild(node);
    }
    m_jiangpaiArray->removeAllObjects();
    
    S_KC->freshDataWithKcid(kcid);
    
    CCString* str=CCString::createWithFormat("%d",S_KC->getKcid());
    m_kechengLabel->setString(str->getCString());
    
    CCLayer* kuangziLayer=(CCLayer*)this->getChildByTag(TAG_KUANGZI);
    kuangziLayer->removeAllChildren();
    
    m_hanziLabelMenu->removeAllChildren();
    
    int i=0;
    int lastKechengId=S_KC->getLastKechengId();
    for (vector<Hanzi *>::iterator it = S_KC->getHanziVector()->begin(); it != S_KC->getHanziVector()->end(); it ++){
        if (NULL != *it){
            i++;
            
            CCSprite* kuangzi=CCSprite::create();
            kuangzi->setDisplayFrame(S_SF->spriteFrameByName("renwu_zikuang.png"));
            
            if (kcid<=lastKechengId) {
                CCSprite* badge=CCSprite::createWithSpriteFrameName("renwu_dialog_badge.png");
                CCSize size=S_RM->getSizeWithName("renwu_zi_kuangzi_badge_offset");
                badge->setPosition(ccp(size.width, size.height));
                badge->setScale(0.8);
                badge->setTag(kTagBadge);
                kuangzi->addChild(badge);
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
            m_hanziLabelMenu->addChild(labelItem);
            
        }
    }
}

void KechengDialogLayer::ceShiTiXing(float t){
    m_ceshiItem->runAction(CCSequence::create(CCFadeOut::create(0.3),CCFadeIn::create(0.3),NULL));
}

void KechengDialogLayer::dingShiTiXing(float t){
    CCArray* labelItems=m_hanziLabelMenu->getChildren();
    CCObject* object;
    float delay=0.2f;
    int i=0;
    CCArray* audios=CCArray::create();
    CCARRAY_FOREACH(labelItems, object){
        CCNode* node=(CCNode*)object;
        node->runAction(CCSequence::create(CCDelayTime::create(delay),CCScaleTo::create(0.3, 1.5),CCScaleTo::create(0.3, 1),NULL));
        delay+=1.5;
        Audio* audio=new Audio();
        audio->autorelease();
        audio->setAudioName((CCFileUtils::sharedFileUtils()->getWritablePath().append(((*S_KC->getHanziVector())[i++])->getcnAudioPath())));
        audio->setDuration(1.5);
        audios->addObject(audio);
    }
    S_ALP->play(audios);
}

void KechengDialogLayer::menuCallback(CCObject* object){
    //停止其他动作
    m_heimao->stop();
    
    CCObject* obj;
    CCARRAY_FOREACH(m_hanziLabelMenu->getChildren(), obj){
        CCNode* node=(CCNode*)object;
        node->stopAllActions();
    }
    //取消定时器
    this->unschedule(schedule_selector(KechengDialogLayer::dingShiTiXing));
    
    if(!this->isCloseItem(object)){
        CCNode* node=(CCNode*)object;
        int tag=node->getTag();
        if (tag>0) {
//            S_AE->playEffect("renwu_hanzi.mp3");
//            
//            m_tableLayer->setVisible(false);
//            
//            XieziCoverLayer* xieziCoverLayer=XieziCoverLayer::create(node->getTag(),m_heimao);
//            xieziCoverLayer->setDelegate(this);
//            this->addChild(xieziCoverLayer);
            
            for (vector<Hanzi *>::iterator it = S_KC->getHanziVector()->begin(); it != S_KC->getHanziVector()->end(); it ++){
                if (NULL != *it){
                    Hanzi* hanzi=(*it);
                    if(hanzi->getid()==tag){
                        const char* audio=(CCFileUtils::sharedFileUtils()->getWritablePath()+hanzi->getcnAudioPath()).c_str();
                        S_AE->playEffect(audio);
                        break;
                    }
                }
            }
            
            
        }else{
            if (tag==TAG_EXAM) {
                if (S_KC->getKcid()==S_KC->getLastKechengId()+1) {
                    S_UD->setIntegerForKey(LAST_BEGIN_KECHENG_ID, S_KC->getKcid());
                    S_UD->flush();
                }
                S_DR->pushScene(KechengExamScene::scene(this));
            }
        }
    }else{
        m_guideDialogDelegate->dialogCallBack(kDialogCMDClose);
        this->removeFromParent();
    }
    
}

void KechengDialogLayer::heimaoAction(){
    Kecheng* kecheng=new Kecheng();
    kecheng->setIntid(0);
    kecheng->setIntwin(0);
    S_DM->getByKey(kecheng, S_KC->getKcid());
    
    HomeScene* homeScene=(HomeScene*)this->getDelegate();
    
    
    bool isKechengFinished=S_UD->getBoolForKey(KECHENG_FINISHED,false);
    if (S_KC->getLastKechengId()==S_KC->getKcCount()&&!isKechengFinished) {
        homeScene->startGuide("HomeScene_Normal","heimao_11");
        S_UD->setBoolForKey(KECHENG_FINISHED, true);
    }else{
        bool isNewOpenKecheng=S_UD->getBoolForKey(NEW_OPEN_KECHENG,true);
        if (isNewOpenKecheng) {
            int lastBegin=S_UD->getIntegerForKey(LAST_BEGIN_KECHENG_ID);
            if (S_KC->getLastKechengId()==S_KC->getKcCount()) {
                //完成所有
                homeScene->startGuide("HomeScene_Normal","heimao_11");
            }else if(lastBegin>S_KC->getLastKechengId()&&S_KC->getDayRenwuCount()==0){
                //上次未完成
                homeScene->startGuide("HomeScene_Normal","heimao_14");
            }
            else if (kecheng->getid()==0){
                //新任务
                homeScene->startGuide("HomeScene_Normal","heimao_13");
            }else{
                //黑猫无动作，解决报错情况
                homeScene->startGuide("HomeScene_Normal","heimao_16");
            }
            
            S_UD->setBoolForKey(NEW_OPEN_KECHENG, false);
        }else{
            //黑猫无动作，解决报错情况
            homeScene->startGuide("HomeScene_Normal","heimao_16");
        }
    }
    
    S_UD->flush();
    
    
    CC_SAFE_DELETE(kecheng);
}


void KechengDialogLayer::examAllRightCallback(){
    
    m_tableLayer->setVisible(true);
    
    
    
    //判断，并更新数据
    if (S_KC->updateDataWhenCompleteKecheng()) {
        //提升成就
        S_AEM->achieveUp(kAchieveCGYS);
        
        //执行奖励动画
        this->runRewardAnimate();
        
        //刷新tableview
        m_tableLayer->examAllRightCallback(S_KC->getKcid());
    }
    
}

void KechengDialogLayer::runRewardAnimate(){
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
        
        CCString* pointStr=CCString::createWithFormat("renwu_dialog_zi_%d",i+1);
        CCPoint point=S_RM->getPositionWithName(pointStr->getCString());
        CCSize offset=S_RM->getSizeWithName("renwu_zi_badge_offset");
        jiangli->setPosition(ccp(point.x+offset.width, point.y+offset.height));
        this->addChild(jiangli);
        m_jiangpaiArray->addObject(jiangli);
        
        CCArray* actionArray=CCArray::create();
        actionArray->addObject(CCDelayTime::create(delay));
        delay+=0.5f;
        actionArray->addObject(CCFadeIn::create(0.2));
        actionArray->addObject(CCSpawn::create(CCScaleTo::create(0.3, 0.08),CCRotateBy::create(0.3, 315),NULL));
        actionArray->addObject(CCCallFuncN::create(this, callfuncN_selector(KechengDialogLayer::runRewardAnimateCallBack)));
        CCFiniteTimeAction* sequence=CCSequence::create(actionArray);
        jiangpai->runAction(sequence);
    }
    
}

void KechengDialogLayer::runRewardAnimateCallBack(CCObject* obj){
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
        //星星控制点数组
        CCPointArray* array=CCPointArray::create(S_KC->getHanziVector()->size());
        int i=0;
        for (vector<Hanzi *>::iterator it = S_KC->getHanziVector()->begin(); it != S_KC->getHanziVector()->end(); it ++){
            if (NULL != *it){
                //添加星星控制点
                CCString* point=CCString::createWithFormat("renwu_dialog_zi_%d",++i);
                array->addControlPoint(S_RM->getPositionWithName(point->getCString()));
            }
        }
        S_LM->gain("RENWU",S_RM->getJpgBgPosition(),array);
    }
}




