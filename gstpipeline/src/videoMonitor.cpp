#include "videoMonitor.h"
#include "engine.h"

VideoMonitor *VideoMonitor::mInstance = NULL;

//--------------------------------------------------------------------------
/**
 *  @brief  default constructor
 */
//--------------------------------------------------------------------------
VideoMonitor::VideoMonitor(): Base("VideoMonitor")
{
	VideoMonitor::mInstance = this;
    mCurrentPipelineMode = PIPELINE_WORK_MODE_DAILY;
}

//--------------------------------------------------------------------------
/**
 *  @brief  default destructor
 */
//--------------------------------------------------------------------------
VideoMonitor::~VideoMonitor()
{
    delete mInstance;
}

VideoMonitor *VideoMonitor::getInstance()
{
    if (NULL == mInstance) {
        mInstance = new VideoMonitor();
    }
    return mInstance;
}

void VideoMonitor::Init(void *data)
{
    LOGGER_DBG("VideoMonitor::Init");
    ctxEngine = static_cast<Engine*>(data);
}

void * VideoMonitor::Run(void *arg)
{	
	LOGGER_DBG("VideoMonitor::Run");
    /* Start the Gst main thread*/
    LOGGER_DBG("VideoMonitor::Start the thread gstMainThread");
	mInstance->startNewThread(gstMainThread,mInstance);

	void *psMessage = NULL;
    VideoMonitor *ctxVideoMonitor = static_cast<VideoMonitor *>(arg);
    while(1)
    {
        //waiting..............
        //lock
        pthread_mutex_lock(ctxVideoMonitor->getMutexPtr());
        while(ctxVideoMonitor->msgListEmpty())
        {
            LOGGER_DBG(">>>thread sleepping, VideoMonitor::Run");
            //wait  wake up
            pthread_cond_wait(ctxVideoMonitor->getCondPtr(), ctxVideoMonitor->getMutexPtr());
            LOGGER_DBG(">>>thread is waked up, VideoMonitor::Run");
        }
        psMessage = ctxVideoMonitor->getMsgFromListFront();
        //unlock
        pthread_mutex_unlock(ctxVideoMonitor->getMutexPtr());
        ctxVideoMonitor->handleMsg(psMessage,arg);
    }
    return NULL;
}

void VideoMonitor::handleMsg(void *msg,void *data)
{
    LOGGER_DBG("VideoMonitor::handleMsg");
    if(NULL == msg ||data == NULL)
    {
        LOGGER_ERR("Invalid message,msg or data is NULL");
        return;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    if(PIPELINE_SWITCH_MODE__MAX_NUMBER <= ctxMsg->opertation_id)
    {
        LOGGER_ERR("Invalid message,have no opertation");
        delete ctxMsg;
        return;
    }
    VideoMonitor *ctxVideoMonitor = static_cast<VideoMonitor *>(data);
    (ctxVideoMonitor->*videoMonitorAPI[ctxMsg->opertation_id])(msg, data);
}

void VideoMonitor::startNewThread(void *(threadFunc)(void *),void *data)
{	
    if(threadFunc==NULL||data==NULL)
    {   
        LOGGER_DBG("VideoMonitor::startNewThread threadFunc or data is NULL");
        return;
    }
    VideoMonitor *ctxVideoMonitor = static_cast<VideoMonitor *>(data);
    Message *ctxMsg = new Message();
    ctxMsg->origin = getClassName();
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = E_ENGINE_FUNCTION_START_NEW_THREAD;
    ctxMsg->dataListVoid.push_back(ctxVideoMonitor);
    ctxMsg->funcList.push_back(threadFunc);
    sendMessage(ctxMsg, ctxVideoMonitor);
}

void VideoMonitor::sendMessage(void *msg,void *data){
    
    if(msg==NULL||data==NULL)
    {
        LOGGER_DBG("VideoMonitor::sendMessage threadFunc or data is NULL");
        return;
    }
    VideoMonitor *ctxVideoMonitor = static_cast<VideoMonitor *>(data);
    //lock
    pthread_mutex_lock(ctxVideoMonitor->ctxEngine->getMutexPtr());
    ctxVideoMonitor->ctxEngine->addMsgToListBack(msg);
    //wake up Engine::Run
    pthread_cond_signal(ctxVideoMonitor->ctxEngine->getCondPtr());
    //unlock
    pthread_mutex_unlock(ctxVideoMonitor->ctxEngine->getMutexPtr());
}

