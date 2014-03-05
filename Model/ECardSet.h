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
    
    SYNTHESIZE_INT(id);
    SYNTHESIZE_STRING(name);
    SYNTHESIZE_INT(show);
    
    virtual void registTablenameAndProperty(){
        tablename="ecard_set";
        INSERT_PROPERTYMAP(ECardSet,id);
        INSERT_PROPERTYMAP(ECardSet,name);
        INSERT_PROPERTYMAP(ECardSet,show);
    }
};



#endif
