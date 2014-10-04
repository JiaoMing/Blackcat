//
//  MicRecorePlayer.cpp
//  PointPen
//
//  Created by wanghongjun on 13-9-22.
//
//

#include "MicRecordPlayer.h"
#include "MicRecordController.h"

static MicRecordController *micController=NULL;

MicRecordPlayer::MicRecordPlayer(MicRecordPlayerDelegate* is)
{
    micController = [[MicRecordController alloc]initWithScene:is];
}

MicRecordPlayer::~MicRecordPlayer()
{
    [micController release];
}

void MicRecordPlayer::changeMode(MicDelegateMode mode)
{
    [micController changeMode:mode];
}
