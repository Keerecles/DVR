#include "videoMonitor.h"
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

void *VideoMonitor::gstMainThread(void *data)
{
    LOGGER_DBG("VideoMonitor::gstMainThread Start!");
	  /* Create our own GLib Main Context and make it the default one */
  	mInstance->mContext = g_main_context_new ();
  	g_main_context_push_thread_default(mInstance->mContext);
    /* init the gstreamer*/
    //gst_init(NULL,NULL);
    if(!gst_init_check (NULL,NULL,&mInstance->mError)){
      LOGGER_DBG("VideoMonitor::gstMainThread gst init failed!");
      return NULL;
    }
  	/* Create the origin pipeline (Daily Mode) and play it*/
	  mInstance->createOriginPipeline();
	  /* Instruct the bus to emit signals for each received message, and connect to the interesting signals */
	  //mInstance->setPipelineBusandSignal();

	  /*Check if all conditions are met to report GStreamer as initialized.*/
  	// gst_x_overlay_set_window_handle  getNativeWindow()



  	/*Play the init pipeline*/
    gst_element_set_state (mInstance->mPipeline, GST_STATE_PLAYING);
    usleep(10000);
    gst_element_set_state (mInstance->mPipeline, GST_STATE_PAUSED);
    gst_pad_unlink (mInstance->mTeePad1,mInstance->mBinPad1);
    //gst_element_set_state (mInstance->mPipeline, GST_STATE_READY);
	  gst_element_set_state (mInstance->mPipeline, GST_STATE_PLAYING);
    // sleep(1);
    // gst_element_set_state (mInstance->mPipeline, GST_STATE_PAUSED);
    // gst_pad_unlink (mInstance->mTeePad1,mInstance->mBinPad1);
    // gst_element_set_state (mInstance->mPipeline, GST_STATE_PLAYING);
    // sleep(3);
    // gst_element_set_state (mInstance->mPipeline, GST_STATE_PAUSED);
    // gst_pad_link (mInstance->mTeePad1,mInstance->mBinPad1);
    // // gst_object_unref (mInstance->mPipeline);
    // gst_element_set_state (mInstance->mPipeline, GST_STATE_READY);
    // gst_element_set_state (mInstance->mPipeline, GST_STATE_PLAYING);


    // sleep(5);
    // gst_element_set_state (mInstance->mPipeline, GST_STATE_NULL);
    // gst_pad_link (mInstance->mTeePad1,mInstance->mBinPad1);
    // gst_element_set_state (mInstance->mPipeline, GST_STATE_PLAYING);




	
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
    LOGGER_DBG("VideoMonitor::createOriginPipeline");
	  mPipeline = gst_pipeline_new ("DVR_WORK_PIPEPLINE");

	  /* Create the elements */
    mVideoSrc = gst_element_factory_make ("videotestsrc", "mVideoSrc");
    mTee 	    = gst_element_factory_make ("tee", "Tee");

    mBin1     = gst_bin_new ("mBin1");
    mBin2     = gst_bin_new ("mBin2");
    mBin3     = gst_bin_new ("mBin3");
    mBin4     = gst_bin_new ("mBin4");
    mBin5     = gst_bin_new ("mBin5");
    mBin6     = gst_bin_new ("mBin6");
    mBin7     = gst_bin_new ("mBin7");

    mQueue1   = gst_element_factory_make ("queue", "mQueue1");
    mQueue2   = gst_element_factory_make ("queue", "mQueue2");
    mQueue3   = gst_element_factory_make ("queue", "mQueue3");
    mQueue4   = gst_element_factory_make ("queue", "mQueue4");
    mQueue5   = gst_element_factory_make ("queue", "mQueue5");
    mQueue6   = gst_element_factory_make ("queue", "mQueue6");
    mQueue7   = gst_element_factory_make ("queue", "mQueue7");

    mTextOverlay1   = gst_element_factory_make ("textoverlay", "mTextOverlay1");
    mTextOverlay2   = gst_element_factory_make ("textoverlay", "mTextOverlay2");
    mTextOverlay3   = gst_element_factory_make ("textoverlay", "mTextOverlay3");
    mTextOverlay4   = gst_element_factory_make ("textoverlay", "mTextOverlay4");
    mTextOverlay5   = gst_element_factory_make ("textoverlay", "mTextOverlay5");
    mTextOverlay6   = gst_element_factory_make ("textoverlay", "mTextOverlay6");
    mTextOverlay7   = gst_element_factory_make ("textoverlay", "mTextOverlay7");


    /*Elements for DailyMonitor Pipeline*/
  	mDailyMonitorSink = gst_element_factory_make ("autovideosink", "mDailyMonitorSink");
  	
  	/*Elements for EmergencyMonitor Pipeline*/
  	mEmergencySink    = gst_element_factory_make ("autovideosink", "mEmergencySink");
  	
  	/*Elements for HMI Pipeline*/
  	mHIMSink = gst_element_factory_make ("autovideosink", "mHIMSink");
  	
  	/*Elements for Video Segment record Pipeline*/
  	mVideoSegRecordSink = gst_element_factory_make ("autovideosink", "mVideoSegRecordSink");
   	
	/*Elements for SnapShoot Pipeline*/
   	mSnapshootSink = gst_element_factory_make ("autovideosink", "mSnapshootSink");
  	mPicFormat = gst_element_factory_make ("pngenc", "mPicFormat");


	/*Elements for Phone Link Pipeline*/
  	mUdpPhoneSink = gst_element_factory_make ("autovideosink", "mUdpPhoneSink");
  	
	/*Elements for Cloud Pipeline*/
  	mUdpCloudSink = gst_element_factory_make ("autovideosink", "mUdpCloudSink");

    // mFakeSink     = gst_element_factory_make ("fakesink", "mFakeSink");
  	
  	if (!mPipeline || !mVideoSrc || !mTee
    		|| !mBin1 || !mBin2 || !mBin3 || !mBin4 || !mBin5 || !mBin6 || !mBin7
        || !mQueue1 || !mQueue2 || !mQueue3 || !mQueue4 || !mQueue5 || !mQueue6 || !mQueue7
        || !mTextOverlay1 || !mTextOverlay2 || !mTextOverlay3 || !mTextOverlay4 || !mTextOverlay5 || !mTextOverlay6 || !mTextOverlay7
    	 	|| !mDailyMonitorSink
    		|| !mEmergencySink
    		|| !mHIMSink
    		|| !mVideoSegRecordSink
    		|| !mSnapshootSink || !mPicFormat
    		|| !mUdpPhoneSink
    		|| !mUdpCloudSink) { 
      LOGGER_DBG("VideoMonitor::createOriginPipeline Elements could not be created");
      gst_object_unref (mPipeline);
  	  return ;
  	}
	  LOGGER_DBG("VideoMonitor::createOriginPipeline Elements have been created");
	
    /* Configure elements */
 //   g_object_set (mVideoSrc, "device", "/dev/video1", NULL);
 //  	g_object_set (mDailyMonitorSink, "location", "/mnt/sdcard/DVRDATA/DAILY/", NULL);
 //  	g_object_set (mEmergencySink, "location", "/mnt/sdcard/DVRDATA/EMERGENCY/", NULL);
 //  	g_object_set (mHIMSink, "width",1280,"height",720, NULL);
 //  	g_object_set (mVideoSegRecordSink, "location", "/mnt/sdcard/DVRDATA/Vod", NULL);
 //  	g_object_set (mSnapshootSink, "location", "/mnt/sdcard/DVRDATA/Pic", NULL);
 //  	g_object_set (mUdpPhoneSink, "host", "127.0.0.1", "port", 8004, NULL);  
 //  	g_object_set (mUdpCloudSink, "host", "127.0.0.1", "port", 8004, NULL);
    g_object_set (mTextOverlay1, "text", "Daily Mode", NULL);	
    g_object_set (mTextOverlay2, "text", "Emergency Mode",  NULL);
    g_object_set (mTextOverlay3, "text", "HMI Mode", NULL);
    g_object_set (mTextOverlay4, "text", "VideoSeg Mode", NULL);
    g_object_set (mTextOverlay5, "text", "Snapshoot Mode", NULL);
    g_object_set (mTextOverlay6, "text", "Remote Mode", NULL);
    g_object_set (mTextOverlay7, "text", "Cloud Mode", NULL);
  	/* Link all elements that can be automatically linked because they have "Always" pads */
    
  	gst_bin_add_many(GST_BIN (mBin1), mQueue1, mTextOverlay1, mDailyMonitorSink, NULL);
    gst_bin_add_many(GST_BIN (mBin2), mQueue2, mTextOverlay2, mEmergencySink, NULL);
    gst_bin_add_many(GST_BIN (mBin3), mQueue3, mTextOverlay3, mHIMSink, NULL);
    gst_bin_add_many(GST_BIN (mBin4), mQueue4, mTextOverlay4, mVideoSegRecordSink, NULL);
    gst_bin_add_many(GST_BIN (mBin5), mQueue5, mTextOverlay5, mSnapshootSink, NULL);
    gst_bin_add_many(GST_BIN (mBin6), mQueue6, mTextOverlay6, mUdpPhoneSink, NULL);
    gst_bin_add_many(GST_BIN (mBin7), mQueue7, mTextOverlay7, mUdpCloudSink, NULL);

    gst_bin_add_many(GST_BIN (mPipeline), mVideoSrc, mTee ,NULL);
    gst_bin_add_many(GST_BIN (mPipeline), mBin1, NULL);
    gst_bin_add_many(GST_BIN (mPipeline), mBin2, NULL);
    // gst_bin_add_many(GST_BIN (mPipeline), mBin3, NULL);
    // gst_bin_add_many(GST_BIN (mPipeline), mBin4, NULL);
    // gst_bin_add_many(GST_BIN (mPipeline), mBin5, NULL);
    // gst_bin_add_many(GST_BIN (mPipeline), mBin6,NULL);
    // gst_bin_add_many(GST_BIN (mPipeline), mBin7, NULL);

    // gst_bin_add_many(GST_BIN (mPipeline), mFakeSink, NULL);

   	/* Link all elements that can be automatically linked because they have "Always" pads */
  	
    if( gst_element_link_many (mVideoSrc, mTee, NULL) != TRUE ||
        gst_element_link_many (mQueue1, mTextOverlay1, mDailyMonitorSink, NULL) != TRUE ||
        gst_element_link_many (mQueue2, mTextOverlay2, mEmergencySink, NULL) != TRUE ||
        gst_element_link_many (mQueue3, mTextOverlay3, mHIMSink, NULL) != TRUE ||
        gst_element_link_many (mQueue4, mTextOverlay4, mVideoSegRecordSink, NULL) != TRUE ||
        gst_element_link_many (mQueue5, mTextOverlay5, mSnapshootSink, NULL) != TRUE ||
        gst_element_link_many (mQueue6, mTextOverlay6, mUdpPhoneSink, NULL) != TRUE ||
        gst_element_link_many (mQueue7, mTextOverlay7, mUdpCloudSink, NULL) != TRUE
      )
    {
        LOGGER_DBG ("Elements could not be linked\n");
        gst_object_unref (mPipeline);
        return ;
    }

    LOGGER_DBG("VideoMonitor::createOriginPipeline Elements have been linked");
    /*Aquire the "Request" pads */
    tee_src_pad_template = gst_element_class_get_pad_template (GST_ELEMENT_GET_CLASS (mTee), "src_%u");     

    mTeePad1    = gst_element_request_pad (mTee, tee_src_pad_template, NULL, NULL);
    mTeePad2    = gst_element_request_pad (mTee, tee_src_pad_template, NULL, NULL);
    // mTeePad3    = gst_element_request_pad (mTee, tee_src_pad_template, NULL, NULL);
    // mTeePad4    = gst_element_request_pad (mTee, tee_src_pad_template, NULL, NULL);
    // mTeePad5    = gst_element_request_pad (mTee, tee_src_pad_template, NULL, NULL);
    // mTeePad6    = gst_element_request_pad (mTee, tee_src_pad_template, NULL, NULL);
    // mTeePad7    = gst_element_request_pad (mTee, tee_src_pad_template, NULL, NULL);
    // if(!mTeePad1 || !mTeePad2 || !mTeePad3 || !mTeePad4 || !mTeePad5 || !mTeePad6 || !mTeePad7 ){
    //     LOGGER_DBG(" mTeePad could not be created");
    //     return ;
    // }
// mFakeSinkPad   = gst_element_get_static_pad (mFakeSink, "sink");
    mQueuePad1  = gst_element_get_static_pad (mQueue1, "sink");
    mQueuePad2  = gst_element_get_static_pad (mQueue2, "sink");
    mQueuePad3  = gst_element_get_static_pad (mQueue3, "sink");
    mQueuePad4  = gst_element_get_static_pad (mQueue4, "sink");
    mQueuePad5  = gst_element_get_static_pad (mQueue5, "sink");
    mQueuePad6  = gst_element_get_static_pad (mQueue6, "sink");
    mQueuePad7  = gst_element_get_static_pad (mQueue7, "sink");
    if(!mQueuePad1 || !mQueuePad2 || !mQueuePad3 || !mQueuePad4 || !mQueuePad5 || !mQueuePad6 || !mQueuePad7 ){
        LOGGER_DBG("mQueuePad could not be created");
        return ;
    }
    /*Create the Ghost pads for Bin*/
    mBinPad1    = gst_ghost_pad_new ("sink", mQueuePad1);
    mBinPad2    = gst_ghost_pad_new ("sink", mQueuePad2);
    mBinPad3    = gst_ghost_pad_new ("sink", mQueuePad3);
    mBinPad4    = gst_ghost_pad_new ("sink", mQueuePad4);
    mBinPad5    = gst_ghost_pad_new ("sink", mQueuePad5);
    mBinPad6    = gst_ghost_pad_new ("sink", mQueuePad6);
    mBinPad7    = gst_ghost_pad_new ("sink", mQueuePad7);
    if(!mBinPad1 || !mBinPad2 || !mBinPad3 || !mBinPad4 || !mBinPad5 || !mBinPad6 || !mBinPad7 ){
        LOGGER_DBG(" mBinPad could not be created");
        return ;
    }

    gst_element_add_pad (mBin1,mBinPad1);
    gst_element_add_pad (mBin2,mBinPad2);
    gst_element_add_pad (mBin3,mBinPad3);
    gst_element_add_pad (mBin4,mBinPad4);
    gst_element_add_pad (mBin5,mBinPad5);
    gst_element_add_pad (mBin6,mBinPad6);
    gst_element_add_pad (mBin7,mBinPad7);

    /*Link the tee and bins*/
    if(	gst_pad_link (mTeePad1, mBinPad1) != GST_PAD_LINK_OK ||
		    gst_pad_link (mTeePad2, mBinPad2) != GST_PAD_LINK_OK 
		    // gst_pad_link (mTeePad3, mBinPad3) != GST_PAD_LINK_OK ||
	 	   //  gst_pad_link (mTeePad4, mBinPad4) != GST_PAD_LINK_OK ||
		    // gst_pad_link (mTeePad5, mBinPad5) != GST_PAD_LINK_OK ||
  		  // gst_pad_link (mTeePad6, mBinPad6) != GST_PAD_LINK_OK ||
  		  // // gst_pad_link (mTeePad6, mFakeSinkPad) != GST_PAD_LINK_OK ||
      //   gst_pad_link (mTeePad7, mBinPad7) != GST_PAD_LINK_OK   
     	) 
  {  
    LOGGER_DBG ("Tee could not be linked.\n");  
  	gst_object_unref (mPipeline); 
		gst_object_unref (mBinPad1); 
		gst_object_unref (mBinPad2); 
		gst_object_unref (mBinPad3); 
		gst_object_unref (mBinPad4); 
		gst_object_unref (mBinPad5); 
		gst_object_unref (mBinPad6);
		gst_object_unref (mBinPad7);   
  		return ;  
	}  
     /* Free pads */
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
    gst_element_set_state (mPipeline, GST_STATE_NULL);


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