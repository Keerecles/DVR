#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <message.h>

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/app/gstappsink.h>
#include <gst/interfaces/xoverlay.h>
#include <gst/video/video.h>
#include "videomonitor.h"

VideoMonitor *VideoMonitor::mInstance = NULL;

//--------------------------------------------------------------------------
/**
 *  @brief  default constructor
 */
//--------------------------------------------------------------------------
VideoMonitor::VideoMonitor()
		: mClassName("VideoMonitor")
{
	pluginInit();
}

//--------------------------------------------------------------------------
/**
 *  @brief  default destructor
 */
//--------------------------------------------------------------------------
VideoMonitor::~VideoMonitor(){

}

std::string VideoMonitor::getClassName()
{
    return mClassName;
}

VideoMonitor *VideoMonitor::getInstance()
{
    if (NULL == mInstance) {
        mInstance = new VideoMonitor();
    }
    return mInstance;
}



void VideoMonitor::pluginInit(void *data){

  	g_mutex_init (&mMutex);
    ctxEngine = static_cast<Engine*>(data);
    p_queue = g_async_queue_new ();

}

void * VideoMonitor::pluginRun(void *data){
	
	/* Start the Gst main thread*/
	startNewThread(gstMainThread);

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
}
    
void VideoMonitor::pluginPause(){

}
    
void VideoMonitor::pluginResume(){

}

void VideoMonitor::pluginStop(){

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
    PluginSocket *ctxVideoMonitor = static_cast<PluginSocket *>(data);
    (ctxVideoMonitor->*videoMonitorAPI[ctxMsg->opertation_id])(msg, data);
}

void VideoMonitor::startNewThread(void *thread){
	Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = E_ENGINE_FUNCTION_START_NEW_THREAD;
    ctxMsg->dataListVoid.push_back(VideoMonitor::getInstance());
    ctxMsg->funcList.push_back(thread);
    sendMessage(VideoMonitor::getInstance(), ctxMsg);
}

