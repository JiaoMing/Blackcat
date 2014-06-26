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

XieziCoverLayer* XieziCoverLayer::create(int hid,Heimao* heimao){
    XieziCoverLayer* xieziCoverLayer=new XieziCoverLayer(hid);
    xieziCoverLayer->m_heimao=heimao;
    xieziCoverLayer->init();
    xieziCoverLayer->autorelease();
    return xieziCoverLayer;
}

bool XieziCoverLayer::init(){
    if (!CoverLayer::init(ccc4(0, 0, 0, 130))) {
        return false;
    }
    
    UserBarLayer* userBarLayer=S_LM->getDelegate();
    if (userBarLayer!=NULL) {
        m_userBarLayerPoint=userBarLayer->getPosition();
        userBarLayer->setPosition(S_RM->getPositionWithName("xiezi_userbar"));
        userBarLayer->setZOrder(INT_MAX);
    }
    
    m_xieziLayer=XieziLayer::create(m_hanzi,true,m_heimao);
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
        
        UserBarLayer* userBarLayer=S_LM->getDelegate();
        if(userBarLayer){
            //恢复userbarlayer位置
            userBarLayer->setPosition(m_userBarLayerPoint);
            userBarLayer->setZOrder(ORDER_USERBAR);
        }
    }
}