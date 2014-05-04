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
    SYNTHESIZE_INT(star);
    
    virtual void registTablenameAndProperty(){
        tablename="kecheng";
        INSERT_PROPERTYMAP(Kecheng,id);
        INSERT_PROPERTYMAP(Kecheng,star);
    }
    
public:
};

#endif
