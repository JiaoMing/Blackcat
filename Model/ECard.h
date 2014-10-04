//
//  ECard.h
//  blackcat
//
//  Created by 杨飞 on 13-10-9.
//
//

#ifndef blackcat_ECard_h
#define blackcat_ECard_h


#include "BaseModel.h"
using namespace std;

class ECard: public BaseModel{
public:
    EMPTY_CONSTRUCT_DESTRUCT(ECard);
    
    SYNTHESIZE_KEY(id);
    SYNTHESIZE_INT(pid);
    SYNTHESIZE_INT(hid);
    SYNTHESIZE_STRING(zi);
    SYNTHESIZE_STRING(pyyd);
    SYNTHESIZE_INT(x);
    SYNTHESIZE_INT(y);
    SYNTHESIZE_INT(size);
    
    virtual void registTablenameAndProperty(){
        tablename="ecard";
        INSERT_PROPERTYMAP(ECard,id);
        INSERT_PROPERTYMAP(ECard,pid);
        INSERT_PROPERTYMAP(ECard,hid);
        INSERT_PROPERTYMAP(ECard,zi);
        INSERT_PROPERTYMAP(ECard,pyyd);
        INSERT_PROPERTYMAP(ECard,x);
        INSERT_PROPERTYMAP(ECard,y);
        INSERT_PROPERTYMAP(ECard,size);
    }
};

#endif
