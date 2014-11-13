//
//  ImageDialogLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14/10/13.
//
//

#include "ImageDialogLayer.h"
#include "ImageEditLayer.h"
#include "ImagePickerDialogLayer.h"

enum{
    kTagXiangce=100,
    kTagPaizhao
};

ImageDialogLayer::ImageDialogLayer(){
    _assets=CCArray::create();
    _assets->retain();
}

ImageDialogLayer::~ImageDialogLayer(){
    _assets->release();
}


bool ImageDialogLayer::init(){
    if (!DialogLayer::init()) {
        return false;
    }
    
    this->setTitle("设置头像");
    
    CCSprite* xiangce=CCSprite::createWithSpriteFrameName("touxiang_xiangce.png");
    xiangce->setScale(0.8);
    CCMenuItemSprite* xiangceItem=CCMenuItemSprite::create(xiangce, xiangce,this,menu_selector(ImageDialogLayer::menuCallback));
    xiangceItem->setPosition(ccp(134,162));
    xiangceItem->setTag(kTagXiangce);
    this->addMenuItem(xiangceItem,true);
    
    CCSprite* paizhao=CCSprite::createWithSpriteFrameName("touxiang_paizhao.png");
    paizhao->setScale(0.8);
    CCMenuItemSprite* paizhaoItem=CCMenuItemSprite::create(paizhao, paizhao,this,menu_selector(ImageDialogLayer::menuCallback));
    paizhaoItem->setPosition(ccp(454, 162));
    paizhaoItem->setTag(kTagPaizhao);
    this->addMenuItem(paizhaoItem,true);
    
    return true;
}

void ImageDialogLayer::menuCallback(cocos2d::CCObject *obj){
    DialogLayer::menuCallback(obj);
    
    if(!this->isCloseItem(obj)){
        int tag=((CCNode*)obj)->getTag();
        
        switch (tag) {
            case kTagXiangce:{
                ImagePickerDialogLayer* layer=ImagePickerDialogLayer::create(kImagePickerTypeAssetsGroup,"");
                layer->setPosition(this->getParent()->convertToNodeSpace(CCPointZero));
                this->pushDialog(layer);
            }
                break;
            case kTagPaizhao:{
                CCImagePicker::sharedImagePicker()->setDelegate(this);
                CCImagePicker::sharedImagePicker()->pickCamera();
            }
                break;
        }
        
    }
}

void ImageDialogLayer::callbackFresh(){
        AlbumObject* obj=(AlbumObject*)this->getAssets()->objectAtIndex(0);
        ImageEditLayer* layer=ImageEditLayer::create(obj->getImage());
        layer->setPosition(this->getParent()->convertToNodeSpace(CCPointZero));
        this->pushDialog(layer);
}