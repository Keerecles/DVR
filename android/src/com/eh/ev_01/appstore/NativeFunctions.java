package com.eh.ev_01.appstore;

public class NativeFunctions {

    public static native void onReceiveNativeAppInstalled(String packageName);
    public static native void onReceiveNativeAppUpdated(String packageName);
    public static native void onReceiveNativeAppUninstalled(String packageName);
}
