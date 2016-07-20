#include <jni.h>
#include <QMetaObject>
#include <QAndroidJniObject>
#include <QDebug>
#include "packagemanager.h"

static void onReceiveNativeAppInstalled(JNIEnv *env, jobject obj,jstring packageName)
{
    Q_UNUSED(obj)
    const char *str = env->GetStringUTFChars(packageName,0);
    QMetaObject::invokeMethod(&PackageManager::instance(),"onReceivedAppInstalled"
                              ,Qt::QueuedConnection,Q_ARG(QString, str));
    env->ReleaseStringUTFChars(packageName,str);
}

static void onReceiveNativeAppUpdated(JNIEnv *env, jobject obj,jstring packageName)
{
    Q_UNUSED(obj)
    const char *str = env->GetStringUTFChars(packageName,0);
    QMetaObject::invokeMethod(&PackageManager::instance(),"onReceivedAppUpdated"
                              ,Qt::QueuedConnection,Q_ARG(QString, str));
    env->ReleaseStringUTFChars(packageName,str);
}

static void onReceiveNativeAppUninstalled(JNIEnv *env, jobject obj,jstring packageName)
{
    Q_UNUSED(obj)
    const char *str = env->GetStringUTFChars(packageName,0);
    QMetaObject::invokeMethod(&PackageManager::instance(),"onReceivedAppUninstalled"
                              ,Qt::QueuedConnection,Q_ARG(QString, str));
    env->ReleaseStringUTFChars(packageName,str);
}

static JNINativeMethod methods[]={
    {"onReceiveNativeAppInstalled","(Ljava/lang/String;)V",(void *)onReceiveNativeAppInstalled},
    {"onReceiveNativeAppUpdated","(Ljava/lang/String;)V",(void *)onReceiveNativeAppUpdated},
    {"onReceiveNativeAppUninstalled","(Ljava/lang/String;)V",(void *)onReceiveNativeAppUninstalled},
};

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    Q_UNUSED(reserved)
    JNIEnv* env;

    qDebug()<<"enter";
    if(vm->GetEnv(reinterpret_cast<void**>(&env),JNI_VERSION_1_6) != JNI_OK)
        return JNI_ERR;

    qDebug()<<"find class";
    jclass javaClass = env->FindClass("com/eh/ev_01/appstore/NativeFunctions");
    if(!javaClass)
        return JNI_ERR;

    qDebug()<<"registernatives";
    if(env->RegisterNatives(javaClass,methods,sizeof(methods)/sizeof(methods[0]))<0)
    {
        return JNI_ERR;
    }

    qDebug()<<"end";
    return JNI_VERSION_1_6;
}
