#include "../include/fsm.h"


/*const FSM::func FSM::FSMAPI[] = {
    &FSM::modeChangeDailyToEmergency(),
    &FSM::modeChangeDailyToCapture(),
    &FSM::modeChangeDailyToRecord(),
    &FSM::modeChangeDailyToRemote(),
    &FSM::modeChangeRemoteToEmergency(),
    &FSM::modeChangeRemoteToCapture(),
    &FSM::modeChangeRemoteToRecord(),
    &FSM::modeChangeRemoteToDaily(),
    &FSM::modeChangeRecordToEmergency(),
    &FSM::modeChangeRecordToCapture(),
    &FSM::modeChangeRecordToDaily(),
    &FSM::modeChangeCaptureToEmergency(),
    &FSM::modeChangeCaptureToRecord(),
    &FSM::modeChangeCaptureToDaily(),
};*/

const FSM::func FSM::FSMAPI[] = {
    &FSM::trigger,
};

t_int FSM::trigger(void *msg, void *data)
{
    if( NULL== msg ||  NULL== data)
    {
        return E_OPERATION_ERROR_PARA;
    }

    EVENT_CODE event;
    event = PIPELINE_SWITCH_MODE_DAILY;
    
    Message *ctxMsg = static_cast<Message *>(msg);
    delete ctxMsg;

    // Find the transition with the current state and given event.
    std::list<Transition>::iterator i;
    for (i = mTransitionList.begin(); i != mTransitionList.end(); ++i){
        if (i->modeFrom == mCurrentWorkMode && i->event == event){
            mCurrentWorkMode = i->modeTo;
            i->onTransition();
            return E_OPERATION_ERROR_NONE;
        }
    }
}

/*FSM Transition call back function*/
void FSM::modeChangeDailyToEmergency()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mInstance->getClassName();
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_DAILY_TO_EMERGENCY;
    mInstance->sendMessage(ctxMsg,mInstance);
    return ;
}

void FSM::modeChangeDailyToCapture()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mInstance->getClassName();
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_DAILY_TO_CAPTURE;
    mInstance->sendMessage(ctxMsg,mInstance);
    return ;
}

void FSM::modeChangeDailyToRecord()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mInstance->getClassName();
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_DAILY_TO_RECORD;
    mInstance->sendMessage(ctxMsg,mInstance);
    return ;
}

void FSM::modeChangeDailyToRemote()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mInstance->getClassName();
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_DAILY_TO_REMOTE;
    mInstance->sendMessage(ctxMsg,mInstance);
    return ;
}

void FSM::modeChangeRemoteToEmergency()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mInstance->getClassName();
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_REMOTE_TO_EMERGENCY;
    mInstance->sendMessage(ctxMsg,mInstance);
    return ;
}
void FSM::modeChangeRemoteToCapture()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mInstance->getClassName();
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_REMOTE_TO_CAPTURE;
    mInstance->sendMessage(ctxMsg,mInstance);
    return ;
}
void FSM::modeChangeRemoteToRecord()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mInstance->getClassName();
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_REMOTE_TO_RECORD;
    mInstance->sendMessage(ctxMsg,mInstance);
    return ;
}
void FSM::modeChangeRemoteToDaily()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mInstance->getClassName();
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_REMOTE_TO_DAILY;
    mInstance->sendMessage(ctxMsg,mInstance);
    return ;
}
void FSM::modeChangeRecordToEmergency()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mInstance->getClassName();
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_RECORD_TO_EMERGENCY;
    mInstance->sendMessage(ctxMsg,mInstance);
    return ;
}
void FSM::modeChangeRecordToCapture()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mInstance->getClassName();
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_RECORD_TO_CAPRURE;
    mInstance->sendMessage(ctxMsg,mInstance);
    return ;
}
void FSM::modeChangeRecordToDaily()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mInstance->getClassName();
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_DRECORD_TO_DAILY;
    mInstance->sendMessage(ctxMsg,mInstance);
    return ;
}
void FSM::modeChangeCaptureToEmergency()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mInstance->getClassName();
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_CAPTURE_TO_EMERGENCY;
    mInstance->sendMessage(ctxMsg,mInstance);
    return ;
}
void FSM::modeChangeCaptureToRecord()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mInstance->getClassName();
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_CAPTURE_TO_RECORD;
    mInstance->sendMessage(ctxMsg,mInstance);
    return ;
}
void FSM::modeChangeCaptureToDaily()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mInstance->getClassName();
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_CAPTURE_TO_DAILY;
    mInstance->sendMessage(ctxMsg,mInstance);
    return ;
}
