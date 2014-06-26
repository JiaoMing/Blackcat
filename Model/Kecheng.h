//
//  EBook.h
//  blackcat
//
//  Created by haojunhua on 13-10-18.
//
//

#ifndef blackcat_Kecheng_h
#define blackcat_Kecheng_h

#include "BaseModel.h"

class Kecheng: public BaseModel{
public:
    EMPTY_CONSTRUCT_DESTRUCT(Kecheng);
    
    SYNTHESIZE_KEY(id);
    SYNTHESIZE_INT(win);
    SYNTHESIZE_INT(lose);
    SYNTHESIZE_INT(lastTime);
    
    virtual void registTablenameAndProperty(){
        tablename="kecheng";
        INSERT_PROPERTYMAP(Kecheng,id);
        INSERT_PROPERTYMAP(Kecheng,win);
        INSERT_PROPERTYMAP(Kecheng,lose);
        INSERT_PROPERTYMAP(Kecheng,lastTime);
    }
    
public:
};

#endif
