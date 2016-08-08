#include "videoMonitor.h"
#include "engine.h"

const VideoMonitor::func VideoMonitor::videoMonitorAPI[] = 
{
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
    if(!gst_init_check (NULL,NULL,&mInstance->mGstPipeline.mError))
    {
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
    gst_element_set_state (mInstance->mGstPipeline.mPipeline, GST_STATE_PLAYING);
    
    sleep(1);
    LOGGER_DBG("VideoMonitor::gstMainThread CHANCHE THE PIPELINE!");
    if(!gst_bin_add(GST_BIN (mInstance->mGstPipeline.mPipeline), mInstance->mGstPipeline.mBin2))
    {
      LOGGER_DBG("VideoMonitor::gstMainThread mGstPipeline.mBin2 failed to add!");
    }
    gst_pad_link (mInstance->mGstPipeline.mTeePad2,mInstance->mGstPipeline.mBinPad2);
    gst_element_set_state (mInstance->mGstPipeline.mPipeline, GST_STATE_READY);
    gst_element_set_state (mInstance->mGstPipeline.mPipeline, GST_STATE_PLAYING);
    LOGGER_DBG("VideoMonitor::gstMainThread CHANCHE THE PIPELINE END!");

    sleep(1);
    LOGGER_DBG("VideoMonitor::gstMainThread CHANCHE THE PIPELINE!");
    if(!gst_bin_add(GST_BIN (mInstance->mGstPipeline.mPipeline), mInstance->mGstPipeline.mBin3))
    {
      LOGGER_DBG("VideoMonitor::gstMainThread mGstPipeline.mBin2 failed to add!");
    }
    gst_pad_link (mInstance->mGstPipeline.mTeePad3,mInstance->mGstPipeline.mBinPad3);
    gst_element_set_state (mInstance->mGstPipeline.mPipeline, GST_STATE_READY);
    gst_element_set_state (mInstance->mGstPipeline.mPipeline, GST_STATE_PLAYING);
    LOGGER_DBG("VideoMonitor::gstMainThread CHANCHE THE PIPELINE END!");


    sleep(1);
    LOGGER_DBG("VideoMonitor::gstMainThread CHANCHE THE PIPELINE!");
  
    gst_element_set_state (mInstance->mGstPipeline.mPipeline, GST_STATE_READY);
    //gst_object_ref(mInstance->mGstPipeline.mBinPad2);
    gst_element_set_state (mInstance->mGstPipeline.mBin2, GST_STATE_NULL);
    if(!gst_pad_unlink (mInstance->mGstPipeline.mTeePad2,mInstance->mGstPipeline.mBinPad2))
    {
      LOGGER_DBG("VideoMonitor::gstMainThread mGstPipeline.mBinPad2 failed to unlink!");
    }
    if(!gst_bin_remove (GST_BIN(mInstance->mGstPipeline.mPipeline),mInstance->mGstPipeline.mBin2))
    {
      LOGGER_DBG("VideoMonitor::gstMainThread mGstPipeline.mBinPad2 failed to remove!");
    }

    gst_element_set_state (mInstance->mGstPipeline.mPipeline, GST_STATE_PAUSED);
    gst_element_set_state (mInstance->mGstPipeline.mPipeline, GST_STATE_PLAYING);
    LOGGER_DBG("VideoMonitor::gstMainThread CHANCHE THE PIPELINE END!");


	  /* Create a GLib Main Loop and set it to run */
  	mInstance->mPluginGMainLoop = g_main_loop_new (mInstance->mContext, FALSE);
  	g_main_loop_run (mInstance->mPluginGMainLoop);
  	g_main_loop_unref (mInstance->mPluginGMainLoop);
  	mInstance->mPluginGMainLoop = NULL;
	
  	/* Free resources */
  	g_main_context_pop_thread_default(mInstance->mContext);
  	g_main_context_unref (mInstance->mContext);
  	gst_element_set_state (mInstance->mGstPipeline.mPipeline, GST_STATE_NULL);
  	gst_object_unref (mInstance->mGstPipeline.mPipeline);
}

void VideoMonitor::createOriginPipeline()
{
	  /* Create the empty pipeline */
    LOGGER_DBG("VideoMonitor::createOriginPipeline");
	  mGstPipeline.mPipeline = gst_pipeline_new ("DVR_WORK_PIPEPLINE");

	  /* Create the elements */
    mGstPipeline.mVideoSrc = gst_element_factory_make ("videotestsrc", "mGstPipeline.mVideoSrc");
    mGstPipeline.mTee 	    = gst_element_factory_make ("tee", "Tee");

    mGstPipeline.mBin1     = gst_bin_new ("mGstPipeline.mBin1");
    mGstPipeline.mBin2     = gst_bin_new ("mGstPipeline.mBin2");
    mGstPipeline.mBin3     = gst_bin_new ("mGstPipeline.mBin3");
    mGstPipeline.mBin4     = gst_bin_new ("mGstPipeline.mBin4");
    mGstPipeline.mBin5     = gst_bin_new ("mGstPipeline.mBin5");
    mGstPipeline.mBin6     = gst_bin_new ("mGstPipeline.mBin6");
    mGstPipeline.mBin7     = gst_bin_new ("mGstPipeline.mBin7");

    mGstPipeline.mQueue1   = gst_element_factory_make ("queue", "mGstPipeline.mQueue1");
    mGstPipeline.mQueue2   = gst_element_factory_make ("queue", "mGstPipeline.mQueue2");
    mGstPipeline.mQueue3   = gst_element_factory_make ("queue", "mGstPipeline.mQueue3");
    mGstPipeline.mQueue4   = gst_element_factory_make ("queue", "mGstPipeline.mQueue4");
    mGstPipeline.mQueue5   = gst_element_factory_make ("queue", "mGstPipeline.mQueue5");
    mGstPipeline.mQueue6   = gst_element_factory_make ("queue", "mGstPipeline.mQueue6");
    mGstPipeline.mQueue7   = gst_element_factory_make ("queue", "mGstPipeline.mQueue7");

    mGstPipeline.mTextOverlay1   = gst_element_factory_make ("textoverlay", "mGstPipeline.mTextOverlay1");
    mGstPipeline.mTextOverlay2   = gst_element_factory_make ("textoverlay", "mGstPipeline.mTextOverlay2");
    mGstPipeline.mTextOverlay3   = gst_element_factory_make ("textoverlay", "mGstPipeline.mTextOverlay3");
    mGstPipeline.mTextOverlay4   = gst_element_factory_make ("textoverlay", "mGstPipeline.mTextOverlay4");
    mGstPipeline.mTextOverlay5   = gst_element_factory_make ("textoverlay", "mGstPipeline.mTextOverlay5");
    mGstPipeline.mTextOverlay6   = gst_element_factory_make ("textoverlay", "mGstPipeline.mTextOverlay6");
    mGstPipeline.mTextOverlay7   = gst_element_factory_make ("textoverlay", "mGstPipeline.mTextOverlay7");


    /*Elements for DailyMonitor Pipeline*/
  	mGstPipeline.mDailyMonitorSink = gst_element_factory_make ("autovideosink", "mDailyMonitorSink");
  	
  	/*Elements for EmergencyMonitor Pipeline*/
  	mGstPipeline.mEmergencySink    = gst_element_factory_make ("autovideosink", "mEmergencySink");
  	
  	/*Elements for HMI Pipeline*/
  	mGstPipeline.mHIMSink = gst_element_factory_make ("autovideosink", "mHIMSink");
  	
  	/*Elements for Video Segment record Pipeline*/
  	mGstPipeline.mVideoSegRecordSink = gst_element_factory_make ("autovideosink", "mVideoSegRecordSink");
   	
	/*Elements for SnapShoot Pipeline*/
   	mGstPipeline.mSnapshootSink = gst_element_factory_make ("autovideosink", "mSnapshootSink");
  	mGstPipeline.mPicFormat = gst_element_factory_make ("pngenc", "mPicFormat");


	/*Elements for Phone Link Pipeline*/
  	mGstPipeline.mUdpPhoneSink = gst_element_factory_make ("autovideosink", "mUdpPhoneSink");
  	
	/*Elements for Cloud Pipeline*/
  	mGstPipeline.mUdpCloudSink = gst_element_factory_make ("autovideosink", "mUdpCloudSink");

    // mFakeSink     = gst_element_factory_make ("fakesink", "mFakeSink");
  	
  	if (!mGstPipeline.mPipeline || !mGstPipeline.mVideoSrc || !mGstPipeline.mTee
    		|| !mGstPipeline.mBin1 || !mGstPipeline.mBin2 || !mGstPipeline.mBin3 || !mGstPipeline.mBin4 || !mGstPipeline.mBin5 || !mGstPipeline.mBin6 || !mGstPipeline.mBin7
        || !mGstPipeline.mQueue1 || !mGstPipeline.mQueue2 || !mGstPipeline.mQueue3 || !mGstPipeline.mQueue4 || !mGstPipeline.mQueue5 || !mGstPipeline.mQueue6 || !mGstPipeline.mQueue7
        || !mGstPipeline.mTextOverlay1 || !mGstPipeline.mTextOverlay2 || !mGstPipeline.mTextOverlay3 || !mGstPipeline.mTextOverlay4 || !mGstPipeline.mTextOverlay5 || !mGstPipeline.mTextOverlay6 || !mGstPipeline.mTextOverlay7
    	 	|| !mGstPipeline.mDailyMonitorSink
    		|| !mGstPipeline.mEmergencySink
    		|| !mGstPipeline.mHIMSink
    		|| !mGstPipeline.mVideoSegRecordSink
    		|| !mGstPipeline.mSnapshootSink || !mGstPipeline.mPicFormat
    		|| !mGstPipeline.mUdpPhoneSink
    		|| !mGstPipeline.mUdpCloudSink) 
    { 
      LOGGER_DBG("VideoMonitor::createOriginPipeline Elements could not be created");
      gst_object_unref (mGstPipeline.mPipeline);
  	  return ;
  	}
	  LOGGER_DBG("VideoMonitor::createOriginPipeline Elements have been created");
	
    /* Configure elements */
 //   g_object_set (mGstPipeline.mVideoSrc, "device", "/dev/video1", NULL);
 //  	g_object_set (mGstPipeline.mDailyMonitorSink, "location", "/mnt/sdcard/DVRDATA/DAILY/", NULL);
 //  	g_object_set (mGstPipeline.mEmergencySink, "location", "/mnt/sdcard/DVRDATA/EMERGENCY/", NULL);
 //  	g_object_set (mGstPipeline.mHIMSink, "width",1280,"height",720, NULL);
 //  	g_object_set (mGstPipeline.mVideoSegRecordSink, "location", "/mnt/sdcard/DVRDATA/Vod", NULL);
 //  	g_object_set (mGstPipeline.mSnapshootSink, "location", "/mnt/sdcard/DVRDATA/Pic", NULL);
 //  	g_object_set (mGstPipeline.mUdpPhoneSink, "host", "127.0.0.1", "port", 8004, NULL);  
 //  	g_object_set (mGstPipeline.mUdpCloudSink, "host", "127.0.0.1", "port", 8004, NULL);
    g_object_set (mGstPipeline.mTextOverlay1, "text", "Daily Mode", NULL);	
    g_object_set (mGstPipeline.mTextOverlay2, "text", "Emergency Mode",  NULL);
    g_object_set (mGstPipeline.mTextOverlay3, "text", "HMI Mode", NULL);
    g_object_set (mGstPipeline.mTextOverlay4, "text", "VideoSeg Mode", NULL);
    g_object_set (mGstPipeline.mTextOverlay5, "text", "Snapshoot Mode", NULL);
    g_object_set (mGstPipeline.mTextOverlay6, "text", "Remote Mode", NULL);
    g_object_set (mGstPipeline.mTextOverlay7, "text", "Cloud Mode", NULL);
  	/* Link all elements that can be automatically linked because they have "Always" pads */
    
  	gst_bin_add_many(GST_BIN (mGstPipeline.mBin1), mGstPipeline.mQueue1, mGstPipeline.mTextOverlay1, mGstPipeline.mDailyMonitorSink, NULL);
    gst_bin_add_many(GST_BIN (mGstPipeline.mBin2), mGstPipeline.mQueue2, mGstPipeline.mTextOverlay2, mGstPipeline.mEmergencySink, NULL);
    gst_bin_add_many(GST_BIN (mGstPipeline.mBin3), mGstPipeline.mQueue3, mGstPipeline.mTextOverlay3, mGstPipeline.mHIMSink, NULL);
    gst_bin_add_many(GST_BIN (mGstPipeline.mBin4), mGstPipeline.mQueue4, mGstPipeline.mTextOverlay4, mGstPipeline.mVideoSegRecordSink, NULL);
    gst_bin_add_many(GST_BIN (mGstPipeline.mBin5), mGstPipeline.mQueue5, mGstPipeline.mTextOverlay5, mGstPipeline.mSnapshootSink, NULL);
    gst_bin_add_many(GST_BIN (mGstPipeline.mBin6), mGstPipeline.mQueue6, mGstPipeline.mTextOverlay6, mGstPipeline.mUdpPhoneSink, NULL);
    gst_bin_add_many(GST_BIN (mGstPipeline.mBin7), mGstPipeline.mQueue7, mGstPipeline.mTextOverlay7, mGstPipeline.mUdpCloudSink, NULL);

    gst_bin_add_many(GST_BIN (mGstPipeline.mPipeline), mGstPipeline.mVideoSrc, mGstPipeline.mTee ,NULL);
    gst_bin_add(GST_BIN (mGstPipeline.mPipeline), mGstPipeline.mBin1);
    // gst_bin_add_many(GST_BIN (mGstPipeline.mPipeline), mGstPipeline.mBin2, NULL);
    // gst_bin_add_many(GST_BIN (mGstPipeline.mPipeline), mGstPipeline.mBin3, NULL);
    // gst_bin_add_many(GST_BIN (mGstPipeline.mPipeline), mGstPipeline.mBin4, NULL);
    // gst_bin_add_many(GST_BIN (mGstPipeline.mPipeline), mGstPipeline.mBin5, NULL);
    // gst_bin_add_many(GST_BIN (mGstPipeline.mPipeline), mGstPipeline.mBin6,NULL);
    // gst_bin_add_many(GST_BIN (mGstPipeline.mPipeline), mGstPipeline.mBin7, NULL);

    // gst_bin_add_many(GST_BIN (mGstPipeline.mPipeline), mFakeSink, NULL);

   	/* Link all elements that can be automatically linked because they have "Always" pads */
  	
    if( gst_element_link_many (mGstPipeline.mVideoSrc, mGstPipeline.mTee, NULL) != TRUE ||
        gst_element_link_many (mGstPipeline.mQueue1, mGstPipeline.mTextOverlay1, mGstPipeline.mDailyMonitorSink, NULL) != TRUE ||
        gst_element_link_many (mGstPipeline.mQueue2, mGstPipeline.mTextOverlay2, mGstPipeline.mEmergencySink, NULL) != TRUE ||
        gst_element_link_many (mGstPipeline.mQueue3, mGstPipeline.mTextOverlay3, mGstPipeline.mHIMSink, NULL) != TRUE ||
        gst_element_link_many (mGstPipeline.mQueue4, mGstPipeline.mTextOverlay4, mGstPipeline.mVideoSegRecordSink, NULL) != TRUE ||
        gst_element_link_many (mGstPipeline.mQueue5, mGstPipeline.mTextOverlay5, mGstPipeline.mSnapshootSink, NULL) != TRUE ||
        gst_element_link_many (mGstPipeline.mQueue6, mGstPipeline.mTextOverlay6, mGstPipeline.mUdpPhoneSink, NULL) != TRUE ||
        gst_element_link_many (mGstPipeline.mQueue7, mGstPipeline.mTextOverlay7, mGstPipeline.mUdpCloudSink, NULL) != TRUE
      )
    {
        LOGGER_DBG ("Elements could not be linked\n");
        gst_object_unref (mGstPipeline.mPipeline);
        return ;
    }

    LOGGER_DBG("VideoMonitor::createOriginPipeline Elements have been linked");
    /*Aquire the "Request" pads */
    mGstPipeline.mTeeSrcPadTemplate = gst_element_class_get_pad_template (GST_ELEMENT_GET_CLASS (mGstPipeline.mTee), "src_%u");     

    mGstPipeline.mTeePad1    = gst_element_request_pad (mGstPipeline.mTee, mGstPipeline.mTeeSrcPadTemplate, NULL, NULL);
    mGstPipeline.mTeePad2    = gst_element_request_pad (mGstPipeline.mTee, mGstPipeline.mTeeSrcPadTemplate, NULL, NULL);
    mGstPipeline.mTeePad3    = gst_element_request_pad (mGstPipeline.mTee, mGstPipeline.mTeeSrcPadTemplate, NULL, NULL);
    mGstPipeline.mTeePad4    = gst_element_request_pad (mGstPipeline.mTee, mGstPipeline.mTeeSrcPadTemplate, NULL, NULL);
    mGstPipeline.mTeePad5    = gst_element_request_pad (mGstPipeline.mTee, mGstPipeline.mTeeSrcPadTemplate, NULL, NULL);
    mGstPipeline.mTeePad6    = gst_element_request_pad (mGstPipeline.mTee, mGstPipeline.mTeeSrcPadTemplate, NULL, NULL);
    mGstPipeline.mTeePad7    = gst_element_request_pad (mGstPipeline.mTee, mGstPipeline.mTeeSrcPadTemplate, NULL, NULL);
    if(!mGstPipeline.mTeePad1 || !mGstPipeline.mTeePad2 || !mGstPipeline.mTeePad3 || !mGstPipeline.mTeePad4 || !mGstPipeline.mTeePad5 || !mGstPipeline.mTeePad6 || !mGstPipeline.mTeePad7 )
    {
        LOGGER_DBG(" mTeePad could not be created");
        return ;
    }
// mFakeSinkPad   = gst_element_get_static_pad (mFakeSink, "sink");
    mGstPipeline.mQueuePad1  = gst_element_get_static_pad (mGstPipeline.mQueue1, "sink");
    mGstPipeline.mQueuePad2  = gst_element_get_static_pad (mGstPipeline.mQueue2, "sink");
    mGstPipeline.mQueuePad3  = gst_element_get_static_pad (mGstPipeline.mQueue3, "sink");
    mGstPipeline.mQueuePad4  = gst_element_get_static_pad (mGstPipeline.mQueue4, "sink");
    mGstPipeline.mQueuePad5  = gst_element_get_static_pad (mGstPipeline.mQueue5, "sink");
    mGstPipeline.mQueuePad6  = gst_element_get_static_pad (mGstPipeline.mQueue6, "sink");
    mGstPipeline.mQueuePad7  = gst_element_get_static_pad (mGstPipeline.mQueue7, "sink");
    if(!mGstPipeline.mQueuePad1 || !mGstPipeline.mQueuePad2 || !mGstPipeline.mQueuePad3 || !mGstPipeline.mQueuePad4 || !mGstPipeline.mQueuePad5 || !mGstPipeline.mQueuePad6 || !mGstPipeline.mQueuePad7 )
    {
        LOGGER_DBG("mQueuePad could not be created");
        return ;
    }
    /*Create the Ghost pads for Bin*/
    mGstPipeline.mBinPad1    = gst_ghost_pad_new ("sink", mGstPipeline.mQueuePad1);
    mGstPipeline.mBinPad2    = gst_ghost_pad_new ("sink", mGstPipeline.mQueuePad2);
    mGstPipeline.mBinPad3    = gst_ghost_pad_new ("sink", mGstPipeline.mQueuePad3);
    mGstPipeline.mBinPad4    = gst_ghost_pad_new ("sink", mGstPipeline.mQueuePad4);
    mGstPipeline.mBinPad5    = gst_ghost_pad_new ("sink", mGstPipeline.mQueuePad5);
    mGstPipeline.mBinPad6    = gst_ghost_pad_new ("sink", mGstPipeline.mQueuePad6);
    mGstPipeline.mBinPad7    = gst_ghost_pad_new ("sink", mGstPipeline.mQueuePad7);
    if(!mGstPipeline.mBinPad1 || !mGstPipeline.mBinPad2 || !mGstPipeline.mBinPad3 || !mGstPipeline.mBinPad4 || !mGstPipeline.mBinPad5 || !mGstPipeline.mBinPad6 || !mGstPipeline.mBinPad7 )
    {
        LOGGER_DBG(" mBinPad could not be created");
        return ;
    }

    gst_element_add_pad (mGstPipeline.mBin1,mGstPipeline.mBinPad1);
    gst_element_add_pad (mGstPipeline.mBin2,mGstPipeline.mBinPad2);
    gst_element_add_pad (mGstPipeline.mBin3,mGstPipeline.mBinPad3);
    gst_element_add_pad (mGstPipeline.mBin4,mGstPipeline.mBinPad4);
    gst_element_add_pad (mGstPipeline.mBin5,mGstPipeline.mBinPad5);
    gst_element_add_pad (mGstPipeline.mBin6,mGstPipeline.mBinPad6);
    gst_element_add_pad (mGstPipeline.mBin7,mGstPipeline.mBinPad7);

    /*Link the tee and bins*/
    if(	gst_pad_link (mGstPipeline.mTeePad1, mGstPipeline.mBinPad1) != GST_PAD_LINK_OK 
		    // gst_pad_link (mGstPipeline.mTeePad2, mGstPipeline.mBinPad2) != GST_PAD_LINK_OK 
		    // gst_pad_link (mGstPipeline.mTeePad3, mGstPipeline.mBinPad3) != GST_PAD_LINK_OK ||
	 	    // gst_pad_link (mGstPipeline.mTeePad4, mGstPipeline.mBinPad4) != GST_PAD_LINK_OK ||
		    // gst_pad_link (mGstPipeline.mTeePad5, mGstPipeline.mBinPad5) != GST_PAD_LINK_OK ||
  		  // gst_pad_link (mGstPipeline.mTeePad6, mGstPipeline.mBinPad6) != GST_PAD_LINK_OK ||
  		  // // gst_pad_link (mGstPipeline.mTeePad6, mFakeSinkPad) != GST_PAD_LINK_OK ||
      //   gst_pad_link (mGstPipeline.mTeePad7, mGstPipeline.mBinPad7) != GST_PAD_LINK_OK   
     	) 
  {  
    LOGGER_DBG ("Tee could not be linked.\n");  
  	gst_object_unref (mGstPipeline.mPipeline); 
		gst_object_unref (mGstPipeline.mBinPad1); 
		gst_object_unref (mGstPipeline.mBinPad2); 
		gst_object_unref (mGstPipeline.mBinPad3); 
		gst_object_unref (mGstPipeline.mBinPad4); 
		gst_object_unref (mGstPipeline.mBinPad5); 
		gst_object_unref (mGstPipeline.mBinPad6);
		gst_object_unref (mGstPipeline.mBinPad7);   
  		return ;  
	}  
     /* Free pads */
    gst_object_unref (mGstPipeline.mQueuePad1); 
    gst_object_unref (mGstPipeline.mQueuePad2); 
    gst_object_unref (mGstPipeline.mQueuePad3); 
    gst_object_unref (mGstPipeline.mQueuePad4); 
    gst_object_unref (mGstPipeline.mQueuePad5); 
    gst_object_unref (mGstPipeline.mQueuePad6);
    gst_object_unref (mGstPipeline.mQueuePad7);
    LOGGER_DBG("VideoMonitor::createOriginPipeline end!");
	return ;
}

void VideoMonitor::pipelineStateChangedCallBack(GstBus *bus, GstMessage *msg)
{
	GstState old_state, new_state, pending_state;
  	gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
  	/* Only pay attention to messages coming from the pipeline, not its children */
  	if (GST_MESSAGE_SRC (msg) == GST_OBJECT (mInstance->mGstPipeline.mPipeline)) 
    {
    gchar *message = g_strdup_printf("State changed to %s", gst_element_state_get_name(new_state));
    /*Use of the gchar message*/
    g_free (message);
  }
}   

void VideoMonitor::workModeChangedCallBack()
{

}   

//--------------------------------------------------------------------------
/**
 *  @brief  set the bus,message,error call back,etc of gstreamer pipeline
 */
//--------------------------------------------------------------------------
void VideoMonitor::setPipelineBusandSignal()
{
	/* Instruct the bus to emit signals for each received message, and connect to the interesting signals */
	mGstPipeline.mGstBus = gst_element_get_bus (mGstPipeline.mPipeline);
	gst_bus_add_signal_watch (mGstPipeline.mGstBus);
  	g_signal_connect (G_OBJECT (mGstPipeline.mGstBus), "message::error", (GCallback)gstErrorCallBack, NULL);
	g_signal_connect (G_OBJECT (mGstPipeline.mGstBus), "message::state-changed", (GCallback)pipelineStateChangedCallBack, NULL);
	gst_object_unref (mGstPipeline.mGstBus);
}  

void VideoMonitor::getNativeWindow()
{
	//aquire the native window from Qt view
}

void VideoMonitor::gstErrorCallBack(GstBus *bus, GstMessage *msg)
{
	GError *err;
  	gchar *debug_info;
  	/* Print error details on the screen */
  	gst_message_parse_error (msg, &err, &debug_info);
	g_clear_error (&err);
	g_free (debug_info);
	g_main_loop_quit (mInstance->mPluginGMainLoop);
}                       

void VideoMonitor::logout()
{

}

void VideoMonitor::finalize()
{

}

/*Function to change the mode of pipeline*/
t_int VideoMonitor::modeChangeDailyToEmergency(void *msg, void *data)
{
    
    if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    gst_element_set_state (mGstPipeline.mPipeline, GST_STATE_NULL);


    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeDailyToCapture(void *msg, void *data)
{

     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeDailyToRecord(void *msg, void *data)
{
   
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeDailyToRemote(void *msg, void *data)
{
    
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeRemoteToEmergency(void *msg, void *data)
{
    
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeRemoteToCapture(void *msg, void *data)
{
    
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeRemoteToRecord(void *msg, void *data)
{
   
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeRemoteToDaily(void *msg, void *data)
{
    
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeRecordToEmergency(void *msg, void *data)
{
    
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeRecordToCapture(void *msg, void *data)
{
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeRecordToDaily(void *msg, void *data)
{ 
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeCaptureToEmergency(void *msg, void *data)
{
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeCaptureToRecord(void *msg, void *data)
{    
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
t_int VideoMonitor::modeChangeCaptureToDaily(void *msg, void *data)
{   
     if(NULL == msg || NULL == data)
    {
        return E_OPERATION_ERROR_PARA;
    }
    Message *ctxMsg = static_cast<Message *>(msg);
    
    LOGGER_DBG("VideoMonitor::modeChangeDailyToEmergency");

    delete ctxMsg;
    return E_OPERATION_ERROR_NONE;
}
