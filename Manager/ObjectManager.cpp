//
//  ObjectManager.cpp
//  Blackcat
//
//  Created by haojunhua on 14-6-26.
//
//

#include "ObjectManager.h"

static ObjectManager *g_instance = NULL;

ObjectManager* ObjectManager::sharedObjectManager()
{
	if (g_instance == NULL)
	{
		g_instance = new ObjectManager();
	}
	return g_instance;
}

ObjectManager::ObjectManager()
{
    m_webView=NULL;
    m_webVisible=false;
}

ObjectManager::~ObjectManager()
{
    
}

void ObjectManager::hideNativeView(){
    if(m_webView){
        m_webVisible=m_webView->isVisible();
        m_webView->setVisible(false);
    }
}


void ObjectManager::resumeNativeView(){
    if(m_webView)m_webView->setVisible(m_webVisible);
}

