//
//  HanziDetailLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-6-27.
//
//

#include "HanziDetailLayer.h"

enum{
    kTagCi1=0,
    kTagCi2,
    kTagJu,
    kTagMenu
};

HanziDetailLayer* HanziDetailLayer::create(Hanzi* hanzi)
{
    HanziDetailLayer *layer = new HanziDetailLayer();
    layer->m_hanzi=hanzi;
    if (layer && layer->init())
    {
        layer->autorelease();
    }
    else
    {
        delete layer;
        layer = NULL;
    }
    return layer;
}

HanziDetailLayer::HanziDetailLayer(){
    m_zucijus=new vector<Zuciju*>();
}

HanziDetailLayer::~HanziDetailLayer(){
    DELETE_MODEL_VECTOR(Zuciju, m_zucijus);
}


void HanziDetailLayer::setHandlerPriority(int priority){
    m_priority=priority;
}

void HanziDetailLayer::onEnter(){
    CCLayer::onEnter();
    this->scheduleOnce(schedule_selector(HanziDetailLayer::enableTouch), 0);
}

void HanziDetailLayer::enableTouch(){
    if(m_menu!=NULL)m_menu->setHandlerPriority(m_priority);
}

bool HanziDetailLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    CCSprite* bg=CCSprite::createWithSpriteFrameName("hanzi_detail.png");
    bg->setPosition(S_RM->getPositionWithName("xiezi_main"));
    this->addChild(bg);
    
    m_menu=CCMenu::create();
//    menu->setPosition(S_RM->getPositionWithName("xiezi_main"));
//    menu->setAnchorPoint(ccp(0.5,0.5));
    m_menu->setPosition(CCPointZero);
    bg->addChild(m_menu);
    
    float height=bg->getContentSize().height;
    
    CCLabelTTF* bushou=CCLabelTTF::create(m_hanzi->getbushou().c_str(), "KaiTi.ttf", 30);
    bushou->setPosition(S_RM->getRelativePosition("xiezi_zucizaoju_dian1", height) );
    bushou->setColor(ccc3(0, 0, 0));
    bushou->setAnchorPoint(ccp(0,0.5));
    bg->addChild(bushou);
    
    CCString* bihuaStr=CCString::createWithFormat("%d",m_hanzi->getbihuashu());
    CCLabelTTF* bihua=CCLabelTTF::create(bihuaStr->getCString(), "KaiTi.ttf", 30);
    bihua->setPosition(S_RM->getRelativePosition("xiezi_zucizaoju_dian2", height) );
    bihua->setColor(ccc3(0, 0, 0));
    bihua->setAnchorPoint(ccp(0,0.5));
    bg->addChild(bihua);
    
    CLAUSE_INIT;
    CCString* where=CCString::createWithFormat("zid=%d",m_hanzi->getid());
    whereClause.push_back(where->getCString());
    orderbyClause.push_back("ci1 desc");
    S_DM->findScrollData(m_zucijus,"*",whereClause, orderbyClause, groupByClause);
    
    string pinyin;
    for (int i=0; i<m_zucijus->size(); i++) {
        Zuciju* zuciju=(*m_zucijus)[i];
        if (zuciju->getci1().length()>0) {
            if (i>0) {
                pinyin.append(" , ");
            }
            pinyin.append(zuciju->getpinyinyindiao());
        }
    }
    
    CCLabelTTF* pinyinyindiao=CCLabelTTF::create(pinyin.c_str(), "Pinyinok.ttf", 30);
    pinyinyindiao->setPosition(S_RM->getRelativePosition("xiezi_zucizaoju_dian3", height) );
    pinyinyindiao->setColor(ccc3(0, 0, 0));
    pinyinyindiao->setAnchorPoint(ccp(0,0.5));
    bg->addChild(pinyinyindiao);
    
    float width=0;
    CCPoint ciStartPoint=S_RM->getRelativePosition("xiezi_zucizaoju_dian4", height);
    for (int i=0; i<m_zucijus->size(); i++) {
        Zuciju* zuciju=(*m_zucijus)[i];
        if (zuciju->getci1().length()>0) {
            if (i>0) {
                
            }
            
            CCLabelTTF* zuci=CCLabelTTF::create(zuciju->getci1().c_str(), "KaiTi.ttf", 30);
            zuci->setColor(ccc3(0, 0, 0));
            CCMenuItem* item=CCMenuItemLabel::create(zuci, this, menu_selector(HanziDetailLayer::menuCallback));
            item->setPosition(ccp(ciStartPoint.x+width, ciStartPoint.y));
            item->setAnchorPoint(ccp(0,0.5));
            item->setTag(kTagCi1);
            item->setZOrder(i);//zorder保存i
            m_menu->addChild(item);
            width+=zuciju->getci1().length()*10+10;
        }
        
        if (zuciju->getci2().length()>0) {
            
            CCLabelTTF* zuci=CCLabelTTF::create(zuciju->getci2().c_str(), "KaiTi.ttf", 30);
            zuci->setColor(ccc3(0, 0, 0));
            CCMenuItem* item=CCMenuItemLabel::create(zuci, this, menu_selector(HanziDetailLayer::menuCallback));
            item->setPosition(ccp(ciStartPoint.x+width, ciStartPoint.y));
            item->setAnchorPoint(ccp(0,0.5));
            m_menu->addChild(item);
            item->setTag(kTagCi2);
            item->setZOrder(i);//zorder保存i
            width+=zuciju->getci2().length()*10+10;
        }
    }
    
    
    CCPoint juPoint=S_RM->getRelativePosition("xiezi_zucizaoju_dian5", height);
    for (int i=0; i<m_zucijus->size(); i++) {
        Zuciju* zuciju=(*m_zucijus)[i];
        if (zuciju->getju().length()>0) {
            CCPoint point=ccp(juPoint.x, juPoint.y-i*40);
            CCLabelTTF* ju=CCLabelTTF::create(zuciju->getju().c_str(), "KaiTi.ttf", 25);
            ju->setColor(ccc3(0, 0, 0));
            CCMenuItem* item=CCMenuItemLabel::create(ju, this, menu_selector(HanziDetailLayer::menuCallback));
            item->setPosition(point);
            item->setAnchorPoint(ccp(0,0.5));
            item->setTag(kTagJu);
            item->setZOrder(i);//zorder保存i
            m_menu->addChild(item);
        }
    }
    
    return true;
}

void HanziDetailLayer::menuCallback(CCObject* pSender){
    CCMenuItem* item=(CCMenuItem*)pSender;
    int id=item->getZOrder();
    Zuciju* zuciju=(*m_zucijus)[id];
    
    CCString* str;
    switch (item->getTag()) {
        case kTagCi1:{
            str=CCString::createWithFormat("ci1_%d.mp3",zuciju->getid());
        }
            break;
        case kTagCi2:{
            str=CCString::createWithFormat("ci2_%d.mp3",zuciju->getid());
            
        }
            break;
        case kTagJu:{
            str=CCString::createWithFormat("ju_%d.mp3",zuciju->getid());
            
        }
            break;
            
        default:
            break;
    }
    S_ALP->play(str->getCString(),NULL);
}