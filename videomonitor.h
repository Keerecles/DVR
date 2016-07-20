#ifndef VIDEOMONITOR_H
#define VIDEOMONITOR_H



class VideoMonitor  
{     
public:

    static VideoMonitor* mInstance;
    GMutex mMutex;
    GAsyncQueue *p_queue;
    std::string mClassName;

    //get instance of VideoMonitor
    static VideoMonitor* getInstance();

    //default constructor
    VideoMonitor();

    //destructor
    ~VideoMonitor();

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


    //API for changing the pipeline state.(Pipeline state: PIPELINE_STATE_PLAY PIPELINE_STATE_READY PIPELINE_STATE_STOP PIPELINE_STATE_PAUSE PIPELINE_STATE_NULL)
    void    setPipelineState(GstElement *pipeline,uint state);

    uint    getPipelineState(GstElement *pipeline);
    
    //API for changing the pipeline mode.(Pipeline mode: PIPELINE_MODE_FOREGROUND PIPELINE_MODE_BACKGROUND)
    void    setPipelineMode(GstElement *pipeline,uint mode); 

    uint    getPipelineMode(GstElement *pipeline);       

    //API for FSM to set the work mode. (Work mode: PIPELINE_CHANGED_MODE_EMERGENCY PIPELINE_CHANGED_MODE_DAILY PIPELINE_CHANGED_MODE_REMOTE PIPELINE_CHANGED_MODE_LOWPOWER PIPELINE_CHANGED_MODE_RECORD PIPELINE_CHANGED_MODE_CAPTURE)
    void    setWorkMode(GstElement *pipeline,uint mode,Gerror error);    

    uint    getWorkMode(GstElement *pipeline);

    //API for sharing the video seg or snapshoot to the cloud
    void    upload(string path,string filename);                         

    //API for recording a video segment
    void    record(uint time);                                            
    
    //API for taking a snapshoot
    void    capture(); 

private:

    GMainLoop *mPluginGMainLoop;     /* GLib's Main Loop */
    GMainContext *mContext;         /* GLib context used to run the main loop */
    guint mSourceid;                /* To control the GSource */
    Engine *ctxEngine;
    GSource *busSource;

    // pthread_t mGstMainThread;
    uint mNativeWindow;
    uint mMsgQueue;
    uint mCurrentPipelineMode;
    uint mCurrentWorkMode;

    GstElement *mPipeline, *mVideoSrc;
    GstElement *mTee;
    GstElement *mQueue1,*mQueue2,*mQueue3,*mQueue4,*mQueue5,*mQueue6,*mQueue7;
    GstElement *mDailyMonitorSink;
    GstElement *mEmergencySink;
    GstElement *mHIMSink;
    GstElement *mVideoSegRecordSink;
    GstElement *mSnapshootSink,*mPicFormat;
    GstElement *mUdpPhoneSink;
    GstElement *mUdpCloudSink;
    
    
    GstPadTemplate *tee_src_pad_template;
    GstPad *mTeePad1,*mTeePad2,*mTeePad3,*mTeePad4,*mTeePad5,*mTeePad6,*mTeePad7,;
    GstPad *mQueuePad1,*mQueuePad2,*mQueuePad3,*mQueuePad4,*mQueuePad5,*mQueuePad6,*mQueuePad7;
   
    GstVideoInfo mInfo;
    GstCaps *mVideoCaps;
   
    GstBus* mGstBus;
    Gerror  mError;
    
    //the main gst loop
    static void * gstMainThread(void *data);
    
    //Store the snapshoot to SD Card
    void    storeImage(string path,string filename);                     
    
    //store the video segment to SD Card
    void    storeVideoSeg(string path,string filename,uint time);        
    
    //up load the image to cloud
    void    uploadImage(string path,string filename);  

    //up load the video to cloud
    void    uploadVideo(string path,string filename);        

    //Notify UI of the changes of the pipeline state
    void    pipelineStateChangedCallBack(GstBus *bus, GstMessage *msg);                              

    //Notify UI of the changes of the pipeline mode
    void    pipelineModeChangedCallBack();

    //Notify UI of the changes of the work mode    
    void    workModeChangedCallBack();    

    //Set the bus and signal of the gst pipeline
    void    setPipelineBusandSignal();                                   
    
    //Get the native window from qml
    void    getNativeWindow();                                           

    //Create the pipeline when the vehicle launch
    void    createOriginPipeline(); 
    
    //Call back function for processing the message queue
    void    receiveMsgFromFSMCallBack();                 

    //Process the message                     
    void    msgProcess(void*  msg);                             

    //Retrieve errors from the bus 
    void    gstErrorCallBack();

    //Logging                                              
    void    logout(); 

    //Clear the resource
    void    finalize();
};

#endif // VIDEOMONITOR_H
