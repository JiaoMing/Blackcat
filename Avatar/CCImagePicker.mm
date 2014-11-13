//
//  AlbumFinder.m
//  Blackcat
//
//  Created by haojunhua on 14/10/12.
//
//

#include "CCImagePicker.h"
#include "EAGLView.h"
#include <string>
using namespace std;

#import <Foundation/Foundation.h>
#import <AssetsLibrary/AssetsLibrary.h>


#define CCIMAGEPICKER_DELEGATE (((CCImagePicker*)_ccImagePicker)->getDelegate())

@interface CCImagePickerIOS :NSObject<UINavigationControllerDelegate,UIImagePickerControllerDelegate>{
    void* _ccImagePicker;
}
@property(nonatomic, assign) void*  ccImagePicker;
@property (strong,nonatomic) UIPopoverController *popoverImageViewController;

- (id)initWithCCImagePicker:(void *)ccImagePicker;
- (NSArray*)findAssetsWithType:(ImagePickerType)type url:(string)url;
- (void)pickCamera;

@end

@implementation CCImagePickerIOS

@synthesize ccImagePicker = _ccImagePicker;

- (id)initWithCCImagePicker:(void *)ccImagePicker
{
    self = [super init];
    if (self)
    {
        self.ccImagePicker=ccImagePicker;
    }
    return self;
}

-(NSArray*)findAssetsWithType:(ImagePickerType)type url:(string)url{
    CCArray* array=CCIMAGEPICKER_DELEGATE->getAssets();
    ALAssetsLibrary* assetsLibrary = [[ALAssetsLibrary alloc] init];
    dispatch_queue_t dispatchQueue =dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    dispatch_async(dispatchQueue, ^(void) {
        
        switch (type) {
            case kImagePickerTypeAssetsGroup:{
                array->removeAllObjects();
                // 遍历所有相册
                [assetsLibrary enumerateGroupsWithTypes:ALAssetsGroupAll usingBlock:^(ALAssetsGroup *group, BOOL *stop) {
                    [group setAssetsFilter:[ALAssetsFilter allPhotos]];
                    dispatch_async(dispatch_get_main_queue(), ^(void) {
                        if (group) {
                            
                            CGImageRef posterImageRef = [group posterImage];
                            UIImage* uiImage = [UIImage imageWithCGImage:posterImageRef];
                            CCImage* ccImage=new CCImage();
                            ccImage->autorelease();
                            CCImagePicker::uImage2cImage(uiImage, ccImage);
                            
                            AlbumObject* albumObject=new AlbumObject();
                            albumObject->autorelease();
                            albumObject->setImage(ccImage);
                            albumObject->setName([[group valueForProperty:ALAssetsGroupPropertyName] UTF8String]);
                            NSURL* url=[group valueForProperty:ALAssetsGroupPropertyURL];
                            albumObject->setUrl([url.absoluteString UTF8String]);
                            array->addObject(albumObject);
                            
                        }else{
                            CCIMAGEPICKER_DELEGATE->callbackFresh();
                        }
                    });
                } failureBlock:^(NSError *error) {
                    NSLog(@"Failed to enumerate the asset groups.");
                }
                ];
            }
                break;
                
            case kImagePickerTypeAssets:{
                array->removeAllObjects();
                
                NSURL* groupUrl=[NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]];
                [assetsLibrary groupForURL:groupUrl resultBlock:^(ALAssetsGroup *group) {
                    [group setAssetsFilter:[ALAssetsFilter allPhotos]];
                    
                    [group enumerateAssetsUsingBlock:^(ALAsset *assets, NSUInteger index,BOOL *stop) {
                        
                        dispatch_async(dispatch_get_main_queue(), ^(void) {
                            if (assets) {
                                CGImageRef posterImageRef = [assets thumbnail];
                                UIImage* uiImage = [UIImage imageWithCGImage:posterImageRef];
                                CCImage* ccImage=new CCImage();
                                ccImage->autorelease();
                                CCImagePicker::uImage2cImage(uiImage, ccImage);
                                
                                AlbumObject* albumObject=new AlbumObject();
                                albumObject->autorelease();
                                albumObject->setImage(ccImage);
                                albumObject->setName("");
                                NSURL* url=[assets valueForProperty:ALAssetPropertyAssetURL];
                                albumObject->setUrl([url.absoluteString UTF8String]);
                                array->addObject(albumObject);
                            }else{
                                CCIMAGEPICKER_DELEGATE->callbackFresh();
                            }
                        });
                    }];
                } failureBlock:^(NSError *error) {
                    NSLog(@"Failed to enumerate the asset groups.");
                }];
                
            }
                break;
            case kImagePickerTypePhoto:{
                NSURL* photoUrl=[NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]];
                [assetsLibrary assetForURL:photoUrl resultBlock:^(ALAsset *asset) {
                    ALAssetRepresentation *assetRepresentation =[asset defaultRepresentation];
                    CGFloat imageScale = [assetRepresentation scale];
                    UIImageOrientation imageOrientation = (UIImageOrientation)[assetRepresentation orientation];
                    dispatch_async(dispatch_get_main_queue(), ^(void) {
                        if (asset) {
                            CGImageRef imageReference = [assetRepresentation fullScreenImage];
                            // 对找到的图片进行操作
                            UIImage *uiImage =[[UIImage alloc] initWithCGImage:imageReference scale:imageScale orientation:imageOrientation];
                            CCImage* ccImage=new CCImage();
                            ccImage->autorelease();
                            CCImagePicker::uImage2cImage(uiImage, ccImage);
                            
                            AlbumObject* albumObject=new AlbumObject();
                            albumObject->autorelease();
                            albumObject->setImage(ccImage);
                            albumObject->setName("");
                            NSURL* url=[asset valueForProperty:ALAssetPropertyAssetURL];
                            albumObject->setUrl([url.absoluteString UTF8String]);
                            CCIMAGEPICKER_DELEGATE->callbackWithPhoto(albumObject);
                            
                        }else{
                            CCLog("未放入");
                        }
                    });
                } failureBlock:^(NSError *error) {
                    NSLog(@"Failed to enumerate the asset groups.");
                }];
                
            }
                break;
        }
        
    });
    
    return nil;
}

-(void)pickCamera{
    
    UIImagePickerController* picker = [[UIImagePickerController alloc] init];
    picker.delegate=self;
    
    UIViewController* rvc=[UIApplication sharedApplication].keyWindow.rootViewController;
    
    if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera]) {
        picker.sourceType=UIImagePickerControllerSourceTypeCamera;
        picker.cameraCaptureMode = UIImagePickerControllerCameraCaptureModePhoto;

        if(false){
//        if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad) {
        
            if([[UIDevice currentDevice]orientation]==UIDeviceOrientationLandscapeLeft) {
               picker.cameraViewTransform = CGAffineTransformMakeRotation(-M_PI/2);
            }else if([[UIDevice currentDevice]orientation]==UIDeviceOrientationLandscapeRight){
                picker.cameraViewTransform = CGAffineTransformMakeRotation(M_PI/2);
            }
                
//            picker.allowsEditing = NO;
//            picker.showsCameraControls = NO;
            
//            UIImageView *overLayImg = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 576, 300)];
//            overLayImg.image = [UIImage imageNamed:@"global/touxiang_zhezhao.png"];
//            picker.cameraOverlayView = overLayImg;

//            picker.contentSizeForViewInPopover=CGSizeMake(576, 300);
            
            UIPopoverController *popover = [[UIPopoverController alloc] initWithContentViewController:picker];
            self.popoverImageViewController=popover;
//            popover.popoverContentSize = CGSizeMake(400, 300);
            [popover presentPopoverFromRect:CGRectMake(CCEGLView::sharedOpenGLView()->getFrameSize().width/2,410, 1, 1) inView:rvc.view permittedArrowDirections:0 animated:YES];
            [popover autorelease];
            
            
        } else {
            [rvc presentViewController:picker animated:YES completion:nil];
        }
        
//        picker.allowsEditing = NO;
////        picker.showsCameraControls = NO;
//        [picker.view setFrame:CGRectMake(0, 0, 500, 300)];
//        
////
////        [[EAGLView sharedEGLView] addSubview:picker.view];
//        
//        
//        UIView *controllerView = picker.view;
//        controllerView.alpha = 0.0;
//        controllerView.transform = CGAffineTransformMakeScale(0.5, 0.5);
//        controllerView.backgroundColor=[UIColor clearColor];
//        
//        [[EAGLView sharedEGLView] addSubview:picker.view];
//        
//        [UIView animateWithDuration:0.3
//                              delay:0.0
//                            options:UIViewAnimationOptionCurveLinear
//                         animations:^{
//                             controllerView.alpha = 1;
//                         }
//                         completion:nil
//         ];
        
        
    }else{
        NSLog(@"模拟器无法打开相机");
    }
}

-(UIImage *)scaleImage:(UIImage *)image toScale:(float)scaleSize
{
    UIGraphicsBeginImageContext(CGSizeMake(image.size.width*scaleSize,image.size.height*scaleSize));
    [image drawInRect:CGRectMake(0, 0, image.size.width * scaleSize, image.size.height *scaleSize)];
    UIImage *scaledImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return scaledImage;
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info{
    if (picker.sourceType == UIImagePickerControllerSourceTypeCamera)
    {
        
        CCArray* array=CCIMAGEPICKER_DELEGATE->getAssets();
        array->removeAllObjects();
        
        //如果是 来自照相机的image，那么先保存
        UIImage* original_image = [info objectForKey:@"UIImagePickerControllerOriginalImage"];
        UIImageWriteToSavedPhotosAlbum(original_image, nil,nil,
                                       nil);
        //获得编辑过的图片
        UIImage* uiImage = [self scaleImage:original_image toScale:0.3];
        
        
        CCImage* ccImage=new CCImage();
        ccImage->autorelease();
        CCImagePicker::uImage2cImage(uiImage, ccImage);
        
        AlbumObject* albumObject=new AlbumObject();
        albumObject->autorelease();
        albumObject->setImage(ccImage);
        albumObject->setName("");
        albumObject->setUrl("");
        array->addObject(albumObject);
        CCIMAGEPICKER_DELEGATE->callbackFresh();
        
    }
    [picker release];
    
    [self.popoverImageViewController dismissPopoverAnimated:false];
    UIViewController* rvc=[UIApplication sharedApplication].keyWindow.rootViewController;
    [rvc dismissViewControllerAnimated:false completion:nil];
}


@end

static CCImagePickerIOS* g_ccImagePickerIOS;
static CCImagePicker* s_ccImagePicker=NULL;

CCImagePicker* CCImagePicker::sharedImagePicker(){
    if (s_ccImagePicker==NULL) {
        s_ccImagePicker=new CCImagePicker();
        g_ccImagePickerIOS=[[CCImagePickerIOS alloc] initWithCCImagePicker:s_ccImagePicker];
    }
    return s_ccImagePicker;
}

void CCImagePicker::purgeImagePicker(){
    CC_SAFE_DELETE(s_ccImagePicker);
}

CCImagePicker::CCImagePicker(){
}

CCImagePicker::~CCImagePicker(){
    [g_ccImagePickerIOS release];
}

void CCImagePicker::findAssets(ImagePickerType imagePickerType,std::string groupUrl){
    [g_ccImagePickerIOS findAssetsWithType:imagePickerType url:groupUrl];
}

void CCImagePicker::pickCamera(){
    [g_ccImagePickerIOS pickCamera];
}

bool CCImagePicker::uImage2cImage(void* uimg, CCImage* cimg)
{
    UIImage* img = (UIImage*) uimg;
    NSData* imgData = UIImageJPEGRepresentation(img, 1.0);
    void* data = malloc([imgData length]);
    [imgData getBytes:data];
    bool ret =  cimg->initWithImageData(data, [imgData length], CCImage::kFmtPng, img.size.width, img.size.height);
    free(data);
    return ret;
}
