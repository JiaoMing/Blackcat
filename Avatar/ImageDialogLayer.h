//
//  ImageDialogLayer.h
//  Blackcat
//
//  Created by haojunhua on 14/10/13.
//
//

#ifndef __Blackcat__ImageDialogLayer__
#define __Blackcat__ImageDialogLayer__

#include "cocos2d.h"
#include "DialogLayer.h"
#include "CCImagePicker.h"
USING_NS_CC;

class ImageDialogLayer : public DialogLayer,public CCImagePickerDelegate
{
public:
    ImageDialogLayer();
    ~ImageDialogLayer();
    virtual bool init();
    CREATE_FUNC(ImageDialogLayer);
    
    virtual void menuCallback(CCObject* obj);
    
    virtual void callbackFresh();
    virtual CCArray* getAssets(){return _assets;}
private:
    CCArray* _assets;
    
};

#endif /* defined(__Blackcat__ImageDialogLayer__) */
