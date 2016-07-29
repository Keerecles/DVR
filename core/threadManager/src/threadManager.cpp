#include "threadManager.h"

ThreadManager *ThreadManager::mInstance = NULL;
//--------------------------------------------------------------------------
/**
 *  @brief  default constructor
 */
//--------------------------------------------------------------------------
ThreadManager::ThreadManager():Base("ThreadManager")
{
    ThreadManager::mInstance = this;
}

//--------------------------------------------------------------------------
/**
 *  @brief  default destructor
 */
//--------------------------------------------------------------------------
ThreadManager::~ThreadManager()
{
    delete mInstance;
}

//--------------------------------------------------------------------------
/**
 *  @brief  get instance of ThreadManager
 *
 *  @return instance of ThreadManager
 */
//--------------------------------------------------------------------------
ThreadManager* ThreadManager::getInstance()
{
    if (NULL == mInstance) {
        mInstance = new ThreadManager();
    }
    return mInstance;
}
//--------------------------------------------------------------------------
/**
 *  @brief   get properties of ThreadManager
 *
 *  @param [in]  key        key of properties
 *  @param [out] value      value of properties
 */
//--------------------------------------------------------------------------
t_int ThreadManager::getProperties(std::string &key, std::string &value)
{
    return E_OPERATION_ERROR_NONE;
}

//--------------------------------------------------------------------------
/**
 *  @brief   set properties of ThreadManager
 *
 *  @param [in]  key        key of properties
 *  @param [out] value      value of properties
 */
//--------------------------------------------------------------------------
t_int ThreadManager::setProperties(std::string &key, std::string &value)
{
    return E_OPERATION_ERROR_NONE;
}

//--------------------------------------------------------------------------
/**
 *  @brief   start thread
 *
 *  @param [in]  threadMain        main function of this thread
 *  @param [out] data              user data
 */
//--------------------------------------------------------------------------
t_int ThreadManager::startThread(void *(threadMain)(void *), void *data)
{
    int ret = -1;
    pthread_t threadId = 0;
    
    if(NULL == threadMain)
    {
        LOGGER_ERR("threadMain is NULL!");
        return E_OPERATION_ERROR_PARA;
    }
    
    if(NULL == data)
    {
         LOGGER_ERR("data is NULL!");
        return E_OPERATION_ERROR_PARA;
    }

    Base* ctxPlugin = static_cast<Base*>(data);
    if(!mThreadList.empty())
    {
        std::map<std::string,void*>::iterator it;
        it = mThreadList.find(ctxPlugin->getClassName());
        if(it != mThreadList.end())
        {
            ThreadData *psThreadData = (ThreadData *)(it->second);
             LOGGER_DBG("threads of this class exist. class name :%s",ctxPlugin->getClassName().c_str());
            if(E_THREAD_NUM_MAX == psThreadData->thread_num)
            {
                LOGGER_DBG("thread number of this class is maximum");
                return E_OPERATION_ERROR_THREAD_NUM_MAX;
            }
            
            ret = pthread_create(&threadId, NULL, threadMain, data);
    
            if(0 != ret)
            {
                LOGGER_DBG("thread create error!");
                return E_OPERATION_ERROR_THREAD_CREATE;
            }
    
            psThreadData->id[psThreadData->thread_num-1] = threadId;
            psThreadData->thread_num++;
            psThreadData->ctx = data;
            LOGGER_DBG("thread created , name : %s, threadId:%d",ctxPlugin->getClassName().c_str(),threadId);
            mThreadList.insert(std::pair<std::string, void *>(ctxPlugin->getClassName(), (void *)psThreadData));
        }
	    else
	    {
            LOGGER_DBG("threads of this class non-exist : ", ctxPlugin->getClassName().c_str());
	        ret = pthread_create(&threadId, NULL, threadMain, data);
	    
	        if(0 != ret)
	        {
                LOGGER_ERR("thread create error!");
	            return E_OPERATION_ERROR_THREAD_CREATE;
	        }
            LOGGER_DBG("thread created , name : %s,  threadId:%d",ctxPlugin->getClassName().c_str() ,threadId);
            ThreadData *psThreadData =new ThreadData();
            psThreadData->id[0] = threadId;
            psThreadData->thread_num = 1;
            psThreadData->ctx = data;
            mThreadList.insert(std::pair<std::string, void *>(ctxPlugin->getClassName(), (void *)psThreadData));
	    }
    }
    else
    {
         LOGGER_DBG("threads of this class non-exist : %s",ctxPlugin->getClassName().c_str());
        ret = pthread_create(&threadId, NULL, threadMain, data);
    
        if(0 != ret)
        {
            LOGGER_DBG("thread create error!");
            return E_OPERATION_ERROR_THREAD_CREATE;
        }
        LOGGER_DBG("thread created , name : %s, threadId: %d",ctxPlugin->getClassName().c_str(),threadId);
        ThreadData *psThreadData =new ThreadData();
        psThreadData->id[0] = threadId;
        psThreadData->thread_num = 1;
        psThreadData->ctx = data;
        mThreadList.insert(std::pair<std::string, void *>(ctxPlugin->getClassName(), (void *)psThreadData));
    }
    return E_OPERATION_ERROR_NONE;
}

//--------------------------------------------------------------------------
/**
 *  @brief   pause Thread by thread name.
 *
 *  @param [in] threadName        name of thread
 */
//--------------------------------------------------------------------------
t_int ThreadManager::pauseThread(std::string &threadName)
{
    return E_OPERATION_ERROR_NONE;
}

//--------------------------------------------------------------------------
/**
 *  @brief   resume Thread by thread name.
 *
 *  @param [in] threadName        name of thread
 */
//--------------------------------------------------------------------------
t_int ThreadManager::resumeThread(std::string &threadName)
{
    return E_OPERATION_ERROR_NONE;
}

//--------------------------------------------------------------------------
/**
 *  @brief   terminate Thread by thread name.
 *
 *  @param [in] threadName        name of thread
 */
//--------------------------------------------------------------------------
t_int ThreadManager::terminateThread(std::string &threadName)
{
    return E_OPERATION_ERROR_NONE;
}

//--------------------------------------------------------------------------
/**
 *  @brief   onThreadTerminate
 *
 *  @param [in]  pluginName   plugin name
 *  @param [out] threadId    id of this thread
 */
//--------------------------------------------------------------------------
t_int ThreadManager::onThreadTerminate(std::string &pluginName, pthread_t threadId)
{
    if("" == pluginName || 0 == threadId)
    {
        return E_OPERATION_ERROR_PARA;
    }
    
    if(!mThreadList.empty())
    {
        std::map<std::string,void*>::iterator it;
        it = mThreadList.find(pluginName);
        if(it != mThreadList.end())
        {
            ThreadData *psThreadData = (ThreadData *)(it->second);
            if(0 == psThreadData->thread_num)
            {
                return E_OPERATION_ERROR_NONE;
            }
            
            //find and clean thread id info
            t_int i = 0;
            for(i = 0; i < psThreadData->thread_num; i++)
            {
                if(threadId == psThreadData->id[i])
                {
                    psThreadData->id[i] = 0;
                    psThreadData->thread_num--;
                    if(0 >= psThreadData->thread_num)
                        psThreadData->thread_num = 0;
                    break;
                }
            }
        }
    }
    else
    {
    }
    return E_OPERATION_ERROR_NONE;
}
