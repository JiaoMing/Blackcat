//
//  ExamScene.cpp
//  Blackcat
//
//  Created by haojunhua on 13-10-8.
//
//

#include "KechengExamScene.h"
#include "DBManager.h"
#include "resource.h"
#include "ResManager.h"
#include "SimpleAudioEngine.h"
#include "Utils.h"
#include "Kecheng.h"
#include "KechengController.h"

using namespace CocosDenshion;

#define TAG_MAIN_MENU -2
#define TAG_BACK  -3
#define TAG_YSQ -4
#define TAG_YSQ_XIAN -5
#define TAG_HANZI 100
#define TAG_XINGLAYER 200
#define TAG_HANZI_ZI 1
#define TAG_NUMBER_START 10
#define TAG_QUESTION -6
#define TAG_XIAOHONGHUA -7
#define SELECT_COUNT 4

KechengExamScene::KechengExamScene(KechengExamSceneDelegate* kechengExamSceneDelegate):
m_examCount(0),
m_rightCount(0),
m_kechengExamSceneDelegate(kechengExamSceneDelegate),
m_daojishi(10)
{
    S_RM->addSceneRes("RenwuScene", "renwu");
}

KechengExamScene::~KechengExamScene(){
    S_RM->removeSceneRes("RenwuScene");
}

CCScene* KechengExamScene::scene(KechengExamSceneDelegate* kechengExamSceneDelegate)
{
    CCScene *scene = CCScene::create();
    KechengExamScene *layer = new KechengExamScene(kechengExamSceneDelegate);
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

bool KechengExamScene::init()
{
    if ( !KechengBaseLayer::init() )
    {
        return false;
    }
    
    S_KC->insertKechengIfNotExists();
    
    CCMenu* menu=CCMenu::create();
    menu->setTag(TAG_MAIN_MENU);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    
    //返回按钮
    CCSprite* pFanhui_1=CCSprite::createWithSpriteFrameName("fanhui.png");
    CCMenuItemSprite* pbackItem=CCMenuItemSprite::create(pFanhui_1, pFanhui_1, this, menu_selector(KechengExamScene::menuCallback));;
    pbackItem->setPosition(S_RM->getPositionWithName("global_back"));
    pbackItem->setTag(TAG_BACK);
    menu->addChild(pbackItem);
    
    
    CCLayer* questionLayer=CCLayer::create();
    questionLayer->setTag(TAG_QUESTION);
    this->addChild(questionLayer);
    
    CCLayer* xiaohonghua=CCLayer::create();
    xiaohonghua->setTag(TAG_XIAOHONGHUA);
    this->addChild(xiaohonghua);
    
    CCSprite* question=CCSprite::createWithSpriteFrameName("renwu_shuzi.png");
    question->setPosition(S_RM->getPositionWithName("renwu_question"));
    questionLayer->addChild(question);
    
    for (int i=0; i<S_KC->getHanziVector()->size(); i++) {
        CCLabelTTF* number=CCLabelTTF::create("?", "KaiTi.ttf", 30);
        number->setColor(ccc3(0, 0, 0));
        CCString* pointStr=CCString::createWithFormat("renwu_jieguo_%d",i+1);
        CCPoint point=S_RM->getPositionWithName(pointStr->getCString());
        CCSize size=S_RM->getSizeWithName("renwu_question_jieguo_offset");
        number->setPosition(ccp(point.x,point.y+size.height));
        number->setTag(TAG_NUMBER_START+i);
        questionLayer->addChild(number);
    }
    
    
    
    //汉字和鼹鼠
    for (int i=SELECT_COUNT; i>0; i--) {
        CCString* postion=CCString::createWithFormat("renwu_tudui_hou_%d",i);
        CCPoint point=S_RM->getPositionWithName(postion->getCString());
        CCSprite* tuduiHou=CCSprite::createWithSpriteFrameName("renwu_tudui_hou.png");
        tuduiHou->setAnchorPoint(ccp(0.5, 0));
        tuduiHou->setPosition(point);
        this->addChild(tuduiHou);
        
        CCMenu* menu=CCMenu::create();
        CCSprite* yanshuHanzi=CCSprite::createWithSpriteFrameName("renwu_yanshu.png");
        CCPoint ziPoint=S_RM->getRelativePosition("renwu_yanshu_hanzi", yanshuHanzi->getContentSize().height);
        CCLabelTTF *txtLabel = CCLabelTTF::create("", "KaiTi.ttf", 80.0);
        txtLabel->setColor(ccc3(0, 0, 0));
        txtLabel->setPosition(ziPoint);
        txtLabel->setTag(TAG_HANZI_ZI);
        yanshuHanzi->addChild(txtLabel);
        
        CCMenuItemSprite* yanshiHanziItem=CCMenuItemSprite::create(yanshuHanzi, yanshuHanzi, this,menu_selector(KechengExamScene::menuCallback));
        yanshiHanziItem->setAnchorPoint(ccp(0.5, 1));
        yanshiHanziItem->setPosition(point);
        yanshiHanziItem->setTag(TAG_HANZI+i);
        menu->addChild(yanshiHanziItem);
        menu->setPosition(CCPointZero);
        menu->setTag(TAG_HANZI+i);
        this->addChild(menu);
        
        CCString* qian=CCString::createWithFormat("renwu_tudui_qian_%d",i);
        CCPoint qianpoint=S_RM->getPositionWithName(qian->getCString());
        CCString* qianRes=CCString::createWithFormat("renwu_tudui_qian_%d.png",i);
        CCSprite* tuduiQian=CCSprite::createWithSpriteFrameName(qianRes->getCString());
        tuduiQian->setPosition(qianpoint);
        this->addChild(tuduiQian);
    }
    
    //打乱顺序
    S_KC->disOrderHanzi();
    
    float time=m_heimao->action("heimao_renwuKaishi");
    //延时后开始测试
    scheduleOnce(schedule_selector(KechengExamScene::exam), time);
    
    //打开android按键响应
    this->setKeypadEnabled(true);
    
    return true;
}

void KechengExamScene::onEnter(){
    KechengBaseLayer::onEnter();
    BaiduStat::onStatEvent(kBaiduOneEventStart,"SceneRetain","KechengExamScene");
    if(DEBUG_OPEN)this->reward(0);
}

void KechengExamScene::onExit(){
    KechengBaseLayer::onExit();
    BaiduStat::onStatEvent(kBaiduOneEventEnd,"SceneRetain","KechengExamScene");
    
    S_KC->reOrderHanzi();
}

void KechengExamScene::yanshuOut(float delay){
    for (int i=SELECT_COUNT; i>0; i--) {
        CCString* postion=CCString::createWithFormat("renwu_tudui_hou_%d",i);
        CCPoint point=S_RM->getPositionWithName(postion->getCString());
        CCMenu* yanshuHanziMenu=(CCMenu*)this->getChildByTag(TAG_HANZI+i);
        CCMenuItem* yanshuHanzi=(CCMenuItem*)yanshuHanziMenu->getChildren()->objectAtIndex(0);
        yanshuHanzi->stopAllActions();
        yanshuHanzi->runAction(CCMoveTo::create(delay, point));
    }
}

void KechengExamScene::reExam(){
    float delay=0.2f;
    this->yanshuOut(delay);
    scheduleOnce(schedule_selector(KechengExamScene::exam), delay);
}

void KechengExamScene::exam(){
    //放大当前题号精灵
    CCLayer* questionLayer=(CCLayer*)this->getChildByTag(TAG_QUESTION);
    CCLabelTTF* hanzi=(CCLabelTTF*)questionLayer->getChildByTag(TAG_NUMBER_START+m_examCount);
    hanzi->runAction(CCScaleTo::create(0.2, 1.3));
    if (m_examCount>0) {
        CCLabelTTF* hanzi=(CCLabelTTF*)questionLayer->getChildByTag(TAG_NUMBER_START+m_examCount-1);
        hanzi->setString(m_hanzi->getzi().c_str());
        hanzi->runAction(CCScaleTo::create(0.2, 1));
    }
    
    int random=(int) (CCRANDOM_0_1()*4);//生成当前汉字位置
    for (int i=SELECT_COUNT; i>0; i--) {
        CCMenu* hanziMenu=(CCMenu*)this->getChildByTag(TAG_HANZI+i);
        CCMenuItem* item=(CCMenuItem*)hanziMenu->getChildren()->objectAtIndex(0);
        CCSprite* sprite=(CCSprite*)item->getChildren()->objectAtIndex(0);
        CCLabelTTF* label=(CCLabelTTF*)(sprite->getChildByTag(TAG_HANZI_ZI));
        if (i-1==random) {
            label->setString(S_KC->getHanziVector()->at(m_examCount)->getzi().c_str());
        }else{
            label->setString(S_KC->getHanziVector()->at((i+m_examCount)%S_KC->getHanziVector()->size())->getzi().c_str());
        }
    }
    
    //播放汉字音频
    m_hanzi=S_KC->getHanziVector()->at(m_examCount);
    m_isPlayDing=true;
    this->readHanzi(0);
    m_isPlayDing=false;
    
    float longestDelayTime=1.0f;//最长延迟时间
    for (int i=SELECT_COUNT; i>0; i--) {
        CCString* postion=CCString::createWithFormat("renwu_tudui_hou_%d",i);
        CCPoint point=S_RM->getPositionWithName(postion->getCString());
        CCMenu* yanshuHanziMenu=(CCMenu*)this->getChildByTag(TAG_HANZI+i);
        CCMenuItem* yanshuHanzi=(CCMenuItem*)yanshuHanziMenu->getChildren()->objectAtIndex(0);
        CCArray* actionArray=CCArray::create();
        float delay=CCRANDOM_0_1()*0.5f;//随机生成延时0～1秒
        if(delay>longestDelayTime)longestDelayTime=delay;
        actionArray->addObject(CCDelayTime::create(delay));
        actionArray->addObject(CCMoveTo::create(0.5f, ccp(point.x, point.y+yanshuHanzi->getContentSize().height)));
        CCFiniteTimeAction* sequence=CCSequence::create(actionArray);
        yanshuHanzi->runAction(sequence);
    }
    
    scheduleOnce(schedule_selector(KechengExamScene::startAndCountdown), longestDelayTime+0.5f);
    m_examCount++;//题目编号+1
}

void KechengExamScene::startAndCountdown(){
    m_isClickable=true;
    //注意！每个循环会被执行SELECT_COUNT次
    for (int i=SELECT_COUNT; i>0; i--) {
        CCString* postion=CCString::createWithFormat("renwu_tudui_hou_%d",i);
        CCPoint point=S_RM->getPositionWithName(postion->getCString());
        CCMenu* yanshuHanziMenu=(CCMenu*)this->getChildByTag(TAG_HANZI+i);
        CCMenuItem* yanshuHanzi=(CCMenuItem*)yanshuHanziMenu->getChildren()->objectAtIndex(0);
        CCArray* actionArray=CCArray::create();
        actionArray->addObject(CCMoveTo::create(10, point));
        actionArray->addObject(CCCallFunc::create(this, callfunc_selector(KechengExamScene::timeUp)));
        CCFiniteTimeAction* sequence=CCSequence::create(actionArray);
        yanshuHanzi->runAction(sequence);
    }
    /**
     *从5秒、5秒，修改为4秒、6秒
     */
    scheduleOnce(schedule_selector(KechengExamScene::readHanzi), 4.0f);
}

void KechengExamScene::readHanzi(float t){
    //播放汉字音频
    string audioPath=FileUtils::getContentFilePath(m_hanzi->getcnAudioPath());
    if (m_isPlayDing) {
        S_ALP->play("renwu_ding.mp3:1.5",audioPath.c_str(),NULL);
    }else{
        S_ALP->play(audioPath.c_str(),NULL);
    }
}

void KechengExamScene::timeUp(){
    CCSprite* wrong=CCSprite::createWithSpriteFrameName("renwu_ku.png");
    CCString* point=CCString::createWithFormat("renwu_jieguo_%d",m_examCount);
    wrong->setPosition(S_RM->getPositionWithName(point->getCString()));
    wrong->setScale(0.3);
    this->addChild(wrong);
    
    this->answerResult(kAnswerResultWrong);
    
    if (m_examCount<S_KC->getHanziVector()->size()) {
        this->reExam();
    }else{
        this->examEnd();
    }
}

void KechengExamScene::menuCallback(CCObject* object){
    
    CCNode* node=(CCNode*)object;
    int tag=node->getTag();
    if (tag>0) {
        if (!m_isClickable) {
            return;
        }
        m_isClickable=false;//关闭可点击
        unschedule(schedule_selector(KechengExamScene::readHanzi));//关闭重读汉字
        
        CCSprite* sprite=(CCSprite*)node->getChildren()->objectAtIndex(0);
        
        CCLabelTTF* label=(CCLabelTTF*)(sprite->getChildByTag(TAG_HANZI_ZI));
        const char* zi=label->getString();

        
        CCLayer* xiaohonghua=(CCLayer*)this->getChildByTag(TAG_XIAOHONGHUA);
        
        CCSprite* result=CCSprite::create();
        result->setPosition(node->getPosition());
        
        if (!strcmp(zi, m_hanzi->getzi().c_str())) {
            this->answerResult(kAnswerResultRight);
            result->setDisplayFrame(S_SF->spriteFrameByName("renwu_xiao.png"));
        }else{
            this->answerResult(kAnswerResultWrong);
            result->setDisplayFrame(S_SF->spriteFrameByName("renwu_ku.png"));
        }
        xiaohonghua->addChild(result);
        
        CCString* pointStr=CCString::createWithFormat("renwu_jieguo_%d",m_examCount);
        CCPoint point=S_RM->getPositionWithName(pointStr->getCString());
        result->runAction(CCSpawn::create(CCMoveTo::create(0.5, point),CCScaleTo::create(0.5, 0.3),NULL));
        
        //显示汉字
        CCLayer* questionLayer=(CCLayer*)this->getChildByTag(TAG_QUESTION);
        CCLabelTTF* hanzi=(CCLabelTTF*)questionLayer->getChildByTag(TAG_NUMBER_START+m_examCount-1);
        hanzi->setString(m_hanzi->getzi().c_str());
        hanzi->runAction(CCScaleTo::create(0.2, 1));
        
        if (m_examCount<S_KC->getHanziVector()->size()) {
            scheduleOnce(schedule_selector(KechengExamScene::reExam), 1.0f);
        }else{
            this->yanshuOut(0.1);
            this->examEnd();
        }
        
        
        
    }else{
        switch (tag) {
            case TAG_BACK:
                S_ALP->stop();
                S_AE->stopAllEffects();
                S_DR->popScene();
                break;
                
            default:
                break;
        }
    }
    
    
}

void KechengExamScene::answerResult(AnswerResult ar){
    switch (ar) {
        case kAnswerResultRight:{
            //答对了
            m_rightCount++;
            
            m_heimao->action("heimao_renwuRight");
            S_AE->playEffect("renwu_guzhang.mp3");
        }
            
            break;
            
        case kAnswerResultWrong:{
            //答错了
            m_heimao->action("heimao_renwuWrong");
            S_AE->playEffect("renwu_cuo.mp3");
        }
            
            break;
    }
}

void KechengExamScene::examEnd(){
    //还原标题
    CCLayer* questionLayer=(CCLayer*)this->getChildByTag(TAG_QUESTION);
    CCLabelTTF* hanzi=(CCLabelTTF*)questionLayer->getChildByTag(TAG_NUMBER_START+m_examCount-1);
    hanzi->runAction(CCScaleTo::create(0.2, 1));
    hanzi->setString(m_hanzi->getzi().c_str());
    
    
    S_KC->updateKecheng(m_rightCount>=S_KC->getHanziVector()->size());
    
    if (m_rightCount>=S_KC->getHanziVector()->size()) {
        //全部答完
        
        CCLayer* questionLayer=(CCLayer*)this->getChildByTag(TAG_QUESTION);
        questionLayer->removeFromParentAndCleanup(false);
        CCLayer* xiaohonghua=(CCLayer*)this->getChildByTag(TAG_XIAOHONGHUA);
        xiaohonghua->removeAllChildrenWithCleanup(false);
        float time=m_heimao->action("heimao_renwuGuoguan");
        this->scheduleOnce(schedule_selector(KechengExamScene::reward), time);
        
        BaiduStat::onStatEvent(kBaiduOneEvent, "Kecheng", "任务成功");
    }else{
        m_heimao->action("heimao_renwuJiayou");
        this->scheduleOnce(schedule_selector(KechengExamScene::showDialog), 5);
        
        BaiduStat::onStatEvent(kBaiduOneEvent, "Kecheng", "任务失败");
    }
}

void KechengExamScene::reward(float t){
    //需要禁止掉返回按钮
    
    m_isSuccessNew=false;
    //判断，并更新数据
    if (S_KC->updateDataWhenCompleteKecheng()) {
        m_isSuccessNew=true;
        //提升成就
        bool isLevelUp=S_AEM->achieveUp(kAchieveCGYS,1,this,callfunc_selector(KechengExamScene::achiveCallback));
        if (!isLevelUp) {
            this->achiveCallback();
        }
    }else{
        this->achiveCallback();
    }
}

void KechengExamScene::achiveCallback(){
    S_DR->popScene();
    if(m_kechengExamSceneDelegate)m_kechengExamSceneDelegate->examAllRightCallback(m_isSuccessNew);
}

void KechengExamScene::showDialog(){
    GuideDialog* guideDialog=new GuideDialog();
    guideDialog->autorelease();
    guideDialog->setText("再来一遍？");
    guideDialog->setMode(kGuideDialogYesOrNo);
    m_gudieDialogLayer=GuideDialogLayer::create(kDialogWithText);
    m_gudieDialogLayer->setDelegate(this);
    this->addChild(m_gudieDialogLayer);
    m_gudieDialogLayer->setGuideDialogData(guideDialog);
}

void KechengExamScene::dialogCallBack(GuideDialogCMD cmd){
    m_gudieDialogLayer->removeFromParentAndCleanup(true);
    switch (cmd) {
        case kDialogCMDYes:{
            m_examCount=0;
            m_rightCount=0;
            CCLayer* xiaohonghua=(CCLayer*)this->getChildByTag(TAG_XIAOHONGHUA);
            xiaohonghua->removeAllChildrenWithCleanup(false);
            this->reExam();
        }
            break;
        case kDialogCMDBlank:
        case kDialogCMDNo:{
            S_DR->popToRootScene();
        }
            break;
        default:
            break;
    }
}

void KechengExamScene::keyBackClicked(){
    S_DR->popScene();
}

void KechengExamScene::keyMenuClicked(){
    
}