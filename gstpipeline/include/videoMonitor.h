#ifndef VIDEOMONITOR_H
#define VIDEOMONITOR_H

#include <pthread.h>
#include <map>
#include <iostream>
//#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string>
#include <list>
//#include <sstrseam>
#include "base.h"
#include "dataType.h"
#include "commonType.h"
#include "message.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/app/gstappsink.h>
//#include <gst/interfaces/xoverlay.h>
#include <gst/video/video.h>

class Engine;

class VideoMonitor : public Base
{     
public:
    static VideoMonitor* getInstance();     //get instance of VideoMonitor
    VideoMonitor();                         //default constructor
    ~VideoMonitor();                        //destructor
    void Init(void *data);                  //plugin initialize
    static void * Run(void *arg);                 //run plugin
    void handleMsg(void *msg,void *data); //Process the message      
    void startNewThread(void *(threadFunc)(void *),void *data); //create a thread(add a msg of create thread, to list of <engine>, create thread at <engine>)
    void sendMessage(void *msg,void *data);

    typedef t_int (VideoMonitor::*func)(void *msg, void *data);
    static const func videoMonitorAPI[];

private:
    static void * gstMainThread(void *data);  //the main gst loop
    void    getNativeWindow();                //Get the native window from qml                               
    void    createOriginPipeline();           //Create the pipeline when the vehicle launch

    static void    pipelineStateChangedCallBack(GstBus *bus, GstMessage *msg);      //Notify UI of the changes of the pipeline state                            
    void    pipelineModeChangedCallBack();                                   //Notify UI of the changes of the pipeline mode
    void    workModeChangedCallBack();                                       //Notify UI of the changes of the work mode
    void    setPipelineBusandSignal();                                       //Set the bus and signal of the gst pipeline                                  
    static void    gstErrorCallBack(GstBus *bus, GstMessage *msg);                                              //Retrieve errors from the bus               
    
    void    logout();   //Logging   
    void    finalize(); //Clear the resource

    t_int modeChangeDailyToEmergency(void *msg, void *data);
    t_int modeChangeDailyToCapture(void *msg, void *data);
    t_int modeChangeDailyToRecord(void *msg, void *data);
    t_int modeChangeDailyToRemote(void *msg, void *data);
    t_int modeChangeRemoteToEmergency(void *msg, void *data);
    t_int modeChangeRemoteToCapture(void *msg, void *data);
    t_int modeChangeRemoteToRecord(void *msg, void *data);
    t_int modeChangeRemoteToDaily(void *msg, void *data);
    t_int modeChangeRecordToEmergency(void *msg, void *data);
    t_int modeChangeRecordToCapture(void *msg, void *data);
    t_int modeChangeRecordToDaily(void *msg, void *data);
    t_int modeChangeCaptureToEmergency(void *msg, void *data);
    t_int modeChangeCaptureToRecord(void *msg, void *data);
    t_int modeChangeCaptureToDaily(void *msg, void *data);

private:
    static VideoMonitor* mInstance;
    Engine *ctxEngine;

    /*property own by VideoMonitor*/
    GMainLoop *mPluginGMainLoop;     /* GLib's Main Loop */
    GMainContext *mContext;         /* GLib context used to run the main loop */
    
    uint mNativeWindow;
    uint mCurrentPipelineMode;
    WorkMode mCurrentWorkMode;

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
    GstPad *mTeePad1,*mTeePad2,*mTeePad3,*mTeePad4,*mTeePad5,*mTeePad6,*mTeePad7;
    GstPad *mQueuePad1,*mQueuePad2,*mQueuePad3,*mQueuePad4,*mQueuePad5,*mQueuePad6,*mQueuePad7;

    GstBus* mGstBus;
    GError*  mError;
};

#endif // VIDEOMONITOR_H
