//
//  KechengController.cpp
//  Blackcat
//
//  Created by haojunhua on 14-6-3.
//
//

#include "KechengController.h"
#include "Utils.h"
#include "resource.h"


static KechengController* g_instance = NULL;

KechengController* KechengController::getInstance()
{
	if (g_instance == NULL)
	{
		g_instance = new KechengController();
	}
	return g_instance;
}

void KechengController::purgeInstance()
{
    g_instance = NULL;
}

KechengController::KechengController(){
    m_kcid=0;
    m_dayRenwuCount=0;
    m_hanziVector=new vector<Hanzi*>();
    this->initData();
}

KechengController::~KechengController(){
    DELETE_MODEL_VECTOR(Hanzi, m_hanziVector);
}

void KechengController::initData(){
    
    //初始化课程数量
    map<string, string>* data=new map<string, string>();
    map<string, string> expClause=map<string, string>();
    expClause.insert(pair<string, string>("maxKcid","max(kcid)"));
    vector<const char*> whereClause=vector<const char*>();
    S_DM->statistic(data, "hanzi", expClause,whereClause);
    
    m_kcCount=0;
    map<string, string>::iterator it;
    it=data->find("maxKcid");
    if(it!=data->end()){
        m_kcCount=atoi(it->second.c_str());
    }
    CC_SAFE_DELETE(data);
    
    //初始化kcid、m_lastKechengId、m_dayRenwuCount
    m_lastKechengId=S_UD->getIntegerForKey(LAST_KECHENG_ID,0);
    this->changeKecheng(m_lastKechengId+1);
    
    this->fetchDayRenwuCount();
}

bool KechengController::changeKecheng(int kcid){
    if (m_kcid==kcid&&m_hanziVector->size()>0) {
        return false;
    }
    m_kcid=kcid;
    DELETE_MODEL_VECTOR(Hanzi, m_hanziVector);
    m_hanziVector=new vector<Hanzi*>();
    
    CLAUSE_INIT;
    CCString* where=CCString::createWithFormat("kcid=%d",m_kcid);
    whereClause.push_back(where->getCString());
    orderbyClause.push_back("sort asc");
    
    S_DM->findScrollData(m_hanziVector,"id,zi,lastAnswer,cnAudioPath,isCollected,sort",whereClause, orderbyClause, groupByClause);
    return true;
}

int KechengController::fetchDayRenwuCount(){
    string time=TimeUtils::getYYYYMMDD();
    string lastTime=S_UD->getStringForKey(LAST_RENWU_DAY);
    m_dayRenwuCount=0;
    if (time==lastTime) {
        m_dayRenwuCount=S_UD->getIntegerForKey(LAST_RENWU_DAY_COUNT, 0);
    }
    return m_dayRenwuCount;
}

bool KechengController::updateDataWhenCompleteKecheng(){
    if (m_kcid==m_lastKechengId+1){
        
        for (vector<Hanzi *>::iterator it = m_hanziVector->begin(); it != m_hanziVector->end(); it ++){
            Hanzi* hanzi=*it;
            if (NULL != hanzi){
                CCString* sql=CCString::createWithFormat("update hanzi set isReward=1,isCollected=1 where id=%d",hanzi->getid());
                S_DM->executeSql(sql->getCString());
                if (hanzi->getisCollected()==0) {
                    static_userDefaultIncrement(COLLECT_HANZI_COUNT,18);
                }
            }
        }

        int count=S_UD->getIntegerForKey(LAST_RENWU_DAY_COUNT);
        string time=TimeUtils::getYYYYMMDD();
        string lastTime=S_UD->getStringForKey(LAST_RENWU_DAY);
        if (time!=lastTime) {
            count=1;
            S_UD->setStringForKey(LAST_RENWU_DAY, time);
            S_UD->setIntegerForKey(LAST_RENWU_DAY_COUNT, 1);
        }else if (count<2) {
            S_UD->setIntegerForKey(LAST_RENWU_DAY_COUNT, ++count);
        }
        m_dayRenwuCount=count;
        S_UD->setIntegerForKey(LAST_KECHENG_ID, ++m_lastKechengId);
        S_UD->flush();
        
        //自动进入下一个任务
//        if (m_kcid<m_kcCount&&m_dayRenwuCount<2) {
        if (m_kcid<m_kcCount) {
            this->changeKecheng(m_kcid+1);
        }
        return true;
    }
    return  false;
}

bool KechengController::insertKechengIfNotExists(){
    Kecheng* kecheng=new Kecheng();
    kecheng->setIntid(0);
    S_DM->getByKey(kecheng, m_kcid);
    
    int now=(int)(TimeUtils::millisecondNow().tv_sec);
    int kcid=kecheng->getid();
    CC_SAFE_DELETE(kecheng);
    if (kcid==0) {
        CCString* sql=CCString::createWithFormat("insert into kecheng(id,win,lose,lastTime) values(%d,0,0,%d)",m_kcid,now);
        return S_DM->executeSql(sql->getCString());
    }
    return  false;
}

bool KechengController::updateKecheng(bool isWin){
    Kecheng* kecheng=new Kecheng();
    kecheng->setIntid(0);
    kecheng->setIntwin(0);
    kecheng->setIntlose(0);
    S_DM->getByKey(kecheng, m_kcid);
    
    if (isWin) {
        kecheng->setIntwin(kecheng->getwin()+1);
    }else{
        kecheng->setIntlose(kecheng->getlose()+1);
    }
    
    CCString* sql;
    int now=(int)(TimeUtils::millisecondNow().tv_sec);
    if (kecheng->getid()==0) {
        sql=CCString::createWithFormat("insert into kecheng(id,win,lose,lastTime) values(%d,%d,%d,%d)",m_kcid,kecheng->getwin(),kecheng->getlose(),now);
    }else{
        sql=CCString::createWithFormat("update kecheng set win=%d,lose=%d,lastTime=%d where id=%d",m_kcid,kecheng->getwin(),kecheng->getlose(),now);
    }
    CC_SAFE_DELETE(kecheng);
    return S_DM->executeSql(sql->getCString());
}

void KechengController::disOrderHanzi(){
    size_t size=S_KC->getHanziVector()->size();
    int i=0,random=0;
    for (i=0; i<size; i++) {
        random=(int) (CCRANDOM_0_1()*size);
        Hanzi* swp=(*S_KC->getHanziVector())[i];
        (*S_KC->getHanziVector())[i]=(*S_KC->getHanziVector())[random];
        (*S_KC->getHanziVector())[random]=swp;
    }
}

void KechengController::reOrderHanzi(){
    size_t size=S_KC->getHanziVector()->size();
    for (int i=0; i<size; i++) {
        int swpCount=0;
        for (int j=0; j<size-i-1; j++) {
            Hanzi* hanzi1=(*S_KC->getHanziVector())[j];
            Hanzi* hanzi2=(*S_KC->getHanziVector())[j+1];
            if (hanzi1->getsort()>hanzi2->getsort()) {
                Hanzi* swp=(*S_KC->getHanziVector())[j];
                (*S_KC->getHanziVector())[j]=(*S_KC->getHanziVector())[j+1];
                (*S_KC->getHanziVector())[j+1]=swp;
                swpCount++;
            }
        }
        if (swpCount==0) {
            break;
        }
    }
}