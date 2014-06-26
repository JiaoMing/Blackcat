//
//  ResMananger.h
//  iFrameExtractor
//
//  Created by shuang on 12-6-18.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef iFrameExtractor_ResManager_h
#define iFrameExtractor_ResManager_h

#include "cocos2d.h"
using namespace cocos2d;

class ResManager : public CCObject
{
public:
    ResManager();
    ~ResManager();
   
    static ResManager * sharedResManager();
    
    //AppDelegate.cpp中必须进行ResManager初始化，可使用一下两种初始化方式
    //仅初始化坐标和缩放比例，***坐标文件改为plist格式；使用此方式，则图片资源使用addSceneRes进行添加
    void init(const char *pszPosName,float ratio);
    //初始化坐标和缩放比例,以及首次加载的场景和plist文件，多个plist文件以逗号分割
    void init(const char *pszPosName,float ratio,const char *pszSceneName, const char *pszPlistData);
    
    //中心点坐标，大背景坐标
    CCPoint getJpgBgPosition();
    //根据名称获取size
    CCSize getSizeWithName(const char *pszName);
    //根据名称获取rect
    CCRect getRectWithName(const char *pszName);
    
    /**
     根据名称获取相对，数据本身是左下角坐标
     */
    CCRect getGLRectWithName(const char *pszName);
    
    /**
     *  根据名称获取坐标，经过左上角到左下角的变换
     *
     *  @param pszName 对应plist中的key
     *
     *  @return 转换后坐标
     */
    CCPoint getPositionWithName(const char *pszName);
    
    /**
     *  根据名称获取相对坐标
     *
     *  @param pszName       对应plist中的key
     *  @param nHeightParent 父亲节点的高度
     *
     *  @return 转换后坐标
     */
    CCPoint getRelativePosition(const char *pszName, float nHeightParent);
    //根据名称获取缩放比例
    float getScaleWithName(const char *pszName);
    
    
    // 特别注意：下面的两个方法，主要是为了独立的添加该场景的plist，然后释放内存
    // 把addSceneRes放在场景的构造函数中，把removeSceneRes放在析构函数中
    
    void addSceneRes(const char *pszSceneName, const char *pszPlistData);
    void removeSceneRes(const char *pszSceneName);
    
private:
    void setRatio(float ratio);
    void loadPlistFile(const char *pszSceneName);
    void loadPosFile(const char *pszPosName);

private:
    CCDictionary * m_pScenePlistMap;
    CCDictionary* m_pPositionDict;
    float m_fRatio;
};

#endif
