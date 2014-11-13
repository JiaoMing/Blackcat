//
//  ImagePickerDialogLayer.h
//  Blackcat
//
//  Created by haojunhua on 14/10/12.
//
//

#ifndef __Blackcat__ImagePickerDialogLayer__
#define __Blackcat__ImagePickerDialogLayer__

#include "DialogLayer.h"
#include "KEGridView.h"
#include "CCImagePicker.h"
USING_NS_CC;
USING_NS_CC_EXT;


class ImagePickerDialogLayer : public DialogLayer, public CCTableViewDataSource,public KEGridViewDelegate,public CCImagePickerDelegate
{
public:
    ImagePickerDialogLayer();
    ~ImagePickerDialogLayer();
    static ImagePickerDialogLayer* create(ImagePickerType imagePickType=kImagePickerTypeAssetsGroup,std::string groupUrl="");
    virtual bool init();
    virtual void enableTouch();
    
    virtual void scrollViewDidScroll(CCScrollView* view) {};
    virtual void scrollViewDidZoom(CCScrollView* view) {};
    virtual void gridCellTouched(CCTableView* table, CCTableViewCell* cell,int indexInIdx);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
    virtual void callbackFresh();
    virtual void callbackWithPhoto(AlbumObject* obj);
    virtual CCArray* getAssets(){return _assets;}
    
    virtual void menuCallback(CCObject* obj);
protected:
    CC_SYNTHESIZE(ImagePickerType, _imagePickType, ImagePickerType);
    CC_SYNTHESIZE(std::string, _groupUrl, GroupUrl);
    CC_SYNTHESIZE(CCArray*, _assets, Assets);
private:
    KEGridView* _gridView;
    bool _lock;
};


#endif /* defined(__Blackcat__ImagePickerDialogLayer__) */
