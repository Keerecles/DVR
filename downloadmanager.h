#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H
#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>
#include <vector>
#include <QtAndroid>
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>

#define     E_DLMGR_DLSTATUSLIST_KIND_NODOWNLOADTASK    0
#define     E_DLMGR_DLSTATUSLIST_KIND_DOWNLOADING       1
#define     E_DLMGR_DLSTATUSLIST_KIND_DOWNLOADED        2
#define     E_DLMGR_DLSTATUSLIST_KIND_DOWNLOADFAILED   -1



class DownloadManager : public QObject
{
    Q_OBJECT
	Q_PROPERTY(QString appDownloadStatus  READ   getAppDownloadStatus  WRITE  setAppDownloadStatus    NOTIFY   AppDownloadStatusChanged )
	Q_PROPERTY(QString appInfo  READ   getAppInfo  WRITE  setAppInfo  NOTIFY   AppInfoChanged)

public:

    static DownloadManager &instance(QObject *parent = 0);

    Q_INVOKABLE QString getAppDownloadStatus();
    Q_INVOKABLE QString getAppInfo();

    Q_INVOKABLE void startDownload(const QString uri);
    Q_INVOKABLE void continueDownload();
    Q_INVOKABLE void pauseDownload();
    Q_INVOKABLE void cancelDownload();

    void setAppDownloadStatus(const QString &name);
    void setAppInfo(const QString &name);


signals:
    void AppDownloadStatusChanged();
    void AppInfoChanged();

public slots:
    void onAppDownloadStatusChanged(const QString &name);
    void onAppInfoChanged(const QString &name);

public:
    explicit DownloadManager(QObject *parent = 0);

private:
    void Download(QString uri);
    QString mDownloadedAppName;
    QString mDownloadedAppPath;
    int mDownloadedAppStatus;
};
#endif // DOWNLOADMANAGER_H
