//
//  RecordManager.h
//  Blackcat
//
//  Created by haojunhua on 14-9-15.
//
//

#ifndef __Blackcat__RecordManager__
#define __Blackcat__RecordManager__

#include "RankingBarLayer.h"
#include "resource.h"
using namespace std;


class RecordManager :public CCObject{
    
public:
    static RecordManager* sharedRecordManager();
public:
    RecordManager();
    ~RecordManager();
    
protected:
    CC_SYNTHESIZE(RankingBarLayer*, m_delegate, Delegate);
};

#endif /* defined(__Blackcat__RecordManager__) */
