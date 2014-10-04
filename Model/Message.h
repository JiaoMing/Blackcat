//
//  Message.h
//  Blackcat
//
//  Created by haojunhua on 14-7-24.
//
//

#ifndef Blackcat_XieziLog_h
#define Blackcat_XieziLog_h


#include "BaseModel.h"
class Message:public BaseModel{
public:
    EMPTY_CONSTRUCT_DESTRUCT(Message);
    
    SYNTHESIZE_KEY(id);
    SYNTHESIZE_STRING(title);
    SYNTHESIZE_STRING(url);
    SYNTHESIZE_INT(createTime);
    SYNTHESIZE_INT(isReaded);
    
    virtual void registTablenameAndProperty(){
        tablename="message";
        INSERT_PROPERTYMAP(Message,id);
        INSERT_PROPERTYMAP(Message,title);
        INSERT_PROPERTYMAP(Message,url);
        INSERT_PROPERTYMAP(Message,createTime);
        INSERT_PROPERTYMAP(Message,isReaded);
    }
};

#endif
