//
//  ImageEditLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14/10/11.
//
//

#include "ImageEditLayer.h"
#include "resource.h"
#include "ImageDialogLayer.h"

#define STENCIL_SCALE 1.5

enum{
    kTagConfirm,
    kTagBack
};

ImageEditLayer::ImageEditLayer(){
    _assets=CCArray::create();
    _assets->retain();
    
    _allTargetedHandlers=CCArray::create();
    _allTargetedHandlers->retain();
    
    _touchs=CCArray::create();
    _touchs->retain();
}

ImageEditLayer::~ImageEditLayer(){
    _assets->release();
    
    _image->release();
    
    _allTargetedHandlers->release();
    
    _touchs->release();
}

ImageEditLayer* ImageEditLayer::create(CCImage* image){
    ImageEditLayer* layer=new ImageEditLayer();
    layer->_image=image;
    layer->_image->retain();
    if (layer&&layer->init()) {
        layer->autorelease();
    }else{
        CC_SAFE_DELETE(layer);
    }
    return layer;
}


bool ImageEditLayer::init(){
    if (DialogLayer::init()) {
        
        
        this->setTitle("头像编辑");
        
        CCPoint middle=ccp(286, 154);
        CCSize size=CCSizeMake(570,300);
        
        CCClippingNode* clippingNode = CCClippingNode::create();
        //设置裁剪区域大小
        clippingNode->setContentSize(size);
        clippingNode->setAnchorPoint(ccp(0.5, 0.5));
        clippingNode->setPosition(middle);
        m_contentLayer->addChild(clippingNode);
        
        CCTexture2D* textrue=new CCTexture2D();
        textrue->autorelease();
        textrue->initWithImage(_image);
        _sprite=CCSprite::createWithTexture(textrue);
        _initPoint=ccp(clippingNode->getContentSize().width/2, clippingNode->getContentSize().height/2);
        _sprite->setPosition(_initPoint);
        
        CCSize spriteSize=_sprite->getContentSize();
        if (size.width/spriteSize.width<size.height/spriteSize.height) {
            _scale=size.width/spriteSize.width;
        }else{
            _scale=size.height/spriteSize.height;
        }
        
        _sprite->setScale(_scale);
        clippingNode->addChild(_sprite);
        
        //创建裁剪模板，裁剪节点将按照这个模板来裁剪区域
        CCDrawNode *stencil = CCDrawNode::create();
        CCPoint rectangle[4];
        rectangle[0] = ccp(0, 0);
        rectangle[1] = ccp(clippingNode->getContentSize().width, 0);
        rectangle[2] = ccp(clippingNode->getContentSize().width, clippingNode->getContentSize().height);
        rectangle[3] = ccp(0, clippingNode->getContentSize().height);
        
        ccColor4F white = {1, 1, 1, 1};
        //画一个多边形 这画一个200x200的矩形作为模板
        stencil->drawPolygon(rectangle, 4, white, 1, white);
        clippingNode->setStencil(stencil);
        
        //用来设置显示裁剪区域还是非裁剪区域的
        clippingNode->setInverted(false);//在裁剪区域内显示加入的内容
        
        _clip=CCClippingNode::create();//创建裁剪节点，成员变量
        _clip->setInverted(true);//设置底板可见
        _clip->setAlphaThreshold(0.0f);//设置alpha为0
        m_contentLayer->addChild(_clip);//添加裁剪节点
        
        _mask=CCLayerColor::create(ccc4(0,0,0,160),size.width,size.height);
        CCPoint point=ccp(middle.x-size.width/2,middle.y-size.height/2);
        _mask->setPosition(point);
        _clip->addChild(_mask);//为裁剪节点添加一个黑色带透明（看起了是灰色）的底板
        
        _stencil=CCSprite::create("default_avatar.png");//使用头像原图作为模板
        _stencil->setPosition(middle);
        _stencil->setScale(STENCIL_SCALE);
        _clip->setStencil(_stencil);//设置模版
        
        CCSprite* queding=CCSprite::createWithSpriteFrameName("touxiang_queding.png");
        CCMenuItemSprite* item=CCMenuItemSprite::create(queding, queding, this, menu_selector(ImageEditLayer::menuCallback));
        item->setPosition(ccp(290,-30));//注意contentlayer坐标原点位置
        item->setScale(0.7);
        item->setTag(kTagConfirm);
        this->addMenuItem(item,true);
        
        CCSprite* fanhui=CCSprite::createWithSpriteFrameName("touxiang_fanhui.png");
        fanhui->setScale(0.87);
        CCMenuItemSprite* fanhuiItem=CCMenuItemSprite::create(fanhui, fanhui, this, menu_selector(ImageEditLayer::menuCallback));
        fanhuiItem->setPosition(ccp(19,368));
        fanhuiItem->setTag(kTagBack);
        this->addMenuItem(fanhuiItem,true);
        
        CCPoint contentPoint=m_contentLayer->getPosition();
        _rect=CCRectMake(contentPoint.x+point.x, contentPoint.y+point.y, size.width,size.height);
        
        return true;
    }
    return false;
}

void ImageEditLayer::onEnter(){
    DialogLayer::onEnter();
    
    S_DR->getTouchDispatcher()->removeDelegate(this);
    S_DR->getTouchDispatcher()->removeDelegate(m_menu);
    
    this->unscheduleAllSelectors();
    CCArray* array=CCDirector::sharedDirector()->getTouchDispatcher()->getAllTargetedHander();
    _allTargetedHandlers->removeAllObjects();
    CCObject* obj;
    CCARRAY_FOREACH(array, obj){
        _allTargetedHandlers->addObject(obj);
    }
    S_DR->getTouchDispatcher()->removeAllDelegates();
    
    S_DR->getTouchDispatcher()->addStandardDelegate(this, 0);
}

void ImageEditLayer::onExit(){
    DialogLayer::onExit();
    
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void ImageEditLayer::callbackFresh(){
    if(_assets->count()>0){
        AlbumObject* obj=(AlbumObject*)this->getAssets()->objectAtIndex(0);
        CCTexture2D* textrue=new CCTexture2D();
        textrue->autorelease();
        textrue->initWithImage(obj->getImage());
        _sprite->setTexture(textrue);
        
        _sprite->setScale(_scale);
    }
}

void ImageEditLayer::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
    CCSetIterator iter = pTouches->begin();
    for (int i=0; i<pTouches->count(); i++) {
        CCTouch* pTouch = (CCTouch*)(*iter++);
        _touchs->addObject(pTouch);
    }
    
    if(_touchs->count()>=2)  //如果触摸点不少于两个
    {
        CCPoint point1=((CCTouch*)_touchs->objectAtIndex(0))->getLocation();
        CCPoint point2=((CCTouch*)_touchs->objectAtIndex(1))->getLocation();
        
        _distance=sqrt((point2.x-point1.x)*(point2.x-point1.x)+(point2.y-point1.y)*(point2.y-point1.y));//计算两个触摸点距离
        _deltaX = (point1.x + point2.x)/2 - _sprite->getPositionX();     //得到两个触摸点中点和精灵锚点的差值
        _deltaY = (point1.y + point2.y)/2 - _sprite->getPositionY();
    }else if(_touchs->count()==1){
        CCPoint point=((CCTouch*)_touchs->objectAtIndex(0))->getLocation();
        
        CCNode* confirm=m_menu->getChildByTag(kTagConfirm);
        CCNode* back=m_menu->getChildByTag(kTagBack);
        if(!this->touchMenu(confirm, point)){
            if(!this->touchMenu(back, point)){
                this->touchMenu(m_closeItem, point);
            }
        }
        
    }
}

void ImageEditLayer::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
    this->moveAndEnd();
}

void ImageEditLayer::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
    this->moveAndEnd();
    CCSetIterator iter = pTouches->begin();
    for (int i=0; i<pTouches->count(); i++) {
        _touchs->removeObject(*iter++);
    }
}

void ImageEditLayer::moveAndEnd(){
    if(_touchs->count()>=2)  //如果触摸点不少于两个
    {
        //        CCLog("多点");
        
        if (_distance==0) {
            return;
        }
        
        CCPoint point1=((CCTouch*)_touchs->objectAtIndex(0))->getLocation();
        CCPoint point2=((CCTouch*)_touchs->objectAtIndex(1))->getLocation();
        
        
        double preDistance=_distance;
        _distance = sqrt((point2.x-point1.x)*(point2.x-point1.x)+(point2.y-point1.y)*(point2.y-point1.y));//计算两个触摸点距离
        
        double preScale=_scale;
        _scale = _distance/preDistance * preScale;                      //   新的距离 / 老的距离  * 原来的缩放比例，即为新的缩放比例
        //        CCLog("preDistance=%lf,_distance=%lf,preScale=%lf,_scale=%lf",preDistance,_distance,preScale,_scale);
        
        if (_scale<0.1) {
            _scale=0.1;
            _distance=_scale*preDistance/preScale;
        }else{
            _sprite->setScale(_scale);
            double x = (point2.x+point1.x)/2 - _deltaX;      //计算两触点中点与精灵锚点的差值
            double y = (point2.y+point1.y)/2 - _deltaY;
            _sprite->setPosition(ccp(x,y));                        //保持两触点中点与精灵锚点的差值不变
            _deltaX = (point1.x+ point2.x)/2 - _sprite->getPositionX();       //计算新的偏移量
            _deltaY = (point2.y + point1.y)/2 - _sprite->getPositionY();
        }
        
    }else if(_touchs->count()==1){
        CCTouch* touch=(CCTouch*)_touchs->objectAtIndex(0);
        CCPoint pt=touch->getLocation();
        
        if (_rect.containsPoint(pt)) {
            CCPoint lastPt=touch->getPreviousLocation();
            CCPoint subPt=ccpSub(pt, lastPt);
            _sprite->setPosition(ccpAdd(_sprite->getPosition(), subPt));
        }
    }
}

bool ImageEditLayer::touchMenu(cocos2d::CCNode *item, cocos2d::CCPoint point){
    CCSize size=item->getContentSize();
    CCPoint pt= item->convertToNodeSpace(point);
    if (CCRectMake(0, 0, size.width, size.height).containsPoint(pt)) {
        this->menuCallback(item);
        return true;
    }else return false;
}

void ImageEditLayer::menuCallback(cocos2d::CCObject *obj){
    DialogLayer::menuCallback(obj);
    
    if(!this->isCloseItem(obj)){
        
        int tag=((CCNode*)obj)->getTag();
        if (tag==kTagBack) {
            this->popDialog();
            
        }else {
            
            //未实现飞行动画✈️
            
            CCPoint point=_sprite->getPosition();
            CCPoint offsetPoint=ccpSub(point, _initPoint);
            offsetPoint=ccp(offsetPoint.x, offsetPoint.y);
            CCSize spriteSize=_sprite->getContentSize()*_scale;
            CCSize stencilSize=_stencil->getContentSize()*STENCIL_SCALE;
            CCPoint p=ccp(spriteSize.width/2-stencilSize.width/2-offsetPoint.x,spriteSize.height/2-stencilSize.height/2+offsetPoint.y);
            
            CCRect rect=CCRectMake(p.x/_scale,p.y/_scale,stencilSize.width/_scale,stencilSize.height/_scale);
            
            CCLog("%f %f %f %f",rect.origin.x,rect.origin.y,rect.size.width,rect.size.height);
            
            CCRenderTexture* render = CCRenderTexture::create(150,150);
            //渲染纹理开始捕捉
            render->begin();
            //当前场景参与绘制
            
            CCSprite* rs=CCSprite::createWithTexture(_sprite->getTexture(), rect);
            rs->setScale(150/rect.size.width);
            rs->setPosition(ccp(150/2,150/2));
            rs->visit();
            //结束捕捉
            render->end();
            //保存为jpg
            render->saveToFile("avatar.jpg", kCCImageFormatJPEG);
            
            UserBarLayer* userBarLayer=S_LM->getDelegate();
            userBarLayer->delayToFreshAvatar();
            this->removeFromParent();
            
        }
        
    }
    
    
    //恢复底层事件
    CCObject* object;
    CCARRAY_FOREACH(_allTargetedHandlers, object){
        CCTargetedTouchHandler* handler=(CCTargetedTouchHandler*)object;
        if(handler->getDelegate()){
            S_DR->getTouchDispatcher()->addTargetedDelegate(handler->getDelegate(), handler->getPriority(), handler->isSwallowsTouches());
        }
    }
    
}
