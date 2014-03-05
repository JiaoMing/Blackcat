//
//  KEMicPlayer.cpp
//  Blackcat
//
//  Created by haojunhua on 14-1-22.
//
//

#include "KEMicPlayer.h"
#include "KEMicPlayerImpl.h"


KEMicPlayer::KEMicPlayer(){
    
}

KEMicPlayer::~KEMicPlayer(){
    CC_SAFE_DELETE(m_pMicPlayerImpl);
}

KEMicPlayer* KEMicPlayer::create(){
    KEMicPlayer *pRet = new KEMicPlayer();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool KEMicPlayer::init(){
    m_pMicPlayerImpl = __createSystemMicPlayer(this);
    m_pMicPlayerImpl->init();
    return true;
}

void KEMicPlayer::setDelegate(KEMicPlayerDelegate* pDelegate)
{
    m_pMicPlayerDelegate = pDelegate;
    if (m_pMicPlayerImpl != NULL)
    {
        m_pMicPlayerImpl->setDelegate(pDelegate);
    }
}

void KEMicPlayer::changeMode(KEMicDelegateMode mode)
{
    m_pMicPlayerImpl->changeMode(mode);
}
