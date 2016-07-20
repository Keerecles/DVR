#include "packagemanager.h"
#include <QDebug>

PackageManager::PackageManager(QObject *parent) : QObject(parent)
{
    mInstalledPackageName = "";
    mUpdatedPackageName = "";
    mUninstalledPackageName = "";
    mPackageInfoListKind = E_PKGMGR_PKGINFOLIST_KIND_ALL;
}

PackageManager &PackageManager::instance(QObject *parent)
{
    static PackageManager packageManager(parent);
    return packageManager;
}

PackageManager::~PackageManager()
{
    qDebug() << "~PackageManager";
    PackageInfo *ctxPackageInfo = NULL;
    for(uint i=0;i<mPackageInfoList.size();i++)
    {
        ctxPackageInfo = mPackageInfoList[i];
        delete ctxPackageInfo;
    }

    //qmlClearTypeRegistrations();
    //QtAndroid::androidActivity().callMethod<void>("unregisterBroadcastReceiver","()V");
}

void PackageManager::getAllApplications()
{
    getApplications(E_PKGMGR_PKGINFOLIST_KIND_ALL);
}

void PackageManager::getSystemApplications()
{
    getApplications(E_PKGMGR_PKGINFOLIST_KIND_SYSTEM);
}

void PackageManager::getUserApplications()
{
    getApplications(E_PKGMGR_PKGINFOLIST_KIND_USER);
}

void PackageManager::getApplications(int kind)
{
    if(E_PKGMGR_PKGINFOLIST_KIND_MAX <= kind)
    {
        return;
    }

    PackageInfo *ctxPackageInfo = NULL;
    for(uint i=0;i<mPackageInfoList.size();i++)
    {
        ctxPackageInfo = mPackageInfoList[i];
        delete ctxPackageInfo;
    }

    mPackageInfoList.clear();
    ctxPackageInfo = NULL;

    jint listType = kind;

    QAndroidJniObject resultObj = QtAndroid::androidActivity().callObjectMethod(
                                    "getApplications",
                                    "(I)[Ljava/lang/Object;",
                                    listType);
    if(resultObj == NULL)
    {
        return;
    }

    jobjectArray resultList = resultObj.object<jobjectArray>();


    QAndroidJniEnvironment env;
    jsize count = env->GetArrayLength(resultList);
    qDebug() << "result length = "<< count;

    for(int i=0;i<count;i++)
    {
        ctxPackageInfo = new PackageInfo();

        //get first object
        jobject result = env->GetObjectArrayElement(resultList,i);
        jclass c = env->GetObjectClass(result);

        //get app name
        jfieldID fidAppName = env->GetFieldID(c,"appName","Ljava/lang/String;");
        jstring appName = (jstring)env->GetObjectField(result,fidAppName);
        const char *appname = env->GetStringUTFChars(appName,0);
        ctxPackageInfo->appName = appname;
        env->ReleaseStringUTFChars(appName,appname);
        env->DeleteLocalRef(appName);

        //get package name
        jfieldID fidPkgName = env->GetFieldID(c,"packageName","Ljava/lang/String;");
        jstring pkgName = (jstring)env->GetObjectField(result,fidPkgName);
        const char *pkgname = env->GetStringUTFChars(pkgName,0);
        ctxPackageInfo->packageName = pkgname;
        env->ReleaseStringUTFChars(pkgName,pkgname);
        env->DeleteLocalRef(pkgName);

        //get version name
        jfieldID fidVerName = env->GetFieldID(c,"versionName","Ljava/lang/String;");
        jstring verName = (jstring)env->GetObjectField(result,fidVerName);
        const char *vername = env->GetStringUTFChars(verName,0);
        ctxPackageInfo->versionName = vername;
        env->ReleaseStringUTFChars(verName,vername);
        env->DeleteLocalRef(verName);

        //get version code
        jfieldID fidVerCode = env->GetFieldID(c,"versionCode","I");
        ctxPackageInfo->versionCode = env->GetIntField(result,fidVerCode);

        mPackageInfoList.push_back(ctxPackageInfo);
        env->DeleteLocalRef(result);
        env->DeleteLocalRef(c);
    }

    dumpPackageInfoList();

    emit packageInfoListUpdated();

}

void PackageManager::install(QString url)
{
    if( url == "")
    {
        return;
    }

    QAndroidJniObject jniUrl = QAndroidJniObject::fromString(url);
    QtAndroid::androidActivity().callMethod<void>(
                                        "install",
                                        "(Ljava/lang/String;)V",
                                        jniUrl.object<jstring>());
}

void PackageManager::uninstall(QString packageName)
{
    if( packageName == "")
    {
        return;
    }

    QAndroidJniObject jniPackageName = QAndroidJniObject::fromString(packageName);
    QtAndroid::androidActivity().callMethod<void>(
                                        "uninstall",
                                        "(Ljava/lang/String;)V",
                                        jniPackageName.object<jstring>());
}

uint PackageManager::getPackageInfoListSize()
{
    return mPackageInfoList.size();
}

uint PackageManager::getPackageInfoListType()
{
    return mPackageInfoListKind;
}

QString PackageManager::getPackageName(uint index)
{
    if(index>=mPackageInfoList.size())
    {
        return "";
    }

    return mPackageInfoList[index]->packageName;
}

QString PackageManager::getApplicationName(uint index)
{
    if(index>=mPackageInfoList.size())
    {
        return "";
    }

    return mPackageInfoList[index]->appName;
}

QString PackageManager::getVersionName(uint index)
{
    if(index>=mPackageInfoList.size())
    {
        return "";
    }

    return mPackageInfoList[index]->versionName;
}

uint PackageManager::getVersionCode(uint index)
{
    if(index>=mPackageInfoList.size())
    {
        return 0;
    }

    return mPackageInfoList[index]->versionCode;
}

QBitmap PackageManager::getIcon(uint index)
{
    if(index>=mPackageInfoList.size())
    {
        QBitmap empty;
        return empty;
    }

    return mPackageInfoList[index]->icon;
}

void PackageManager::setInstalledPackageName(const QString &name)
{
    mInstalledPackageName = name;
    emit applicationInstalled();
}

QString PackageManager::getInstalledPackageName() const
{
    return mInstalledPackageName;
}

void PackageManager::setUpdatedPackageName(const QString &name)
{
    mUpdatedPackageName = name;
    emit applicationUpdated();
}

QString PackageManager::getUpdatedPackageName() const
{
    return mUpdatedPackageName;
}

void PackageManager::setUninstalledPackageName(const QString &name)
{
    mUninstalledPackageName = name;
    emit applicationUninstalled();
}

QString PackageManager::getUninstalledPackageName() const
{
    return mUninstalledPackageName;
}

void PackageManager::setPackageInfoListKind(int type)
{
    mPackageInfoListKind = type;
    emit applicationUninstalled();
}

int PackageManager::getPackageInfoListKind()
{
    return mPackageInfoListKind;
}

void PackageManager::onReceivedAppInstalled(const QString &name)
{
    setInstalledPackageName(name);
}

void PackageManager::onReceivedAppUpdated(const QString &name)
{
    setUpdatedPackageName(name);
}

void PackageManager::onReceivedAppUninstalled(const QString &name)
{
    setUninstalledPackageName(name);
}

QAndroidJniObject PackageManager::getIconDrawable(uint index)
{
    if(index>=mPackageInfoList.size())
    {
        return NULL;
    }

    QAndroidJniObject pkgname = QAndroidJniObject::fromString(mPackageInfoList[index]->packageName);
    QAndroidJniObject drawable = QtAndroid::androidActivity().callObjectMethod(
                                        "getApplicationIcon",
                                        "(Ljava/lang/String;)Landroid/graphics/drawable/Drawable;",
                                        pkgname.object<jstring>());
    return drawable;
}

void PackageManager::dumpPackageInfoList()
{
    qDebug()<<"PackageInfoList total = " << mPackageInfoList.size();

    for(uint i=0;i<mPackageInfoList.size();i++)
    {
        qDebug()<<"num = "<<i;

        qDebug()<<"packageName = "<<mPackageInfoList[i]->packageName;
        qDebug()<<"appName = "<<mPackageInfoList[i]->appName;
        qDebug()<<"versionName = "<<mPackageInfoList[i]->versionName;
        qDebug()<<"versionCode = "<<mPackageInfoList[i]->versionCode;
    }
}

void PackageManager::launchApp(uint index)
{
    //Send the pkgName to the Java Launcher to launch the app
    qWarning() << "PackageManager launchApp " ;
    QAndroidJniObject jnipkgName = QAndroidJniObject::fromString(mPackageInfoList[index]->packageName);
    QtAndroid::androidActivity().callMethod<void>(
                                            "launchApp",
                                            "(Ljava/lang/String;)V",
                                            jnipkgName.object<jstring>());
}
