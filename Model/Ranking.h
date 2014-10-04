//
//  Ranking.h
//  Blackcat
//
//  Created by haojunhua on 14-4-11.
//
//

#ifndef Blackcat_Ranking_h
#define Blackcat_Ranking_h

#include "BaseModel.h"

class Ranking: public BaseModel{
public:
    EMPTY_CONSTRUCT_DESTRUCT(Ranking);
    
    SYNTHESIZE_KEY(uid);
    SYNTHESIZE_STRING(username);
    SYNTHESIZE_INT(zika);
    SYNTHESIZE_INT(tuka);
    SYNTHESIZE_INT(xunzhang);
    SYNTHESIZE_INT(xingxing);
    SYNTHESIZE_INT(jingyan);
    
    virtual void registTablenameAndProperty(){
        tablename="ranking";
        INSERT_PROPERTYMAP(Ranking,uid);
        INSERT_PROPERTYMAP(Ranking,username);
        INSERT_PROPERTYMAP(Ranking,zika);
        INSERT_PROPERTYMAP(Ranking,tuka);
        INSERT_PROPERTYMAP(Ranking,xunzhang);
        INSERT_PROPERTYMAP(Ranking,xingxing);
        INSERT_PROPERTYMAP(Ranking,jingyan);
    }
};


#endif
