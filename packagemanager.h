#ifndef PACKAGEMANAGER_H
#define PACKAGEMANAGER_H
#include <QObject>
#include <QGuiApplication>
#include <QQmlEngine>
#include <QJSEngine>
#include <vector>
#include <QtAndroid>
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include "packageinfo.h"

enum PKGMGR_PKGINFOLIST_KIND{
    E_PKGMGR_PKGINFOLIST_KIND_ALL,
    E_PKGMGR_PKGINFOLIST_KIND_SYSTEM,
    E_PKGMGR_PKGINFOLIST_KIND_USER,
    E_PKGMGR_PKGINFOLIST_KIND_MAX,
};


class PackageManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString installedPackageName READ getInstalledPackageName WRITE setInstalledPackageName NOTIFY applicationInstalled)
    Q_PROPERTY(QString updatedPackageName READ getUpdatedPackageName WRITE setUpdatedPackageName NOTIFY applicationUpdated)
    Q_PROPERTY(QString uninstalledPackageName READ getUninstalledPackageName WRITE setUninstalledPackageName NOTIFY applicationUninstalled)
    Q_PROPERTY(int packageInfoListKind READ getPackageInfoListKind WRITE setPackageInfoListKind NOTIFY packageInfoListUpdated)

public:
    //PackageManager();
    ~PackageManager();
    Q_INVOKABLE static PackageManager &instance(QObject *parent = 0);
    static QObject *qobject_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine);

    Q_INVOKABLE void getAllApplications();
    Q_INVOKABLE void getSystemApplications();
    Q_INVOKABLE void getUserApplications();
    Q_INVOKABLE void install(QString url);
    Q_INVOKABLE void uninstall(QString packageName);
    Q_INVOKABLE uint getPackageInfoListSize();
    Q_INVOKABLE uint getPackageInfoListType();
    Q_INVOKABLE QString getPackageName(uint index);
    Q_INVOKABLE QString getApplicationName(uint index);
    Q_INVOKABLE QString getVersionName(uint index);
    Q_INVOKABLE uint getVersionCode(uint index);
    Q_INVOKABLE QBitmap getIcon(uint index);
    QAndroidJniObject getIconDrawable(uint index);

    void setInstalledPackageName(const QString &name);
    QString getInstalledPackageName() const;
    void setUpdatedPackageName(const QString &name);
    QString getUpdatedPackageName() const;
    void setUninstalledPackageName(const QString &name);
    QString getUninstalledPackageName() const;
    void setPackageInfoListKind(int type);
    int getPackageInfoListKind();

//**************Launch************************
    Q_INVOKABLE void launchApp(uint index);

signals:
    void applicationInstalled();
    void applicationUpdated();
    void applicationUninstalled();
    void packageInfoListUpdated();

public slots:
    void onReceivedAppInstalled(const QString &name);
    void onReceivedAppUpdated(const QString &name);
    void onReceivedAppUninstalled(const QString &name);

public:
    explicit PackageManager(QObject *parent = 0);

private:
    void getApplications(int kind);
    void dumpPackageInfoList();
    QString mInstalledPackageName;
    QString mUpdatedPackageName;
    QString mUninstalledPackageName;
    int mPackageInfoListKind;
    std::vector<PackageInfo*>mPackageInfoList;

    QString mDownloadedAppName;
    QString mDownloadedAppPath;
    QString m_appUri;
};

#endif // PACKAGEMANAGER_H
