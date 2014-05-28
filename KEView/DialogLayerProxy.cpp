//
//  DialogLayerProxy.cpp
//  Blackcat
//
//  Created by haojunhua on 14-5-27.
//
//

#include "DialogLayerProxy.h"

DialogLayerProxy* DialogLayerProxy::create(DialogLayerProxyObjectDelegate *node){
    DialogLayerProxy* dialogLayerProxy=new DialogLayerProxy();
    dialogLayerProxy->init();
    dialogLayerProxy->autorelease();
    dialogLayerProxy->node=node;
    return dialogLayerProxy;
}

void DialogLayerProxy::enableTouch(){
    DialogLayer::enableTouch();
    node->setHandlerPriority(this->getDelegate()->topHandlerPriority());
}