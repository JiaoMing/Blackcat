#ifndef __ASYNC_RESOURCE_H__
#define __ASYNC_RESOURCE_H__

#include "cocos2d.h"
using namespace cocos2d;


typedef CCScene* (*SEL_SceneHandler)();
#define scene_selector(_SELECTOR) (SEL_SceneHandler)(&_SELECTOR)

class AsyncResource
{
public:
	AsyncResource();
	~AsyncResource();
	static AsyncResource *sharedAsyncResource();
	const char *getPlistData(const char *pszSceneName);
	const char *getJpgData(const char *pszSceneName);
	SEL_SceneHandler getSelector(const char *pszSceneName);
};

#endif // __ASYNC_RESOURCE_H__
