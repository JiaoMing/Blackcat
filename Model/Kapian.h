//
//  Kapian.h
//  blackcat
//
//  Created by haojunhua on 13-9-4.
//
//

#ifndef blackcat_Kapian_h
#define blackcat_Kapian_h
#include <iostream>
#include "BaseModel.h"
#include "cocos2d.h"
using namespace std;

typedef enum{
    kHanzi=0,
    kTupian
}KapianDataMode;

typedef enum{
    kCollect=0,
    kAll
}KapianShowMode;

class Kapian: public BaseModel{
public:
    EMPTY_CONSTRUCT_DESTRUCT(Kapian);
    
    SYNTHESIZE_INT(id);
    SYNTHESIZE_INT(isCollected);
    
    CC_SYNTHESIZE(float, progress, Progress);
    CC_SYNTHESIZE(string, text, Text);
    CC_SYNTHESIZE(string, path, Path);
};

#endif
