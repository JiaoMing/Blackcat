//
//  GameLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-3-30.
//
//

#include "GameLayer.h"

void GameLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority+1, true);
}

void GameLayer::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    return true;
}

bool GameLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    vector<Hanzi*>* hanzis=new vector<Hanzi*>();
    CLAUSE_INIT;
    whereClause.push_back(HANZI_VERIFY_PASS);
    whereClause.push_back("isCollected=1");
    S_DM->findScrollData((vector<Hanzi*>*)hanzis,"id",whereClause, orderbyClause, groupByClause);
    int random=(int) (CCRANDOM_0_1()*hanzis->size());
    int id=(*hanzis)[random]->getid();
    DELETE_MODEL_VECTOR(Hanzi,hanzis);
    m_hanzi=new Hanzi();
    S_DM->getByKey(m_hanzi, id);
    
    CCSprite* xiaoboLabel=CCSprite::createWithSpriteFrameName("pet_xiaobo.png");
    xiaoboLabel->setPosition(ccp(512,768-64));
    xiaoboLabel->setOpacity(0);
    this->addChild(xiaoboLabel);
    CCAction* fadeInAndMoveTo1=CCSpawn::create(CCFadeIn::create(0.5),CCMoveTo::create(0.5, ccp(266,768-64)),NULL);
    xiaoboLabel->runAction(CCSequence::create(CCDelayTime::create(0.5),fadeInAndMoveTo1,NULL));
    
    CCLayer* xiaobo=CCLayer::create();
    CCSprite* lankuang=CCSprite::createWithSpriteFrameName("pet_lankuang.png");
    lankuang->setPosition(ccp(271,768-328));
    xiaobo->addChild(lankuang);
    m_xiaoboWriteLayer=XiaoboWriteLayer::create(m_hanzi,kGamePlayerXiaobo);
    m_xiaoboWriteLayer->setPosition(ccp(271-200,768-328-200));
    m_xiaoboWriteLayer->setDelegate(this);
    xiaobo->addChild(m_xiaoboWriteLayer);
    xiaobo->setPosition(0,400);
    this->addChild(xiaobo);
    xiaobo->runAction(CCJumpTo::create(0.5, ccp(0, 0), -400, 1));
    
    CCSprite* woLabel=CCSprite::createWithSpriteFrameName("pet_wo.png");
    woLabel->setPosition(ccp(512,768-64));
    woLabel->setOpacity(0);
    this->addChild(woLabel);
    CCAction* fadeInAndMoveTo2=CCSpawn::create(CCFadeIn::create(0.5),CCMoveTo::create(0.5, ccp(757,768-64)),NULL);
    woLabel->runAction(CCSequence::create(CCDelayTime::create(0.5),fadeInAndMoveTo2,NULL));
    
    CCLayer* wo=CCLayer::create();
    CCSprite* lvkuang=CCSprite::createWithSpriteFrameName("pet_lvkuang.png");
    lvkuang->setPosition(ccp(757,768-328));
    wo->addChild(lvkuang);
    m_webView=CCWebView::create("AppStroke.html",CCRectMake(757-200,328-200,400,400));
    m_webView->setDelegate(this);
    wo->addChild(m_webView);
    m_webView->setVisible(false);
    m_woWriteLayer=XiaoboWriteLayer::create(m_hanzi,kGamePlayerWo);
    m_woWriteLayer->setPosition(ccp(757-200,768-328-200));
    wo->addChild(m_woWriteLayer);
    wo->setPosition(0,400);
    this->addChild(wo);
    wo->runAction(CCSequence::create(CCDelayTime::create(0.1),CCJumpTo::create(0.5, ccp(0, 0), -400, 1),NULL));
    
    CCSprite* vs=CCSprite::createWithSpriteFrameName("pet_VS.png");
    vs->setPosition(ccp(508,768-64));
    vs->setScale(5);
    this->addChild(vs);
    vs->runAction(CCSequence::create(CCScaleTo::create(0.3, 2),CCScaleTo::create(0.1, 1),NULL));
    
    m_guanzhong=CCSprite::createWithSpriteFrameName("pet_guanzhong.png");
    m_guanzhong->setPosition(ccp(497, -m_guanzhong->getContentSize().width/2));
    this->addChild(m_guanzhong);
    m_guanzhong->runAction(CCSequence::create(CCDelayTime::create(0.2),CCMoveTo::create(2, ccp(497, 768-627)),NULL));
    
    
    return true;
}

void GameLayer::webCallBack(CCWebView* webview,std::string cmd){
    vector<string> splitCmd;
    StringUtils::split(cmd,"$",splitCmd);
    int icmd=atoi(splitCmd[0].c_str());
    
    switch (icmd) {
        case kWebCallBackLoadedFinish:{
            //加载完成
            float scaleX=CCEGLView::sharedOpenGLView()->getScaleX();
            float scaleY=CCEGLView::sharedOpenGLView()->getScaleY();
            CCString* str=CCString::createWithFormat("init('%s','%s',%f,%f,0)",m_hanzi->getcontour().c_str(),m_hanzi->getlocus().c_str(),400*scaleX,400*scaleY);
            webview->callWebWithJs(str->getCString());
            
            S_ALP->play("heimao041_1.mp3:2",(CCFileUtils::sharedFileUtils()->getWritablePath().append(m_hanzi->getcnAudioPath()).append(":1.2")).c_str(),"heimao041_2.mp3:4.5","heimao044.mp3",NULL);
            S_AE->playEffect("renwu_ding.mp3");
            this->scheduleOnce(schedule_selector(GameLayer::startGame),9.5);
            
            m_xiaoboWriteLayer->runHanziLabelAnimate(2);
            m_woWriteLayer->runHanziLabelAnimate(2.5);
        }
            break;
        case kWebCallBackWriteStrokeOk:{
            userStrokeIndex=atoi(splitCmd[1].c_str());
        }
            break;
        case kWebCallBackWriteStrokeError:{
            //错误次数决定进度
        }
            break;
        case kWebCallBackWriteHanziOk:{
            if(!m_isEnd)this->gameEnd(kGamePlayerWo);
        }
            break;
    }
}

void GameLayer::startGame(){
    m_webView->setVisible(true);
    m_webView->callWebWithJs("setMode(Modes.kWrite);");
    m_woWriteLayer->setVisible(false);
    m_xiaoboWriteLayer->startGame();
    S_DR->getScheduler()->scheduleSelector(schedule_selector(XiaoboWriteLayer::loop), m_xiaoboWriteLayer, 0, kCCRepeatForever, 0, false);
    this->schedule(schedule_selector(GameLayer::mainloop), 1, kCCRepeatForever, 0);
    userStrokeIndex=0;
}

void GameLayer::xiaoboEndCallBack(){
    if(!m_isEnd)this->gameEnd(kGamePlayerXiaobo);
}

void GameLayer::gameEnd(GamePlayer winner){
    m_isEnd=true;
    m_winner=winner;
    S_DR->getScheduler()->unscheduleSelector(schedule_selector(XiaoboWriteLayer::loop), m_xiaoboWriteLayer);
    this->unschedule(schedule_selector(GameLayer::mainloop));
    
    CCPoint point;
    switch (winner) {
        case kGamePlayerXiaobo:{
            point=ccp(116,768-68);
        }
            break;
        case kGamePlayerWo:{
            point=ccp(921,768-68);
        }
            break;
    }
    CCSprite* sheng=CCSprite::createWithSpriteFrameName("pet_sheng.png");
    sheng->setPosition(point);
    sheng->setScale(2);
    this->addChild(sheng);
    sheng->runAction(CCSequence::create(CCScaleTo::create(0.5, 1),CCDelayTime::create(3),CCCallFunc::create(this, callfunc_selector(GameLayer::gameEndAnimateEnd)),NULL));
    
    CCAnimation* animation=CCAnimation::create();
    for (int i=0;i<12;i++) {
        CCString* fileName=CCString::createWithFormat("guanzhong_%d.png",i+1);
        animation->addSpriteFrame(S_SF->spriteFrameByName(fileName->getCString()));
    }
    animation->setDelayPerUnit(0.1);
    animation->setRestoreOriginalFrame(true);
    CCAnimate* animate=CCAnimate::create(animation);
    m_guanzhong->runAction(CCRepeatForever::create(animate));
    S_AE->playEffect("pet_huanhu.mp3");
    
}

void GameLayer::gameEndAnimateEnd(){
    m_delegate->gameEnd(m_winner);
}

void GameLayer::mainloop(float t){
    int xbStrokeIndex=m_xiaoboWriteLayer->getParseredHanzi()->getStrokeIndex();
    if (xbStrokeIndex>userStrokeIndex) {
        m_xiaoboWriteLayer->getParseredHanzi()->setHeartBeatFactor(0.5);
    }else if(xbStrokeIndex<userStrokeIndex){
        m_xiaoboWriteLayer->getParseredHanzi()->setHeartBeatFactor(1.5);
    }else{
        m_xiaoboWriteLayer->getParseredHanzi()->setHeartBeatFactor(1);
    }
}