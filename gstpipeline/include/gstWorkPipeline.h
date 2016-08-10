#ifndef GSTWORKPIPELINE_H
#define GSTWORKPIPELINE_H

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/app/gstappsink.h>
#include <gst/video/video.h>


class GstWorkPipeline 
{     
public:
    GstWorkPipeline();                         //default constructor
    ~GstWorkPipeline();                        //destructor

    GstElement *mPipeline,*mVideoSrc;
    GstElement *mTee;
    GstElement *mBin1,*mBin2,*mBin3,*mBin4,*mBin5,*mBin6,*mBin7;
    GstElement *mQueue1,*mQueue2,*mQueue3,*mQueue4,*mQueue5,*mQueue6,*mQueue7;
    GstElement *mTextOverlay1,*mTextOverlay2,*mTextOverlay3,*mTextOverlay4,*mTextOverlay5,*mTextOverlay6,*mTextOverlay7;
    GstElement *mDailyMonitorSink,*mFFmpegcolorspace1,*mFFmpegcolorspace2,*mDecodeBin;
    GstElement *mEmergencySink;
    GstElement *mHMISink;
    GstElement *mVideoSegRecordSink;
    GstElement *mSnapshotSink,*mPicFormat;
    GstElement *mUdpPhoneSink;
    GstElement *mUdpCloudSink;
    
    GstPadTemplate *mTeeSrcPadTemplate;
    GstPad *mTeePad1,*mTeePad2,*mTeePad3,*mTeePad4,*mTeePad5,*mTeePad6,*mTeePad7;
    GstPad *mBinPad1,*mBinPad2,*mBinPad3,*mBinPad4,*mBinPad5,*mBinPad6,*mBinPad7;
    GstPad *mQueuePad1,*mQueuePad2,*mQueuePad3,*mQueuePad4,*mQueuePad5,*mQueuePad6,*mQueuePad7;

    GstBus* mGstBus;
    GError* mError;
};

#endif // GstPipeline_H
