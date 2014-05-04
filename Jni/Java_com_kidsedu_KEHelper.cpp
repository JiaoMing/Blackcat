#include <stdlib.h>
#include <jni.h>
#include <android/log.h>
#include <string>
#include <map>
#include "platform/android/jni/JniHelper.h"
#include "cocoa/CCString.h"
#include "Java_com_kidsedu_KEHelper.h"

#include "CCDirector.h"
using namespace cocos2d;

#define  LOG_TAG    "Java_com_kidsedu_KEHelper.cpp"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

#define  CLASS_NAME "com/kidsedu/KEHelper"


using namespace cocos2d;
using namespace std;

static WebViewCallback s_pfWebViewCallback = NULL;

extern "C" {
    JNIEXPORT void JNICALL Java_com_kidsedu_KEHelper_nativeWebCallBack(JNIEnv * env, jobject obj,int tag, jstring jcmd) {
        string cmd=JniHelper::jstring2string(jcmd);
        if (s_pfWebViewCallback) s_pfWebViewCallback(tag,cmd);
    }
    
    JNIEXPORT void JNICALL Java_com_kidsedu_KEHelper_exitApp(JNIEnv * env) {
        CCDirector::sharedDirector()->end();
    }
    
    JNIEXPORT void JNICALL Java_com_kidsedu_KEHelper_popScene(JNIEnv * env) {
        CCDirector::sharedDirector()->popScene();
    }
}

void addWebViewJNI(int key,const char* htmlFilename, int x,int y,int width,int height, WebViewCallback pfWebViewCallback, void* ctx) {
    s_pfWebViewCallback = pfWebViewCallback;

    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "addWebView", "(ILjava/lang/String;IIII)V")) {

        jstring stringArg1 = t.env->NewStringUTF(htmlFilename);

        t.env->CallStaticVoidMethod(t.classID, t.methodID,key, stringArg1, x,y,width,height);

        t.env->DeleteLocalRef(stringArg1);
    }
}

void removeWebViewJNI(int key){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "removeWebView", "(I)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID,key);
    }
}

void setVisibleJNI(int key,bool visible){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setWebViewVisible", "(IZ)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID,key,visible);
    }
}


void callWebWithJsJNI(int key,const char* js){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "callWebWithJs", "(ILjava/lang/String;)V")) {
        
        jstring stringArg1 = t.env->NewStringUTF(js);
        
        t.env->CallStaticVoidMethod(t.classID, t.methodID,key, stringArg1);
        
        t.env->DeleteLocalRef(stringArg1);
    }
}

void showTipDialogJNI(const char *title, const char *msg)
{
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "showTipDialog", "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring jTitle = t.env->NewStringUTF(title);
        jstring jMsg = t.env->NewStringUTF(msg);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jTitle, jMsg);
        t.env->DeleteLocalRef(jTitle);
        t.env->DeleteLocalRef(jMsg);
    }
}

void onStatEventJNI(int eventType, const char* event_id, const char* label, int shareData){
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t, CLASS_NAME, "onStatEvent", "(Ljava/lang/String;Ljava/lang/String;II)V"))
    {
        jstring jEvent_id = t.env->NewStringUTF(event_id);
        jstring jLabel = t.env->NewStringUTF(label);
        t.env->CallStaticVoidMethod(t.classID, t.methodID,jEvent_id,jLabel,eventType,shareData);
        t.env->DeleteLocalRef(jEvent_id);
        t.env->DeleteLocalRef(jLabel);
    }
}

