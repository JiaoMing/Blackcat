//
//  EBook.h
//  blackcat
//
//  Created by haojunhua on 13-9-9.
//
//

#ifndef blackcat_EBook_h
#define blackcat_EBook_h

#include "BaseModel.h"

class EBook: public BaseModel{
public:
    EMPTY_CONSTRUCT_DESTRUCT(EBook);
    
    SYNTHESIZE_INT(id);
    SYNTHESIZE_STRING(name);
    SYNTHESIZE_INT(bring_video);
    SYNTHESIZE_INT(total_page_number);
    SYNTHESIZE_INT(readed_Page_number);
    
    virtual void registTablenameAndProperty(){
        tablename="ebook";
        INSERT_PROPERTYMAP(EBook,id);
        INSERT_PROPERTYMAP(EBook,name);
        INSERT_PROPERTYMAP(EBook,bring_video);
        INSERT_PROPERTYMAP(EBook,total_page_number);
        INSERT_PROPERTYMAP(EBook,readed_Page_number);
    }
    
public:
    bool isBringVideo(){
        return bring_video==1;
    }
};

#endif
