//
//  CCAlbum.mm
//  Blackcat
//
//  Created by haojunhua on 14-7-2.
//
//

#include "CCAlbum.h"
#include "EAGLView.h"
#import "AGSimpleImageEditorView.h"

#define AVATAR_DELEGATE (((CCAlbum*)m_ccAvatar)->getDelegate())

@interface AGViewController : UIViewController
{
    AGSimpleImageEditorView *simpleImageEditorView;
    UISegmentedControl *ratioSegmentedControl;
    UIButton *rotateLeftButton, *rotateRightButton,* saveButton;
    void* m_ccAvatar;
}
@property(nonatomic, assign) void*  ccAvatar;

@end

@interface AGViewController ()

- (id)initWithImage:(UIImage*)image avatar:(void *)avatar;

- (void)saveImage:(id)sender;

- (void)rotateLeft:(id)sender;
- (void)rotateRight:(id)sender;

- (void)didChangeRatio:(id)sender;
- (void)arrangeItemsForInterfaceOrientation:(UIInterfaceOrientation)forInterfaceOrientation;

@end

@implementation AGViewController

@synthesize ccAvatar = m_ccAvatar;


- (void)dealloc
{
    [simpleImageEditorView release];
    [ratioSegmentedControl release];
    [rotateLeftButton release], [rotateRightButton release], [saveButton release];
    
    [super dealloc];
}

- (id)initWithImage:(UIImage*)image avatar:(void *)avatar
{
    self = [super init];
    if (self)
    {
        self.ccAvatar=avatar;
        //        simpleImageEditorView = [[AGSimpleImageEditorView alloc] initWithImage:[UIImage imageNamed:@"springboard.jpg"]];
        simpleImageEditorView = [[AGSimpleImageEditorView alloc] initWithImage:image];
        //        simpleImageEditorView = [[AGSimpleImageEditorView alloc] init];
        //        simpleImageEditorView.image = [UIImage imageNamed:@"apple.jpg"];
        simpleImageEditorView.borderWidth = 1.f;
        simpleImageEditorView.borderColor = [UIColor darkGrayColor];
        simpleImageEditorView.ratioViewBorderWidth = 1.5f;
        simpleImageEditorView.ratioViewBorderColor = [UIColor greenColor];
        
        
//        ratioSegmentedControl = [[UISegmentedControl alloc] initWithItems:[NSArray arrayWithObjects:@"None", @"4:3", @"3:2", @"16:10", @"18:10", nil]];
//        [ratioSegmentedControl addTarget:self action:@selector(didChangeRatio:) forControlEvents:UIControlEventValueChanged];
        
        rotateLeftButton = [[UIButton buttonWithType:UIButtonTypeRoundedRect] retain];
        [rotateLeftButton setTitle:@"<-" forState:UIControlStateNormal];
        [rotateLeftButton addTarget:self action:@selector(rotateLeft:) forControlEvents:UIControlEventTouchUpInside];
        rotateRightButton = [[UIButton buttonWithType:UIButtonTypeRoundedRect] retain];
        [rotateRightButton setTitle:@"->" forState:UIControlStateNormal];
        [rotateRightButton addTarget:self action:@selector(rotateRight:) forControlEvents:UIControlEventTouchUpInside];
        
        saveButton = [[UIButton buttonWithType:UIButtonTypeSystem] retain];
        [saveButton setTitle:@"save" forState:UIControlStateNormal];
        [saveButton addTarget:self action:@selector(saveImage:) forControlEvents:UIControlEventTouchUpInside];
        
        [self arrangeItemsForInterfaceOrientation:self.interfaceOrientation];
        [self.view addSubview:simpleImageEditorView];
//        [self.view addSubview:ratioSegmentedControl];
        [self.view addSubview:rotateLeftButton];
        [self.view addSubview:rotateRightButton];
        [self.view addSubview:saveButton];
        
//        simpleImageEditorView.cropRect=CGRectMake(0, 0, 180, 180);
        simpleImageEditorView.ratio=1;
        
        
        [self.view setBackgroundColor:[UIColor whiteColor]];
        
    }
    
    return self;
}


- (void)didChangeRatio:(id)sender
{
    CGFloat ratio = 0;
    switch (ratioSegmentedControl.selectedSegmentIndex) {
        case 1:
            ratio = 4./3.;
            break;
            
        case 2:
            ratio = 3./2.;
            break;
            
        case 3:
            ratio = 16./10.;
            break;
            
        case 4:
            ratio = 18./10.;
            break;
    }
    
    simpleImageEditorView.ratio = ratio;
}

- (void)arrangeItemsForInterfaceOrientation:(UIInterfaceOrientation)forInterfaceOrientation
{
    CGRect bounds = [[UIScreen mainScreen] bounds];
    CGFloat width = bounds.size.width, height = bounds.size.height;
    
    if (UIInterfaceOrientationIsLandscape(forInterfaceOrientation)) {
        width = bounds.size.height;
        height = bounds.size.width;
    }
    
    CGRect segmentedFrame = CGRectMake(
                                       (width - ratioSegmentedControl.frame.size.width) / 2,
                                       height - ratioSegmentedControl.frame.size.height - (20.f * 2),
                                       ratioSegmentedControl.frame.size.width,
                                       ratioSegmentedControl.frame.size.height);
    ratioSegmentedControl.frame = segmentedFrame;
    CGRect editorFrame = CGRectMake(20.f, 20.f, width - (20.f * 2), segmentedFrame.origin.y - (20.f * 2));
    simpleImageEditorView.frame = editorFrame;
    
    rotateLeftButton.frame = CGRectMake(20.f, segmentedFrame.origin.y, 40.f, 44.f);
    rotateRightButton.frame = CGRectMake(width - 20.f - 40.f, segmentedFrame.origin.y, 40.f, 44.f);
    
    
    saveButton.frame = CGRectMake(width/2-20.f, segmentedFrame.origin.y, 40.f, 44.f);
}

- (void)rotateLeft:(id)sender
{
    [simpleImageEditorView rotateLeft];
}

- (void)rotateRight:(id)sender
{
    [simpleImageEditorView rotateRight];
}


- (void)saveImage:(id)sender
{
    
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *docDir = [paths objectAtIndex:0];
    
    NSString* saveImg=[NSString stringWithFormat:@"%s/avatar.jpg",[docDir UTF8String]];
    
    NSData *data = UIImageJPEGRepresentation(simpleImageEditorView.output, 1);
    [data writeToFile:saveImg atomically:YES];
    
    [[UIApplication sharedApplication].keyWindow.rootViewController dismissViewControllerAnimated:YES completion:nil];
    
    if(AVATAR_DELEGATE){
        UIImage *image=simpleImageEditorView.output;
        NSData* imgData = UIImagePNGRepresentation(image);
        void* data = malloc([imgData length]);
        [imgData getBytes:data];
        
        CCImage* cimg=new CCImage();
        cimg->initWithImageData(data, (int)[imgData length], CCImage::kFmtPng);
        cimg->autorelease();
        
        CCTexture2D *texture = new CCTexture2D();
        texture->initWithImage(cimg);
        texture->autorelease();
        
        free(data);
        
        AVATAR_DELEGATE->pickCallback(texture,image.size.width,image.size.height);
        
    }
    
    
}

@end



#import "AGImagePickerController.h"

@interface CCAlbumIOS : NSObject<AGImagePickerControllerDelegate,UIActionSheetDelegate,UINavigationControllerDelegate,UIImagePickerControllerDelegate>{
    NSMutableArray* selectedPhotos;
    void* m_ccAvatar;
}

@property (nonatomic, strong) NSMutableArray *selectedPhotos;
@property(nonatomic, assign) void*  ccAvatar;

-(id) initWithAvatar:(void*)avatar;
@end


@interface CCAlbumIOS ()
{
    AGImagePickerController *ipc;
}

@end

@implementation CCAlbumIOS

#pragma mark - Properties

@synthesize selectedPhotos;
@synthesize ccAvatar = m_ccAvatar;

#pragma mark - Object Lifecycle

- (void)dealloc{
    [[UIApplication sharedApplication].keyWindow.rootViewController dismissViewControllerAnimated:YES completion:nil];
    [super dealloc];
}


- (id)initWithAvatar:(void *)avatar{
    self = [super init];
    
    if (self)
    {
        self.ccAvatar=avatar;
        
        
        UIActionSheet *chooseImageSheet = [[UIActionSheet alloc] initWithTitle:nil delegate:self cancelButtonTitle:@"Cancel" destructiveButtonTitle:nil otherButtonTitles:@"拍照",@"从相册获取", nil];
        [chooseImageSheet showInView:[EAGLView sharedEGLView]];
        
    }
    
    return self;
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
        //如果是 来自照相机的image，那么先保存
        UIImage* original_image = [info objectForKey:@"UIImagePickerControllerOriginalImage"];
        UIImageWriteToSavedPhotosAlbum(original_image, nil,nil,
                                       nil);
        
        //获得编辑过的图片
        UIImage* scaleImage = [self scaleImage:original_image toScale:0.3];
        AGViewController* agv=[[AGViewController alloc]initWithImage:scaleImage avatar:m_ccAvatar];
        [picker presentViewController:agv animated:true completion:nil];
    }
    [picker release];
    [self dealloc];
}

-(void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
    UIImagePickerController * picker = [[UIImagePickerController alloc] init];
    picker.delegate = self;
    
    switch (buttonIndex) {
        case 0://Take picture
            
            if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera]) {
                picker.sourceType = UIImagePickerControllerSourceTypeCamera;
                
            }else{
                NSLog(@"模拟器无法打开相机");
            }
            [[UIApplication sharedApplication].keyWindow.rootViewController presentViewController:picker animated:YES completion:nil];
            break;
            
        case 1://From album
        {
            
            self.selectedPhotos = [NSMutableArray array];
            
            __block CCAlbumIOS *blockSelf = self;
            
            ipc = [[AGImagePickerController alloc] initWithDelegate:self];
            ipc.didFailBlock = ^(NSError *error) {
                NSLog(@"Fail. Error: %@", error);
                
                if (error == nil) {
                    [blockSelf.selectedPhotos removeAllObjects];
                    NSLog(@"User has cancelled.");
                    [self dealloc];
                } else {
                    
                    // We need to wait for the view controller to appear first.
                    double delayInSeconds = 0.5;
                    dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, delayInSeconds * NSEC_PER_SEC);
                    dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
                        
                        [self dealloc];
                    });
                }
                
                
            };
            ipc.didFinishBlock = ^(NSArray *info) {
                [blockSelf.selectedPhotos setArray:info];
                
                if ([blockSelf.selectedPhotos count]>0) {
                    if(AVATAR_DELEGATE){
                        ALAsset *asset=[blockSelf.selectedPhotos objectAtIndex:0];
                        UIImage *originImage=[UIImage imageWithCGImage:asset.defaultRepresentation.fullScreenImage];
                        
                        UIImage* scaleImage = [self scaleImage:originImage toScale:0.3];
                        
                        AGViewController* agv=[[AGViewController alloc]initWithImage:scaleImage avatar:m_ccAvatar];
                        
                        [ipc presentViewController:agv animated:true completion:nil];
                    }
                    
                }
                //            [self dealloc];
            };
            
            
            ipc.shouldShowSavedPhotosOnTop = NO;
            ipc.shouldChangeStatusBarStyle = YES;
            ipc.selection = self.selectedPhotos;
            ipc.maximumNumberOfPhotosToBeSelected = 1;
            ipc.shouldShowSavedPhotosOnTop=false;
            [[UIApplication sharedApplication].keyWindow.rootViewController presentViewController:ipc animated:true completion:nil];
        }
            break;
            
        default:
            
            break;
    }
}


@end




bool CCAlbum::init(){
    if (!CCLayer::init()) {
        return false;
    }
    [[CCAlbumIOS alloc]initWithAvatar:this];
    
    return true;
}
