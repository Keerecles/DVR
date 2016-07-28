#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <map>
#include "commonType.h"
#include "base.h"
#include "pluginSocket.h"
#include "cJSON.h"
#include "log.h"

/*ENUM type for DVR*/
enum EVENT_CODE {
    PIPELINE_SWITCH_MODE_DAILY,
    PIPELINE_SWITCH_MODE_EMERGENCY,
    PIPELINE_SWITCH_MODE_REMOTE,
    PIPELINE_SWITCH_MODE_LOWPOWER,
    PIPELINE_SWITCH_MODE_RECORD,
    PIPELINE_SWITCH_MODE_CAPTURE,
    PIPELINE_SWITCH_MODE__MAX_NUMBER,
};

enum WorkMode{
    PIPELINE_WORK_MODE_DAILY,
    PIPELINE_WORK_MODE_EMERGENCY,
    PIPELINE_WORK_MODE_REMOTE,
    PIPELINE_WORK_MODE_LOWPOWER,
    PIPELINE_WORK_MODE_RECORD,
    PIPELINE_WORK_MODE_CAPTURE,
    PIPELINE_WORK_MODE_ANY,
};

enum WORK_MODE_CHANGE_CODE{
    PIPELINE_WORKMODE_DAILY_TO_EMERGENCY,
    PIPELINE_WORKMODE_DAILY_TO_CAPTURE,
    PIPELINE_WORKMODE_DAILY_TO_RECORD,
    PIPELINE_WORKMODE_DAILY_TO_REMOTE,
    PIPELINE_WORKMODE_REMOTE_TO_EMERGENCY,
    PIPELINE_WORKMODE_REMOTE_TO_CAPTURE,
    PIPELINE_WORKMODE_REMOTE_TO_RECORD,
    PIPELINE_WORKMODE_REMOTE_TO_DAILY,
    PIPELINE_WORKMODE_RECORD_TO_EMERGENCY,
    PIPELINE_WORKMODE_RECORD_TO_CAPRURE,
    PIPELINE_WORKMODE_DRECORD_TO_DAILY,
    PIPELINE_WORKMODE_CAPTURE_TO_EMERGENCY,
    PIPELINE_WORKMODE_CAPTURE_TO_RECORD,
    PIPELINE_WORKMODE_CAPTURE_TO_DAILY,
};

/*ENUM type for DVR end*/

enum OPERATION_CODE_ENGINE {
    E_ENGINE_FUNCTION_START_NEW_THREAD,
    E_ENGINE_FUNCTION_ON_THREAD_TERMINATE,
    E_ENGINE_FUNCTION_MAX_NUMBER,
};

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
