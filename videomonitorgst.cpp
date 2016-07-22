#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <message.h>

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/app/gstappsink.h>
#include <gst/interfaces/xoverlay.h>
#include <gst/video/video.h>
#include "videomonitor.h"

const VideoMonitor::func VideoMonitor::videoMonitorAPI[] = {
	&VideoMonitor::modeChangeDailyToEmergency()£¬
    &VideoMonitor::modeChangeDailyToCapture()£¬
    &VideoMonitor::modeChangeDailyToRecord()£¬
    &VideoMonitor::modeChangeDailyToRemote()£¬
    &VideoMonitor::modeChangeRemoteToEmergency()£¬
    &VideoMonitor::modeChangeRemoteToCapture()£¬
    &VideoMonitor::modeChangeRemoteToRecord()£¬
    &VideoMonitor::modeChangeRemoteToDaily()£¬
    &VideoMonitor::modeChangeRecordToEmergency()£¬
    &VideoMonitor::modeChangeRecordToCapture()£¬
    &VideoMonitor::modeChangeRecordToDaily()£¬
    &VideoMonitor::modeChangeCaptureToEmergency()£¬
    &VideoMonitor::modeChangeCaptureToRecord()£¬
    &VideoMonitor::modeChangeCaptureToDaily()£¬
};

static void *VideoMonitor::gstMainThread(void *usrdata){
	/* init the gstreamer*/
	gst_init(NULL,NULL);
	/* Create our own GLib Main Context and make it the default one */
  	mContext = g_main_context_new ();
  	g_main_context_push_thread_default(mContext);

  	/* Create the origin pipeline (Daily Mode) and play it*/
	createOriginPipeline();
	/* Instruct the bus to emit signals for each received message, and connect to the interesting signals */
	setPipelineBusandSignal();

	/*Check if all conditions are met to report GStreamer as initialized.*/
    // gst_x_overlay_set_window_handle  getNativeWindow()



	/* Create a GLib Main Loop and set it to run */
    mPluginGMainLoop = g_main_loop_new (mContext, FALSE);
    g_main_loop_run (mPluginGMainLoop);
    g_main_loop_unref (mPluginGMainLoop);
    mPluginGMainLoop = NULL;
  
    /* Free resources */
    g_main_context_pop_thread_default(mContext);
    g_main_context_unref (mContext);
    gst_element_set_state (mPipeline, GST_STATE_NULL);
    gst_object_unref (mPipeline);
}

void VideoMonitor::createOriginPipeline(){

	/* Create the empty pipeline */
	mPipeline = gst_pipeline_new ("DVR_WORK_PIPEPLINE");

	/* Create the elements */
    mVideoSrc = gst_element_factory_make ("videotextsrc", "mVideoSrc");
    mTee 	  = gst_element_factory_make ("tee", "Tee");
    mQueue1   = gst_element_factory_make ("queue", "mQueue1");
    mQueue2   = gst_element_factory_make ("queue", "mQueue2");
    mQueue3   = gst_element_factory_make ("queue", "mQueue3");
    mQueue4   = gst_element_factory_make ("queue", "mQueue4");
    mQueue5   = gst_element_factory_make ("queue", "mQueue5");
    mQueue6   = gst_element_factory_make ("queue", "mQueue6");
    mQueue7   = gst_element_factory_make ("queue", "mQueue7");

    /*Elements for DailyMonitor Pipeline*/
  	mDailyMonitorSink = gst_element_factory_make ("filesink", "mDailyMonitorSink");
  	
  	/*Elements for EmergencyMonitor Pipeline*/
  	mEmergencySink    = gst_element_factory_make ("filesink", "mEmergencySink");
  	
  	/*Elements for HMI Pipeline*/
  	mHIMSink = gst_element_factory_make ("autovideosink", "mHIMSink");
  	
  	/*Elements for Video Segment record Pipeline*/
  	mVideoSegRecordSink = gst_element_factory_make ("fakesink", "mVideoSegRecordSink");
   	
	/*Elements for SnapShoot Pipeline*/
   	mSnapshootSink = gst_element_factory_make ("filesink", "mSnapshootSink");
  	mPicEnc = gst_element_factory_make ("pngenc", "mPicEnc");


	/*Elements for Phone Link Pipeline*/
  	mUdpPhoneSink = gst_element_factory_make ("udpsink", "mUdpPhoneSink");
  	
	/*Elements for Cloud Pipeline*/
  	mUdpCloudSink = gst_element_factory_make ("udpsink", "mUdpCloudSink");
  	
	if (!mPipeline || !mVideoSrc || !mTee
		|| !mQueue1 || !mQueue2 || !mQueue3 || !mQueue4 || !mQueue5 || !mQueue6 || !mQueue7
	 	|| !mDailyMonitorSink
		|| !mEmergencySink
		|| !mHIMSink
		|| !mVideoSegRecordSink
		|| !mSnapshootSink || !mPicEnc
		|| !mUdpPhoneSink
		|| !mUdpCloudSink) { 
	  
	  g_printerr ("Elements could not be created\n");
      gst_object_unref (mPipeline);
	  return NULL;
	}
	
	/* Configure elements */
	g_object_set (mVideoSrc, "device", "/dev/video1", NULL);
  	g_object_set (mDailyMonitorSink, "location", "/mnt/sdcard/DVRDATA/DAILY/", NULL);
  	g_object_set (mEmergencySink, "location", "/mnt/sdcard/DVRDATA/EMERGENCY/", NULL);
  	g_object_set (mHIMSink, "width",1280,"height",720, NULL);
  	g_object_set (mVideoSegRecordSink, "location", "/mnt/sdcard/DVRDATA/Vod", NULL);
  	g_object_set (mSnapshootSink, "location", "/mnt/sdcard/DVRDATA/Pic", NULL);
  	g_object_set (mUdpPhoneSink, "host", "127.0.0.1", "port", 8004, NULL);  
  	g_object_set (mUdpCloudSink, "host", "127.0.0.1", "port", 8004, NULL);	

  	/* Link all elements that can be automatically linked because they have "Always" pads */
  	gst_bin_add_many (GST_BIN (pipeline),mVideoSrc
										,mTee
										,mQueue1,mQueue2,mQueue3,mQueue4,mQueue5,mQueue6,,mQueue7
										,mDailyMonitorSink
										,mEmergencySink
										,mHIMSink
										,mVideoSegRecordSink
										,mSnapshootSink,mPicEnc
										,mUdpPhoneSink
										,mUdpCloudSink
										,NULL);
	
	/* Link all elements that can be automatically linked because they have "Always" pads */
  	if (gst_element_link_many (mVideoSrc, mTee, NULL) != TRUE ||
      	gst_element_link_many (mQueue1, mDailyMonitorSink, NULL) != TRUE ||
      	gst_element_link_many (mQueue2, mEmergencySink, NULL) != TRUE ||
      	gst_element_link_many (mQueue3, mHIMSink, NULL) != TRUE ||
      	gst_element_link_many (mQueue4, mVideoSegRecordSink, NULL) != TRUE ||
      	gst_element_link_many (mQueue5, mPicEnc, mSnapshootSink, NULL) != TRUE ||
      	gst_element_link_many (mQueue6, mUdpPhoneSink, NULL) != TRUE ||
      	gst_element_link_many (mQueue7, mUdpCloudSink, NULL) != TRUE) {
    
      	g_printerr ("Elements could not be linked\n");
      	gst_object_unref (mPipeline);
      	return NULL;
  	}
   
    /*Aquire the "Request" pads */
    tee_src_pad_template = gst_element_class_get_pad_template (GST_ELEMENT_GET_CLASS (mTee), "src%d");      
    mTeePad1 = gst_element_request_pad (mTee, tee_src_pad_template, NULL, NULL);
    mQueuePad1 = gst_element_get_static_pad (mQueue1, "sink");
     
    mTeePad2 = gst_element_request_pad (mTee, tee_src_pad_template, NULL, NULL);
    mQueuePad2 = gst_element_get_static_pad (mQueue2, "sink");

    mTeePad3 = gst_element_request_pad (mTee, tee_src_pad_template, NULL, NULL);
    mQueuePad3 = gst_element_get_static_pad (mQueue3, "sink");

    mTeePad4 = gst_element_request_pad (mTee, tee_src_pad_template, NULL, NULL);
    mQueuePad4 = gst_element_get_static_pad (mQueue4, "sink");

    mTeePad5 = gst_element_request_pad (mTee, tee_src_pad_template, NULL, NULL);
    mQueuePad5 = gst_element_get_static_pad (mQueue5, "sink");

    mTeePad6 = gst_element_request_pad (mTee, tee_src_pad_template, NULL, NULL);
    mQueuePad6 = gst_element_get_static_pad (mQueue6, "sink");

    mTeePad7 = gst_element_request_pad (mTee, tee_src_pad_template, NULL, NULL);
    mQueuePad7 = gst_element_get_static_pad (mQueue7, "sink");

    if (	gst_pad_link (mTeePad1, mQueuePad1) != GST_PAD_LINK_OK ||
		    gst_pad_link (mTeePad2, mQueuePad2) != GST_PAD_LINK_OK ||
		    gst_pad_link (mTeePad3, mQueuePad3) != GST_PAD_LINK_OK ||
		    gst_pad_link (mTeePad4, mQueuePad4) != GST_PAD_LINK_OK ||
		    gst_pad_link (mTeePad5, mQueuePad5) != GST_PAD_LINK_OK ||
		    gst_pad_link (mTeePad6, mQueuePad6) != GST_PAD_LINK_OK ||
		    gst_pad_link (mTeePad7, mQueuePad7) != GST_PAD_LINK_OK ||  
    	) {  
    	g_printerr ("Tee could not be linked.\n");  
  		gst_object_unref (pipeline); 
  		gst_object_unref (mQueuePad1); 
		gst_object_unref (mQueuePad2); 
		gst_object_unref (mQueuePad3); 
		gst_object_unref (mQueuePad4); 
		gst_object_unref (mQueuePad5); 
		gst_object_unref (mQueuePad6);
		gst_object_unref (mQueuePad7);   
  		return NULL;  
	}  
	gst_object_unref (mQueuePad1); 
	gst_object_unref (mQueuePad2); 
	gst_object_unref (mQueuePad3); 
	gst_object_unref (mQueuePad4); 
	gst_object_unref (mQueuePad5); 
	gst_object_unref (mQueuePad6);
	gst_object_unref (mQueuePad7);   
	
	return NULL;
}

void VideoMonitor::setPipelineState(GstElement *pipeline,GstState state){
	GstStateChangeReturn gstStateChangeReturn;
	gstStateChangeReturn=gst_element_set_state(pipeline, state);
	if(gstStateChangeReturn == GST_STATE_CHANGE_FAILURE){
		//logout()
	}
}

GstState VideoMonitor::getPipelineState(GstElement *pipeline){
	//TBD
}

void VideoMonitor::setPipelineMode(GstElement *pipeline,uint mode){
	if(mode != mCurrentPipelineMode){
		if(mode == PIPELINE_MODE_FOREGROUND){
			mCurrentPipelineMode = mode;
			//change the fakesink to video-sink
		}
		if(mode == PIPELINE_MODE_FOREGROUND){
			mCurrentPipelineMode = mode;
			//change the video-sink to fakesink
		}
	}
}

uint VideoMonitor::getPipelineMode(GstElement *pipeline){
	return mCurrentPipelineMode;
}

void VideoMonitor::setWorkMode(GstElement *pipeline,uint mode){
	if(mode != mCurrentWorkMode){
		switch(mode){
			case PIPELINE_CHANGED_MODE_DAILY: 
				mCurrentWorkMode = mode;
				//change the sink of the pipeline to video-sink
				break;
			case PIPELINE_CHANGED_MODE_EMERGENCY:
				mCurrentWorkMode = mode;
				//change the sink of the pipeline to SpecialStore-sink
				break;
			case PIPELINE_CHANGED_MODE_REMOTE:
				mCurrentWorkMode = mode;
				//change the sink of the pipeline to udp-sink
				break;
			case PIPELINE_CHANGED_MODE_LOWPOWER:
				mCurrentWorkMode = mode;
				//shut down the DVR?
				break;
			case PIPELINE_CHANGED_MODE_RECORD:
				mCurrentWorkMode = mode;
				//change the sink of the pipeline to SpecialStore-sink
				break;
			case PIPELINE_CHANGED_MODE_CAPTURE:
				mCurrentWorkMode = mode;
				//change the sink of the pipeline to SpecialStore-sink
				break;
			default : {
				break;
			}
		}
	}
	return NULL;
}

uint VideoMonitor::getWorkMode(GstElement *pipeline){
	return mCurrentWorkMode;
}

void VideoMonitor::storeImage(string path,string filename){
	//change or help pipeline sink to store image
}

void VideoMonitor::storeVideoSeg(string path,string filename,uint time){
	//change or help pipeline sink to store video segment
}

void VideoMonitor::record(uint time){
	//change the pipeline sink to filesink to save the video segment
	// Refer to $GSTL imxv4l2src device=$DEVICE num-buffers=300 ! $INPUT_CAPS ! queue ! vpuenc ! $ $MUXER ! filesink location=output.$EXTENSION
}

void VideoMonitor::capture(){
	//change the pipeline sink to filesink to save the image
	// Refer to photobin = gst_parse_launch("v4lsrc name=src ! videoscale ! ffmpegcolorspace ! pngenc ! filesink name=photo_sink", &error);
}

void VideoMonitor::upload(string path,string filename,uint type){
	
	/*Invoke a funchtion to detect the video/image file*/
	
	if(type == UPLOAD_IMAGE){
		uploadImage(path,filename);
	}
	if(type == UPLOAD_VIDEO){
		uploadVideo(path,filename);
	}
	return NULL;
}         

void VideoMonitor::uploadImage(string path,string filename){
	//Send message to engin to invoke CloudConnect class instance to upload the image
}    

void VideoMonitor::uploadVideo(string path,string filename){
	//Send message to engin to invoke CloudConnect class instance to upload the video
}     

void VideoMonitor::pipelineStateChangedCallBack(GstBus *bus, GstMessage *msg){
	GstState old_state, new_state, pending_state;
  	gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
  	/* Only pay attention to messages coming from the pipeline, not its children */
  	if (GST_MESSAGE_SRC (msg) == GST_OBJECT (mPipeline)) {
    gchar *message = g_strdup_printf("State changed to %s", gst_element_state_get_name(new_state));
    /*Use of the gchar message*/
    g_free (message);
  }
}   

void VideoMonitor::workModeChangedCallBack(){

}   

//--------------------------------------------------------------------------
/**
 *  @brief  set the bus,message,error call back,etc of gstreamer pipeline
 */
//--------------------------------------------------------------------------
void VideoMonitor::setPipelineBusandSignal(){
	
	/* Instruct the bus to emit signals for each received message, and connect to the interesting signals */
	mGstBus = gst_element_get_bus (mPipeline);
	gst_bus_add_signal_watch (mGstBus);
  	g_signal_connect (G_OBJECT (mGstBus), "message::error", (GCallback)gstErrorCallBack, NULL);
	g_signal_connect (G_OBJECT (mGstBus), "message::state-changed", (GCallback)pipelineStateChangedCallBack, NULL);
	gst_object_unref (mGstBus);
}  

void VideoMonitor::getNativeWindow(){
	//aquire the native window from Qt view
}

void *VideoMonitor::receiveMsgFromFSM(void *  msg){
	
    void *ps_message = NULL;
    VideoMonitor *ctxVideoMonitor = static_cast<VideoMonitor *>(msg);
    while(TRUE)
    {
        ps_message = g_async_queue_pop(ctxVideoMonitor->p_queue);
        ctxVideoMonitor->msgProcess(ps_message);
    }
    return NULL;
}

void VideoMonitor::sendMessage(void *obj, void *msg){
	
	VideoMonitor *ctxVideoMonitor = static_cast<VideoMonitor *>(obj);
    g_async_queue_lock (ctxVideoMonitor->ctxEngine->p_queue);
    g_async_queue_push_unlocked (ctxVideoMonitor->ctxEngine->p_queue,msg);
    g_async_queue_unlock (ctxVideoMonitor->ctxEngine->p_queue);   
}

void VideoMonitor::gstErrorCallBack(GstBus *bus, GstMessage *msg){
	GError *err;
  	gchar *debug_info;
  	/* Print error details on the screen */
  	gst_message_parse_error (msg, &err, &debug_info);
	g_clear_error (&err);
	g_free (debug_info);
	g_main_loop_quit (mGstMainThread);
}                       

void VideoMonitor::logout(){

}

void VideoMonitor::finalize(){

}

/*Function to change the mode of pipeline*/
void FSM::modeChangeDailyToEmergency(){
    

    return NULL;
}
void FSM::modeChangeDailyToCapture(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_DAILY_TO_CAPTURE,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeDailyToRecord(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_DAILY_TO_RECORD,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeDailyToRemote(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_DAILY_TO_REMOTE,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeRemoteToEmergency(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_REMOTE_TO_EMERGENCY,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeRemoteToCapture(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_REMOTE_TO_CAPTURE,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeRemoteToRecord(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_REMOTE_TO_RECORD,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeRemoteToDaily(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_REMOTE_TO_DAILY,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeRecordToEmergency(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_RECORD_TO_EMERGENCY,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeRecordToCapture(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_RECORD_TO_CAPRURE,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeRecordToDaily(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_DRECORD_TO_DAILY,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeCaptureToEmergency(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_CAPTURE_TO_EMERGENCY,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeCaptureToRecord(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_CAPTURE_TO_RECORD,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
void FSM::modeChangeCaptureToDaily(){
    Message *ctxMsg = new Message();
    ctxMsg->origin = mClassName;
    ctxMsg->destination = "Engine";
    ctxMsg->opertation_id = PIPELINE_WORKMODE_CAPTURE_TO_DAILY,;
    ctxMsg->dataListVoid.push_back(mInstance);
    ctxMsg->funcList.push_back(thread);
    sendMessage(mInstance, ctxMsg);
    return NULL;
}
