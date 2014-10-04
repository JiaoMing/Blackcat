//
//  Content.h
//  blackcat
//
//
//

#ifndef blackcat_Content_h
#define blackcat_Content_h


#include "BaseModel.h"
using namespace std;

class Content: public BaseModel{
public:
    EMPTY_CONSTRUCT_DESTRUCT(Content);
    
    SYNTHESIZE_KEY(id);
    SYNTHESIZE_STRING(contentType);
    SYNTHESIZE_STRING(MD);
    SYNTHESIZE_STRING(DD);
    SYNTHESIZE_STRING(fileUrl);
    
    virtual void registTablenameAndProperty(){
        tablename="content";
        INSERT_PROPERTYMAP(Content,id);
        INSERT_PROPERTYMAP(Content,contentType);
        INSERT_PROPERTYMAP(Content,MD);
        INSERT_PROPERTYMAP(Content,DD);
        INSERT_PROPERTYMAP(Content,fileUrl);
    }
};

#endif
