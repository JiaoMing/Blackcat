//
//  DialogLayerProxy.h
//  Blackcat
//
//  Created by haojunhua on 14-5-27.
//
//

#ifndef __Blackcat__DialogLayerProxy__
#define __Blackcat__DialogLayerProxy__

#include "DialogLayer.h"

class DialogLayerProxyObjectDelegate;

class DialogLayerProxy:public DialogLayer{
public:
    static DialogLayerProxy* create(DialogLayerProxyObjectDelegate* node);
    
    virtual void enableTouch();
private:
    DialogLayerProxyObjectDelegate* node;
};

class DialogLayerProxyObjectDelegate{
public:
    virtual void setHandlerPriority(int handlerPriority)=0;
};

#endif /* defined(__Blackcat__DialogLayerProxy__) */
