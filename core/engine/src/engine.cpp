#include "engine.h"
Engine *Engine::mInstance = NULL;
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
Engine::Engine():Base("Engine")
{
    Engine::mInstance = this;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
Engine::~Engine()
{
    delete mInstance;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
Engine * Engine::getInstance()
{
    if (NULL == mInstance) {
        mInstance = new Engine();
    }
    return mInstance;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void Engine::Init(void *data)
{
    LOGGER_DBG("Engine::Init start");
    createModeObject(data);
    LOGGER_DBG("Engine::Init ends");
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void *Engine::Run(void *arg)
{
    void *ps_message = NULL;
    LOGGER_DBG("Engine::Run");
    Engine *ctxEngine = static_cast<Engine *>(arg);
    while(1)
    {
        //add msg to list of a module
        //wating...............
        //lock
        pthread_mutex_lock(ctxEngine->getMutexPtr());
        while(ctxEngine->msgListEmpty())
        {
            LOGGER_DBG(">>>thread sleepping, Engine::Run");
            //wait  wake up
            pthread_cond_wait(ctxEngine->getCondPtr(), ctxEngine->getMutexPtr());
            LOGGER_DBG(">>>thread waked up, Engine::Run");
        }
        ps_message = ctxEngine->getMsgFromListFront();
        //unlock
        pthread_mutex_unlock(ctxEngine->getMutexPtr());

        ctxEngine->handleMsg(ps_message,arg);
    }
    return NULL;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void Engine::createModeObject(void *data)
{
    //add mode at here
    //use threadManager,launch thread of every module,handle msg
    ThreadManager *ctxTM = ThreadManager::getInstance();
    
    //load other modules at here
   /* PluginSocket *pluginSocket = PluginSocket::getInstance();
    pluginSocket->Init(data);
    ctxTM->startThread(pluginSocket->Run, (void *)pluginSocket );*/
    VideoMonitor *videoMonitor = VideoMonitor::getInstance();
    videoMonitor->Init(data);
    ctxTM->startThread(videoMonitor->Run, (void *)videoMonitor );

/*    FSM *fsm = FSM::getInstance();
    fsm->Init(data);
    ctxTM->startThread(fsm->Run, (void *)fsm );  */  

}
void Engine::handleMsg(void * msg,void *data)
{
    //distribute msg, according to the key(destination)
    if(msg==NULL || data==NULL)
    {
            return;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    if(getClassName() == ctxMsg->destination)
    {
       LOGGER_DBG("this message is for <engine>,  opertation_id :  %d ",ctxMsg->opertation_id);
        switch(ctxMsg->opertation_id)
        {
            case E_ENGINE_FUNCTION_START_NEW_THREAD:
                    for (int i=0; i < ctxMsg->funcList.size(); i++) {
                        startNewThread(ctxMsg->funcList[i],ctxMsg->dataListVoid[i]);
                    }
                break;
            case E_ENGINE_FUNCTION_ON_THREAD_TERMINATE:
                onThreadTerminate(ctxMsg->origin, ctxMsg->dataListUINT[0]);
                break;

            default :
                LOGGER_DBG("invailed request !");
                break;
        }
        return;
    }

    if(!mModuleList.empty())
    {
        std::map<std::string,void*>::iterator it;
        it = mModuleList.find(ctxMsg->destination);
        if(it != mModuleList.end())
        {
            Base *ctx = static_cast<Base*>(it->second);
            //lock
            pthread_mutex_lock(ctx->getMutexPtr());
            ctx->addMsgToListBack(msg);
            //wake up Engine::Run
            pthread_cond_signal(ctx->getCondPtr());
            //unlock
            pthread_mutex_unlock(ctx->getMutexPtr());
            LOGGER_DBG("add msg to module<%s>",ctx->getClassName().c_str());
        }
        else
        {
           LOGGER_DBG("have no such plugin , according to plugin name : %s",ctxMsg->destination.c_str());
            //todo: need free msg? but this case can not be reached because this msg must be rejected by sendMessage().
            delete ctxMsg;
        }
    }
    else
    {
        LOGGER_DBG("mModuleList is empty");
        delete ctxMsg;
    }
    return;
}

t_int Engine::startNewThread(void *(threadMain)(void *), void *data)
{
    LOGGER_DBG("Engine::startNewThread");
    t_int ret = E_OPERATION_ERROR_UNKNOW;

    if(NULL == threadMain || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }

    //get ThreadManager's obj
    ThreadManager *ctxTM = ThreadManager::getInstance();

    //run plugin thread
    ret = ctxTM->startThread(threadMain, data);

    return ret;
}

t_int Engine::onThreadTerminate(std::string &pluginName, pthread_t thread_id)
{
    t_int ret = E_OPERATION_ERROR_UNKNOW;

    if("" == pluginName || 0 == thread_id)
    {
        return E_OPERATION_ERROR_PARA;
    }

    //get ThreadManager's obj
    ThreadManager *ctxTM = ThreadManager::getInstance();

    ret = ctxTM->onThreadTerminate(pluginName, thread_id );

    return ret;
}
