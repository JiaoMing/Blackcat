//
//  BaseModel.h
//  blackcat
//
//  Created by haojunhua on 13-9-5.
//
//

#ifndef blackcat_BaseModel_h
#define blackcat_BaseModel_h
#include <cstdlib>
#include <iostream>
#include <map>
using namespace std;


#define SYNTHESIZE_STRING(propertyName)\
protected: string propertyName;\
public: string get##propertyName() const { return propertyName; }\
public: void set##propertyName(const char* var){ propertyName = var; }

#define SYNTHESIZE_INT(propertyName)\
protected: int propertyName;\
public: int get##propertyName() const { return propertyName; }\
public: void set##propertyName(const char* var){ propertyName = std::atoi(var); }\
public: void setInt##propertyName(int var){ propertyName = var; }

//目前KEY只支持int型
#define SYNTHESIZE_KEY(propertyName)\
SYNTHESIZE_INT(propertyName)\
public: virtual string getKeyName() const {return #propertyName;}\
public: virtual int getKeyValue() const {return get##propertyName();}

#define INSERT_PROPERTYMAP(className,propertyName)\
m_propertyMap.insert(pair<string, SEL_SetHandler>("set"#propertyName, set_selector(className::set##propertyName)));

#define EMPTY_CONSTRUCT_DESTRUCT(className)\
public: className(){};\
public: virtual ~className(){};

#define DELETE_MODEL_VECTOR(className,object) \
do{\
for (vector<className *>::iterator it = object->begin(); it != object->end(); it ++)\
if (NULL != *it)\
{\
delete *it;\
*it = NULL;\
}\
object->clear();\
delete object;\
object=NULL;}while(0)

class BaseModel;

typedef void (BaseModel::*SEL_SetHandler)(void* v);
#define set_selector(_SELECTOR) (SEL_SetHandler)(&_SELECTOR)

class BaseModel
{
public:
    
public:
    //构造与析构函数
    EMPTY_CONSTRUCT_DESTRUCT(BaseModel);
    //注册表名及属性
    virtual void registTablenameAndProperty(){};
    //设置属性哈希map
    map<string, SEL_SetHandler> m_propertyMap;
    //表名
    string tablename;
    //主键，目前仅支持int型
    virtual string getKeyName() const=0;
    virtual int getKeyValue() const=0;
};

#endif
