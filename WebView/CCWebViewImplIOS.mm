//
//  CCWebViewImplIOS.cpp
//  Blackcat
//
//  Created by haojunhua on 13-10-15.
//
//

#include "CCWebViewImplIOS.h"
#include "EAGLView.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define WEBVIEW_DELEGATE (((CCWebViewImpl*)m_ccWebview)->getDelegate())
#define CCWEBVIEW (((CCWebViewImpl*)m_ccWebview)->getCCWebView())

@implementation WebViewImplIOS
@synthesize uiWebview = m_uiWebview;
@synthesize ccWebview = m_ccWebview;


- (void)dealloc{
    [m_uiWebview removeFromSuperview];
    self.uiWebview=nil;
    [super dealloc];
}

-(id) initWithFrame: (CGRect) frameRect webView:(void*) webView htmlFilename: (const char*) htmlFilename{
    self = [super init];
    
    if (self) {
        //清空缓存
//        [[NSURLCache sharedURLCache] removeAllCachedResponses];
        
        self.uiWebview = [[[UIWebView alloc] initWithFrame:frameRect] autorelease];
        m_uiWebview.delegate=self;

        NSString *filename = [NSString stringWithUTF8String:htmlFilename];
        NSArray *components = [filename componentsSeparatedByString:@"."];
        NSString* path = [[NSBundle mainBundle] pathForResource:(NSString *)[components objectAtIndex:0] ofType:(NSString *)[components objectAtIndex:1]];
        NSURL* url = [NSURL fileURLWithPath:path];
        NSURLRequest* request = [NSURLRequest requestWithURL:url] ;
        [m_uiWebview loadRequest:request];
        
        [(UIScrollView *)[[m_uiWebview subviews] objectAtIndex:0] setScrollEnabled:NO];
        m_uiWebview.alpha=0;
        [[EAGLView sharedEGLView] addSubview:m_uiWebview];
        
        self.ccWebview=webView;
    }
    
    return self;
}

- (void)webViewDidFinishLoad:(UIWebView *)webView {
    [UIView animateWithDuration:0.3 animations:^{
     webView.alpha = 1.0;
    }];
    if(WEBVIEW_DELEGATE)WEBVIEW_DELEGATE->webCallBack(CCWEBVIEW,"0");//加载完成
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
    NSString *requestString = [[request URL] absoluteString];
    NSArray *components = [requestString componentsSeparatedByString:@":"];
    if ([components count] > 1 && [(NSString *)[components objectAtIndex:0] isEqualToString:@"callnative"]) {
        NSString* cmdStr=(NSString *)[components objectAtIndex:1];
        if(WEBVIEW_DELEGATE)WEBVIEW_DELEGATE->webCallBack(CCWEBVIEW,[cmdStr UTF8String]);
            return NO;
    }
    return YES;
}

-(void)callWebWithJs:(const char*)js{
    NSString *jsString = [NSString stringWithUTF8String:js];
    [m_uiWebview stringByEvaluatingJavaScriptFromString:jsString];
}

-(void)setVisible:(BOOL)var{
    [m_uiWebview setHidden:!var];
}

@end

CCWebViewImpl* __createSystemWebView(CCWebView* pWebView)
{
    return new CCWebViewImplIOS(pWebView);
}

CCWebViewImplIOS::CCWebViewImplIOS(CCWebView* pWebView):CCWebViewImpl(pWebView)
{
    
}

CCWebViewImplIOS::~CCWebViewImplIOS(){
    [m_systemControl release];
}

bool CCWebViewImplIOS::initWithRectAndHtmlFilename(const CCRect& uiRect,const char* htmlFilename)
{
    do
    {
        float scaleFactor=[[EAGLView sharedEGLView] contentScaleFactor];
        CGRect cgRect = CGRectMake(uiRect.origin.x/scaleFactor,uiRect.origin.y/scaleFactor,uiRect.size.width/scaleFactor,uiRect.size.height/scaleFactor);
        m_systemControl = [[WebViewImplIOS alloc] initWithFrame:cgRect webView:this htmlFilename:htmlFilename];
        if (!m_systemControl) break;
        return true;
    }while (0);
    
    return false;
}

void CCWebViewImplIOS::callWebWithJs(const char* js){
    [m_systemControl callWebWithJs:js];
}

void CCWebViewImplIOS::setVisible(bool var){
    [m_systemControl setVisible:var];
}

#endif