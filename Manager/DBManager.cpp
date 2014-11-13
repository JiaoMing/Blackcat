//
//  DBManager.cpp
//  blackcat
//
//  Created by haojunhua on 13-9-6.
//
//

#include "DBManager.h"
#include "resource.h"
#include "Utils.h"

#define DB_FILE "data.sqlite"

static DBManager * s_pDBManager = NULL;

DBManager * DBManager::sharedDBManager()
{
    if (s_pDBManager == NULL) {
        s_pDBManager = new DBManager();
    }
    return s_pDBManager;
}

DBManager::DBManager(){
    sqlite3_config(SQLITE_CONFIG_SERIALIZED);
    
    m_dbFilePath=CCFileUtils::sharedFileUtils()->getWritablePath()+DB_FILE;
    //拷贝数据库
    bool isDBDeposited=S_UD->getBoolForKey("isDBDeposited",false);
    if (!isDBDeposited) {
        if(FileUtils::depositFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(DB_FILE),m_dbFilePath)){
            S_UD->setBoolForKey("isDBDeposited",true);
            S_UD->flush();
        }
    }
}

DBManager::~DBManager(){
    m_stmt=NULL;
    m_db=NULL;
}

bool DBManager::openDB(){
    m_db = NULL;
    m_stmt= NULL;
    int result;
    
    result = sqlite3_open(m_dbFilePath.c_str(), &m_db);
    if (result != SQLITE_OK) {
        CCLog("DBManager: open %s faild: %d", m_dbFilePath.c_str(), result);
        return false;
    }
    return true;
}

void DBManager::closeDB(){
    sqlite3_step(m_stmt);
    sqlite3_finalize(m_stmt);
    sqlite3_close(m_db);
    m_stmt=NULL;
    m_db=NULL;
}

bool DBManager::executeSql(const char* sql){
    bool bRet=true;
    if (this->openDB()) {
        int success=sqlite3_exec(m_db, sql, NULL, NULL, 0);
        if(success != SQLITE_OK)
        {
            bRet=false;
        }
    }else{
        bRet=false;
    }
    this->closeDB();
    return bRet;
}

bool DBManager::saveOrUpdate(BaseModel* model){
    if (this->openDB()) {
        model->registTablenameAndProperty();//注册模型属性
        CCString* sql=CCString::createWithFormat("select * from %s where %s='%d';",model->tablename.c_str(),model->getKeyName().c_str(),model->getKeyValue());
        int sqliteResultCode=sqlite3_prepare(m_db, sql->getCString(), -1, &m_stmt, 0);
        if (sqliteResultCode==SQLITE_OK) {
            if(sqlite3_step(m_stmt)==SQLITE_ROW){
                //TODO
            }
        }
    }
    this->closeDB();
    return true;
}

BaseModel* DBManager::getByKey(BaseModel* model,int key){
    if (this->openDB()) {
        model->registTablenameAndProperty();//注册模型属性
        CCString* sql=CCString::createWithFormat("select * from %s where %s='%d';",model->tablename.c_str(),model->getKeyName().c_str(),key);
        string keyname=model->getKeyName();
        int sqliteResultCode=sqlite3_prepare(m_db, sql->getCString(), -1, &m_stmt, 0);
        if (sqliteResultCode==SQLITE_OK) {
            int column_count=sqlite3_column_count(m_stmt);
            if(sqlite3_step(m_stmt)==SQLITE_ROW){
                //数据获取成功
                for (int i = 0; i < column_count; i++) {
                    const char* column_name=sqlite3_column_name(m_stmt, i);
                    char* column_text=(char*)sqlite3_column_text(m_stmt, i);
                    if (column_text!=NULL) {
                        CCString* propertySetMethod=CCString::createWithFormat("set%s",column_name);
                        SEL_SetHandler handler=model->m_propertyMap[propertySetMethod->getCString()];
                        if(handler){
                            (model->*handler)(column_text);
                        }
                    }
                }
            }
        }
    }
    this->closeDB();
    return model;
}

BaseModel* DBManager::getByProperty(BaseModel *model, string name, string value){
    if (this->openDB()) {
        model->registTablenameAndProperty();//注册模型属性
        CCString* sql=CCString::createWithFormat("select * from %s where %s='%s';",model->tablename.c_str(),name.c_str(),value.c_str());
        string keyname=model->getKeyName();
        int sqliteResultCode=sqlite3_prepare(m_db, sql->getCString(), -1, &m_stmt, 0);
        if (sqliteResultCode==SQLITE_OK) {
            int column_count=sqlite3_column_count(m_stmt);
            if(sqlite3_step(m_stmt)==SQLITE_ROW){
                //数据获取成功
                for (int i = 0; i < column_count; i++) {
                    const char* column_name=sqlite3_column_name(m_stmt, i);
                    char* column_text=(char*)sqlite3_column_text(m_stmt, i);
                    if (column_text!=NULL) {
                        CCString* propertySetMethod=CCString::createWithFormat("set%s",column_name);
                        SEL_SetHandler handler=model->m_propertyMap[propertySetMethod->getCString()];
                        if(handler){
                            (model->*handler)(column_text);
                        }
                    }
                }
            }
        }
    }
    this->closeDB();
    return model;
}


BaseModel* DBManager::getByQuery(BaseModel* model,vector<const char*> whereClause){
    if (this->openDB()) {
        model->registTablenameAndProperty();//注册模型属性
        CCString* sql=CCString::createWithFormat("select * from %s %s;",model->tablename.c_str(),buildWhereClause(whereClause).c_str());
        CCLog("%s",sql->getCString());
        string keyname=model->getKeyName();
        int sqliteResultCode=sqlite3_prepare(m_db, sql->getCString(), -1, &m_stmt, 0);
        if (sqliteResultCode==SQLITE_OK) {
            int column_count=sqlite3_column_count(m_stmt);
            if(sqlite3_step(m_stmt)==SQLITE_ROW){
                //数据获取成功
                for (int i = 0; i < column_count; i++) {
                    const char* column_name=sqlite3_column_name(m_stmt, i);
                    char* column_text=(char*)sqlite3_column_text(m_stmt, i);
                    if (column_text!=NULL) {
                        CCString* propertySetMethod=CCString::createWithFormat("set%s",column_name);
                        SEL_SetHandler handler=model->m_propertyMap[propertySetMethod->getCString()];
                        if(handler){
                            (model->*handler)(column_text);
                        }
                    }
                }
            }
        }
    }
    this->closeDB();
    return model;
}

//统计
map<string, string>*  DBManager::statistic(map<string, string>*  data,string table,map<string, string> expClause,vector<const char*> whereClause,vector<const char*> orderbyClause,vector<const char*> groupByClause){
    string groupby;
    for(vector<const char*>::iterator it=groupByClause.begin();it!=groupByClause.end();++it){
        groupby.append(*it).append(",");
    }
    
    string expSB;
    for(map<string, string>::iterator it=expClause.begin();it!=expClause.end();++it){
        expSB.append(it->second).append(" as ").append(it->first).append(",");
    }
    expSB=expSB.substr(0,expSB.length()-1);
    
    if (this->openDB()) {
        CCString* sql=CCString::createWithFormat("select %s %s from %s %s %s %s;",groupby.c_str(),expSB.c_str(),table.c_str(),buildWhereClause(whereClause).c_str(),buildOrderByClause(orderbyClause).c_str(),buildGroupByClause(groupByClause).c_str());
        int sqliteResultCode=sqlite3_prepare(m_db, sql->getCString(), -1, &m_stmt, 0);
        if (sqliteResultCode==SQLITE_OK) {
            int column_count=sqlite3_column_count(m_stmt);
            if(sqlite3_step(m_stmt)==SQLITE_ROW){
                //数据获取成功
                for (int i = 0; i < column_count; i++) {
                    const char* column_name=sqlite3_column_name(m_stmt, i);
                    char* column_text=(char*)sqlite3_column_text(m_stmt, i);
                    data->insert(pair<string, string>(column_name,column_text));
                }
            }
        }
    }
    this->closeDB();
    return data;
}


string DBManager::buildWhereClause(vector<const char*> whereClause){
    string where;
    if (whereClause.size()>0) {
        where=" where ";
        vector<const char*>::iterator it;
        for(it=whereClause.begin();it!=whereClause.end();++it){
            where.append(" (").append(*it).append(") and ");
        }
        where=where.substr(0,where.find_last_of("and")-2);
    }
    return where;
}

string DBManager::buildOrderByClause(vector<const char*> orderbyClause){
    string orderby;
    if (orderbyClause.size()>0) {
        orderby=" order by ";
        vector<const char*>::iterator it;
        for(it=orderbyClause.begin();it!=orderbyClause.end();++it){
            orderby.append(" ").append(*it).append(",");
        }
        orderby=orderby.substr(0,orderby.length()-1);
    }
    return orderby;
}

//c++ map 不能按照插入排序
//string DBManager::buildOrderByClause(map<const char*, const char*> orderbyClause){
//    string orderby;
//    if (orderbyClause.size()>0) {
//        orderby=" order by";
//        map<const char*, const char*>::iterator it;
//        for(it=orderbyClause.begin();it!=orderbyClause.end();++it){
//            orderby.append(" ").append(it->first).append(" ").append(it->second).append(",");
//        }
//        orderby=orderby.substr(0,orderby.length()-1);
//    }
//    return orderby;
//}

string DBManager::buildGroupByClause(vector<const char*> groupByClause){
    string groupby;
    if (groupByClause.size()>0) {
        groupby=" group by ";
        vector<const char*>::iterator it;
        for(it=groupByClause.begin();it!=groupByClause.end();++it){
            groupby.append(*it).append(",");
        }
        groupby=groupby.substr(0,groupby.length()-1);
    }
    return groupby;
}