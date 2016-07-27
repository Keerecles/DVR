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
#include "commonType.h"

#include "cJSON.h"
#include "threadManager.h"
#include "message.h"
class Engine;

class VideoMonitor : public Base
{     
public:
    static VideoMonitor* getInstance();     //get instance of VideoMonitor
    VideoMonitor();                         //default constructor
    ~VideoMonitor();                        //destructor
    void Init(void *data);                  //plugin initialize
    static void * Run(void *data);                 //run plugin
    void handleMsg(void *msg,void *data); //Process the message      
    void startNewThread(void *(threadFunc)(void *),void *data); //create a thread(add a msg of create thread, to list of <engine>, create thread at <engine>)
    void sendMessage(void *msg,void *data);

    typedef void (VideoMonitor::*func)(void *msg, void *data);
    static const func videoMonitorAPI[];

private:
    static void * gstMainThread(void *data);  //the main gst loop
    void    getNativeWindow();                //Get the native window from qml                               
    void    createOriginPipeline();           //Create the pipeline when the vehicle launch
    
    void    storeImage(string path,string filename);                        //Store the snapshoot to SD Card 
    void    storeVideoSeg(string path,string filename,uint time);          //store the video segment to SD Card
    void    uploadImage(string path,string filename);      //up load the image to cloud
    void    uploadVideo(string path,string filename);   //up load the video to cloud
    void    upload(string path,string filename);        //API for sharing the video seg or snapshoot to the cloud                       
    void    record(uint time);                          //API for recording a video segment                                 
    void    capture();                                  //API for taking a snapshoot

    void    setPipelineState(GstElement *pipeline,uint state);              //API for changing the pipeline state.(Pipeline state: PIPELINE_STATE_PLAY PIPELINE_STATE_READY PIPELINE_STATE_STOP PIPELINE_STATE_PAUSE PIPELINE_STATE_NULL)
    uint    getPipelineState(GstElement *pipeline);    
    void    setPipelineMode(GstElement *pipeline,uint mode);                //API for changing the pipeline mode.(Pipeline mode: PIPELINE_MODE_FOREGROUND PIPELINE_MODE_BACKGROUND)
    uint    getPipelineMode(GstElement *pipeline);       
    void    setWorkMode(GstElement *pipeline,uint mode,Gerror error);        //API for VideoMonitor to set the work mode. (Work mode: PIPELINE_CHANGED_MODE_EMERGENCY PIPELINE_CHANGED_MODE_DAILY PIPELINE_CHANGED_MODE_REMOTE PIPELINE_CHANGED_MODE_LOWPOWER PIPELINE_CHANGED_MODE_RECORD PIPELINE_CHANGED_MODE_CAPTURE)
    uint    getWorkMode(GstElement *pipeline);
    
    void    pipelineStateChangedCallBack(GstBus *bus, GstMessage *msg);      //Notify UI of the changes of the pipeline state                            
    void    pipelineModeChangedCallBack();                                   //Notify UI of the changes of the pipeline mode
    void    workModeChangedCallBack();                                       //Notify UI of the changes of the work mode
    void    setPipelineBusandSignal();                                       //Set the bus and signal of the gst pipeline                                  
    void    gstErrorCallBack();                                              //Retrieve errors from the bus               
    
    void    logout();   //Logging   
    void    finalize(); //Clear the resource

    void modeChangeDailyToEmergency();
    void modeChangeDailyToCapture();
    void modeChangeDailyToRecord();
    void modeChangeDailyToRemote();
    void modeChangeRemoteToEmergency();
    void modeChangeRemoteToCapture();
    void modeChangeRemoteToRecord();
    void modeChangeRemoteToDaily();
    void modeChangeRecordToEmergency();
    void modeChangeRecordToCapture();
    void modeChangeRecordToDaily();
    void modeChangeCaptureToEmergency();
    void modeChangeCaptureToRecord();
    void modeChangeCaptureToDaily();

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
    GstPad *mTeePad1,*mTeePad2,*mTeePad3,*mTeePad4,*mTeePad5,*mTeePad6,*mTeePad7,;
    GstPad *mQueuePad1,*mQueuePad2,*mQueuePad3,*mQueuePad4,*mQueuePad5,*mQueuePad6,*mQueuePad7;

    GstBus* mGstBus;
    Gerror  mError;
};

#endif // VIDEOMONITOR_H
