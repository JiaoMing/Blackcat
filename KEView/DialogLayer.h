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

class DialogLayerDelegate;

class DialogLayer: public CoverLayer,public CoverLayerDelegate{
public:
    DialogLayer(){m_title=NULL;m_forwardDialogLayer=NULL;}
    virtual bool init(const ccColor4B& color=ccc4(0, 0, 0, 80));
    CREATE_FUNC(DialogLayer);
    
    virtual void replaceDialog(DialogLayer* dialogLayer);
    
    // register touch
    virtual void onEnter();
    virtual void onExit();
    
    virtual void setDelegate(DialogLayerDelegate* delegate);
    
    virtual int topHandlerPriority();
    
    virtual void enableTouch();
    
    virtual void menuCallback(CCObject* obj);

public:
    virtual void setTitle(const char* title);
    
    CCMenu* getMainMenu(){return m_menu;}
    void addMenuItem(CCMenuItem* item){m_menu->addChild(item);};
    bool isCloseItem(CCObject* item){if(item==m_closeItem)return true;else return false;};
    
    void hideBg(){m_bg->setVisible(false);m_closeItem->setVisible(false);};
    void hideClose(){m_closeItem->setVisible(false);}
    
protected:
    CC_SYNTHESIZE(DialogLayer*, m_forwardDialogLayer, ForwardDialogLayer);
protected:
    CCSprite* m_bg;
    CCMenu* m_menu;
    CCMenuItemSprite* m_closeItem;
    CCLabelTTF* m_title;
    CCLayer* m_contentLayer;//暂时只在childlayer中使用
    DialogLayerDelegate* m_dialogLayerDelegate;
};


class DialogLayerDelegate : public CoverLayerDelegate{
public:
    /**
     * 对话框按钮事件回调
     */
    virtual void dialogCloseCallBack(){};
};

#endif
