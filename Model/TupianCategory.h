//
//  Category.h
//  Blackcat
//
//  Created by haojunhua on 13-11-4.
//
//

#ifndef Blackcat_Category_h
#define Blackcat_Category_h

#include "BaseModel.h"

class TupianCategory: public BaseModel{
public:
    EMPTY_CONSTRUCT_DESTRUCT(TupianCategory);
    
    SYNTHESIZE_KEY(cid);
    SYNTHESIZE_INT(catid);
    
    virtual void registTablenameAndProperty(){
        tablename="ciyu_cat";
        INSERT_PROPERTYMAP(TupianCategory,cid);
        INSERT_PROPERTYMAP(TupianCategory,catid);
    }
};


#endif
