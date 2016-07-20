package com.eh.ev_01.appstore;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import java.io.File;
import android.net.Uri;
import android.os.Environment;
import android.app.DownloadManager;
import android.util.Log;



public class PkgDownloadManager
{
    private PkgDownloadManager m_instance;

    public static final String     DOWNLOAD_FOLDER_NAME = "Download";
    public static String     DOWNLOAD_FILE_NAME   = "";
    public static final String     KEY_NAME_DOWNLOAD_ID = "downloadId";
    private static Context m_Context;
    private DownloadManager        downloadManager;
    private long                   downloadId           = 0;


    public PkgDownloadManager(Activity activity)
    {
        m_instance = this;
        m_Context = activity.getApplicationContext();
        downloadManager = (DownloadManager)m_Context.getSystemService(Context.DOWNLOAD_SERVICE);

    }


    public void download(String uri)
    {
        Log.i("PkgDownloadManager"," Download uri = " + uri);
        DOWNLOAD_FILE_NAME = uri.substring(uri.lastIndexOf("/") + 1, uri.length());
        File folder = Environment.getExternalStoragePublicDirectory(DOWNLOAD_FOLDER_NAME);
        if (!folder.exists() || !folder.isDirectory()) {
            folder.mkdirs();
        }

        DownloadManager.Request request = new DownloadManager.Request(Uri.parse(uri));
        request.setDestinationInExternalPublicDir(DOWNLOAD_FOLDER_NAME, DOWNLOAD_FILE_NAME);
        request.setTitle("DL :"+DOWNLOAD_FILE_NAME);
        request.setDescription(DOWNLOAD_FILE_NAME+" desc....");
        request.setNotificationVisibility(DownloadManager.Request.VISIBILITY_VISIBLE_NOTIFY_COMPLETED);
        downloadId = downloadManager.enqueue(request);
    }
    public void redownload(){}
    public void continueDownload(){}
    public void pauseDownload(){}
    public void cancelDownload(){}

}
