#ifndef __THREAD_MANAGER_H__
#define __THREAD_MANAGER_H__

#include <string>
#include <map>
#include <pthread.h>
#include "base.h"
#include "commonType.h"
#include "log.h"
class ThreadManager : public Base
{
    
public:
    static ThreadManager* getInstance();
    ThreadManager();
    ~ThreadManager();

    t_int getProperties(std::string &key, std::string &value);

    t_int setProperties(std::string &key, std::string &value);

    //start a new thread with function threadMain.
    t_int startThread(void *(threadMain)(void *), void *data);
    
    t_int pauseThread(std::string &threadName);
    
    t_int resumeThread(std::string &threadName);
    
    t_int terminateThread(std::string &threadName);

    t_int onThreadTerminate(std::string &pluginName, pthread_t thread_id);
private:
    static ThreadManager *mInstance;
    //thread container for managing threads, key is thread name
    std::map<std::string , void *> mThreadList;
};

#endif // __THREAD_MANAGER_H__
