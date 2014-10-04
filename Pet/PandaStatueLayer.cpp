//
//  PandaStatueLayer.cpp
//  Blackcat
//
//  Created by wanghongjun on 13-10-30.
// Modified by haojunhua on 13-11-20
//
//

#include "PandaStatueLayer.h"
#include "SimpleAudioEngine.h"
#include "LoadingScene.h"
#include "Utils.h"
#include "resource.h"
#include "ResManager.h"

#define XINQING_XIAOHAO_8_20 5
#define XINQING_XIAOHAO_20_8 50
#define JIE_XIAOHAO_8_20 10
#define JIE_XIAOHAO_20_8 20

using namespace CocosDenshion;

bool PandaStatueLayer::init()
{
    if(! CCLayer::init())
        return false;
    
    CCSprite *kuangzhi = CCSprite::createWithSpriteFrame(S_SF->spriteFrameByName("1.png"));
    kuangzhi->setPosition(S_RM->getPositionWithName("xiaoxiong"));
    this->addChild(kuangzhi);
    
    m_Head = CCSprite::createWithSpriteFrame(S_SF->spriteFrameByName("2.png"));
    m_Head->setPosition(S_RM->getPositionWithName("touxiang"));
    this->addChild(m_Head);
    
    CCSprite *hanbao = CCSprite::createWithSpriteFrame(S_SF->spriteFrameByName("8.png"));
    hanbao->setPosition(S_RM->getPositionWithName("hanbao"));
    hanbao->setScale(0.9);
    this->addChild(hanbao);
    
    CCSprite *hongxin = CCSprite::createWithSpriteFrame(S_SF->spriteFrameByName("7.png"));
    hongxin->setPosition(S_RM->getPositionWithName("hongxin"));
    hongxin->setScale(0.9);
    this->addChild(hongxin);
    
    CCSprite *bar1 = CCSprite::createWithSpriteFrame(S_SF->spriteFrameByName("4.png"));
    bar1->setPosition(S_RM->getPositionWithName("bar1"));
    bar1->setScale(0.9);
    this->addChild(bar1);
    
    CCSprite *bar2 = CCSprite::createWithSpriteFrame(S_SF->spriteFrameByName("4.png"));
    bar2->setPosition(S_RM->getPositionWithName("bar2"));
    bar2->setScale(0.9);
    this->addChild(bar2);
    
    CCSprite *pic1 = CCSprite::createWithSpriteFrame(S_SF->spriteFrameByName("6.png"));
    m_ProgressXinQing = CCProgressTimer::create(pic1);
    m_ProgressXinQing->setType(kCCProgressTimerTypeBar);
    m_ProgressXinQing->setMidpoint(ccp(0, 0));
    m_ProgressXinQing->setBarChangeRate(ccp(1, 0));
    m_ProgressXinQing->setScale(0.9);
    m_ProgressXinQing->setPosition(S_RM->getPositionWithName("bar1"));
    this->addChild(m_ProgressXinQing);
    
    CCSprite *pic2 = CCSprite::createWithSpriteFrame(S_SF->spriteFrameByName("5.png"));
    m_ProgressJiE = CCProgressTimer::create(pic2);
    m_ProgressJiE->setType(kCCProgressTimerTypeBar);
    m_ProgressJiE->setMidpoint(ccp(0, 0));
    m_ProgressJiE->setBarChangeRate(ccp(1, 0));
    m_ProgressJiE->setScale(0.9);
    m_ProgressJiE->setPosition(S_RM->getPositionWithName("bar2"));
    this->addChild(m_ProgressJiE);
    
    
    updateProgress(kPetStatusPrgressInit);
    
    schedule(schedule_selector(PandaStatueLayer::updateProgress), 60);
    
    return true;
}


void PandaStatueLayer::updateProgress(PetStatusPrgress origin,float progressXinQing,float progressJiE)
{
    //当前时间
    cc_timeval time=TimeUtils::millisecondNow();
    
    //获取数据
    float percentageXinQing=S_UD->getFloatForKey("percentageXinQing");
    float percentageJiE=S_UD->getFloatForKey("percentageJiE");
    long lastPetTime=S_UD->getDoubleForKey("PetTime");
    
    struct tm* lastTime= TimeUtils::getLocaltime(lastPetTime);
    
    switch (origin) {
        case kPetStatusPrgressInit:{
            //进入界面
            int hourDistance=(time.tv_sec-lastPetTime)/3600;
            
            for(int i=0;i<hourDistance;i++)
            {
                int hour = (lastTime->tm_hour + i) % 24;
                if(hour > 8 && hour <= 20)
                {
                    percentageXinQing-=XINQING_XIAOHAO_8_20;
                    percentageJiE-=JIE_XIAOHAO_8_20;
                }else{
                    percentageXinQing-=XINQING_XIAOHAO_20_8;
                    percentageJiE-=JIE_XIAOHAO_20_8;
                }
                if (percentageXinQing<=0) {
                    percentageXinQing=0;
                }
                if (percentageJiE<=0) {
                    percentageJiE=0;
                }
                if (percentageXinQing==0&&percentageJiE==0) {
                    break;
                }
            }
        }
            break;
        case kPetStatusPrgressManual:{
            //手动更新（礼品和食物）
            percentageXinQing+=progressXinQing;
            percentageJiE+=progressJiE;
        }
            
            break;
            //因Schedule不能传递参数
        case kPetStatusPrgressSchedule:
        default:{
            //定时刷新
            struct tm* locationTime= TimeUtils::getLocaltime(time.tv_sec);
            int nowHour = locationTime->tm_hour;
            if(nowHour > 8 && nowHour <= 20)
            {
                percentageXinQing-=XINQING_XIAOHAO_8_20/60.0;
                percentageJiE-=JIE_XIAOHAO_8_20/60.0;
            }else{
                percentageXinQing-=XINQING_XIAOHAO_20_8/60.0;
                percentageJiE-=JIE_XIAOHAO_20_8/60.0;
            }
        }
            
            break;
    }
    
    if (percentageXinQing<=0) {
        percentageXinQing=0;
    }else if (percentageXinQing>=100) {
        percentageXinQing=100;
    }
    if (percentageJiE<=0) {
        percentageJiE=0;
    }else if (percentageJiE>=100) {
        percentageJiE=100;
    }
    
    //保存进度
    S_UD->setFloatForKey("percentageXinQing", percentageXinQing);
    S_UD->setFloatForKey("percentageJiE", percentageJiE);
    S_UD->setDoubleForKey("PetTime", time.tv_sec);
    S_UD->flush();
    
    //设置进度
    if (origin==kPetStatusPrgressInit) {
        m_ProgressXinQing->setPercentage(percentageXinQing);
        m_ProgressJiE->setPercentage(percentageJiE);
    }else{
        CCProgressFromTo* progressFromToXinqing = CCProgressFromTo::create(1.0, m_ProgressXinQing->getPercentage(), percentageXinQing);
        m_ProgressXinQing->runAction(progressFromToXinqing);
        
        CCProgressFromTo* progressFromToJiE = CCProgressFromTo::create(1.0, m_ProgressJiE->getPercentage(), percentageJiE);
        m_ProgressJiE->runAction(progressFromToJiE);
    }
    
    
    //更新表情
    if(percentageXinQing > 50 || percentageJiE > 50)
    {
        m_Head->setDisplayFrame(S_SF->spriteFrameByName("3.png"));
    }else{
        m_Head->setDisplayFrame(S_SF->spriteFrameByName("2.png"));
    }
}
