package com.eh.ev_01.appstore;

import java.util.List;
import java.util.ArrayList;
import java.io.File;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.content.pm.PackageInfo;
import android.content.pm.ApplicationInfo;
import android.content.Intent;
import android.net.Uri;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.util.Log;
import org.qtproject.qt5.android.bindings.QtActivity;

public class PkgmgrActivity extends QtActivity
{
    private PkgmgrActivity m_instance;
    private PackageManager pm;
    private static PkgDownloadManager mPkgDownlaodManager;

    public List<PackageInfo> mPackageInfoList;
    public List<AppInfo> mAppInfoList;
    public String TAG = "PKGMGR";

    public PkgmgrActivity()
    {
        m_instance = this;
    }

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //create instance
        mPkgDownlaodManager = new PkgDownloadManager(this);
    }

    //this method is called by C++ to register the BroadcastReceiver.
    public void registerBroadcastReceiver()
    {
        //Qt is running on a different thread than Android.
        //In order to register the receiver we need to execute it in the Android UI thread.
        runOnUiThread(new RegisterReceiverRunnable(this));
    }

    public Object[] getApplications(int type)
    {
        if(pm==null)
        {
            pm = m_instance.getApplicationContext().getPackageManager();
        }

        Object[] result = null;
        Log.i(TAG,"getApplications");
        switch(type)
        {
            case 0://ALL
                result = getAllApplications();
                break;

            case 1://SYSTEM
                result = getSystemApplications();
                break;

            case 2://USER
                result = getUserApplications();
                break;

            default:
                break;
        }

        return result;
    }

    private Object[] getAllApplications()
    {
        Log.i(TAG,"getAllApplications");
        mPackageInfoList = pm.getInstalledPackages(0);
        Log.i(TAG,"mPackageInfoList size = "+ mPackageInfoList.size());
        List<AppInfo> appInfoList = new ArrayList();
        for(int i=0;i<mPackageInfoList.size();i++)
        {
            PackageInfo p = mPackageInfoList.get(i);
            if(p.versionName == null)
            {
                continue;
            }

            AppInfo newInfo = new AppInfo();
            newInfo.appName = p.applicationInfo.loadLabel(pm).toString();
            newInfo.packageName = p.packageName;
            newInfo.versionName = p.versionName;
            newInfo.versionCode = p.versionCode;
            appInfoList.add(newInfo);
        }
        return appInfoList.toArray();
    }

    private Object[] getSystemApplications()
    {
        Log.i(TAG,"getSystemApplications");
        mPackageInfoList = pm.getInstalledPackages(0);
        Log.i(TAG,"mPackageInfoList size = "+ mPackageInfoList.size());
        List<AppInfo> appInfoList = new ArrayList();
        for(int i=0;i<mPackageInfoList.size();i++)
        {
            PackageInfo p = mPackageInfoList.get(i);
            if(p.versionName == null)
            {
                continue;
            }

            if((p.applicationInfo.flags&ApplicationInfo.FLAG_SYSTEM) == 0)
            {
                continue;
            }

            AppInfo newInfo = new AppInfo();
            newInfo.appName = p.applicationInfo.loadLabel(pm).toString();
            newInfo.packageName = p.packageName;
            newInfo.versionName = p.versionName;
            newInfo.versionCode = p.versionCode;
            appInfoList.add(newInfo);
        }
        return appInfoList.toArray();
    }

    private Object[] getUserApplications()
    {
        Log.i(TAG,"getUserApplications");
        mPackageInfoList = pm.getInstalledPackages(0);
        Log.i(TAG,"mPackageInfoList size = "+ mPackageInfoList.size());
        List<AppInfo> appInfoList = new ArrayList();
        for(int i=0;i<mPackageInfoList.size();i++)
        {
            PackageInfo p = mPackageInfoList.get(i);
            if(p.versionName == null)
            {
                continue;
            }

            if((p.applicationInfo.flags&ApplicationInfo.FLAG_SYSTEM) != 0)
            {
                continue;
            }

            AppInfo newInfo = new AppInfo();
            newInfo.appName = p.applicationInfo.loadLabel(pm).toString();
            newInfo.packageName = p.packageName;
            newInfo.versionName = p.versionName;
            newInfo.versionCode = p.versionCode;
            Log.i(TAG,"packageName = "+ p.packageName);
            appInfoList.add(newInfo);
        }
        return appInfoList.toArray();
    }

    public void install(String path)
    {
        Log.i(TAG,"install path = " + path);
        Intent intent = new Intent(Intent.ACTION_VIEW);
        intent.setDataAndType(Uri.fromFile(new File(path)),
        "application/vnd.android.package-archive");
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        startActivity(intent);
    }

    public void uninstall(String name)
    {
        Log.i(TAG,"uninstall name = " + name);
        Uri packageURI = Uri.parse("package:"+name);
        Intent intent = new Intent(Intent.ACTION_DELETE,packageURI);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        startActivity(intent);
    }

    public void download(String uri)
    {
        Log.i(TAG,"PkgmgrActivity download uri = " + uri);
        mPkgDownlaodManager.download(uri);
    }
    public void redownload()
    {

    }

    public void continueDownload()
    {

    }

    public void pauseDownload()
    {

    }

    public void cancelDownload()
    {

    }

    public void launchApp(String pkgName)
    {
        Intent intent = getPackageManager().getLaunchIntentForPackage(pkgName);
        startActivity(intent);
    }
	
    public Drawable getApplicationIcon(String pkgName)
    {
        if(pm==null)
        {
            pm = m_instance.getApplicationContext().getPackageManager();
        }

        Drawable icon = null;
        Log.i(TAG,"getApplicationIcon");
        try
        {
            icon = pm.getApplicationIcon(pkgName);
        }
        catch (Exception e)
        {
            icon = null;
        }

        return icon;
    }

}
