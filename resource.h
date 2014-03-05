//
//  resource.h
//  blackcat
//
//  Created by haojunhua on 13-8-26.
//
//


#ifndef blackcat_resource_h
#define blackcat_resource_h

/*** 系统相关start ***/
#include "cocos2d.h"
USING_NS_CC;
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#define S_DR CCDirector::sharedDirector()
#define S_SF CCSpriteFrameCache::sharedSpriteFrameCache()
#define S_UD CCUserDefault::sharedUserDefault()

#define S_RM ResManager::sharedResManager()
#define S_DM DBManager::sharedDBManager()
#define S_AM AsyncManager::sharedAsyncManager()
#define S_PM PropsManager::sharedPropsManager()
#define S_AE SimpleAudioEngine::sharedEngine()
#define S_PTM PlistManager::sharedPlistManager()
#define S_AOM AudioManager::sharedAudioManager()
#define S_ALP AudioListPlayer::sharedAudioListPlayer()
#define S_AFM ActionFlowManager::sharedActionFlowManager()
#include "ResManager.h"
#include "DBManager.h"
#include "AsyncManager.h"
#include "PropsManager.h"
#include "AudioListPlayer.h"
#include "AudioManager.h"
#include "ActionFlowManager.h"
#include "PlistManager.h"
#include "Utils.h"


#define W_SIZE S_DR->getWinSize()
/*** 系统相关end ***/

/*** 全局start ***/
#define LAST_HANZI_ID  "LAST_HANZI_ID"
#define LAST_OPEN_TIMESTAMP  "LAST_OPEN_TIMESTAMP"
#define NEW_OPEN "NEW_OPEN"

#define IS_INSTALLED "IS_INSTALLED"

#define IS_SHOW_GONGKAIXIN "IS_SHOW_GONGKAIXIN"

/*** 全局end ***/


/*** 卡片写字相关start ***/
#define TUPIAN_SIZE 600

#define HANZI_VERIFY_PASS "status=4"
#define TUPIAN_VERIFY_PASS "status=2"

#define COLLECT_LIMIT 60
#define WRITE_LIMIT 20

#define COLLECT_HANZI_COUNT  "COLLECT_HANZI_COUNT"
#define COLLECT_TUPIAN_COUNT  "COLLECT_TUPIAN_COUNT"
#define COLLECT_XINGXING_COUNT  "COLLECT_XINGXING_COUNT"
#define OVER_XINGXING_COUNT  "OVER_XINGXING_COUNT"

typedef enum{
    kCallWebRewrite=0,
    kCallWebYanshi,
    kCallWebWrite
}CallWebCMD;

typedef enum{
    kWebCallBackLoadedFinish=0,
    kWebCallBackWriteHanziOk,
    kWebCallBackWriteStrokeOk,
    kWebCallBackWriteStrokeError,
    kWebCallBackChangeToWriteMode,
    kWebCallBackChangeToYanshiMode,
    kWebCallBackCanvasData
}WebCallBackCMD;
/*** 卡片写字相关end ***/


/*** 任务相关start ***/

#define LAST_RENWU_DAY "LAST_RENWU_DAY"
#define LAST_RENWU_DAY_COUNT "LAST_RENWU_DAY_COUNT"
#define LAST_KECHENG_ID "LAST_KECHENG_ID"

static int static_getDayRenwuCount(){
    string time=TimeUtils::getYYYYMMDD();
    string lastTime=S_UD->getStringForKey(LAST_RENWU_DAY);
    int count=0;
    if (time==lastTime) {
        count=S_UD->getIntegerForKey(LAST_RENWU_DAY_COUNT, 0);
    }
    return count;
}
/*** 任务相关start ***/


static void static_userDefaultIncrement(const char* key,int defaultValue){
    int count=S_UD->getIntegerForKey(key,defaultValue)+1;
    S_UD->setIntegerForKey(key, count);
    S_UD->flush();
}

#endif
