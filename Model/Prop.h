//
//  Prop.h
//  Blackcat
//
//  Created by haojunhua on 14/10/27.
//
//

#ifndef Blackcat_Prop_h
#define Blackcat_Prop_h

#include "BaseModel.h"

class Prop:public BaseModel{
public:
    EMPTY_CONSTRUCT_DESTRUCT(Prop);
    
    SYNTHESIZE_KEY(id);
    SYNTHESIZE_STRING(key);
    SYNTHESIZE_STRING(day);
    
    virtual void registTablenameAndProperty(){
        tablename="prop";
        INSERT_PROPERTYMAP(Prop,id);
        INSERT_PROPERTYMAP(Prop,key);
        INSERT_PROPERTYMAP(Prop,day);
    }
};

#endif
