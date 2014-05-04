//
//  EBookPage.h
//  blackcat
//
//  Created by haojunhua on 13-9-9.
//
//

#ifndef blackcat_EBookPage_h
#define blackcat_EBookPage_h


#include "BaseModel.h"
using namespace std;

class EBookPage: public BaseModel{
public:
    EMPTY_CONSTRUCT_DESTRUCT(EBookPage);
    
    SYNTHESIZE_KEY(id);
    SYNTHESIZE_INT(ebook_id);
    SYNTHESIZE_STRING(content);
    SYNTHESIZE_INT(content_pos_x);
    SYNTHESIZE_INT(content_pos_y);
    SYNTHESIZE_STRING(voice);
    
    virtual void registTablenameAndProperty(){
        tablename="ebook_page";
        INSERT_PROPERTYMAP(EBookPage,id);
        INSERT_PROPERTYMAP(EBookPage,ebook_id);
        INSERT_PROPERTYMAP(EBookPage,content);
        INSERT_PROPERTYMAP(EBookPage,content_pos_x);
        INSERT_PROPERTYMAP(EBookPage,content_pos_y);
        INSERT_PROPERTYMAP(EBookPage,voice);
    }
};



#endif
