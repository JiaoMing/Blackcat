//
//  Zuciju.h
//  Blackcat
//
//  Created by haojunhua on 14-6-27.
//
//

#ifndef __Blackcat__Zuciju__
#define __Blackcat__Zuciju__

#include "BaseModel.h"

class Zuciju: public BaseModel{
public:
    EMPTY_CONSTRUCT_DESTRUCT(Zuciju);
    
    SYNTHESIZE_KEY(id);
    SYNTHESIZE_STRING(pinyinyindiao);
    SYNTHESIZE_STRING(ci1);
    SYNTHESIZE_STRING(ci2);
    SYNTHESIZE_STRING(ju);
    SYNTHESIZE_INT(zid);
    
    virtual void registTablenameAndProperty(){
        tablename="zuciju";
        INSERT_PROPERTYMAP(Zuciju,id);
        INSERT_PROPERTYMAP(Zuciju,pinyinyindiao);
        INSERT_PROPERTYMAP(Zuciju,ci1);
        INSERT_PROPERTYMAP(Zuciju,ci2);
        INSERT_PROPERTYMAP(Zuciju,ju);
        INSERT_PROPERTYMAP(Zuciju,zid);
    }
};


#endif /* defined(__Blackcat__Zuciju__) */
