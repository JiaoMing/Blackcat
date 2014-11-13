//
//  Zi800Scene.h
//  Blackcat
//
//  Created by haojunhua on 14-9-17.
//
//

#ifndef __Blackcat__Zi800Scene__
#define __Blackcat__Zi800Scene__

#include "PageViewScene.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Hanzi.h"
#include "Content.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class Zi800Scene : public PageViewScene
{
public:
    static CCScene* scene();
    virtual CCScene* changePage(){return this->scene();}
    
    Zi800Scene();
    ~Zi800Scene();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual void beforeReplace();
    CREATE_FUNC(Zi800Scene);
    
    void touchEvent(CCObject* sender,TouchEventType type);
private:
    CCNode* m_rootNode;
    vector<Hanzi*>* m_hanzis;
    map<string, Content*>* m_contentMap;
    bool m_isBgMusicStart;
};

#endif /* defined(__Blackcat__Zi800Scene__) */
