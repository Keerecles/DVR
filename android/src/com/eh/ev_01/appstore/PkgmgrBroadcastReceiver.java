package com.eh.ev_01.appstore;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class PkgmgrBroadcastReceiver extends BroadcastReceiver {

    private String TAG = "PKGMGR";
    @Override
    public void onReceive(Context context, Intent intent)
    {
        if(intent.getAction().equals(Intent.ACTION_PACKAGE_ADDED))
        {
            String packageName = intent.getDataString();
            Log.i(TAG,"onReceiveNativeAppInstalled packageName"+packageName);
            NativeFunctions.onReceiveNativeAppInstalled(packageName);
            return;
         }

         if(intent.getAction().equals(Intent.ACTION_PACKAGE_REPLACED))
         {
             String packageName = intent.getDataString();
             Log.i(TAG,"onReceiveNativeAppUpdated packageName"+packageName);
             NativeFunctions.onReceiveNativeAppUpdated(packageName);
             return;
          }

          if(intent.getAction().equals(Intent.ACTION_PACKAGE_REMOVED))
          {
              String packageName = intent.getDataString();
              Log.i(TAG,"onReceiveNativeAppUninstalled packageName"+packageName);
              NativeFunctions.onReceiveNativeAppUninstalled(packageName);
              return;
           }
     }
}
