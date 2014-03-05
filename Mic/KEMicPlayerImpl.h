//
//  KEMicPlayerImpl.h
//  Blackcat
//
//  Created by haojunhua on 14-1-22.
//
//

#ifndef Blackcat_MicPlayerImpl_h
#define Blackcat_MicPlayerImpl_h

#include "cocos2d.h"
#include "KEMicPlayer.h"

class KEMicPlayerImpl{
public:
    KEMicPlayerImpl(KEMicPlayer* pMicPlayer):m_pMicPlayer(pMicPlayer), m_pMicPlayerDelegate(NULL)  {}
    virtual ~KEMicPlayerImpl() {}
    
    void setDelegate(KEMicPlayerDelegate* pDelegate) { m_pMicPlayerDelegate = pDelegate; };
    KEMicPlayerDelegate* getDelegate() { return m_pMicPlayerDelegate; };
    KEMicPlayer* getMicPlayer() { return m_pMicPlayer; };
public:
    virtual bool init() = 0;
    virtual void changeMode(KEMicDelegateMode mode)=0;
protected:
    KEMicPlayer* m_pMicPlayer;
    KEMicPlayerDelegate* m_pMicPlayerDelegate;
};

// This method must be implemented at each subclass of KEMicPlayerImpl.
extern KEMicPlayerImpl* __createSystemMicPlayer(KEMicPlayer* pMicPlayer);


#endif
