//
//  Zczj.h
//  blackcat
//
//
//

#ifndef blackcat_Zczj_h
#define blackcat_Zczj_h


#include "BaseModel.h"
using namespace std;

class Zczj: public BaseModel{
public:
    Zczj():c1Cid(0),c2Cid(0),juCid(0){
        
    }
    virtual ~Zczj(){};
    
    SYNTHESIZE_KEY(id);
    SYNTHESIZE_INT(hid);
    SYNTHESIZE_STRING(pyyd);
    SYNTHESIZE_INT(c1Cid);
    SYNTHESIZE_INT(c2Cid);
    SYNTHESIZE_INT(juCid);
    
    virtual void registTablenameAndProperty(){
        tablename="zczj";
        INSERT_PROPERTYMAP(Zczj,id);
        INSERT_PROPERTYMAP(Zczj,hid);
        INSERT_PROPERTYMAP(Zczj,pyyd);
        INSERT_PROPERTYMAP(Zczj,c1Cid);
        INSERT_PROPERTYMAP(Zczj,c2Cid);
        INSERT_PROPERTYMAP(Zczj,juCid);
    }
};

#endif
