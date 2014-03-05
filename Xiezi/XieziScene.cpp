#include "XieziScene.h"
#include "ResManager.h"
#include "resource.h"
#include "DBManager.h"
#include "KapianScene.h"
#include "Hanzi.h"
#include "Utils.h"
#include "SimpleAudioEngine.h"
#include "KapianCollectLayer.h"

using namespace CocosDenshion;

enum {
    kTagRewrite=0,
    kTagWrite,
    kTagGuangbo,
    kTagBack,
    kTagXBJ,
    kTagSJX,
    kTagXJT,
    kTagMenu,
    kTagCollect,
    kTagCollectKa,
    kTagCollectChouti,
    kTagStreak,
    kTagSnapshot,
    kTagXingxing1,
    kTagXingxing2,
    kTagXingxing3,
    kTagXingxing4,
    kTagXingxingCount
};

class StarAndBorder :public CCObject{
public:
    ~StarAndBorder(){m_star->release();m_border->release();}
    void setAndRetainStar(CCSprite* star){m_star=star;m_star->retain();};
    void setAndRetainBorder(CCSprite* border){m_border=border;m_border->retain();};
    
    CC_SYNTHESIZE(CCSprite*, m_star, Star);
    CC_SYNTHESIZE(CCSprite*, m_border, Border);
};


#define TABLE_VISIBLE_COUNT 3

#define ICON_PATH CCFileUtils::sharedFileUtils()->getWritablePath()+((Tupian*)m_kapian)->geticonPath()

CCScene* XieziScene::scene(int hid)
{
    return XieziScene::scene(hid, NULL, 0);
}

CCScene* XieziScene::scene(int hid,XieziSceneDelegate* xieziSceneDelegate,int indexInKapianTable)
{
    CCScene *scene = CCScene::create();
    XieziScene *layer = new XieziScene(hid,xieziSceneDelegate,indexInKapianTable);
    
    if (layer && layer->init())
    {
        layer->autorelease();
    }
    else
    {
        delete layer;
        layer = NULL;
    }
    
    scene->addChild(layer);
    
    return scene;
}

XieziScene::XieziScene(int hid,XieziSceneDelegate* xieziSceneDelegate,int indexInKapianTable){
    this->m_xieziSceneDelegate=xieziSceneDelegate;
    this->m_indexInKapianTable=indexInKapianTable;
    
    m_hanzi=new Hanzi();
    S_DM->getById(m_hanzi, hid);
    
    isWriteFinished=false;
    m_gudieDialogLayer=NULL;
}


XieziScene::~XieziScene(){
}

bool XieziScene::init()
{
    if ( !CCLayerColor::initWithColor(ccc4(255, 255, 255,255)) )
    {
        return false;
    }
    
    
    CCSprite* pSprite = CCSprite::createWithSpriteFrameName("beijing.png");
    pSprite->setPosition(S_RM->getJpgBgPosition());
    this->addChild(pSprite);
    
    CCMenu* pMenu = CCMenu::create();
    
    CCSprite* pFanhui_1=CCSprite::create("fanhui.png");
    CCMenuItemSprite* pbackItem=CCMenuItemSprite::create(pFanhui_1, pFanhui_1, this, menu_selector(XieziScene::menuCallback));
    pbackItem->setTag(kTagBack);
    pbackItem->setPosition(S_RM->getPositionWithName("global_back"));
    pMenu->addChild(pbackItem);
    
    pMenu->setPosition( CCPointZero );
    pMenu->setTag(kTagMenu);
    this->addChild(pMenu, 1);

    //小背景
    CCSprite* pXbj = CCSprite::createWithSpriteFrameName("dakuang.png");
    pXbj->setPosition(S_RM->getPositionWithName("xiezi_xbj"));
    pXbj->setTag(kTagXBJ);
    this->addChild(pXbj);
    
    CCSprite* pSjt=CCSprite::createWithSpriteFrameName("jiantou_1.png");
    CCMenuItemSprite* pSjtItem=CCMenuItemSprite::create(pSjt, NULL, this, menu_selector(XieziScene::menuCallback));
    pSjtItem->setPosition(S_RM->getPositionWithName("xiezi_sjt"));
    pSjtItem->setTag(kTagSJX);
    pMenu->addChild(pSjtItem);
    
    CCSprite* pXjt=CCSprite::createWithSpriteFrameName("jiantou_2.png");
    CCMenuItemSprite* pXjtItem=CCMenuItemSprite::create(pXjt, NULL, this, menu_selector(XieziScene::menuCallback));
    pXjtItem->setPosition(S_RM->getPositionWithName("xiezi_xjt"));
    pXjtItem->setTag(kTagXJT);
    pMenu->addChild(pXjtItem);
    
    //相关图片列表，放在前面防止挡住收集动画
    TupianTablePosition tupianTablePosition=TupianTablePosition("xiezi_Tupian_table","xiezi_Tupian_table_size","xiezi_Tupian_table_cell_size","xiezi_Tupian_table_cell_thumb_size","xiezi_Tupiankuang_in_cell","xiezi_Tupianthumb_in_cell",kCCScrollViewDirectionVertical);
    m_tupianTabel=TupianTableLayer::create(this,kHanzi,m_hanzi,tupianTablePosition);
    this->addChild(m_tupianTabel);
    
    //黑猫
    m_heimao=Heimao::create();
    m_heimao->setPosition(S_RM->getPositionWithName("xiezi_heimao"));
    m_heimao->setScale(1.2);
    m_heimao->action("heimao_stand");
    this->addChild(m_heimao);
    
    
    CCSize mainSize=S_RM->getSizeWithName("xiezi_main_size");
    CCPoint mainPoint=S_RM->getPositionWithName("xiezi_main");
    
    //修改上次进入汉字（用于首页）
    S_UD->setIntegerForKey(LAST_HANZI_ID, m_hanzi->getid());
    S_UD->flush();
    
    m_writeCount=S_DM->getCount("hanzi","writeCount>0");
    
    m_collectedCount=0;
    if (m_hanzi->getisCollected()==0) {
        
        m_collectedCount=S_DM->getCount("hanzi","isCollected>0");
        if (m_collectedCount>=COLLECT_LIMIT) {
            GuideDialog* guideDialog=new GuideDialog();
            guideDialog->setText("非常抱歉，本软件为测试版，收藏的卡片数量已经超出了测试版的限制。请关注我们的微信公众号，等待正式版本的发布，谢谢！");
            guideDialog->setMode(kGuideDialogOk);
            m_gudieDialogLayer=GuideDialogLayer::create(kDialogWithText);
            m_gudieDialogLayer->setDelegate(this);
            this->addChild(m_gudieDialogLayer,INT_MAX);
            m_gudieDialogLayer->setGuideDialogData(guideDialog);
            CC_SAFE_DELETE(guideDialog);
        }else{
            //收集到汉字
            CCString *sql=CCString::createWithFormat("update hanzi set isCollected=1 where id=%d;",m_hanzi->getid());
            S_DM->executeSql(sql->getCString());
            
            static_userDefaultIncrement(COLLECT_HANZI_COUNT,15);
            
            CCSprite* hanziSprite=CCSprite::createWithSpriteFrameName("tianzige.png");
            hanziSprite->setPosition(mainPoint);
            
            CCLabelTTF* label=CCLabelTTF::create(m_hanzi->getzi().c_str(), "KaiTi.ttf", 400.0);
            label->setColor(ccc3(100,53,14));
            label->setPosition(ccp(mainSize.width/2, mainSize.height/2));
            hanziSprite->addChild(label);
            hanziSprite->setTag(kTagCollect);
            
            KapianCollectLayer* kapianCollectLayer=KapianCollectLayer::create(hanziSprite);
            this->addChild(kapianCollectLayer);
            kapianCollectLayer->collectAnimate();
        }
    }
    
    //演示或写字切换按钮
    CCMenuItemSprite* pPen=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("pen.png"),NULL,this, menu_selector(XieziScene::callWeb));
    pPen->setTag(kTagWrite);
    pPen->setPosition(S_RM->getPositionWithName("xiezi_bi"));
    pMenu->addChild(pPen);
    
    CCMenuItemSprite* pQingchu=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("xiangpi.png"), NULL, this, menu_selector(XieziScene::callWeb));
    pQingchu->setPosition(S_RM->getPositionWithName("xiezi_xiangpi"));
    pQingchu->setTag(kTagRewrite);
    pMenu->addChild(pQingchu);
    
    //广播
    CCMenuItemSprite* pGuangbo=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("guangbo.png"), NULL, this, menu_selector(XieziScene::callWeb));
    pGuangbo->setPosition(S_RM->getPositionWithName("xiezi_guangbo"));
    pGuangbo->setTag(kTagGuangbo);
    pMenu->addChild(pGuangbo);
    
    //星星
    this->setStar(true);
    
    //拼音
    CCLabelTTF *pinyinLabel = CCLabelTTF::create(m_hanzi->getpinyinyindiao().c_str(), "KaiTi.ttf", 30.0);
    pinyinLabel->setColor(ccc3(255, 255, 255));
    pinyinLabel->setPosition(S_RM->getPositionWithName("xiezi_pinyin"));
    this->addChild(pinyinLabel);
    
    //初始化星星效果
    m_emitter = CCParticleSun::create();
    m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("fire.png") );
    m_emitter->setPosition(S_RM->getPositionWithName("xiezi_huoqiu"));
    this->addChild(m_emitter, 10);
    
    //打开android按键响应
    this->setKeypadEnabled(true);
    
    return true;
}

void XieziScene::onEnter(){
    CCLayerColor::onEnter();
    
    CCPoint mainPoint=S_RM->getPositionWithName("xiezi_main");
    //添加WEBVIEW
    CCSize win_size=S_DR->getWinSize();
    CCSize webViewSize=S_RM->getSizeWithName("xiezi_webview_size");
    CCRect rect=CCRectMake(mainPoint.x-webViewSize.width/2,win_size.height-mainPoint.y-webViewSize.height/2, webViewSize.width, webViewSize.height);//UI坐标,从左上角计算
    m_webView=CCWebView::create("AppStroke.html",rect);
    m_webView->setDelegate(this);
    this->addChild(m_webView);
    if (m_collectedCount>COLLECT_LIMIT) {
        m_webView->setVisible(false);
    }else{
        m_webView->setVisible(true);
    }
}

void XieziScene::onExit(){
    CCLayerColor::onExit();
    this->removeChild(m_webView, false);
}

void XieziScene::keyBackClicked(){
    CCNode* menu=this->getChildByTag(kTagMenu);
    CCNode* item=menu->getChildByTag(kTagBack);
    this->menuCallback(item);
}

void XieziScene::keyMenuClicked(){
    
}

void XieziScene::dingShiTiXing(){
    switch (m_hanzi->getwriteCount()) {
        case 1:
            m_heimao->action("heimao_tishi1");
            break;
        case 2:
            m_heimao->action("heimao_tishi2");
            break;
        case 3:
            m_heimao->action("heimao_tishi3");
            break;
        case 0:
        default:
            m_heimao->action("heimao_tishi0");
            break;
    }
    
}

void XieziScene::setStar(bool isInit){
    int writeCount=m_hanzi->getwriteCount();
    if ((isInit&&writeCount>4)||writeCount==4) {
        float duration=0.5f;
        CCPoint xingxingPoint=S_RM->getPositionWithName("xiezi_xingxing");
        if(!isInit){
            for (int i=0; i<3; i++) {
                //            this->removeChildByTag(kTagXingxing1+i);
                CCSprite* sprite=(CCSprite*)this->getChildByTag(kTagXingxing1+i);
                sprite->runAction(CCSpawn::create(CCMoveTo::create(duration, xingxingPoint),CCFadeOut::create(duration),NULL));
            }
        }
        CCSprite* xingxing=CCSprite::createWithSpriteFrameName("xingxing_2.png");
        xingxing->setPosition(xingxingPoint);
        xingxing->setTag(kTagXingxing4);
        xingxing->setOpacity(0);
        this->addChild(xingxing);
        xingxing->runAction(CCFadeIn::create(duration));
        CCSprite* border=CCSprite::createWithSpriteFrameName("xingxing_border.png");
        border->setPosition(ccp(xingxing->getContentSize().width/2, xingxing->getContentSize().height/2));
        border->setVisible(false);
        xingxing->addChild(border);
        CCSprite* x=CCSprite::createWithSpriteFrameName("X.png");
        x->setPosition(S_RM->getPositionWithName("xiezi_xingxing_x"));
        x->setOpacity(0);
        this->addChild(x);
        x->runAction(CCFadeIn::create(duration));
        CCString* countString=CCString::createWithFormat("%d",writeCount);
        if (!isInit) {
            countString=CCString::createWithFormat("%d",writeCount-1);
        }
        CCLabelBMFont *xingCount = CCLabelBMFont::create(countString->getCString(), "font.fnt");
        xingCount->setPosition(S_RM->getPositionWithName("xiezi_xingxing_count"));
        xingCount->setTag(kTagXingxingCount);
        xingCount->setAnchorPoint(ccp(0.5, 0.5));
        xingCount->setOpacity(0);
        this->addChild(xingCount);
        xingCount->runAction(CCFadeIn::create(duration));
    }
    if (isInit) {
        if (writeCount<=3) {
            for (int i=0; i<3; i++) {
                int star=1;
                if(i<m_hanzi->getwriteCount()){
                    star=2;
                }
                
                CCString *starName = CCString::createWithFormat("xingxing_%d.png", star);
                CCString *posName = CCString::createWithFormat("xiezi_xingxing_%d", i+1);
                CCPoint point=S_RM->getPositionWithName(posName->getCString());
                
                CCSprite* xingxing=CCSprite::createWithSpriteFrameName(starName->getCString());
                xingxing->setPosition(point);
                xingxing->setTag(kTagXingxing1+i);
                this->addChild(xingxing);
                
                CCSprite* border=CCSprite::createWithSpriteFrameName("xingxing_border.png");
                border->setPosition(ccp(xingxing->getContentSize().width/2, xingxing->getContentSize().height/2));
                border->setVisible(false);
                xingxing->addChild(border);
            }
        }
        
    }else{
        const char* posName;
        if (writeCount<=3) {
            posName = CCString::createWithFormat("xiezi_xingxing_%d", writeCount)->getCString();
        }else{
            posName="xiezi_xingxing";
        }
        CCPoint point=S_RM->getPositionWithName(posName);
        
        ccBezierConfig bezier;
        bezier.controlPoint_1 = S_RM->getPositionWithName("xiezi_bezier_start");
        if (writeCount==3) {
            bezier.controlPoint_2 = S_RM->getPositionWithName("xiezi_bezier_end_1");
        }else{
            bezier.controlPoint_2 = S_RM->getPositionWithName("xiezi_bezier_end_2");
        }
        bezier.endPosition = point;
        CCActionInterval*  bezierTo = CCBezierTo::create(1, bezier);
        m_emitter->runAction(CCSequence::create(bezierTo,CCCallFunc::create(this,  callfunc_selector(XieziScene::xingxingAnimate)),NULL));
    }
}

void XieziScene::xingxingAnimate(){
    int writeCount=m_hanzi->getwriteCount();
    CCSprite* xingxing;
    if (writeCount<=3) {
        xingxing=(CCSprite*)this->getChildByTag(kTagXingxing1+writeCount-1);
    }else{
        xingxing=(CCSprite*)this->getChildByTag(kTagXingxing4);
        CCLabelBMFont* xingxingCount=(CCLabelBMFont*)this->getChildByTag(kTagXingxingCount);
        xingxingCount->setString(CCString::createWithFormat("%d",m_hanzi->getwriteCount())->getCString());
        xingxingCount->runAction(CCSequence::createWithTwoActions(CCScaleTo::create(0.2, 1.5), CCScaleTo::create(0.2, 1)));
    }
    xingxing->setDisplayFrame(S_SF->spriteFrameByName("xingxing_2.png"));
    CCSprite* border=(CCSprite*)xingxing->getChildren()->objectAtIndex(0);
    border->setScale(1.f);
    border->setVisible(true);
    border->runAction(CCSpawn::create(CCScaleTo::create(0.5, 5.f),CCFadeOut::create(0.5),NULL));
    m_emitter->setPosition(S_RM->getPositionWithName("xiezi_huoqiu"));
}

void XieziScene::menuCallback(CCObject* pSender)
{
    int tag=((CCNode*)pSender)->getTag();
    switch (tag) {
        case kTagBack:
            m_heimao->action("heimao_stand");
            if(m_xieziSceneDelegate!=NULL){
               m_xieziSceneDelegate->xieziCallback(m_indexInKapianTable, m_hanzi->getProgress());
            }
            CCDirector::sharedDirector()->popScene();
            S_AE->stopAllEffects();
            break;
    }
}

void XieziScene::wenziCallback(CCObject* pSender)
{
    int tag=((CCNode*)pSender)->getTag();
    if (tag>0) {
        CCDirector::sharedDirector()->replaceScene(XieziScene::scene(tag));
    }
}

void XieziScene::callWeb(CCObject* pSender)
{
    CCSprite* snapshot=(CCSprite*)this->getChildByTag(kTagSnapshot);
    if(snapshot!=NULL)snapshot->removeFromParentAndCleanup(false);
//    m_webView->setVisible(true);
    
    int tag=((CCNode*)pSender)->getTag();
    switch (tag) {
        case kTagRewrite:
            m_webView->callWebWithJs("reWrite();");
            isWriteFinished=false;
            break;
        case kTagWrite:
            if (m_writeCount>=WRITE_LIMIT&&m_hanzi->getwriteCount()==0&&m_gudieDialogLayer==NULL) {
                GuideDialog* guideDialog=new GuideDialog();
                guideDialog->setText("非常抱歉，本软件为测试版，练习书写的汉字数量已经超出了测试版的限制。请关注我们的微信公众号，等待正式版本的发布，谢谢！");
                guideDialog->setMode(kGuideDialogOk);
                m_gudieDialogLayer=GuideDialogLayer::create(kDialogWithText);
                m_gudieDialogLayer->setDelegate(this);
                this->addChild(m_gudieDialogLayer,INT_MAX);
                m_gudieDialogLayer->setGuideDialogData(guideDialog);
                CC_SAFE_DELETE(guideDialog);
                m_webView->setVisible(false);
            }else{
                m_webView->callWebWithJs("setMode(Modes.kWrite);");
                if (isWriteFinished) {
                    m_webView->callWebWithJs("reWrite();");
                }
            }
            
            break;
        case kTagGuangbo:
        S_AE->playEffect((CCFileUtils::sharedFileUtils()->getWritablePath()+m_hanzi->getcnAudioPath()).c_str());
            break;
    }
}

void XieziScene::webCallBack(std::string cmd){
    CCLog("cmd:%s",cmd.c_str());
    vector<string> splitCmd;
    StringUtils::split(cmd,",",splitCmd);
    int icmd=atoi(splitCmd[0].c_str());
    switch (icmd) {
        case kWebCallBackLoadedFinish:{
            //加载完成
            CCSize webViewSize=S_RM->getSizeWithName("xiezi_webview_size");
            float scaleX=CCEGLView::sharedOpenGLView()->getScaleX();
            float scaleY=CCEGLView::sharedOpenGLView()->getScaleY();
            CCString* str=CCString::createWithFormat("init('%s','%s',%f,%f)",m_hanzi->getcontour().c_str(),m_hanzi->getlocus().c_str(),webViewSize.width*scaleX,webViewSize.height*scaleY);
            m_webView->callWebWithJs(str->getCString());
            this->scheduleOnce(schedule_selector(XieziScene::dingShiTiXing),5.0f);
            
            const char* audio=(CCFileUtils::sharedFileUtils()->getWritablePath()+m_hanzi->getcnAudioPath()).c_str();
            S_AE->playEffect(audio);
        }
            
            break;
        case kWebCallBackWriteHanziOk:{
            //书写成功
            int writeCount=m_hanzi->getwriteCount();
            m_hanzi->setIntwriteCount(++writeCount);
            CCString *sql=CCString::createWithFormat("update hanzi set writeCount=writeCount+1 where id=%d;",m_hanzi->getid());
            S_DM->executeSql(sql->getCString());
            
            //星星动画
            this->setStar();
            
            //排行数据需要统一调整模型
            static_userDefaultIncrement(COLLECT_XINGXING_COUNT,0);
            static_userDefaultIncrement(OVER_XINGXING_COUNT,0);
            
//            m_heimao->doAction("z-guli","z-daijizhayan:0",NULL);
            string effect="mario1.mp3";
//            CCArray* audioArray=CCArray::create();
            if (writeCount<=2) {
                effect="mario2.mp3";
//                audioArray->addObject(CCString::createWithFormat("heimao028.mp3"));
//                audioArray->addObject(CCString::createWithFormat("heimao029.mp3"));
//                if (writeCount==2) {
//                    audioArray->addObject(CCString::createWithFormat("heimao031.mp3"));
//                }
            }else if (writeCount==3){
//                audioArray->addObject(CCString::createWithFormat("heimao033.mp3"));
            }else if (writeCount>3){
//                audioArray->addObject(CCString::createWithFormat("heimao029.mp3:1,heimao027_2.mp3"));
//                audioArray->addObject(CCString::createWithFormat("heimao031.mp3"));
            }
//            int random=(int)(CCRANDOM_0_1()*audioArray->count());
//            CCString* audioPath=(CCString*)audioArray->objectAtIndex(random);
//            S_ALP->play(audioPath->getCString(),NULL);
            
            if (writeCount<=3) {
                CCString* str=CCString::createWithFormat("heimao_xieziOk%d",writeCount);
                m_heimao->action(str->getCString());
            }else{
                m_heimao->action("heimao_xieziOkgt3");
            }
            S_AE->playEffect(effect.c_str());
            
            isWriteFinished=true;
            
//            if (splitCmd.size()>1&&splitCmd[1].length()>10) {
//                CCLog("splitCmd.size()>1");
//                m_webView->setVisible(false);
//                string pngData=StringUtils::base64Decode(splitCmd[1].c_str(), (int)splitCmd[1].length());
//                CCImage* img=new CCImage();
//                img->initWithImageData(const_cast<char*>(pngData.c_str()), (int)pngData.length());
//                CCTexture2D *texture = new CCTexture2D();
//                texture->initWithImage(img);
//                CCSprite* snapshot= CCSprite::createWithTexture(texture);
//                CCPoint point=S_RM->getPositionWithName("xiezi_main");
//                snapshot->setPosition(ccp(point.x,point.y-3));
//                snapshot->setTag(kTagSnapshot);
//                if(snapshot->getContentSize().width>500){
//                    snapshot->setScale(0.5);
//                }
//                this->addChild(snapshot);
//                CC_SAFE_DELETE(img);
//            }
            
        }
            break;
        case kWebCallBackCanvasData:{
            //图片数据
            //从base64转换未图片数据
            
        }
            
            break;
        case kWebCallBackChangeToWriteMode:{
            //切换到写字模式
            if (m_writeCount>=WRITE_LIMIT&&m_hanzi->getwriteCount()==0&&m_gudieDialogLayer==NULL) {
                GuideDialog* guideDialog=new GuideDialog();
                guideDialog->setText("非常抱歉，本软件为测试版，练习书写的汉字数量已经超出了测试版的限制。请关注我们的微信公众号，等待正式版本的发布，谢谢！");
                guideDialog->setMode(kGuideDialogOk);
                m_gudieDialogLayer=GuideDialogLayer::create(kDialogWithText);
                m_gudieDialogLayer->setDelegate(this);
                this->addChild(m_gudieDialogLayer,INT_MAX);
                m_gudieDialogLayer->setGuideDialogData(guideDialog);
                CC_SAFE_DELETE(guideDialog);
                m_webView->setVisible(false);
            }
            this->unschedule(schedule_selector(XieziScene::dingShiTiXing));
        }
            break;
        case kWebCallBackWriteStrokeOk:
            //字的各个笔画
            m_heimao->action("heimao_strokeOk");
            break;
            
        case kWebCallBackWriteStrokeError:
            //写错时调用
            m_heimao->action("heimao_strokeError");
            break;
            
        default:
            break;
    }
}

void XieziScene::tupianLoadCallBack(int count){
    if (count<=TABLE_VISIBLE_COUNT) {
        CCNode* menu=this->getChildByTag(kTagMenu);
        menu->getChildByTag(kTagSJX)->setVisible(false);
        menu->getChildByTag(kTagXJT)->setVisible(false);
    }
}


void XieziScene::tupianTouchCallBack(Tupian* tupian){
    this->unschedule(schedule_selector(XieziScene::dingShiTiXing));
    
    m_webView->setVisible(false);
    //图片浏览层
    TupianBrowserLayer* m_tupianBrowser=TupianBrowserLayer::create(this,tupian);
    m_tupianBrowser->setPosition(CCPointZero);
    this->addChild(m_tupianBrowser,100);
    
    CCSprite* xbj=(CCSprite*)this->getChildByTag(kTagXBJ);
    xbj->setVisible(false);
    
}

void XieziScene::hideBrowserCallBack(){
    m_webView->setVisible(true);
    CCSprite* xbj=(CCSprite*)this->getChildByTag(kTagXBJ);
    xbj->setVisible(true);
}

void XieziScene::dialogCallBack(GuideDialogCMD cmd){
    m_gudieDialogLayer->removeFromParentAndCleanup(true);
    m_gudieDialogLayer=NULL;
    m_webView->setVisible(true);
    m_webView->callWebWithJs("setMode(Modes.kDrawHanzi);");
}
