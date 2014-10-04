//
//  XieziLog.h
//  Blackcat
//
//  Created by haojunhua on 14-7-24.
//
//

#ifndef Blackcat_XieziLog_h
#define Blackcat_XieziLog_h


#include "BaseModel.h"
class XieziLog:public BaseModel{
public:
    EMPTY_CONSTRUCT_DESTRUCT(XieziLog);
    
    SYNTHESIZE_KEY(id);
    SYNTHESIZE_INT(hid);
    SYNTHESIZE_STRING(locus);
    SYNTHESIZE_INT(createTime);
    SYNTHESIZE_INT(isUploaded);
    
    virtual void registTablenameAndProperty(){
        tablename="xiezi_log";
        INSERT_PROPERTYMAP(XieziLog,id);
        INSERT_PROPERTYMAP(XieziLog,hid);
        INSERT_PROPERTYMAP(XieziLog,locus);
        INSERT_PROPERTYMAP(XieziLog,createTime);
        INSERT_PROPERTYMAP(XieziLog,isUploaded);
    }
};

#endif
