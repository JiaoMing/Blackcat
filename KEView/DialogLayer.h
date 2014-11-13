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
#include <stack>
using namespace std;

class DialogLayerDelegate;
class DialogLayer;

class DialogLayerStack: public CCObject{
public:
    static DialogLayerStack* sharedDialogLayerStack();
    void push(DialogLayer* layer);
    DialogLayer* popAndReturn();
private:
    stack<DialogLayer*> _dialogStack;
};

class DialogLayer: public CoverLayer,public CoverLayerDelegate{
public:
    DialogLayer(){m_title=NULL;}
    virtual bool init(const ccColor4B& color=ccc4(0, 0, 0, 80));
    CREATE_FUNC(DialogLayer);
    
    virtual void replaceDialog(DialogLayer* dialogLayer);
    virtual void pushDialog(DialogLayer* dialogLayer);
    virtual void popDialog();
    
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
    void addMenuItem(CCMenuItem* item,bool isFromDialog=false){
        if(isFromDialog){
            CCPoint p=m_contentLayer->getPosition();
            CCPoint point=item->getPosition();
            item->setPosition(ccpAdd(p, point));
        }
        
        m_menu->addChild(item);
    };
    bool isCloseItem(CCObject* item){if(item==m_closeItem)return true;else return false;};
    
    void hideBg(){m_bg->setVisible(false);m_closeItem->setVisible(false);};
    void hideClose(){m_closeItem->setVisible(false);}
    
    /**
     * 重新定位元素，新界面坐标相对于对话框左下角，而非屏幕
     */
    void rePosition();
    
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
