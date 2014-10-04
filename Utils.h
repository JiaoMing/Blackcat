//
//  Utils.h
//  YidiPlayerHD
//  Created by 俊华 郝 on 12-7-31.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef YidiPlayerHD_Utils_h
#define YidiPlayerHD_Utils_h
#include "cocos2d.h"
#include <iostream>
using namespace std;
#include <fstream>
#include "zip.h"
#include "unzip.h"
#include <sys/stat.h>
#include<wchar.h>
#include <ctime>

using namespace cocos2d;


class TimeUtils{
public:
    //获取当前时间，毫秒级
    static inline cc_timeval millisecondNow() {
        cc_timeval now;
        CCTime::gettimeofdayCocos2d(&now, NULL);
        return now;
    }
    
    static inline struct tm* getLocaltime(time_t time){
        struct tm* locationTime;
        locationTime = localtime(&time);
        return locationTime;
    }
    
    static inline string getYYYYMMDD(){
        cc_timeval time=TimeUtils::millisecondNow();
        struct tm* locationTime= TimeUtils::getLocaltime(time.tv_sec);
        CCString* data=CCString::createWithFormat("%04d%02d%02d",locationTime->tm_year+1900,locationTime->tm_mon+1,locationTime->tm_mday);
        return data->getCString();
    }
    
    static inline time_t getToday0Time(){
        cc_timeval time=TimeUtils::millisecondNow();
        struct tm* dayTime= TimeUtils::getLocaltime(time.tv_sec);
        dayTime->tm_hour=0;
        dayTime->tm_min=0;
        dayTime->tm_sec=0;
        return mktime(dayTime);
    }
};

class ImageUtils{
public:
    static inline CCImage::EImageFormat getImageFormatType(const char *pszFileName)
    {
        CCImage::EImageFormat ret = CCImage::kFmtUnKnown;
        
        std::string lowerCase(pszFileName);
        for (unsigned int i = 0; i < lowerCase.length(); ++i)
        {
            lowerCase[i] = tolower(lowerCase[i]);
        }
        
        if (std::string::npos != lowerCase.find(".png"))
        {
            ret = CCImage::kFmtPng;
        }
        else if (std::string::npos != lowerCase.find(".jpg") || std::string::npos != lowerCase.find(".jpeg"))
        {
            ret = CCImage::kFmtJpg;
        }
        else if (std::string::npos != lowerCase.find(".tif") || std::string::npos != lowerCase.find(".tiff"))
        {
            ret = CCImage::kFmtTiff;
        }
        
        return ret;
    }
    
    static inline const char * getFileName(const char *pszUrl)
    {
        const char *p1 = pszUrl, *p2 = p1;
        while (p1 && (p1 = strstr(p1, "/"))) {
            p1++;
            p2 = p1;
        }
        return p2;
    }
};

class StringUtils{
public:
    static void split(const string& src, const string& separator, vector<string>& dest)
    {
        string str = src;
        string substring;
        string::size_type start = 0, index;
        
        do
        {
            index = str.find_first_of(separator,start);
            if (index != string::npos)
            {
                substring = str.substr(start,index-start);
                dest.push_back(substring);
                int tempIndex = str.find_first_of(separator,start);
                start = str.find_first_not_of(separator,index);
                if (start == string::npos){
                    while (tempIndex<str.length()-1) {
                        dest.push_back("");
                        ++tempIndex;
                    }
                    return;
                }
                while (tempIndex<start-1) {
                    dest.push_back("");
                    ++tempIndex;
                }
            }
        }while(index != string::npos);
        
        substring = str.substr(start);
        dest.push_back(substring);
    }
    
    static string base64Decode(const char* Data,int len)
    {
        int index=0;
        //解码表
        const char DecodeTable[] =
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            62, // '+'
            0, 0, 0,
            63, // '/'
            52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
            0, 0, 0, 0, 0, 0, 0,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
            13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
            0, 0, 0, 0, 0, 0,
            26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
            39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
        };
        //返回值
        string strDecode;
        int nValue;
        int i= 0;
        while (i < len)
        {
            if (*Data != '\r' && *Data!='\n')
            {
                nValue = DecodeTable[*Data++] << 18;
                nValue += DecodeTable[*Data++] << 12;
                strDecode+=(nValue & 0x00FF0000) >> 16;
                index++;
                if (*Data != '=')
                {
                    nValue += DecodeTable[*Data++] << 6;
                    strDecode+=(nValue & 0x0000FF00) >> 8;
                    index++;
                    if (*Data != '=')
                    {
                        nValue += DecodeTable[*Data++];
                        strDecode+=nValue & 0x000000FF;
                        index++;
                    }
                }
                i += 4;
            }
            else// 回车换行,跳过
            {
                Data++;
                i++;
            }
        }
        return strDecode;
    }
    
    
};


class FileUtils{
public:
    static bool depositFile(string srcFullPath,string descFullPath){
        
        // 检查不存在并复制
        if (access(descFullPath.c_str(), 0)) {
            unsigned long pSize=0;
            unsigned char* srcSqliteData=CCFileUtils::sharedFileUtils()->getFileData(srcFullPath.c_str(), "rb", &pSize);
            
            // 拷贝文件
            FILE *fp = fopen(descFullPath.c_str(), "wb");
            fwrite(srcSqliteData, 1, pSize, fp);
            fclose(fp);
            delete [] srcSqliteData;
            srcSqliteData=NULL;
        }
        
//        char buffer[8];
//        int len;
//        FILE* srcFile;
//        FILE* descFile = fopen(descFullPath.c_str(), "wb");
//        if((srcFile = fopen(srcFullPath.c_str(), "rb"))!=NULL)
//        {
//            while ((len = fread(buffer,1,8,srcFile)) > 0) {
//                fwrite(buffer,1,len,descFile);
//            }
//            fclose(srcFile);
//        }else{
//            return false;
//        }
//        fclose(descFile);
        return true;
    }
    
    static bool removeFile(string fileFullPath){
        const char* filePath= fileFullPath.c_str();
        if (!access(filePath, 0)) {
            return remove(filePath);
        }else{
            
            return false;
        }
    }
    
    static bool moveFile(string srcFullPath,string descFullPath){
        depositFile(srcFullPath,descFullPath);
        removeFile(srcFullPath);
        return true;
    }
    
    static string getPrefix(string filename){
        int index=filename.find_last_of(".");
        return filename.substr(0,index);
    }
    
    static string getExtfix(string filename){
        int index=filename.find_last_of(".");
        return filename.substr(index,filename.length());
    }
    
    static bool depositResourceFile(string filename){
        string srcFilePath=CCFileUtils::sharedFileUtils()->fullPathForFilename(filename.c_str());
        string descFilePath=CCFileUtils::sharedFileUtils()->getWritablePath()+filename;
        // 检查不存在并复制
        if (access(descFilePath.c_str(), 0)) {
            unsigned long pSize=0;
            unsigned char* srcSqliteData=CCFileUtils::sharedFileUtils()->getFileData(srcFilePath.c_str(), "rb", &pSize);
            
            // 拷贝文件
            FILE *fp = fopen(descFilePath.c_str(), "wb");
            fwrite(srcSqliteData, 1, pSize, fp);
            fclose(fp);
            delete [] srcSqliteData;
            srcSqliteData=NULL;
        }
        return true;
    }
    
    static bool unzipOpenFile(string fileFullPath,string descPath)
    {
        CCLog("解压:%s",fileFullPath.c_str());
        bool success=true;
        unzFile _unzFile = unzOpen(fileFullPath.c_str());
        
        if( _unzFile )
        {
            
            unz_global_info  globalInfo = {0};
            if( unzGetGlobalInfo(_unzFile, &globalInfo )==UNZ_OK )
            {
                CCLog("upzip success %s",fileFullPath.c_str());
            }
            
            int ret = unzGoToFirstFile( _unzFile );
            unsigned char buffer[4096] = {0};
            if( ret!=UNZ_OK )
            {
                CCLog("upzip error %s",fileFullPath.c_str());
            }
            do{
                
                ret = unzOpenCurrentFile( _unzFile );
                
                int read;
                unz_file_info	fileInfo ={0};
                ret = unzGetCurrentFileInfo(_unzFile, &fileInfo, NULL, 0, NULL, 0, NULL, 0);
                if( ret!=UNZ_OK )
                {
                    CCLog("Error occurs while getting file info");
                    success = false;
                    unzCloseCurrentFile( _unzFile );
                    break;
                }
                char* filename = (char*) malloc( fileInfo.size_filename +1 );
                unzGetCurrentFileInfo(_unzFile, &fileInfo, filename, fileInfo.size_filename + 1, NULL, 0, NULL, 0);
                filename[fileInfo.size_filename] = '\0';
                
                
                string fullname=descPath+filename;
                free(filename);
                
                if (fullname.find_last_of("/")==fullname.length()-1) {
                    //是目录
                    mkdir(fullname.c_str(), S_IRWXU);
                }else{
                    FILE* fp = fopen( fullname.c_str(), "wb");
                    while( fp)
                    {
                        read=unzReadCurrentFile(_unzFile, buffer, 4096);
                        if( read > 0 )
                        {
                            fwrite(buffer, read, 1, fp );
                        }
                        else if( read<0 )
                        {
                            CCLog("Failed to reading zip file");
                            break;
                        }
                        else
                            break;
                    }
                    if( fp )
                    {
                        fclose( fp );
                    }
                }
                
                unzCloseCurrentFile( _unzFile );
                ret = unzGoToNextFile( _unzFile );
                
            }while (ret==UNZ_OK && UNZ_OK!=UNZ_END_OF_LIST_OF_FILE );
            
            
            free(_unzFile);
            
        }else{
            success=false;
        }
        return success;
    }
};


#endif
