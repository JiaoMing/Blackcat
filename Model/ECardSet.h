//
//  ECardSet.h
//  blackcat
//
//  Created by 杨飞 on 13-10-9.
//
//

#ifndef blackcat_ECardSet_h
#define blackcat_ECardSet_h


#include "BaseModel.h"
using namespace std;

enum{
    kECardSetType0=0,
    kECardSetType1,
    kECardSetType2
};

class ECardSet: public BaseModel{
public:
    EMPTY_CONSTRUCT_DESTRUCT(ECardSet);
    
    SYNTHESIZE_KEY(id);
    SYNTHESIZE_STRING(name);
    SYNTHESIZE_INT(sort);
    SYNTHESIZE_INT(show);
    /**
     * 是否锁定
     */
    SYNTHESIZE_INT(isLock);
    /**
     * 是否被打开过
     */
    SYNTHESIZE_INT(isOpen);
    /**
     * 0:看图识字；1：800字；2：有声读物
     */
    SYNTHESIZE_INT(type);
    
    virtual void registTablenameAndProperty(){
        tablename="ecard_set";
        INSERT_PROPERTYMAP(ECardSet,id);
        INSERT_PROPERTYMAP(ECardSet,name);
        INSERT_PROPERTYMAP(ECardSet,sort);
        INSERT_PROPERTYMAP(ECardSet,show);
        INSERT_PROPERTYMAP(ECardSet,isLock);
        INSERT_PROPERTYMAP(ECardSet,isOpen);
        INSERT_PROPERTYMAP(ECardSet,type);
    }
};



#endif
