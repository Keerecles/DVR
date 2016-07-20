#ifndef APPICONPROVIDER_H
#define APPICONPROVIDER_H
#include <QQuickImageProvider>
#include <QtAndroid>
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>

class AppIconProvider : public QQuickImageProvider
{
public:
    AppIconProvider()
        : QQuickImageProvider(QQuickImageProvider::Image){}

    QImage requestImage(const QString &id, QSize *size, const QSize& requestedSize);
    QAndroidJniObject createBitmap(int width, int height);
    QImage toImage(const QAndroidJniObject &drawable, const QRect &bounds);
    QImage toImage(const QAndroidJniObject &bitmap);
};

#endif // APPICONPROVIDER_H
