#ifndef __ENGINE_H__
#define __ENGINE_H__
#include <map>
#include "commonType.h"
#include "base.h"
#include "pluginSocket.h"
#include "videoMonitor.h"
#include "fsm.h"
#include "cJSON.h"
#include "log.h"
typedef int t_int;
typedef unsigned int t_uint;


class Engine : public Base
{
    
public:
    static Engine* getInstance();
    Engine();
    ~Engine();
    void Init(void *data);
    static void * Run(void *arg);//get msg from queue
    void handleMsg(void * msg,void *data);

private:
	static Engine *mInstance;//singleton pattern
    std::map<std::string , void* > mModuleList;//save objects
    void createModeObject(void *data);

    t_int startNewThread(void *(threadMain)(void *), void *data);
    t_int onThreadTerminate(std::string &pluginName, pthread_t thread_id);
};
#endif // __ENGINE_H__
