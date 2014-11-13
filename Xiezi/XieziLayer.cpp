//
//  XieziLayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-5-27.
//
//

#include "XieziLayer.h"
#include "SimpleAudioEngine.h"
#include "KapianCollectLayer.h"
#include "HanziDetailLayer.h"
#include "XieziLog.h"

enum {
    kTagRewrite=0,
    kTagTansuo,
    kTagGuangbo,
    kTagBack,
    kTagXBJ,
    kTagSJX,
    kTagXJT,
    kTagMenu,
    kTagSpriteXiangpi,
    kTagSpriteGuangbo,
    kTagSpriteTansuo,
    kTagXiangpi,
    kTagCollect,
    kTagCollectKa,
    kTagCollectChouti,
    kTagStreak,
    kTagSnapshot,
    kTagXingxing1,
    kTagXingxing2,
    kTagXingxing3,
    kTagXingxing4,
    kTagXingxingCount,
    kTagDetail
};

XieziLayer* XieziLayer::create(Hanzi* hanzi,bool isShowX,Heimao* heimao,string pyyd)
{
    XieziLayer *layer = new XieziLayer();
    layer->m_hanzi=hanzi;
    //获取主拼音Content
    if(pyyd.length()>0){
        S_DM->getByProperty(layer->m_mContent, "MD", pyyd);
    }else{
        S_DM->getByProperty(layer->m_mContent, "MD", hanzi->getZpyyd());
    }
    
    layer->m_isShowX=isShowX;
    layer->m_heimao=heimao;
    if (layer && layer->init())
    {
        layer->autorelease();
    }
    else
    {
        delete layer;
        layer = NULL;
    }
    return layer;
}

XieziLayer::XieziLayer(){
    
    m_isWriteFinished=false;
    
    m_topHandlerPriority=kCCMenuHandlerPriority;
    
    m_delegate=NULL;
    
    m_heimao=NULL;
    
    m_isShowDetail=false;
    
    m_gudieDialogLayer=NULL;
    
    m_mContent=new Content();
    
    m_collected=false;
}

XieziLayer::~XieziLayer(){
    CC_SAFE_DELETE(m_mContent);
}

bool XieziLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCMenu* pMenu = CCMenu::create();
    pMenu->setPosition( CCPointZero );
    pMenu->setTag(kTagMenu);
    this->addChild(pMenu, 1);
    
    //小背景
    CCSprite* pXbj = CCSprite::createWithSpriteFrameName("dakuang.png");
    pXbj->setPosition(S_RM->getPositionWithName("xiezi_xbj"));
    pXbj->setTag(kTagXBJ);
    this->addChild(pXbj);
    
    UserBarLayer* userBarLayer=S_LM->getDelegate();
    if (userBarLayer==NULL) {
        userBarLayer=UserBarLayer::create();
        userBarLayer->setPosition(S_RM->getPositionWithName("xiezi_userbar"));
        this->addChild(userBarLayer,ORDER_USERBAR);
    }
    
    if (m_isShowX) {
        CCSprite* x=CCSprite::create("cha.png");
        x->setScale(1.5);
        x->setPosition(S_RM->getPositionWithName("tupian_close"));
        this->addChild(x);
    }
    
    const char* str;
    const char* pic;
    if (m_hanzi->getwriteCount()>0) {
        str=CCString::createWithFormat("%d",m_hanzi->getwriteCount())->getCString();
        pic="xingxing_2.png";
    }else{
        str="";
        pic="xingxing_1.png";
    }
    m_xingxing=CCSprite::createWithSpriteFrameName(pic);
    m_xingxing->setPosition(S_RM->getPositionWithName("xiezi_xingxing"));
    this->addChild(m_xingxing);
    m_count=CCLabelTTF::create(str, "", 30);
    m_count->setPosition(S_RM->getPositionWithName("xiezi_xingxing"));
    m_count->setColor(ccc3(0, 0, 0));
    this->addChild(m_count);
    
    
    
    //修改上次进入汉字（用于首页）
    S_UD->setIntegerForKey(LAST_HANZI_ID, m_hanzi->getid());
    S_UD->flush();
    
    m_writedHanziCount=S_DM->getCount("hanzi","writeCount>0");
    
    time_t time= TimeUtils::getToday0Time();
    CLAUSE_INIT;
    whereClause.push_back(CCString::createWithFormat("hid=%d",m_hanzi->getid())->getCString());
    whereClause.push_back(CCString::createWithFormat("createTime>%ld",time)->getCString());
    vector<XieziLog*>* logVector=new vector<XieziLog*>();
    S_DM->findScrollData(logVector,"id",whereClause, orderbyClause, groupByClause);
    m_todayXieziCount=logVector->size();
    
    m_collectedCount=0;
    if (m_hanzi->getisCollected()==0) {
        m_collectedCount=S_DM->getCount("hanzi","isCollected>0");
        if (m_collectedCount>=COLLECT_LIMIT) {
            GuideDialog* guideDialog=new GuideDialog();
            guideDialog->autorelease();
            guideDialog->setText("非常抱歉，收藏的卡片数量已经超出了限制，请检查账号状态。");
            guideDialog->setMode(kGuideDialogOk);
            m_gudieDialogLayer=GuideDialogLayer::create(kDialogWithText);
            m_gudieDialogLayer->setDelegate(this);
            this->addChild(m_gudieDialogLayer,ORDER_DIALOG);
            m_gudieDialogLayer->setGuideDialogData(guideDialog);
        }else{
            //收集到汉字
            CCString *sql=CCString::createWithFormat("update hanzi set isCollected=1 where id=%d;",m_hanzi->getid());
            S_DM->executeSql(sql->getCString());
            
            static_userDefaultIncrement(COLLECT_HANZI_COUNT,18);
            
            m_collected=true;
            
        }
    }
    
    typedef struct _ResouceStruct
    {
        string position;
        string pic;
        int tag;
        int spriteTag;
    }XieziActionStruct;
    
    // 在这里配置每个场景要加载的资源
    static XieziActionStruct action[] = {
        {
            "xiezi_tansuo",
            "tansuo.png",
            kTagTansuo,
            kTagSpriteTansuo
        },
        {
            "xiezi_xiangpi",
            "xiangpi.png",
            kTagRewrite,
            kTagSpriteXiangpi
        },
        {
            "xiezi_guangbo",
            "guangbo.png",
            kTagGuangbo,
            kTagSpriteGuangbo
        }
    };
    
    
    for (int i=0; i<3; i++) {
        CCPoint point=S_RM->getPositionWithName(action[i].position.c_str());
        CCSprite* kuang1=CCSprite::createWithSpriteFrameName("xiaokuang_1.png");
        CCSprite* kuang2=CCSprite::createWithSpriteFrameName("xiaokuang_2.png");
        CCMenuItemSprite* kuang=CCMenuItemSprite::create(kuang1, kuang2, this, menu_selector(XieziLayer::callWeb));
        kuang->setPosition(point);
        kuang->setTag(action[i].tag);
        pMenu->addChild(kuang);
        CCSprite* sprite=CCSprite::createWithSpriteFrameName(action[i].pic.c_str());
        sprite->setPosition(ccp(point.x,point.y-3));
        sprite->setTag(action[i].spriteTag);
        this->addChild(sprite,2);
    }
    
    //拼音
    CCLabelTTF *pinyinLabel = CCLabelTTF::create(m_hanzi->getpinyinyindiao().c_str(), "Pinyinok.ttf", 30.0);
    pinyinLabel->setColor(ccc3(255, 255, 255));
    pinyinLabel->setPosition(S_RM->getPositionWithName("xiezi_pinyin"));
    this->addChild(pinyinLabel);
    
    //定时3秒提醒
    if (m_heimao!=NULL) {
        this->scheduleOnce(schedule_selector(XieziLayer::dingShiTiXing), 6);
    }
    
    return true;
}

void XieziLayer::dingShiTiXing(){
    if(!m_isShowDetail){
        if (m_heimao!=NULL) {
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
        m_webView->callWebWithJs("setMode(Modes.kWrite);");
    }
}

void XieziLayer::onEnter(){
    CCLayer::onEnter();
    
    CCSize mainSize=S_RM->getSizeWithName("xiezi_webview_size");
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
    
    //注意收藏顺序
    if (m_collected) {
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
    
    BaiduStat::onStatEvent(kBaiduOneEventStart, "Xiezi", "写字界面停留");
}

void XieziLayer::onExit(){
    CCLayer::onExit();
    this->removeChild(m_webView, false);
    BaiduStat::onStatEvent(kBaiduOneEventEnd, "Xiezi", "写字界面停留");
}

void XieziLayer::setHandlerPriority(int handlerPriority){
    m_topHandlerPriority=handlerPriority;
    this->scheduleOnce(schedule_selector(XieziLayer::enableTouch), 0);
}

void XieziLayer::enableTouch(){
    CCMenu* menu=(CCMenu*) this->getChildByTag(kTagMenu);
    menu->setHandlerPriority(m_topHandlerPriority-1);
}

CCSprite* XieziLayer::generateSnapshot(){
    
    CCSize mainSize=S_RM->getSizeWithName("xiezi_webview_size");
    CCPoint mainPoint=S_RM->getPositionWithName("xiezi_main");
    
    CCSprite* snapshot=(CCSprite*)this->getChildByTag(kTagSnapshot);
    
    const char* data=m_webView->callWebWithJs("getCanvasBase64Data()");
    int length=(int)strlen(data);
    
    if(length>0){
        
        string pngData=StringUtils::base64Decode(data, (int)strlen(data));
        CCImage* img=new CCImage();
        img->autorelease();
        img->initWithImageData(const_cast<char*>(pngData.c_str()), (int)pngData.length());
        CCTexture2D *texture = new CCTexture2D();
        texture->initWithImage(img);
        texture->autorelease();
        
        if (snapshot) {
            snapshot->setTexture(texture);
        }else{
            snapshot= CCSprite::createWithTexture(texture);
            snapshot->setPosition(mainPoint);
            snapshot->setTag(kTagSnapshot);
            
            if (mainSize.width!=snapshot->getContentSize().width) {
                snapshot->setScale(mainSize.width/snapshot->getContentSize().width);
            }
            this->addChild(snapshot);
        }
        
    }else{
        if (!snapshot) {
            snapshot=CCSprite::createWithSpriteFrameName("tianzige.png");
            snapshot->setPosition(mainPoint);
            
            CCLabelTTF* label=CCLabelTTF::create(m_hanzi->getzi().c_str(), "KaiTi.ttf", 400.0);
            label->setColor(ccc3(100,53,14));
            label->setPosition(ccp(mainSize.width/2, mainSize.height/2));
            snapshot->addChild(label);
            
            snapshot->setTag(kTagSnapshot);
            
            this->addChild(snapshot);
        }
    }
    
    return snapshot;
}

void XieziLayer::callWeb(CCObject* pSender)
{
    this->unschedule(schedule_selector(XieziLayer::dingShiTiXing));
    int tag=((CCNode*)pSender)->getTag();
    
    switch (tag) {
        case kTagRewrite:{
            m_webView->callWebWithJs("reWrite();");
            m_isWriteFinished=false;
            m_webView->setVisible(true);
            m_isShowDetail=false;
            CCSprite* snapshot=(CCSprite*)this->getChildByTag(kTagSnapshot);
            if(snapshot)snapshot->removeFromParentAndCleanup(false);
        }
            break;
        case kTagTansuo:{
            CCSprite* snapshot=this->generateSnapshot();
            if (m_isShowDetail==false) {
                m_webView->setVisible(false);
                
                HanziDetailLayer* detailLayer=(HanziDetailLayer*)this->getChildByTag(kTagDetail);
                if (!detailLayer) {
                    detailLayer=HanziDetailLayer::create(m_hanzi);
                    detailLayer->setHandlerPriority(m_topHandlerPriority-1);
                    detailLayer->setTag(kTagDetail);
                    this->addChild(detailLayer);
                }
                detailLayer->setVisible(false);
                
                snapshot->setVisible(true);
                snapshot->runAction(CCSequence::create(CCOrbitCamera::create(0.3, 1, 0, 0, -90, 0, 0),CCHide::create(),NULL));
                detailLayer->runAction(CCSequence::create(CCDelayTime::create(0.3),CCShow::create(),CCOrbitCamera::create(0.3, 1, 0, 90, -90, 0, 0),NULL));
            }else{
                m_webView->setVisible(false);
                CCLayer* detailLayer=(CCLayer*)this->getChildByTag(kTagDetail);
                detailLayer->setVisible(true);
                detailLayer->runAction(CCSequence::create(CCOrbitCamera::create(0.3, 1, 0, 0, -90, 0, 0),CCHide::create(),NULL));
                snapshot->runAction(CCSequence::create(CCDelayTime::create(0.3),CCShow::create(),CCOrbitCamera::create(0.3, 1, 0, 90, -90, 0, 0),NULL));
                m_webView->runAction(CCSequence::create(CCDelayTime::create(0.6),CCShow::create(),NULL));
            }
            m_isShowDetail=!m_isShowDetail;
            
        }
            break;
        case kTagGuangbo:
            string url=FileUtils::getContentFilePath(m_mContent->getfileUrl());
            S_AE->playEffect(url.c_str());
            break;
    }
    
    CCSprite* tansuo=(CCSprite*)this->getChildByTag(kTagSpriteTansuo);
    if (m_isShowDetail) {
        tansuo->setDisplayFrame(S_SF->spriteFrameByName("bi.png"));
    }else{
        tansuo->setDisplayFrame(S_SF->spriteFrameByName("tansuo.png"));
    }
}

void XieziLayer::webCallBack(CCWebView* webview,std::string cmd){
    vector<string> splitCmd;
    StringUtils::split(cmd,"$",splitCmd);
    
    int icmd=atoi(splitCmd[0].c_str());
    
    switch (icmd) {
        case kWebCallBackLoadedFinish:{
            //加载完成
            CCSize webViewSize=S_RM->getSizeWithName("xiezi_webview_size");
            float scaleX=CCEGLView::sharedOpenGLView()->getScaleX();
            float scaleY=CCEGLView::sharedOpenGLView()->getScaleY();
            CCString* str=CCString::createWithFormat("init('%s','%s',%f,%f,%d,Modes.kFillAll)",m_hanzi->getcontour().c_str(),m_hanzi->getlocus().c_str(),webViewSize.width*scaleX,webViewSize.height*scaleY,m_hanzi->getwriteCount());
            webview->callWebWithJs(str->getCString());
            
            string url=FileUtils::getContentFilePath(m_mContent->getfileUrl());
            S_AE->playEffect(url.c_str());
            
            m_webView->callWebWithJs("setIsCanAutoWrite(true);");
        }
            
            break;
        case kWebCallBackWriteHanziOk:{
            //书写成功
            
            BaiduStat::onStatEvent(kBaiduOneEventStart, "Xiezi", "写对");
            
            m_todayXieziCount++;
            int writeCount=m_hanzi->getwriteCount();
            m_hanzi->setIntwriteCount(++writeCount);
            CCString *sql=CCString::createWithFormat("update hanzi set writeCount=writeCount+1 where id=%d;",m_hanzi->getid());
            S_DM->executeSql(sql->getCString());
            
            S_AEM->achieveUp(kAchieveXZDW);
            
            //排行数据需要统一调整模型
            static_userDefaultIncrement(COLLECT_XINGXING_COUNT,0);
            static_userDefaultIncrement(OVER_XINGXING_COUNT,0);
            
            string token=S_UD->getStringForKey(UDKEY_USER_TOKEN);
            if (splitCmd.size()>1&&splitCmd[1].length()>0) {
                int createTime=(int)(TimeUtils::millisecondNow().tv_sec);
                CCString* sql=CCString::createWithFormat("insert into xiezi_log(hid,locus,createTime) values(%d,'%s',%d)",m_hanzi->getid(),splitCmd[1].c_str(),createTime);
                S_DM->executeSql(sql->getCString());
                CLAUSE_INIT;
                whereClause.push_back(CCString::createWithFormat("hid=%d",m_hanzi->getid())->getCString());
                whereClause.push_back(CCString::createWithFormat("createTime=%d",createTime)->getCString());
                vector<XieziLog*>* logVector=new vector<XieziLog*>();
                S_DM->findScrollData(logVector,"id",whereClause, orderbyClause, groupByClause);
                if (logVector->size()==1) {
                    m_xieziLogId=((*logVector)[0])->getid();
                }
                
                if (token.length()>0) {
                    CCString* data=CCString::createWithFormat("hwd.writeData=%s&hwd.hanzi.id=%d",splitCmd[1].c_str(),m_hanzi->getid());
                    CCString* url=CCString::createWithFormat("user_uploadHanziWriteData_feedback?token=%s",token.c_str());
                    ApiStruct apiStruct;
                    apiStruct.url=url->getCString();
                    apiStruct.isBlackcat=true;
                    apiStruct.target=this;
                    apiStruct.sel_response=apiresponse_selector(XieziLayer::apiCallBack);
                    apiStruct.data=data->getCString();
                    apiStruct.requestType=CCHttpRequest::kHttpPost;
                    Api* api=Api::create(apiStruct);
                    api->send();
                }
            }
            
            m_isWriteFinished=true;
            
            if (splitCmd.size()>2&&splitCmd[2].length()>10) {
                m_webView->setVisible(false);
                
                this->generateSnapshot();
            }
            
            //星星动画
            CCSprite* xingxing=CCSprite::createWithSpriteFrameName("xingxing_2.png");
            xingxing->setPosition(S_RM->getJpgBgPosition());
            CCFiniteTimeAction* spawn=CCSpawn::create(CCMoveTo::create(1, S_RM->getPositionWithName("xiezi_xingxing")),CCFadeOut::create(1),NULL);
            CCAction* action=CCSequence::create(spawn,CCCallFunc::create(this,callfunc_selector(XieziLayer::xingxingAnimateEnd)),NULL);
            xingxing->runAction(action);
            this->addChild(xingxing);
            
            if (m_heimao) {
                if (m_todayXieziCount==3) {
                    m_heimao->action("heimao_xieziOkday3");
                }else{
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
            }
            
            CCSprite* xiangpi=(CCSprite*)this->getChildByTag(kTagSpriteXiangpi);;
            xiangpi->runAction(CCSequence::create(CCScaleTo::create(0.3, 1.2),CCScaleTo::create(0.3, 1),NULL));
            
        }
            break;
        case kWebCallBackCanvasData:{
            //图片数据
            //从base64转换未图片数据
            
        }
            
            break;
        case kWebCallBackChangeToWriteMode:{
            
            this->unschedule(schedule_selector(XieziLayer::dingShiTiXing));
            
            //切换到写字模式
            if (m_writedHanziCount>=WRITE_LIMIT&&m_hanzi->getwriteCount()==0&&m_gudieDialogLayer==NULL) {
                GuideDialog* guideDialog=new GuideDialog();
                guideDialog->autorelease();
                guideDialog->setText("非常抱歉，练习书写的汉字数量已经超出了的限制，请检查账号状态。");
                guideDialog->setMode(kGuideDialogOk);
                m_gudieDialogLayer=GuideDialogLayer::create(kDialogWithText);
                m_gudieDialogLayer->setDelegate(this);
                this->addChild(m_gudieDialogLayer,ORDER_DIALOG);
                m_gudieDialogLayer->setGuideDialogData(guideDialog);
                webview->setVisible(false);
            }
        }
            break;
        case kWebCallBackWriteStrokeOk:
            //字的各个笔画
            if(m_heimao)m_heimao->action("heimao_strokeOk");
            break;
            
        case kWebCallBackWriteStrokeError:{
            //写错时调用
            if(m_heimao)m_heimao->action("heimao_strokeError");
            
            BaiduStat::onStatEvent(kBaiduOneEventStart, "Xiezi", "写错");
        }
            
            break;
            
        default:
            break;
    }
    
    if(m_delegate)m_delegate->webCallBack((WebCallBackCMD)icmd);
}

/**
 *注意解决需要等待星星结束后才更新等级数据的问题
 */
void XieziLayer::xingxingAnimateEnd(){
    S_LM->gain("WRITE",ccp(-1, -1));
    
    if(m_hanzi->getwriteCount()==1)m_xingxing->setDisplayFrame(S_SF->spriteFrameByName("xingxing_2.png"));
    CCFiniteTimeAction* action2=CCSequence::create(CCScaleTo::create(0.1, 1.3),CCScaleTo::create(0.1, 1),NULL);
    m_xingxing->runAction(action2);
    
    CCString* count=CCString::createWithFormat("%d",m_hanzi->getwriteCount());
    m_count->setString(count->getCString());
}

void XieziLayer::dialogCallBack(GuideDialogCMD cmd){
    m_gudieDialogLayer->removeFromParentAndCleanup(true);
    m_gudieDialogLayer=NULL;
    m_webView->setVisible(true);
    m_webView->callWebWithJs("setMode(Modes.kDrawHanzi);");
}

void XieziLayer::setVisible(bool isVisible){
    CCNode::setVisible(isVisible);
    m_webView->setVisible(isVisible);
}

void XieziLayer::apiCallBack(cocos2d::CCDictionary *root){
    CCString *sql=CCString::createWithFormat("update xiezi_log set isUploaded=1 where id=%d;",m_xieziLogId);
    S_DM->executeSql(sql->getCString());
}