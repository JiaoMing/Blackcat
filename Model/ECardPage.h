//
//  ECardPage.h
//  blackcat
//
//  Created by 杨飞 on 13-10-9.
//
//

#ifndef blackcat_ECardPage_h
#define blackcat_ECardPage_h


#include "BaseModel.h"
using namespace std;

class ECardPage: public BaseModel{
public:
    EMPTY_CONSTRUCT_DESTRUCT(ECardPage);
    
    SYNTHESIZE_KEY(id);
    SYNTHESIZE_INT(sid);
    SYNTHESIZE_STRING(img);
    
    virtual void registTablenameAndProperty(){
        tablename="ecard_page";
        INSERT_PROPERTYMAP(ECardPage,id);
        INSERT_PROPERTYMAP(ECardPage,sid);
        INSERT_PROPERTYMAP(ECardPage,img);
    }
};



#endif
