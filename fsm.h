#ifndef FSM
#define FSM

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
    FSM();

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

private:

    GMainLoop *mPluginGMainLoop;     /* GLib's Main Loop */
    GMainContext *mContext;          /* GLib context used to run the main loop */
    Engine *ctxEngine;

    uint mCurrentPipelineMode;
    uint mCurrentWorkMode;

    uint mCurrentState;
    uint mNextState;
    
};




#endif // FSM

