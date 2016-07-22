#ifndef FSM_H
#define FSM_H



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

class FSM
{
public:

    static FSM* mInstance;
    GMutex mMutex;
    GAsyncQueue *p_queue;
    std::string mClassName;

    //get instance of FSM
    static FSM* getInstance();

    //default constructor
    FSM(WorkMode initworkmode);

    //destructor
    ~FSM();

    //plugin initialize
    void pluginInit(void *data);

    //run plugin
    void * pluginRun(void *data);

    //pause plugin
    void pluginPause();

    //Resume plugin
    void pluginResume();

    //stop plugin
    void pluginStop();

    std::string getClassName();

    void startNewThread(void *thread);

    //get plugin's queue
    GAsyncQueue *getQueue();

    void sendMessage(void *obj, void *msg);
    
    void handleMsg(void *msg,void *data);

    
private:

    GMainLoop *mPluginGMainLoop;     /* GLib's Main Loop */
    GMainContext *mContext;          /* GLib context used to run the main loop */
    Engine *ctxEngine;

    WorkMode mCurrentWorkMode;
    struct Transition
    {
      WorkMode modeFrom;
      WorkMode modeTo;
      EVENT_CODE event;
      void (*onTransition)();
    };
    list<Transition> mTransitionList;
    void registerTransition();

    void addTransition(WorkMode modeFrom, WorkMode modeTo, EVENT_CODE event,
                      void (*onTransition)());

    void trigger(EVENT_CODE event);

    static Transition createTransition(WorkMode modeFrom, WorkMode modeTo,
                                      EVENT_CODE event, void (*onTransition)());

    void FSM::modeChangeDailyToEmergency();
    void FSM::modeChangeDailyToCapture();
    void FSM::modeChangeDailyToRecord();
    void FSM::modeChangeDailyToRemote();
    void FSM::modeChangeRemoteToEmergency();
    void FSM::modeChangeRemoteToCapture();
    void FSM::modeChangeRemoteToRecord();
    void FSM::modeChangeRemoteToDaily();
    void FSM::modeChangeRecordToEmergency();
    void FSM::modeChangeRecordToCapture();
    void FSM::modeChangeRecordToDaily();
    void FSM::modeChangeCaptureToEmergency();
    void FSM::modeChangeCaptureToRecord();
    void FSM::modeChangeCaptureToDaily();
};

#endif // FSM
