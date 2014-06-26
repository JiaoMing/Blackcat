#include "AsyncResource.h"
#include "KapianScene.h"
#include "KechengListScene.h"
#include "HomeScene.h"
#include "CardShelfScene.h"
#include "NewPetScene.h"
#include "InstallScene.h"
#include "WelcomeScene.h"
#include <string>
using namespace std;

/*** 图片资源相关start ***/
// 资源定义
typedef struct _ResouceStruct
{
    string sceneName;
	string plistData;
	string jpgData;
    SEL_SceneHandler selector;
} ResourceStruct;

// 在这里配置每个场景要加载的资源
static ResourceStruct res[] = {
	{
		"HomeScene",
		"xiezi.plist,heimao.plist,home1.plist,home2.plist,global.plist,achieve.plist",
		"",
        scene_selector(HomeScene::scene)
	},
	{
		"KapianHanziScene",
		"new_kapian.plist",
		"",
        scene_selector(KapianHanziScene::scene)
	},
	{
		"KapianTupianScene",
		"new_kapian.plist",
		"",
        scene_selector(KapianTupianScene::scene)
	},
	{
		"RenwuScene",
		"renwu.plist",
		"",
        scene_selector(KechengListScene::scene)
	},
	{
		"CardSetsScene",
		"ecards.plist",
		"",
        scene_selector(CardShelfScene::scene)
	},
	{
		"NewPetScene",
		"pet.plist,pet2.plist,pet3.plist,xiaobo_1.plist,xiaobo_2.plist,xiaobo_3.plist",
		"",
        scene_selector(NewPetScene::scene)
	},
	{
		"InstallScene",
		"install.plist",
		"",
        scene_selector(InstallScene::scene)
	},
	{
		"WelcomeScene",
		"",
		"",
        scene_selector(WelcomeScene::scene)
	}
};
/*** 图片资源相关end ***/

static AsyncResource *g_pInstance = NULL;

AsyncResource::AsyncResource()
{

}

AsyncResource::~AsyncResource()
{
	delete g_pInstance;
}

AsyncResource *AsyncResource::sharedAsyncResource()
{
	if (g_pInstance == NULL)
	{
		g_pInstance = new AsyncResource();
	}
	return g_pInstance;
}

const char *AsyncResource::getPlistData(const char *pszSceneName)
{
	for (int i = 0; i < sizeof(res); i++)
	{
		if (0 == strcmp(pszSceneName, res[i].sceneName.c_str()))
		{
			return res[i].plistData.c_str();
		}
	}
	return NULL;
}

const char *AsyncResource::getJpgData(const char *pszSceneName)
{
	for (int i = 0; i < sizeof(res); i++)
	{
		if (0 == strcmp(pszSceneName, res[i].sceneName.c_str()))
		{
			return res[i].jpgData.c_str();
		}
	}
	return NULL;
}

SEL_SceneHandler AsyncResource::getSelector(const char *pszSceneName)
{
	for (int i = 0; i < sizeof(res); i++)
	{
		if (0 == strcmp(pszSceneName, res[i].sceneName.c_str()))
		{
			return res[i].selector;
		}
	}
	return NULL;
}