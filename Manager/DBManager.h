//
//  DBManager.h
//  blackcat
//
//  Created by haojunhua on 13-9-2.
//
//

#ifndef blackcat_DBManager_h
#define blackcat_DBManager_h
#include "cocos2d.h"
USING_NS_CC;
#include "sqlite3.h"
#include "BaseModel.h"
#include <stdint.h>

#define CLAUSE_INIT \
vector<const char*> whereClause=vector<const char*>();\
map<const char*, const char*> orderbyClause=map<const char*, const char*>();\
vector<const char*> groupByClause=vector<const char*>();

class DBManager{
public:
    static DBManager* sharedDBManager();
public:
    
    //执行sql语句
    bool executeSql(const char* sql);
    
    //根据ID获取模型
    BaseModel* getById(BaseModel* model,int id);
    
    //统计
    map<string, string>*  statistic(map<string, string>*  data,string table,map<string, string> expClause){
        CLAUSE_INIT;
        return this->statistic(data,table,expClause, whereClause, orderbyClause, groupByClause);
    }
    
    map<string, string>*  statistic(map<string, string>*  data,string table,map<string, string> expClause,vector<const char*> whereClause,map<const char*, const char*> orderbyClause,vector<const char*> groupByClause);
    
    //根据条件获取模型数组
    template <typename T>
    vector<T*>*  findScrollData(vector<T*>* modelVector,const char* column){
        CLAUSE_INIT;
        return this->findScrollData(modelVector, column, whereClause, orderbyClause, groupByClause);
    }
    
    //根据条件获取模型数组
    template <typename T>
    vector<T*>*  findScrollData(vector<T*>* modelVector,const char* column,vector<const char*> whereClause,map<const char*, const char*> orderbyClause,vector<const char*> groupByClause){
        return this->findScrollData(modelVector, column, whereClause, orderbyClause, groupByClause, 0, INT_MAX);
    }
    
    template <typename T>
    vector<T*>*  findScrollData(vector<T*>* modelVector,const char* column,vector<const char*> whereClause,map<const char*, const char*> orderbyClause,vector<const char*> groupByClause,int startIndex,int size){
        if (this->openDB()) {
            T* t=new T();
            t->registTablenameAndProperty();//注册模型属性
            
            CCString* sql=CCString::createWithFormat("select %s from %s %s %s %s limit %d,%d;",column,t->tablename.c_str(),buildWhereClause(whereClause).c_str(),buildOrderByClause(orderbyClause).c_str(),buildGroupByClause(groupByClause).c_str(),startIndex,size);
            int sqliteResultCode=sqlite3_prepare(m_db, sql->getCString(), -1, &m_stmt, 0);
            if (sqliteResultCode==SQLITE_OK) {
                int column_count=sqlite3_column_count(m_stmt);
                while(sqlite3_step(m_stmt)==SQLITE_ROW){
                    T* t=new T();
                    t->registTablenameAndProperty();//注册模型属性
                    //数据获取成功
                    for (int i = 0; i < column_count; i++) {
                        const char* column_name=sqlite3_column_name(m_stmt, i);
                        char* column_text=(char*)sqlite3_column_text(m_stmt, i);
                        if (column_text!=NULL) {
                            CCString* propertySetMethod=CCString::createWithFormat("set%s",column_name);
                            SEL_SetHandler handler=t->m_propertyMap[propertySetMethod->getCString()];
                            if(handler){
                                (t->*handler)(column_text);
                            }
                        }
                        
                    }
                    modelVector->push_back(t);
                }
            }
            CC_SAFE_DELETE(t);//释放用于内存
        }
        this->closeDB();
        return modelVector;
    }
    
    int getCount(const char* model,const char* columnExp){
        map<string, string>* data=new map<string, string>();
        map<string, string> expClause=map<string, string>();
        map<string, string>::iterator it;
        CCString* exp=CCString::createWithFormat("Sum(case when %s then 1 else 0 end)",columnExp);
        expClause.insert(pair<string, string>("count",exp->getCString()));
        this->statistic(data, model, expClause);
        for(it=data->begin();it!=data->end();++it){
            if(it->first=="count"){
                return atoi(it->second.c_str());
            }
            data->erase(it);
        }
        CC_SAFE_DELETE(data);
        return 0;
    }
    
    
private:
    DBManager();
    ~DBManager();
    bool openDB();
    void closeDB();
    string buildWhereClause(vector<const char*> whereClause);
    string buildOrderByClause(map<const char*, const char*> orderbyClause);
    string buildGroupByClause(vector<const char*> groupByClause);
    
private:
    string m_dbFilePath;
    sqlite3 *m_db;
    sqlite3_stmt* m_stmt;
};



#endif
