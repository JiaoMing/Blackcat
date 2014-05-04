//
//  HomeScene.h
//  Blackcat
//
//  Created by haojunhua on 13-10-22.
//
//
#ifndef __Java_com_kidsedu_JniHelper_H__
#define __Java_com_kidsedu_JniHelper_H__
#include <string>

typedef void (*WebViewCallback)(int key,std::string cmd);

extern void addWebViewJNI(int key,const char* htmlFilename, int x,int y,int width,int height, WebViewCallback pfWebViewCallback, void* ctx);
void removeWebViewJNI(int key);
void callWebWithJsJNI(int key,const char* js);
void setVisibleJNI(int key,bool visible);
void showTipDialogJNI(const char* title, const char *msg);
void onStatEventJNI(int eventType, const char* event_id, const char* label, int shareData);


#endif /* __Java_com_kidsedu_JniHelper_H__ */
