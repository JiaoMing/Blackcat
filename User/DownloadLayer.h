//
//  DownloadLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-3-24.
//
//

#ifndef __Blackcat__DownloadLayer__
#define __Blackcat__DownloadLayer__

#include "cocos2d.h"
USING_NS_CC;

class DownloadLayer : public CCLayerColor
{
public:
    virtual bool init();
    CREATE_FUNC(DownloadLayer);
    
    void download(CCObject* obj);
};

#endif /* defined(__Blackcat__DownloadLayer__) */
