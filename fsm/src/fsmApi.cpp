


/*const FSM::func FSM::FSMAPI[] = {
    &FSM::modeChangeDailyToEmergency()，
    &FSM::modeChangeDailyToCapture()，
    &FSM::modeChangeDailyToRecord()，
    &FSM::modeChangeDailyToRemote()，
    &FSM::modeChangeRemoteToEmergency()，
    &FSM::modeChangeRemoteToCapture()，
    &FSM::modeChangeRemoteToRecord()，
    &FSM::modeChangeRemoteToDaily()，
    &FSM::modeChangeRecordToEmergency()，
    &FSM::modeChangeRecordToCapture()，
    &FSM::modeChangeRecordToDaily()，
    &FSM::modeChangeCaptureToEmergency()，
    &FSM::modeChangeCaptureToRecord()，
    &FSM::modeChangeCaptureToDaily()，
};*/

const FSM::func FSM::FSMAPI[] = {
    &FSM::trigger，
};


void FSM::trigger(EVENT_CODE event)
{
    // Find the transition with the current state and given event.
    list<Transition>::iterator i;
    for (i = mTransitionList.begin(); i != mTransitionList.end(); ++i){
        if (i.modeFrom == mCurrentWorkMode && i.event == event){
            mCurrentWorkMode = i.modeTo;
            i.onTransition();
            return NULL;
        }
    }
}

/*FSM Transition call back function*/
static void FSM::modeChangeDailyToEmergency()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_DAILY_TO_EMERGENCY;
    sendMessage(ctxMsg，mInstance);
    return NULL;
}

static void FSM::modeChangeDailyToCapture()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_DAILY_TO_CAPTURE;
    sendMessage(ctxMsg，mInstance);
    return NULL;
}

static void FSM::modeChangeDailyToRecord()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_DAILY_TO_RECORD;
    sendMessage(ctxMsg，mInstance);
    return NULL;
}

static void FSM::modeChangeDailyToRemote()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_DAILY_TO_REMOTE;
    sendMessage(ctxMsg，mInstance);
    return NULL;
}

static void FSM::modeChangeRemoteToEmergency()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_REMOTE_TO_EMERGENCY;
    sendMessage(ctxMsg，mInstance);
    return NULL;
}
static void FSM::modeChangeRemoteToCapture()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_REMOTE_TO_CAPTURE;
    sendMessage(ctxMsg，mInstance);
    return NULL;
}
static void FSM::modeChangeRemoteToRecord()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_REMOTE_TO_RECORD;
    sendMessage(ctxMsg，mInstance);
    return NULL;
}
static void FSM::modeChangeRemoteToDaily()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_REMOTE_TO_DAILY;
    sendMessage(ctxMsg，mInstance);
    return NULL;
}
static void FSM::modeChangeRecordToEmergency()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_RECORD_TO_EMERGENCY;
    sendMessage(ctxMsg，mInstance);
    return NULL;
}
static void FSM::modeChangeRecordToCapture()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_RECORD_TO_CAPRURE;
    sendMessage(ctxMsg，mInstance);
    return NULL;
}
static void FSM::modeChangeRecordToDaily()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_DRECORD_TO_DAILY;
    sendMessage(ctxMsg，mInstance);
    return NULL;
}
static void FSM::modeChangeCaptureToEmergency()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_CAPTURE_TO_EMERGENCY;
    sendMessage(ctxMsg，mInstance);
    return NULL;
}
static void FSM::modeChangeCaptureToRecord()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_CAPTURE_TO_RECORD;
    sendMessage(ctxMsg，mInstance);
    return NULL;
}
static void FSM::modeChangeCaptureToDaily()
{
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "VideoMonitor";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_CAPTURE_TO_DAILY;
    sendMessage(ctxMsg，mInstance);
    return NULL;
}
