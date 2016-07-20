#include "downloadmanager.h"
#include <QDebug>


DownloadManager::DownloadManager(QObject *parent) : QObject(parent)
{
    mDownloadedAppName ="";
    mDownloadedAppPath ="";
    mDownloadedAppStatus = E_DLMGR_DLSTATUSLIST_KIND_NODOWNLOADTASK;
}

DownloadManager & DownloadManager::instance(QObject *parent)
{
    static DownloadManager DownloadManager(parent);
    return DownloadManager;

}

QString DownloadManager::getAppDownloadStatus()
{
    emit AppDownloadStatusChanged();
}
QString DownloadManager::getAppInfo()
{
    emit AppInfoChanged();
}

void DownloadManager::startDownload(const QString uri)
{
//    mDownloadedAppName;
//    mDownloadedAppPath;
    mDownloadedAppStatus = E_DLMGR_DLSTATUSLIST_KIND_DOWNLOADING;
    //Send the uri to the Java DownloadManager and start to download
    qWarning() << "DownloadManager Download " ;
    QAndroidJniObject jniUrl = QAndroidJniObject::fromString(uri);
    QtAndroid::androidActivity().callMethod<void>(
                                            "download",
                                            "(Ljava/lang/String;)V",
                                            jniUrl.object<jstring>());
}

void DownloadManager::continueDownload()
{}
void DownloadManager::pauseDownload()
{}
void DownloadManager::cancelDownload()
{}


void DownloadManager::setAppDownloadStatus(const QString &name)
{

}
void DownloadManager::setAppInfo(const QString &name)
{

}
void DownloadManager::onAppDownloadStatusChanged(const QString &name)
{

}
void DownloadManager::onAppInfoChanged(const QString &name)
{

}

