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
    
    SYNTHESIZE_INT(id);
    SYNTHESIZE_STRING(content);
    SYNTHESIZE_INT(pid);
    SYNTHESIZE_INT(x);
    SYNTHESIZE_INT(y);
    SYNTHESIZE_INT(wid);
    SYNTHESIZE_INT(size);
    
    virtual void registTablenameAndProperty(){
        tablename="ecard";
        INSERT_PROPERTYMAP(ECard,id);
        INSERT_PROPERTYMAP(ECard,content);
        INSERT_PROPERTYMAP(ECard,pid);
        INSERT_PROPERTYMAP(ECard,x);
        INSERT_PROPERTYMAP(ECard,y);
        INSERT_PROPERTYMAP(ECard,wid);
        INSERT_PROPERTYMAP(ECard,size);
    }
};



#endif
