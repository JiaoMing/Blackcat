//
//  resource.h
//  blackcat
//
//  Created by haojunhua on 13-8-26.
//
//


#ifndef blackcat_resource_h
#define blackcat_resource_h

#include "cocos2d.h"
USING_NS_CC;
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
#include "ResManager.h"
#include "DBManager.h"
#include "AsyncManager.h"
#include "PropsManager.h"
#include "AudioListPlayer.h"
#include "AudioManager.h"
#include "ActionFlowManager.h"
#include "PlistManager.h"
#include "Toast.h"
#include "Utils.h"
#include "BaiduStat.h"
#include "udkey.h"
#include "Api.h"
#include "LevelManager.h"
#include "UserBarLayer.h"
#include "AchieveManager.h"

/*** 系统相关start ***/
#define S_APP CCApplication::sharedApplication()
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
#define S_TT Toast::sharedToast()
#define S_LM LevelManager::sharedLevelManager()
#define S_AEM AchieveManager::sharedAchieveManager()

#define W_SIZE S_DR->getWinSize()
#define TAGERT_PLATFORM S_APP->getTargetPlatform()
/*** 系统相关end ***/

//待处理：执行stopAllEffects必须添加S_ALP->stop()

/*** 全局start ***/

#define DEBUG_OPEN 0//调试开关
#define ORDER_DIALOG INT_MAX//对话框层序(order)
#define ORDER_USERBAR (ORDER_DIALOG-1)//用户信息bar层序(order)


//!!!!!有冲突需要解决
#define GUIDE_DIALOG_LAYER INT_MAX
#define TOP_ORDER_INDEX GUIDE_DIALOG_LAYER-1
#define GUIDE_EVENT_COVER_LAYER GUIDE_DIALOG_LAYER-1

/*** 全局end ***/


/*** 卡片写字相关start ***/
#define TUPIAN_SIZE 600

#define HANZI_VERIFY_PASS "status=4"
#define TUPIAN_VERIFY_PASS "status=2"

#define COLLECT_LIMIT 60000
#define WRITE_LIMIT 20000

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

static void static_uploadRanking(){
    string token=S_UD->getStringForKey(UDKEY_USER_TOKEN);
    if(token.length()>0){
        int zika=S_UD->getIntegerForKey(COLLECT_HANZI_COUNT,15);
        int tuka=S_UD->getIntegerForKey(COLLECT_TUPIAN_COUNT,15);
        int xunzhang=S_UD->getIntegerForKey(LAST_KECHENG_ID,0)*6;
        int xingxing=S_UD->getIntegerForKey(COLLECT_XINGXING_COUNT,0);
        CCString* data=CCString::createWithFormat("token=%s&ranking.zika=%d&ranking.tuka=%d&ranking.xunzhang=%d&ranking.xingxing=%d",token.c_str(),zika,tuka,xunzhang,xingxing);
        ApiStruct apiStruct;
        apiStruct.url="user_updateRanking_feedback";
        apiStruct.isBlackcat=true;
        apiStruct.target=NULL;
        apiStruct.data=data->getCString();
        Api* api=Api::create(apiStruct);
        api->send();
    }
}

//！！！！检查是否replaceScene中断了Api请求
static void static_userDefaultIncrement(const char* key,int defaultValue){
    int count=S_UD->getIntegerForKey(key,defaultValue)+1;
    S_UD->setIntegerForKey(key, count);
    S_UD->flush();
    
    static_uploadRanking();
}
/*** 任务相关end ***/


/*** 小波start ***/
typedef enum {
    kGamePlayerXiaobo=0,
    kGamePlayerWo
}GamePlayer;
/*** 小波end ***/

#endif
