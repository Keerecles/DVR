#include "fsm/include/fsm.h"
#include "core/include/engine.h"

FSM *FSM::mInstance = NULL;

//--------------------------------------------------------------------------
/**
 *  @brief  default constructor
 */
//--------------------------------------------------------------------------
FSM::FSM(): Base("FSM")
{
    FSM::mInstance = this;
    mCurrentWorkMode = PIPELINE_WORK_MODE_DAILY;
    mutexTransitionList = mutexTransitionList;
}

FSM::FSM(WorkMode initworkmode): Base("FSM")
{
	FSM::mInstance = this;
    mCurrentWorkMode = initworkmode;
    mutexTransitionList = mutexTransitionList;
}

//--------------------------------------------------------------------------
/**
 *  @brief  default destructor
 */
//--------------------------------------------------------------------------
FSM::~FSM()
{
    delete mInstance;
}

FSM *FSM::getInstance()
{
    if (NULL == mInstance) {
        mInstance = new FSM();
    }
    return mInstance;
}

void FSM::Init(void *data)
{
    ctxEngine = static_cast<Engine*>(data);
    /*Generate the FSM */
    generateFSM();
}

void * FSM::Run(void *arg)
{	
    void *psMessage = NULL;
    LOGGER_DBG("FSM::Run");
    FSM *ctxFSM = static_cast<FSM *>(arg);

    while(1)
    {
        //waiting..............
        //lock
        pthread_mutex_lock(ctxFSM->getMutexPtr());
        while(ctxFSM->msgListEmpty())
        {
            LOGGER_DBG(">>>thread sleepping, FSM::Run");
            //wait  wake up
            pthread_cond_wait(ctxFSM->getCondPtr(), ctxFSM->getMutexPtr());
            LOGGER_DBG(">>>thread is waked up, FSM::Run");
        }
        psMessage = ctxFSM->getMsgFromListFront();
        //unlock
        pthread_mutex_unlock(ctxFSM->getMutexPtr());
        ctxFSM->handleMsg(psMessage,arg);
    }
    return NULL;
}
    
void FSM::startNewThread(void *(threadFunc)(void *),void *data)
{	
    if(threadFunc==NULL||data==NULL)
    {
        return;
    }
    FSM *ctxFSM = static_cast<FSM *>(data);
    Message *ctxMsg = new Message();
    ctxMsg->origin = getClassName();
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = E_ENGINE_FUNCTION_START_NEW_THREAD;
    ctxMsg->dataListVoid.push_back(ctxFSM);
    ctxMsg->funcList.push_back(threadFunc);
    sendMessage(ctxMsg, ctxFSM);
}

void FSM::sendMessage(void *msg,void *data)
{    
    if(msg==NULL||data==NULL)
    {
        return;
    }
    FSM *ctxFSM = static_cast<FSM *>(data);
    //lock
    pthread_mutex_lock(ctxFSM->ctxEngine->getMutexPtr());
    ctxFSM->ctxEngine->addMsgToListBack(msg);
    //wake up Engine::Run
    pthread_cond_signal(ctxFSM->ctxEngine->getCondPtr());
    //unlock
    pthread_mutex_unlock(ctxFSM->ctxEngine->getMutexPtr());
}

void FSM::handleMsg(void *msg,void *data)
{
    LOGGER_DBG("FSM::handleMsg");
    if(NULL == msg ||data == NULL)
    {
        LOGGER_ERR("Invalid message,msg or data is NULL");
        return ;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    if(E_SOCKET_FUNCTION_MAX_NUMBER <= ctxMsg->opertation_id)
    {
        LOGGER_ERR("Invalid message,have no opertation");
        delete ctxMsg;
        return ;
    }
    FSM *ctxFSM = static_cast<FSM *>(data);
    (ctxFSM->*FSMAPI[ctxMsg->opertation_id])(msg, data);
}

void FSM::addTransition(WorkMode modeFrom, WorkMode modeTo, EVENT_CODE event,
                      void (*onTransition)())
{

    if (modeFrom == NULL || modeTo == NULL) return;
    Transition transition = FSM::createTransition(modeFrom, modeTo, event, onTransition);
    mTransitionList.push_back(transition);
}

void FSM::generateFSM()
{
    addTransition(PIPELINE_WORK_MODE_DAILY, PIPELINE_WORK_MODE_EMERGENCY, PIPELINE_SWITCH_MODE_EMERGENCY, &mInstance->modeChangeDailyToEmergency);
    addTransition(PIPELINE_WORK_MODE_DAILY, PIPELINE_WORK_MODE_CAPTURE, PIPELINE_SWITCH_MODE_CAPTURE, &mInstance->modeChangeDailyToCapture);
    addTransition(PIPELINE_WORK_MODE_DAILY, PIPELINE_WORK_MODE_RECORD, PIPELINE_SWITCH_MODE_RECORD, &mInstance->modeChangeDailyToRecord);
    addTransition(PIPELINE_WORK_MODE_DAILY, PIPELINE_WORK_MODE_REMOTE, PIPELINE_SWITCH_MODE_REMOTE, &mInstance->modeChangeDailyToRemote);

    addTransition(PIPELINE_WORK_MODE_REMOTE, PIPELINE_WORK_MODE_EMERGENCY, PIPELINE_SWITCH_MODE_EMERGENCY, &mInstance->modeChangeRemoteToEmergency);
    addTransition(PIPELINE_WORK_MODE_REMOTE, PIPELINE_WORK_MODE_CAPTURE, PIPELINE_SWITCH_MODE_CAPTURE, &mInstance->modeChangeRemoteToCapture);
    addTransition(PIPELINE_WORK_MODE_REMOTE, PIPELINE_WORK_MODE_RECORD, PIPELINE_SWITCH_MODE_RECORD, &mInstance->modeChangeRemoteToRecord);
    addTransition(PIPELINE_WORK_MODE_REMOTE, PIPELINE_WORK_MODE_DAILY, PIPELINE_SWITCH_MODE_DAILY, &mInstance->modeChangeRemoteToDaily);

    addTransition(PIPELINE_WORK_MODE_RECORD, PIPELINE_WORK_MODE_EMERGENCY, PIPELINE_SWITCH_MODE_EMERGENCY, &mInstance->modeChangeRecordToEmergency);
    addTransition(PIPELINE_WORK_MODE_RECORD, PIPELINE_WORK_MODE_CAPTURE, PIPELINE_SWITCH_MODE_CAPTURE,&mInstance->modeChangeRecordToCapture);
    addTransition(PIPELINE_WORK_MODE_RECORD, PIPELINE_WORK_MODE_DAILY, PIPELINE_SWITCH_MODE_DAILY, &mInstance->modeChangeRecordToDaily);

    addTransition(PIPELINE_WORK_MODE_CAPTURE, PIPELINE_WORK_MODE_EMERGENCY, PIPELINE_SWITCH_MODE_EMERGENCY, &mInstance->modeChangeCaptureToEmergency);
    addTransition(PIPELINE_WORK_MODE_CAPTURE, PIPELINE_WORK_MODE_RECORD, PIPELINE_SWITCH_MODE_RECORD, &mInstance->modeChangeCaptureToRecord);
    addTransition(PIPELINE_WORK_MODE_CAPTURE, PIPELINE_WORK_MODE_DAILY, PIPELINE_SWITCH_MODE_DAILY, &mInstance->modeChangeCaptureToDaily);
}

Transition FSM::createTransition(WorkMode modeFrom, WorkMode modeTo,
                                      EVENT_CODE event, void (*onTransition)())
{
    Transition tmp;
    tmp.modeFrom = modeFrom;
    tmp.modeTo = modeTo;
    tmp.event = event;
    tmp.onTransition = onTransition;
  return tmp;
}
