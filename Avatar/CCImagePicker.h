//
//  AlbumFinder.h
//  Blackcat
//
//  Created by haojunhua on 14/10/12.
//
//

#ifndef Blackcat_AlbumFinder_h
#define Blackcat_AlbumFinder_h

#include "cocos2d.h"
USING_NS_CC;

typedef enum{
    kImagePickerTypeAssetsGroup=0,
    kImagePickerTypeAssets,
    kImagePickerTypePhoto
}ImagePickerType;

class AlbumObject:public CCObject{
public:
    ~AlbumObject(){CC_SAFE_RELEASE(_image);}
public:
    CC_SYNTHESIZE(std::string, url, Url);
    CC_SYNTHESIZE(std::string, name, Name);
    CC_SYNTHESIZE_RETAIN(CCImage*, _image, Image);
};

class CCImagePickerDelegate{
public:
    virtual ~CCImagePickerDelegate() {};
    virtual void callbackFresh()=0;
    virtual void callbackWithPhoto(AlbumObject* obj){};
    virtual CCArray* getAssets()=0;
};

class CCImagePicker:public CCObject{
public:
    CCImagePicker();
    ~CCImagePicker();
    static CCImagePicker* sharedImagePicker();
    static void purgeImagePicker();
    static bool uImage2cImage(void* uimg, CCImage* cimg);
    
    void findAssets(ImagePickerType imagePickerType,std::string groupUrl);
    void pickCamera();
public:
    CC_SYNTHESIZE(CCImagePickerDelegate*, m_delegate, Delegate);
};

#endif
