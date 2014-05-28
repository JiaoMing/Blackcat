//
//  XieziCoverLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-5-27.
//
//

#include "XieziCoverLayer.h"

XieziCoverLayer::XieziCoverLayer(int hid){
    m_hanzi=new Hanzi();
    S_DM->getByKey(m_hanzi, hid);
}

XieziCoverLayer::~XieziCoverLayer(){
    CC_SAFE_DELETE(m_hanzi);
}

XieziCoverLayer* XieziCoverLayer::create(int hid){
    XieziCoverLayer* xieziCoverLayer=new XieziCoverLayer(hid);
    xieziCoverLayer->init();
    xieziCoverLayer->autorelease();
    return xieziCoverLayer;
}

bool XieziCoverLayer::init(){
    if (!CoverLayer::init(ccc4(0, 0, 0, 130))) {
        return false;
    }
    
    m_xieziLayer=XieziLayer::create(m_hanzi);
    this->addChild(m_xieziLayer);
    return true;
}

void XieziCoverLayer::onEnter(){
    CoverLayer::onEnter();
    m_xieziLayer->setHandlerPriority(this->getDelegate()->topHandlerPriority()-1);
}

void XieziCoverLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint point=pTouch->getLocationInView();
    CCPoint touchPoint=S_DR->convertToGL(point);
    
    CCRect rect=CCRectMake(500/2,0 ,500, W_SIZE.height);
    if (!rect.containsPoint(touchPoint)) {
        this->removeFromParent();
        
        UserBarLayer* userbarLayer=S_LM->getDelegate();
        if(userbarLayer)userbarLayer->setZOrder(ORDER_USERBAR);
    }
}