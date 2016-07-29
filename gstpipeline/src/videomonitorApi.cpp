#include "videomonitor.h"
#include "engine.h"

const VideoMonitor::func VideoMonitor::videoMonitorAPI[] = {
  &VideoMonitor::modeChangeDailyToEmergency,
  &VideoMonitor::modeChangeDailyToCapture,
  &VideoMonitor::modeChangeDailyToRecord,
  &VideoMonitor::modeChangeDailyToRemote,
  &VideoMonitor::modeChangeRemoteToEmergency,
  &VideoMonitor::modeChangeRemoteToCapture,
  &VideoMonitor::modeChangeRemoteToRecord,
  &VideoMonitor::modeChangeRemoteToDaily,
  &VideoMonitor::modeChangeRecordToEmergency,
  &VideoMonitor::modeChangeRecordToCapture,
  &VideoMonitor::modeChangeRecordToDaily,
  &VideoMonitor::modeChangeCaptureToEmergency,
  &VideoMonitor::modeChangeCaptureToRecord,
  &VideoMonitor::modeChangeCaptureToDaily,
};

void *VideoMonitor::gstMainThread(void *data){
  	/* init the gstreamer*/
	gst_init(NULL,NULL);
	/* Create our own GLib Main Context and make it the default one */
  	mInstance->mContext = g_main_context_new ();
  	g_main_context_push_thread_default(mInstance->mContext);

  	/* Create the origin pipeline (Daily Mode) and play it*/
	mInstance->createOriginPipeline();
	/* Instruct the bus to emit signals for each received message, and connect to the interesting signals */
	mInstance->setPipelineBusandSignal();

	/*Check if all conditions are met to report GStreamer as initialized.*/
  	// gst_x_overlay_set_window_handle  getNativeWindow()



  	/*Play the init pipeline*/
	gst_element_set_state (mInstance->mPipeline, GST_STATE_PLAYING);




	
	/* Create a GLib Main Loop and set it to run */
  	mInstance->mPluginGMainLoop = g_main_loop_new (mInstance->mContext, FALSE);
  	g_main_loop_run (mInstance->mPluginGMainLoop);
  	g_main_loop_unref (mInstance->mPluginGMainLoop);
  	mInstance->mPluginGMainLoop = NULL;
	
  	/* Free resources */
  	g_main_context_pop_thread_default(mInstance->mContext);
  	g_main_context_unref (mInstance->mContext);
  	gst_element_set_state (mInstance->mPipeline, GST_STATE_NULL);
  	gst_object_unref (mInstance->mPipeline);
}

void VideoMonitor::createOriginPipeline(){

	/* Create the empty pipeline */
	  mPipeline = gst_pipeline_new ("DVR_WORK_PIPEPLINE");

	/* Create the elements */
    mVideoSrc = gst_element_factory_make ("videotextsrc", "mVideoSrc");
    mTee 	    = gst_element_factory_make ("tee", "Tee");
    mQueue1   = gst_element_factory_make ("queue", "mQueue1");
    mQueue2   = gst_element_factory_make ("queue", "mQueue2");
    mQueue3   = gst_element_factory_make ("queue", "mQueue3");
    mQueue4   = gst_element_factory_make ("queue", "mQueue4");
    mQueue5   = gst_element_factory_make ("queue", "mQueue5");
    mQueue6   = gst_element_factory_make ("queue", "mQueue6");
    mQueue7   = gst_element_factory_make ("queue", "mQueue7");

    /*Elements for DailyMonitor Pipeline*/
  	mDailyMonitorSink = gst_element_factory_make ("fakesink", "mDailyMonitorSink");
  	
  	/*Elements for EmergencyMonitor Pipeline*/
  	mEmergencySink    = gst_element_factory_make ("fakesink", "mEmergencySink");
  	
  	/*Elements for HMI Pipeline*/
  	mHIMSink = gst_element_factory_make ("fakesink", "mHIMSink");
  	
  	/*Elements for Video Segment record Pipeline*/
  	mVideoSegRecordSink = gst_element_factory_make ("fakesink", "mVideoSegRecordSink");
   	
	/*Elements for SnapShoot Pipeline*/
   	mSnapshootSink = gst_element_factory_make ("fakesink", "mSnapshootSink");
  	mPicFormat = gst_element_factory_make ("pngenc", "mPicFormat");


	/*Elements for Phone Link Pipeline*/
  	mUdpPhoneSink = gst_element_factory_make ("fakesink", "mUdpPhoneSink");
  	
	/*Elements for Cloud Pipeline*/
  	mUdpCloudSink = gst_element_factory_make ("fakesink", "mUdpCloudSink");
  	
	if (!mPipeline || !mVideoSrc || !mTee
		|| !mQueue1 || !mQueue2 || !mQueue3 || !mQueue4 || !mQueue5 || !mQueue6 || !mQueue7
	 	|| !mDailyMonitorSink
		|| !mEmergencySink
		|| !mHIMSink
		|| !mVideoSegRecordSink
		|| !mSnapshootSink || !mPicFormat
		|| !mUdpPhoneSink
		|| !mUdpCloudSink) { 
	  
	  g_printerr ("Elements could not be created\n");
      gst_object_unref (mPipeline);
	  return ;
	}
	
	/* Configure elements */
	// g_object_set (mVideoSrc, "device", "/dev/video1", NULL);
 //  	g_object_set (mDailyMonitorSink, "location", "/mnt/sdcard/DVRDATA/DAILY/", NULL);
 //  	g_object_set (mEmergencySink, "location", "/mnt/sdcard/DVRDATA/EMERGENCY/", NULL);
 //  	g_object_set (mHIMSink, "width",1280,"height",720, NULL);
 //  	g_object_set (mVideoSegRecordSink, "location", "/mnt/sdcard/DVRDATA/Vod", NULL);
 //  	g_object_set (mSnapshootSink, "location", "/mnt/sdcard/DVRDATA/Pic", NULL);
 //  	g_object_set (mUdpPhoneSink, "host", "127.0.0.1", "port", 8004, NULL);  
 //  	g_object_set (mUdpCloudSink, "host", "127.0.0.1", "port", 8004, NULL);	

  	/* Link all elements that can be automatically linked because they have "Always" pads */
  	gst_bin_add_many (GST_BIN (mPipeline),mVideoSrc
										,mTee
										,mQueue1,mQueue2,mQueue3,mQueue4,mQueue5,mQueue6,mQueue7
										,mDailyMonitorSink
										,mEmergencySink
										,mHIMSink
										,mVideoSegRecordSink
										,mSnapshootSink,mPicFormat
										,mUdpPhoneSink
										,mUdpCloudSink
										,NULL);
	
	/* Link all elements that can be automatically linked because they have "Always" pads */
  	if (gst_element_link_many (mVideoSrc, mTee, NULL) != TRUE ||
      	gst_element_link_many (mQueue1, mDailyMonitorSink, NULL) != TRUE ||
      	gst_element_link_many (mQueue2, mEmergencySink, NULL) != TRUE ||
      	gst_element_link_many (mQueue3, mHIMSink, NULL) != TRUE ||
      	gst_element_link_many (mQueue4, mVideoSegRecordSink, NULL) != TRUE ||
      	gst_element_link_many (mQueue5, mPicFormat, mSnapshootSink, NULL) != TRUE ||
      	gst_element_link_many (mQueue6, mUdpPhoneSink, NULL) != TRUE ||
      	gst_element_link_many (mQueue7, mUdpCloudSink, NULL) != TRUE) {
    
      	g_printerr ("Elements could not be linked\n");
      	gst_object_unref (mPipeline);
      	return ;
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
		    gst_pad_link (mTeePad7, mQueuePad7) != GST_PAD_LINK_OK   
    	) {  
    	g_printerr ("Tee could not be linked.\n");  
  		gst_object_unref (mPipeline); 
  		gst_object_unref (mQueuePad1); 
		gst_object_unref (mQueuePad2); 
		gst_object_unref (mQueuePad3); 
		gst_object_unref (mQueuePad4); 
		gst_object_unref (mQueuePad5); 
		gst_object_unref (mQueuePad6);
		gst_object_unref (mQueuePad7);   
  		return ;  
	}  
	gst_object_unref (mQueuePad1); 
	gst_object_unref (mQueuePad2); 
	gst_object_unref (mQueuePad3); 
	gst_object_unref (mQueuePad4); 
	gst_object_unref (mQueuePad5); 
	gst_object_unref (mQueuePad6);
	gst_object_unref (mQueuePad7);   
	
	return ;
}

void VideoMonitor::pipelineStateChangedCallBack(GstBus *bus, GstMessage *msg){
	GstState old_state, new_state, pending_state;
  	gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
  	/* Only pay attention to messages coming from the pipeline, not its children */
  	if (GST_MESSAGE_SRC (msg) == GST_OBJECT (mInstance->mPipeline)) {
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

void VideoMonitor::gstErrorCallBack(GstBus *bus, GstMessage *msg){
	GError *err;
  	gchar *debug_info;
  	/* Print error details on the screen */
  	gst_message_parse_error (msg, &err, &debug_info);
	g_clear_error (&err);
	g_free (debug_info);
	g_main_loop_quit (mInstance->mPluginGMainLoop);
}                       

void VideoMonitor::logout(){

}

void VideoMonitor::finalize(){

}

/*Function to change the mode of pipeline*/
t_int VideoMonitor::modeChangeDailyToEmergency(void *msg, void *data){
    
    if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeDailyToCapture(void *msg, void *data){

     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeDailyToRecord(void *msg, void *data){
   
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeDailyToRemote(void *msg, void *data){
    
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeRemoteToEmergency(void *msg, void *data){
    
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeRemoteToCapture(void *msg, void *data){
    
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeRemoteToRecord(void *msg, void *data){
   
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeRemoteToDaily(void *msg, void *data){
    
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeRecordToEmergency(void *msg, void *data){
    
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeRecordToCapture(void *msg, void *data){
    
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeRecordToDaily(void *msg, void *data){
   
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeCaptureToEmergency(void *msg, void *data){
    
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeCaptureToRecord(void *msg, void *data){
    
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeCaptureToDaily(void *msg, void *data){
    
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
