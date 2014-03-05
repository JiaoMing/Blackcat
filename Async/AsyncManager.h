#ifndef __UTILFUNC_H__
#define __UTILFUNC_H__

#include "cocos2d.h"
#include "AsyncResource.h"
using namespace cocos2d;
using namespace std;

class AsyncManager
{
public:
	AsyncManager();
	~AsyncManager();
	static AsyncManager *sharedAsyncManager();
	void removeSpriteFramesAndTextureFromFile(const char *pszPlist);
	void split(string src, const char *token, vector<string> &array);
    int getResourceCount(const char *pszSceneName);
	void loadAsyncResource(const char *pszSceneName, CCObject *target, SEL_CallFuncO selector);
	void removeAsyncResource(const char *pszSceneName);
	SEL_SceneHandler getSelector(const char *pszSceneName);
    
    void setCurrentSceneName(string currentSceneName){m_currentSceneName=currentSceneName;}
    string getCurrentSceneName(){return m_currentSceneName;}
public:
    string m_currentSceneName;
};


#endif // __UTILFUNC_H__
