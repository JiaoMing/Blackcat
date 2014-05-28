//
//  DialogLayer.h
//  Blackcat
//
//  Created by haojunhua on 14-4-10.
//
//

#ifndef Blackcat_DialogLayer_h
#define Blackcat_DialogLayer_h

#include "CoverLayer.h"

class DialogLayer: public CoverLayer{
public:
    DialogLayer(){m_title=NULL;}
    virtual bool init(const ccColor4B& color=ccc4(0, 0, 0, 80));
    CREATE_FUNC(DialogLayer);
    
    virtual void replaceDialog(DialogLayer* dialogLayer);
    
    // register touch
    virtual void onEnter();
    virtual void onExit();
    
    virtual void enableTouch();
    
    virtual void menuCallback(CCObject* obj);
    
    virtual void setTitle(const char* title);
    
    CCMenu* getMainMenu(){return m_menu;}
    void addMenuItem(CCMenuItem* item){m_menu->addChild(item);};
    bool isCloseItem(CCObject* item){if(item==m_closeItem)return true;else return false;};
    
    void hideBg(){m_bg->setVisible(false);m_closeItem->setVisible(false);};
    void hideClose(){m_closeItem->setVisible(false);}
protected:
    CCSprite* m_bg;
    CCMenu* m_menu;
    CCMenuItemSprite* m_closeItem;
    CCLabelTTF* m_title;
};

#endif
