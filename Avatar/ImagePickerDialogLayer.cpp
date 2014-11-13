//
//  ImagePickerDialogLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14/10/12.
//
//

#include "ImagePickerDialogLayer.h"
#include "ImageEditLayer.h"
#include "ImageDialogLayer.h"
#include "UserBarLayer.h"

#define COLUMN_NUMBER 4

ImagePickerDialogLayer::ImagePickerDialogLayer(){
    _assets=CCArray::create();
    _assets->retain();
    _lock=false;
}

ImagePickerDialogLayer::~ImagePickerDialogLayer(){
    _assets->release();
}

ImagePickerDialogLayer* ImagePickerDialogLayer::create(ImagePickerType imagePickType,std::string groupUrl){
    ImagePickerDialogLayer* layer=new ImagePickerDialogLayer();
    layer->setImagePickerType(imagePickType);
    layer->setGroupUrl(groupUrl);
    if (layer&&layer->init()) {
        layer->autorelease();
    }else{
        CC_SAFE_DELETE(layer);
    }
    return layer;
}

bool ImagePickerDialogLayer::init()
{
    if ( !DialogLayer::init() )
    {
        return false;
    }
    
    
    this->setTitle("相册选择");
    
    _gridView = KEGridView::create(this,CCSizeMake(576, 306),this,CCSizeMake(144, 102));
    _gridView->setDirection(kCCScrollViewDirectionVertical);
    _gridView->setVerticalFillOrder(kCCTableViewFillTopDown);
    _gridView->setClippingToBounds(true);
    m_contentLayer->addChild(_gridView);
    
    CCSprite* fanhui=CCSprite::createWithSpriteFrameName("touxiang_fanhui.png");
    fanhui->setScale(0.87);
    CCMenuItemSprite* fanhuiItem=CCMenuItemSprite::create(fanhui, fanhui, this, menu_selector(ImagePickerDialogLayer::menuCallback));
    fanhuiItem->setPosition(ccp(19,368));
    fanhuiItem->setTag(1000);
    this->addMenuItem(fanhuiItem,true);
    
    //调取assets数据
    CCImagePicker::sharedImagePicker()->setDelegate(this);
    CCImagePicker::sharedImagePicker()->findAssets(_imagePickType,_groupUrl);
    
    return true;
}

void ImagePickerDialogLayer::enableTouch(){
    DialogLayer::enableTouch();
    CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    pDispatcher->setPriority(this->getDelegate()->topHandlerPriority()-1, _gridView);
}

void ImagePickerDialogLayer::gridCellTouched(CCTableView* table, CCTableViewCell* cell,int indexInIdx)
{
    
    if (!_lock) {
        //j计算元素序号
        int row=cell->getIdx();
        int column=indexInIdx;
        int index=COLUMN_NUMBER*row+column;
        
        AlbumObject* obj=(AlbumObject*)_assets->objectAtIndex(index);
        const char* url=obj->getUrl().c_str();
        if(obj->getUrl().length()>0){
            _lock=true;
            if (obj->getUrl().length()>0) {
                switch (_imagePickType) {
                    case kImagePickerTypeAssetsGroup:{
                        ImagePickerDialogLayer* layer=ImagePickerDialogLayer::create(kImagePickerTypeAssets,url);
                        layer->setPosition(this->getParent()->convertToNodeSpace(CCPointZero));
                        this->pushDialog(layer);
                        _lock=false;
                    }
                        break;
                    case kImagePickerTypeAssets:{
                        CCImagePicker::sharedImagePicker()->findAssets(kImagePickerTypePhoto, url);
                    }
                        break;
                    default:
                        break;
                }
            }
        }else{
            CCObject* obj;
            int i=0;
            CCARRAY_FOREACH(_assets, obj){
                AlbumObject* album=(AlbumObject*)obj;
                CCLog("%d ## %s %s",i,album->getName().c_str(),album->getUrl().c_str());
                i++;
            }
            _lock=false;
        }
    }
}

CCSize ImagePickerDialogLayer::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(576, 102);
}

CCTableViewCell* ImagePickerDialogLayer::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    if (_assets->count()==0) {
        //添加没有标记
    }
    
    CCTableViewCell *cell = table->dequeueCell();
    if (true){
        //  if (!cell){
        cell = new CCTableViewCell();
        cell->autorelease();
        
        if (_assets->count()>0) {
            
            for (int i=0; i<COLUMN_NUMBER; i++) {
                int index=idx*COLUMN_NUMBER+i;
                if (index>=_assets->count()) {
                    break;//最后一行判断
                }
                
                CCLayer* gridCell=CCLayer::create();
                
                CCLayerColor* layer=CCLayerColor::create(ccc4(0, 0, 0, 30), 140, 96);
                layer->setPosition(ccp(0, 6));
                
                AlbumObject* albumObject=(AlbumObject*)_assets->objectAtIndex(index);
                CCTexture2D* textrue=new CCTexture2D();
                textrue->autorelease();
                textrue->initWithImage(albumObject->getImage());
                CCSprite* sprite=CCSprite::createWithTexture(textrue);
                sprite->setPosition(ccp(70, 48));
                sprite->setScale(96/sprite->getContentSize().height);
                layer->addChild(sprite);
                gridCell->addChild(layer);
                
                
                if (albumObject->getName().length()>0) {
                    CCLayerColor* labelBg=CCLayerColor::create(ccc4(0, 0, 0, 200), 140, 30);
                    labelBg->setPosition(ccp(0, 6));
                    gridCell->addChild(labelBg);
                    
                    CCLabelTTF* label=CCLabelTTF::create(albumObject->getName().c_str(), "KaiTi.ttf", 20);
                    label->setPosition(ccp(70, 21));
                    gridCell->addChild(label);
                }
                
                gridCell->setPosition(ccp(i*144,0));
                cell->addChild(gridCell);
            }
        }
        
    }
    return cell;
}

unsigned int ImagePickerDialogLayer::numberOfCellsInTableView(CCTableView *table)
{
    int count=(int)_assets->count();
    if (count==0) {
        //没有元素，显示提示
        return 1;
    }
    return (count%COLUMN_NUMBER)==0?count/COLUMN_NUMBER:(count/COLUMN_NUMBER+1);
}

void ImagePickerDialogLayer::callbackFresh(){
    _gridView->reloadData();
    _lock=false;
}

void ImagePickerDialogLayer::callbackWithPhoto(AlbumObject *obj){
    ImageEditLayer* layer=ImageEditLayer::create(obj->getImage());
    layer->setPosition(this->getParent()->convertToNodeSpace(CCPointZero));
    this->pushDialog(layer);
    _lock=false;
}

void ImagePickerDialogLayer::menuCallback(cocos2d::CCObject *obj){
    DialogLayer::menuCallback(obj);
    if(!this->isCloseItem(obj)){
        int tag=((CCNode*)obj)->getTag();
        if (tag==1000) {
            this->popDialog();
        }
    }
}