//
//  Heimao.h
//  Blackcat
//
//  Created by haojunhua on 14-1-12.
//
//

#ifndef Blackcat_Heimao_h
#define Blackcat_Heimao_h
#include "Cartoon.h"

class Heimao : public Cartoon
{
public:
    static Heimao* create();
    
    float action(const char* key,bool random=true,int actionIndex=0,int audioIndex=0);
};

#endif
