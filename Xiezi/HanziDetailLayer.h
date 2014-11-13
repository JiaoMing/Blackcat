//
//  HanziDetailLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-6-27.
//
//

#ifndef __Blackcat__HanziDetailLayer__
#define __Blackcat__HanziDetailLayer__

#include "resource.h"
#include "Hanzi.h"
#include "Zczj.h"
#include "Content.h"

class HanziDetailLayer : public CCLayer
{
public:
    static HanziDetailLayer* create(Hanzi* hanzi);
    HanziDetailLayer();
    ~HanziDetailLayer();
    
public:
    virtual void onEnter();
    virtual bool init();
    void enableTouch();
    void menuCallback(CCObject* pSender);
    void setHandlerPriority(int priority);
private:
    Hanzi* m_hanzi;
    vector<Zczj*>* m_zczjs;
    map<string, Content*>* m_contentMap;
    CCMenu* m_menu;
    int m_priority;
};

#endif /* defined(__Blackcat__HanziDetailLayer__) */
