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
    S_DM->getByKey(m_hanzi, hid);
    
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
    
    UserBarLayer* userBarLayer=UserBarLayer::create();
    userBarLayer->setPosition(S_RM->getPositionWithName("xiezi_userbar"));
    //不可以少，防止从Kapian进入XieziScene时XieziLayer中userBarLayer为空
    S_LM->setDelegate(userBarLayer);
    this->addChild(userBarLayer,ORDER_USERBAR);
    
    CCMenu* pMenu = CCMenu::create();
    
    CCSprite* pFanhui_1=CCSprite::createWithSpriteFrameName("fanhui.png");
    CCMenuItemSprite* pbackItem=CCMenuItemSprite::create(pFanhui_1, pFanhui_1, this, menu_selector(XieziScene::menuCallback));
    pbackItem->setTag(kTagBack);
    pbackItem->setPosition(S_RM->getPositionWithName("global_back"));
    pMenu->addChild(pbackItem);
    
    pMenu->setPosition( CCPointZero );
    pMenu->setTag(kTagMenu);
    this->addChild(pMenu, 1);
    
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
    m_heimao->setBoxEnabled("z-shenti", true);
    m_heimao->setCallback(this, cartoon_selector(XieziScene::pressHeimaoCallback));
    this->addChild(m_heimao);
    
    //主写字层
    m_xieziLayer=XieziLayer::create(m_hanzi,false,m_heimao);
    m_xieziLayer->setDelegate(this);
    this->addChild(m_xieziLayer);
    
    CCSize mainSize=S_RM->getSizeWithName("xiezi_webview_size");
    CCPoint mainPoint=S_RM->getPositionWithName("xiezi_main");
    
    //修改上次进入汉字（用于首页）
    S_UD->setIntegerForKey(LAST_HANZI_ID, m_hanzi->getid());
    S_UD->flush();
    
    m_writeCount=S_DM->getCount("hanzi","writeCount>0");
    
    m_collectedCount=0;
    if (m_hanzi->getisCollected()==0) {
        m_collectedCount=S_DM->getCount("hanzi","isCollected>0");
        if (m_collectedCount>=COLLECT_LIMIT) {
            GuideDialog* guideDialog=GuideDialog::create();
            guideDialog->setText("非常抱歉，本软件为测试版，收藏的卡片数量已经超出了测试版的限制。请关注我们的微信公众号，等待正式版本的发布，谢谢！");
            guideDialog->setMode(kGuideDialogOk);
            m_gudieDialogLayer=GuideDialogLayer::create(kDialogWithText);
            m_gudieDialogLayer->setDelegate(this);
            this->addChild(m_gudieDialogLayer,ORDER_DIALOG);
            m_gudieDialogLayer->setGuideDialogData(guideDialog);
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
            
            KapianCollectLayer* kapianCollectLayer=KapianCollectLayer::create(hanziSprite,kHanzi);
            this->addChild(kapianCollectLayer);
            kapianCollectLayer->collectAnimate();
        }
    }
    
    //打开android按键响应
    this->setKeypadEnabled(true);
    
    return true;
}

void XieziScene::onEnter(){
    CCLayerColor::onEnter();
    BaiduStat::onStatEvent(kBaiduOneEventStart,"SceneRetain","XieziScene");
}

void XieziScene::onExit(){
    CCLayerColor::onExit();
    BaiduStat::onStatEvent(kBaiduOneEventEnd,"SceneRetain","XieziScene");
}

void XieziScene::keyBackClicked(){
    CCNode* menu=this->getChildByTag(kTagMenu);
    CCNode* item=menu->getChildByTag(kTagBack);
    this->menuCallback(item);
}

void XieziScene::keyMenuClicked(){
    
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
            S_DR->popScene();
            
            S_ALP->stop();
            S_AE->stopAllEffects();
            break;
        case kTagXJT:
            m_tupianTabel->scroll(kScrollActionDown);
            break;
        case kTagSJX:
            m_tupianTabel->scroll(kScrollActionUp);
            break;
    }
}

void XieziScene::pressHeimaoCallback(){
    m_heimao->action("heimao_xiezi_press");
}

/**
 * 仅处理非汉字相关，如黑猫事件回调等
 * 其余处理在XieziLayer中
 */
void XieziScene::webCallBack(WebCallBackCMD cmd){
    switch (cmd) {
        case kWebCallBackWriteHanziOk:{
            int writeCount=m_hanzi->getwriteCount();
            
            if (writeCount<=3) {
                CCString* str=CCString::createWithFormat("heimao_xieziOk%d",writeCount);
                m_heimao->action(str->getCString());
            }else if(writeCount<6){
                m_heimao->action("heimao_xieziOkgt3");
            }else if(writeCount==6){
                m_heimao->action("heimao_xieziOk6");
            }else if(writeCount>6){
                m_heimao->action("heimao_xieziOkgt6");
            }
        }
            break;
        case kWebCallBackChangeToWriteMode:{
            
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

/**
 *注意解决需要等待星星结束后才更新等级数据的问题
 */
void XieziScene::xingxingAnimateEnd(){
}

void XieziScene::tupianLoadCallBack(int count){
    if (count<=TABLE_VISIBLE_COUNT) {
        CCNode* menu=this->getChildByTag(kTagMenu);
        menu->getChildByTag(kTagSJX)->setVisible(false);
        menu->getChildByTag(kTagXJT)->setVisible(false);
    }
}

void XieziScene::tupianTouchCallBack(Tupian* tupian,bool isOverLimit){
    m_xieziLayer->unschedule(schedule_selector(XieziLayer::dingShiTiXing));
    
    
    if (isOverLimit) {
        GuideDialog* guideDialog=new GuideDialog();
        guideDialog->autorelease();
        guideDialog->setText("非常抱歉，收藏的卡片数量已经超出了免费版本限制，请检查账号状态。");
        guideDialog->setMode(kGuideDialogOk);
        GuideDialogLayer* gudieDialogLayer=GuideDialogLayer::create(kDialogWithText);
        gudieDialogLayer->setDelegate(this);
        this->addChild(gudieDialogLayer,ORDER_DIALOG);
        gudieDialogLayer->setGuideDialogData(guideDialog);
        
        m_xieziLayer->getWebView()->setVisible(false);
    }else{
        
        m_xieziLayer->setVisible(false);
        
        //图片浏览层
        TupianBrowserLayer* m_tupianBrowser=TupianBrowserLayer::create(this,tupian);
        m_tupianBrowser->setPosition(CCPointZero);
        this->addChild(m_tupianBrowser,100);
        
        LevelDelegate* ld=S_LM->getDelegate();
        if (ld) {
            ld->setPosition(S_RM->getPositionWithName("xiezi_userbar_browser"));
        }
    }
}

void XieziScene::tupianTableScrollCallBack(ScrollState state,int cellCount){
    if (cellCount>TABLE_VISIBLE_COUNT) {
        CCNode* menu=this->getChildByTag(kTagMenu);
        switch (state) {
            case kScrollStateTop:{
                menu->getChildByTag(kTagSJX)->setVisible(false);
            }
                
                break;
            case kScrollStatemiddle:{
                menu->getChildByTag(kTagSJX)->setVisible(true);
                menu->getChildByTag(kTagXJT)->setVisible(true);
            }
                break;
            case kScrollStateBottom:{
                menu->getChildByTag(kTagXJT)->setVisible(false);
            }
                
                break;
                
            default:
                break;
        }
    }
}

void XieziScene::hideBrowserCallBack(){
    m_xieziLayer->setVisible(true);
    
    LevelDelegate* ld=S_LM->getDelegate();
    if (ld) {
        ld->setPosition(S_RM->getPositionWithName("xiezi_userbar"));
    }
}

void XieziScene::dialogCallBack(GuideDialogCMD cmd){
    if (m_gudieDialogLayer!=NULL) {
        m_gudieDialogLayer->removeFromParentAndCleanup(true);
        m_gudieDialogLayer=NULL;
    }
    m_xieziLayer->getWebView()->setVisible(true);
    m_xieziLayer->getWebView()->callWebWithJs("setMode(Modes.kDrawHanzi);");
}
