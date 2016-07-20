package com.eh.ev_01.appstore;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

public class RegisterReceiverRunnable implements Runnable
{
    private Activity m_activity;
    private Context m_context;
    public RegisterReceiverRunnable(Activity activity)
    {
        m_activity = activity;
        m_context = m_activity.getApplicationContext();
    }
    //this method is called on Android Ui Thread
    @Override
    public void run()
    {
        IntentFilter filter = new IntentFilter();
        filter.addAction("android.intent.action.PACKAGE_ADDED");
        filter.addAction("android.intent.action.PACKAGE_REPLACED");
        filter.addAction("android.intent.action.PACKAGE_REMOVED");
        filter.addDataScheme("package");
        m_activity.registerReceiver(new PkgmgrBroadcastReceiver(),filter);
    }
}
