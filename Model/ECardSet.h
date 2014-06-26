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

class ECardSet: public BaseModel{
public:
    EMPTY_CONSTRUCT_DESTRUCT(ECardSet);
    
    SYNTHESIZE_KEY(id);
    SYNTHESIZE_STRING(name);
    SYNTHESIZE_INT(sort);
    SYNTHESIZE_INT(show);
    SYNTHESIZE_INT(isOpen);
    
    virtual void registTablenameAndProperty(){
        tablename="ecard_set";
        INSERT_PROPERTYMAP(ECardSet,id);
        INSERT_PROPERTYMAP(ECardSet,name);
        INSERT_PROPERTYMAP(ECardSet,sort);
        INSERT_PROPERTYMAP(ECardSet,show);
        INSERT_PROPERTYMAP(ECardSet,isOpen);
    }
};



#endif
