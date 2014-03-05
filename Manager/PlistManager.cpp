//
//  PlistManager.cpp
//  Blackcat
//
//  Created by haojunhua on 14-2-24.
//
//

#include "PlistManager.h"

static PlistManager *g_instance = NULL;

PlistManager::PlistManager()
{
    m_dict=CCDictionary::create();
    m_dict->retain();
}

PlistManager::~PlistManager()
{
    m_dict->removeAllObjects();
    m_dict->release();
	CC_SAFE_DELETE(g_instance);
}

PlistManager *PlistManager::sharedPlistManager()
{
	if (g_instance == NULL)
	{
		g_instance = new PlistManager();
	}
	return g_instance;
}

CCDictionary* PlistManager::getPlistDictionary(const char* key){
    CCDictionary* dict=(CCDictionary*)m_dict->objectForKey(key);
    if (dict==NULL) {
        dict=CCDictionary::createWithContentsOfFileThreadSafe(key);
        m_dict->setObject(dict, key);
    }
    return dict;
}
