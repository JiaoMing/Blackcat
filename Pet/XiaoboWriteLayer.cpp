//
//  XiaoboWriteLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-4-1.
//
//

#include "XiaoboWriteLayer.h"
#define TAG_HANZI_LABEL 100

XiaoboWriteLayer* XiaoboWriteLayer::create(Hanzi* hanzi,GamePlayer gamePlayer)
{
    XiaoboWriteLayer* pRet = new XiaoboWriteLayer(hanzi);
    pRet->m_gamePlayer=gamePlayer;
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

XiaoboWriteLayer::XiaoboWriteLayer(Hanzi* hanzi){
    this->m_hanzi=hanzi;
    m_parseredHanzi=new ParseredHanzi(m_hanzi);
}

XiaoboWriteLayer::~XiaoboWriteLayer(){
    CC_SAFE_DELETE(m_parseredHanzi);
}

bool XiaoboWriteLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    CCDrawNode* bg=CCDrawNode::create();
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(0, 0));
    this->addChild(bg);
    bg->drawSegment(ccp(20,20), ccp(50,50), 10, ccc4f(0, 1, 0, 1));
    CCPoint points[] = { CCPoint(0,0), CCPoint(400,0), CCPoint(400,400), CCPoint(0,400) };
    bg->drawPolygon(points, sizeof(points)/sizeof(points[0]),ccc4FFromccc3B(ccc3(253,249,238)), 3,ccc4FFromccc3B(ccc3(84,85,104)));
    
    CCLabelTTF* hanziLabelBg=CCLabelTTF::create(m_hanzi->getzi().c_str(), "KaiTi.ttf", 400);
    hanziLabelBg->setPosition(ccp(200,400));
    hanziLabelBg->setFontFillColor(ccc3(255, 255, 255));
    hanziLabelBg->enableStroke(ccc3(0, 0, 0), 1);
    hanziLabelBg->setAnchorPoint(ccp(0.5, 1));
    hanziLabelBg->setTag(TAG_HANZI_LABEL);
    this->addChild(hanziLabelBg);
    
    m_draw=CCDrawNode::create();
    this->addChild(m_draw);
    
    m_parseredHanzi->step();
    CCPoint startPoint=m_parseredHanzi->getTempPoint();
    
    switch (m_gamePlayer) {
        case kGamePlayerXiaobo:{
            m_touxiang=CCSprite::createWithSpriteFrameName("pet_touxiang.png");
//            m_touxiang->setScale(0.5);
            m_touxiang->setPosition(startPoint);
            this->addChild(m_touxiang);
        }
            break;
        case kGamePlayerWo:{
            m_touxiang=CCSprite::createWithSpriteFrameName("pet_pencil.png");
            m_touxiang->setScale(0.5);
            m_touxiang->setPosition(startPoint);
            m_touxiang->setAnchorPoint(ccp(0, 0));
            this->addChild(m_touxiang);
        }
            break;
    }
    
    return true;
}

void XiaoboWriteLayer::runHanziLabelAnimate(float delay){
    CCLabelTTF* label=(CCLabelTTF*)this->getChildByTag(TAG_HANZI_LABEL);
    label->runAction(CCSequence::create(CCDelayTime::create(delay),CCScaleTo::create(0.2, 1.1),CCScaleTo::create(0.2, 1),NULL));
}

void XiaoboWriteLayer::startGame(){
}

void XiaoboWriteLayer::loop(float t){
    CCPoint start=m_parseredHanzi->getTempPoint();
    HanziStepStatus status=m_parseredHanzi->step();
    CCPoint end=m_parseredHanzi->getTempPoint();
    m_touxiang->setPosition(end);
    switch (status) {
        case kHanziStepMiddlePoint:
        case kHanziStepStrokeEnd:
            m_draw->drawSegment(start, end, 10, ccc4FFromccc3B(ccc3(100,53,14)));
            break;
        case kHanziStepHanziEnd:{
            m_delegate->xiaoboEndCallBack();
        }
        default:
            break;
    }
}

//void XiaoboWriteLayer::reGame(){
//    m_parseredHanzi->reset();
//    m_draw->clear();
//}
