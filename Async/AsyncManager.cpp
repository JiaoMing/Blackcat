#include "AsyncManager.h"
#include "AsyncLoadPlist.h"

static AsyncManager *g_instance = NULL;

AsyncManager::AsyncManager():m_currentSceneName("")
{
}

AsyncManager::~AsyncManager()
{
	CC_SAFE_DELETE(g_instance);
}

AsyncManager *AsyncManager::sharedAsyncManager()
{
	if (g_instance == NULL)
	{
		g_instance = new AsyncManager();
	}
	return g_instance;
}

void AsyncManager::removeSpriteFramesAndTextureFromFile(const char *pszPlist)
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(pszPlist);
	const char *pszPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pszPlist).c_str();
	CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(pszPath);

	std::string texturePath("");

	CCDictionary* metadataDict = (CCDictionary*)dict->objectForKey("metadata");
	if (metadataDict)
	{
		// try to read  texture file name from meta data
		texturePath = metadataDict->valueForKey("textureFileName")->getCString();
	}

	// CCDictionary::createWithContentsOfFileThreadSafe 需要释放dict
	dict->release();

	if (! texturePath.empty())
	{
		// build texture path relative to plist file
		texturePath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(texturePath.c_str(), pszPath);
	}
	else
	{
		CCAssert(0, "在plist文件中没有找到图片文件");
		return;
	}

	// 从texturePath中取文件名 如/../../filename.png 则filename.png
	std::string textureFileName = texturePath.substr(texturePath.rfind('/') + 1);

	// 从TextureCache中除去纹理
	CCTextureCache::sharedTextureCache()->removeTextureForKey(textureFileName.c_str());
}

void AsyncManager::split(string src, const char *token, vector<string> &array)
{
	array.clear();
	int nend=0;
	int nbegin=0;
	while(nend != -1)
	{
		nend = src.find(token, nbegin);
		if(nend == -1)
			array.push_back(src.substr(nbegin, src.length()-nbegin));
		else
			array.push_back(src.substr(nbegin, nend-nbegin));
		nbegin = nend + strlen(token);
	}
}

int AsyncManager::getResourceCount(const char *pszSceneName){
    
	int total = 0;
    
    std::string sPlistData = AsyncResource::sharedAsyncResource()->getPlistData(pszSceneName);
	std::vector<std::string> strPlist,strJpg;
	this->split(sPlistData, ",", strPlist);
	std::string sJpgData = AsyncResource::sharedAsyncResource()->getJpgData(pszSceneName);
	this->split(sJpgData, ",", strJpg);
    
	for (std::vector<std::string>::iterator iter = strPlist.begin(); iter != strPlist.end(); ++iter)
	{
		std::string s = *iter;
		if (s.length() == 0)
		{
			continue;
		}
        total++;
	}
    
	for (std::vector<std::string>::iterator iter = strJpg.begin(); iter != strJpg.end(); ++iter)
	{
		std::string s = *iter;
		if (s.length() == 0)
		{
			continue;
		}
        total++;
	}
    return total;

}

void AsyncManager::loadAsyncResource(const char *pszSceneName, CCObject *target, SEL_CallFuncO selector)
{
    
	std::vector<std::string> strPlist,strJpg;
    
	std::string sPlistData = AsyncResource::sharedAsyncResource()->getPlistData(pszSceneName);
	this->split(sPlistData, ",", strPlist);
    
	for (std::vector<std::string>::iterator iter = strPlist.begin(); iter != strPlist.end(); ++iter)
	{
		std::string s = *iter;
		if (s.length() == 0)
		{
			continue;
		}

		AsyncLoadPlist::sharedAsyncLoadPlist()->loadPlistAsync(s.c_str(), target, selector);
	}
    
	std::string sJpgData = AsyncResource::sharedAsyncResource()->getJpgData(pszSceneName);
	this->split(sJpgData, ",", strJpg);

	for (std::vector<std::string>::iterator iter = strJpg.begin(); iter != strJpg.end(); ++iter)
	{
		std::string s = *iter;
		if (s.length() == 0)
		{
			continue;
		}

		CCTextureCache::sharedTextureCache()->addImageAsync(s.c_str(), target, selector);
	}
}

void AsyncManager::removeAsyncResource(const char *pszSceneName)
{
	std::string sPlistData = AsyncResource::sharedAsyncResource()->getPlistData(pszSceneName);
	std::vector<std::string> strs;
	this->split(sPlistData, ",", strs);
	for (std::vector<std::string>::iterator iter = strs.begin(); iter != strs.end(); ++iter)
	{
		std::string s = *iter;
		if (s.length() == 0)
		{
			continue;
		}

		this->removeSpriteFramesAndTextureFromFile(s.c_str());
	}

	std::string sJpgData = AsyncResource::sharedAsyncResource()->getJpgData(pszSceneName);
	this->split(sJpgData, ",", strs);
	for (std::vector<std::string>::iterator iter = strs.begin(); iter != strs.end(); ++iter)
	{
		std::string s = *iter;
		if (s.length() == 0)
		{
			continue;
		}
		
		CCTextureCache::sharedTextureCache()->removeTextureForKey(s.c_str());
	}
}

SEL_SceneHandler AsyncManager::getSelector(const char *pszSceneName)
{
	return AsyncResource::sharedAsyncResource()->getSelector(pszSceneName);
}