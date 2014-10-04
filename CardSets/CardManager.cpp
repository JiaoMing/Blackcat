//
//  CardManager.cpp
//  ebooks
//
//  Created by 杨飞 on 13-10-9.
//
//

#include "CardManager.h"
#include "DBManager.h"
#include "resource.h"
#include <iostream>
using namespace std;

static CardManager *Instance_CardManager = NULL;


CardManager *CardManager::sharedCardManager()
{
    if (Instance_CardManager == NULL) {
        Instance_CardManager = new CardManager();
    }
    return Instance_CardManager;
}

CardManager::CardManager()
{
    m_isInit=false;
    m_ecardset=new ECardSet();
    m_ecardsets=new vector<ECardSet*>();//声明模型
    m_ecardpages=new vector<ECardPage*>();//声明模型
    m_ecards=new vector<ECard*>();//声明模型
}

CardManager::~CardManager()
{
    CC_SAFE_DELETE(m_ecardset);
    DELETE_MODEL_VECTOR(ECardSet,m_ecardsets);//释放m_ecardsets
    DELETE_MODEL_VECTOR(ECardPage,m_ecardpages);//释放m_ecardpages
    DELETE_MODEL_VECTOR(ECard,m_ecards);//释放m_ecards
}

vector<ECardSet*>* CardManager::getECardSets()
{
    if (!m_isInit) {
        CLAUSE_INIT;//初始化Clause
        CCString* where=CCString::create("show=1");
        whereClause.push_back(where->getCString());
        orderbyClause.push_back("sort desc");
        S_DM->findScrollData(m_ecardsets, "*", whereClause, orderbyClause, groupByClause);//查询指定模型数据
        m_isInit=true;
    }
    return m_ecardsets;
}

void CardManager::setCurrentECardSet(int index){
    if (m_ecardsets->size()>index) {
        m_ecardset=(*m_ecardsets)[index];
        this->setECardPages(m_ecardset->getid());
        m_pageNumer = 0;//初始化第一页
        this->setCurrentECardPage(m_pageNumer);
        
        int isOpen=m_ecardset->getisOpen();
        if (isOpen==0&&!m_ecardset->getisLock()) {
            m_ecardset->setIntisOpen(1);
            CCString* sql=CCString::createWithFormat("update ecard_set set isopen=1 where id=%d",m_ecardset->getid());
            S_DM->executeSql(sql->getCString());
            
            S_LM->gain("NEW_BOOK", ccp(-1,-1));
        }
    }
}

vector<ECardPage*>* CardManager::getECardPages(){
    return m_ecardpages;
}

void CardManager::setECardPages(int sid){
    DELETE_MODEL_VECTOR(ECardPage,m_ecardpages);//释放m_ecardPages
    m_ecardpages=new vector<ECardPage*>();//声明模型
    
    CLAUSE_INIT;//初始化Clause
    CCString* where=CCString::createWithFormat("sid=%d",m_ecardset->getid());
    whereClause.push_back(where->getCString());
    S_DM->findScrollData(m_ecardpages, "*", whereClause, orderbyClause, groupByClause);//查询指定模型数据
}

void CardManager::setCurrentECardPage(int pageNumber)
{
    if (m_ecardpages->size()>pageNumber) {
        m_pageNumer=pageNumber;
        m_ecardpage=(*m_ecardpages)[pageNumber];
        this->setECards(m_ecardpage->getid());
    }
}

vector<ECard*>* CardManager::getECards(){
    return m_ecards;
}

void CardManager::setECards(int pid){
    DELETE_MODEL_VECTOR(ECard,m_ecards);//释放m_ecards
    m_ecards=new vector<ECard*>();//声明模型
    
    CLAUSE_INIT;//初始化Clause
    CCString* where=CCString::createWithFormat("pid=%d",m_ecardpage->getid());
    whereClause.push_back(where->getCString());
    S_DM->findScrollData(m_ecards, "*", whereClause, orderbyClause, groupByClause);//查询指定模型数据
}