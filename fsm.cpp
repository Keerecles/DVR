#include "fsm.h"




FSM *FSM::mInstance = NULL;

//--------------------------------------------------------------------------
/**
 *  @brief  default constructor
 */
//--------------------------------------------------------------------------
FSM::FSM()
		: mClassName("FSM")
{
	pluginInit();
}

//--------------------------------------------------------------------------
/**
 *  @brief  default destructor
 */
//--------------------------------------------------------------------------
FSM::~FSM(){

}

std::string FSM::getClassName()
{
    return mClassName;
}

FSM *FSM::getInstance()
{
    if (NULL == mInstance) {
        mInstance = new FSM();
    }
    return mInstance;
}

void FSM::pluginInit(void *data){

  	g_mutex_init (&mMutex);
    ctxEngine = static_cast<Engine*>(data);
    p_queue = g_async_queue_new ();

}


void *PluginSocket::Run(void *arg)
{
    void *ps_message = NULL;
    LOGGER_DBG("PluginSocket::Run");
    PluginSocket *ctxFSM = static_cast<PluginSocket *>(arg);

    while(1)
    {
        //waiting..............
        //lock
        pthread_mutex_lock(ctxFSM->getMutexP());
        while(ctxFSM->msgListEmpty())
        {
            LOGGER_DBG(">>>thread sleepping, PluginSocket::Run");
            //wait  wake up
            pthread_cond_wait(ctxFSM->getCondP(), ctxFSM->getMutexP());
            LOGGER_DBG(">>>thread is waked up, PluginSocket::Run");
        }
        ps_message = ctxFSM->getMsgFromListFront();
        //unlock
        pthread_mutex_unlock(ctxFSM->getMutexP());
        ctxFSM->handleMsg(ps_message,arg);
    }
    return NULL;
}




void * FSM::pluginRun(void *data){
	/*Generate the FSM */
    generateFSM();

    void *ps_message = NULL;
    LOGGER_DBG("FSM::Run");
    FSM *ctxFSM = static_cast<FSM *>(arg);

    while(1)
    {
        //waiting..............
        //lock
        pthread_mutex_lock(ctxFSM->getMutexP());
        while(ctxFSM->msgListEmpty())
        {
            LOGGER_DBG(">>>thread sleepping, PluginSocket::Run");
            //wait  wake up
            pthread_cond_wait(ctxFSM->getCondP(), ctxFSM->getMutexP());
            LOGGER_DBG(">>>thread is waked up, PluginSocket::Run");
        }
        ps_message = ctxFSM->getMsgFromListFront();
        //unlock
        pthread_mutex_unlock(ctxFSM->getMutexP());
        ctxFSM->handleMsg(ps_message,arg);
    }
    return NULL;


//     /*Create our own GLib Main Context and make it the default one */
//     mContext = g_main_context_new ();
//     g_main_context_push_thread_default(mContext);
//     while(1){
//         /*get Message from the message queue (Block)*/


//     }
//     /* Create a GLib Main Loop and set it to run */
//     mPluginGMainLoop = g_main_loop_new (mContext, FALSE);
//     g_main_loop_run (mPluginGMainLoop);
//     g_main_loop_unref (mPluginGMainLoop);
//     mPluginGMainLoop = NULL;
}
    
void pluginPause(){

}
    
void FSM::pluginResume(){

}

void FSM::pluginStop(){

}

void FSM::startNewThread(void *thread){
	Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = E_ENGINE_FUNCTION_START_NEW_THREAD;
    ctxMsg->dataListVoid.push_back(FSM::getInstance());
    ctxMsg->funcList.push_back(thread);
    sendMessage(FSM::getInstance(), ctxMsg);
}


void FSM::sendMessage(void *obj, void *msg)
{    
    FSM *ctxFSM = static_cast<FSM *>(obj);
    g_async_queue_lock (ctxFSM->ctxEngine->p_queue);
    g_async_queue_push_unlocked (ctxFSM->ctxEngine->p_queue,msg);
    g_async_queue_unlock (ctxFSM->ctxEngine->p_queue);
}

void FSM::handleMsg(void *msg,void *data)
{
    LOGGER_DBG("FSM::handleMsg");
    if(NULL == msg ||data == NULL)
    {
        LOGGER_ERR("Invalid message,msg or data is NULL");
        return NULL;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    if(E_SOCKET_FUNCTION_MAX_NUMBER <= ctxMsg->opertation_id)
    {
        LOGGER_ERR("Invalid message,have no opertation");
        delete ctxMsg;
        return NULL;
    }
    PluginSocket *ctxFSM = static_cast<PluginSocket *>(data);
    (ctxFSM->*socketServiceAPI[ctxMsg->opertation_id])(msg, data);
}

void FSM::addTransition(WorkMode modeFrom, WorkMode modeTo, EVENT_CODE event,
                      void (*onTransition)()){

    if (modeFrom == NULL || modeTo == NULL)
    return;
    Transition transition = Fsm::createTransition(modeFrom, modeTo, event, onTransition);
    mTransitionList.push_back(transition);
}

void FSM::generateFSM(){

    fsm.addTransition(PIPELINE_WORK_MODE_DAILY, PIPELINE_WORK_MODE_EMERGENCY, PIPELINE_SWITCH_MODE_EMERGENCY, &modeChangeDailyToEmergency);
    fsm.addTransition(PIPELINE_WORK_MODE_DAILY, PIPELINE_WORK_MODE_CAPTURE, PIPELINE_SWITCH_MODE_CAPTURE, &modeChangeDailyToCapture);
    fsm.addTransition(PIPELINE_WORK_MODE_DAILY, PIPELINE_WORK_MODE_RECORD, PIPELINE_SWITCH_MODE_RECORD, &modeChangeDailyToRecord);
    fsm.addTransition(PIPELINE_WORK_MODE_DAILY, PIPELINE_WORK_MODE_REMOTE, PIPELINE_SWITCH_MODE_REMOTE, &modeChangeDailyToRemote);

    fsm.addTransition(PIPELINE_WORK_MODE_REMOTE, PIPELINE_WORK_MODE_EMERGENCY, PIPELINE_SWITCH_MODE_EMERGENCY, &modeChangeRemoteToEmergency);
    fsm.addTransition(PIPELINE_WORK_MODE_REMOTE, PIPELINE_WORK_MODE_CAPTURE, PIPELINE_SWITCH_MODE_CAPTURE, &modeChangeRemoteToCapture);
    fsm.addTransition(PIPELINE_WORK_MODE_REMOTE, PIPELINE_WORK_MODE_RECORD, PIPELINE_SWITCH_MODE_RECORD, &modeChangeRemoteToRecord);
    fsm.addTransition(PIPELINE_WORK_MODE_REMOTE, PIPELINE_WORK_MODE_DAILY, PIPELINE_WORK_MODE_DAILY, &modeChangeRemoteToDaily);

    fsm.addTransition(PIPELINE_WORK_MODE_RECORD, PIPELINE_WORK_MODE_EMERGENCY, PIPELINE_SWITCH_MODE_EMERGENCY, &modeChangeRecordToEmergency);
    fsm.addTransition(PIPELINE_WORK_MODE_RECORD, PIPELINE_WORK_MODE_CAPTURE, PIPELINE_SWITCH_MODE_CAPTURE, &modeChangeRecordToCapture);
    fsm.addTransition(PIPELINE_WORK_MODE_RECORD, PIPELINE_WORK_MODE_DAILY, PIPELINE_WORK_MODE_DAILY, &modeChangeRecordToDaily);

    fsm.addTransition(PIPELINE_WORK_MODE_CAPTURE, PIPELINE_WORK_MODE_EMERGENCY, PIPELINE_SWITCH_MODE_EMERGENCY, &modeChangeCaptureToEmergency);
    fsm.addTransition(PIPELINE_WORK_MODE_CAPTURE, PIPELINE_WORK_MODE_RECORD, PIPELINE_WORK_MODE_RECORD, &modeChangeCaptureToRecord);
    fsm.addTransition(PIPELINE_WORK_MODE_CAPTURE, PIPELINE_WORK_MODE_DAILY, PIPELINE_WORK_MODE_DAILY, &modeChangeCaptureToDaily);
}

void FSM::trigger(EVENT_CODE event){
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

static Transition FSM::createTransition(WorkMode modeFrom, WorkMode modeTo,
                                      EVENT_CODE event, void (*onTransition)()){
    Transition tmp;
    tmp.modeFrom = modeFrom;
    tmp.modeTo = modeTo;
    tmp.event = event;
    tmp.onTransition = onTransition;
  return t;
}

/*FSM Transition call back function*/
void FSM::modeChangeDailyToEmergency(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_DAILY_TO_EMERGENCY,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeDailyToCapture(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_DAILY_TO_CAPTURE,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeDailyToRecord(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_DAILY_TO_RECORD,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeDailyToRemote(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_DAILY_TO_REMOTE,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeRemoteToEmergency(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_REMOTE_TO_EMERGENCY,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeRemoteToCapture(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_REMOTE_TO_CAPTURE,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeRemoteToRecord(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_REMOTE_TO_RECORD,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeRemoteToDaily(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_REMOTE_TO_DAILY,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeRecordToEmergency(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_RECORD_TO_EMERGENCY,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeRecordToCapture(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_RECORD_TO_CAPRURE,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeRecordToDaily(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_DRECORD_TO_DAILY,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeCaptureToEmergency(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_CAPTURE_TO_EMERGENCY,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeCaptureToRecord(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_CAPTURE_TO_RECORD,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeCaptureToDaily(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_CAPTURE_TO_DAILY,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
