#ifndef PACKAGEINFO_H
#define PACKAGEINFO_H
#include <QObject>
#include <QBitmap>

class PackageInfo
{
public:
    PackageInfo();

    QString packageName;
    QString appName;
    QString versionName;
    int versionCode;
    QBitmap icon;
};

#endif // PACKAGEINFO_H
