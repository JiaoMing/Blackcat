//
//  RecordManager.cpp
//  Blackcat
//
//  Created by haojunhua on 14-9-15.
//
//

#include "RecordManager.h"

static RecordManager *g_instance = NULL;

RecordManager::RecordManager():m_delegate(NULL)
{
}

RecordManager::~RecordManager()
{
	CC_SAFE_DELETE(g_instance);
}

RecordManager* RecordManager::sharedRecordManager()
{
	if (g_instance == NULL)
	{
		g_instance = new RecordManager();
	}
	return g_instance;
}

