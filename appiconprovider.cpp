#include <android/bitmap.h>
#include "appiconprovider.h"
#include "packagemanager.h"

QImage AppIconProvider::requestImage(const QString &id, QSize *size, const QSize& requestedSize)
{
    QRect bounds(0,0,requestedSize.isValid()?requestedSize.width():100,requestedSize.isValid()?requestedSize.height():100);
    QAndroidJniObject drawable =  PackageManager::instance().getIconDrawable(id.toInt());
    if(drawable == NULL)
    {
        return QImage();
    }

    QImage iconImage = toImage(drawable,bounds);
    *size = iconImage.size();
    return iconImage;
}

QAndroidJniObject AppIconProvider::createBitmap(int width, int height)
{
    QAndroidJniObject config = QAndroidJniObject::getStaticObjectField("android/graphics/Bitmap$Config",
                                                                       "ARGB_8888",
                                                                       "Landroid/graphics/Bitmap$Config;");

    return QAndroidJniObject::callStaticObjectMethod("android/graphics/Bitmap",
                                                     "createBitmap",
                                                     "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;",
                                                     width, height, config.object());
}

QImage AppIconProvider::toImage(const QAndroidJniObject &drawable, const QRect &bounds)
{
    QAndroidJniObject bitmap = createBitmap(bounds.width(), bounds.height());
    QAndroidJniObject canvas("android/graphics/Canvas", "(Landroid/graphics/Bitmap;)V", bitmap.object());
    drawable.callMethod<void>("setBounds", "(IIII)V", bounds.left(), bounds.top(), bounds.right(), bounds.bottom());
    drawable.callMethod<void>("draw", "(Landroid/graphics/Canvas;)V", canvas.object());
    return toImage(bitmap);
}

QImage AppIconProvider::toImage(const QAndroidJniObject &bitmap)
{
    QAndroidJniEnvironment env;
    AndroidBitmapInfo info;
    if (AndroidBitmap_getInfo(env, bitmap.object(), &info) != ANDROID_BITMAP_RESULT_SUCCESS)
        return QImage();

    QImage::Format format;
    switch (info.format) {
    case ANDROID_BITMAP_FORMAT_RGBA_8888:
        format = QImage::Format_RGBA8888;
        break;
    case ANDROID_BITMAP_FORMAT_RGB_565:
        format = QImage::Format_RGB16;
        break;
    case ANDROID_BITMAP_FORMAT_RGBA_4444:
        format = QImage::Format_ARGB4444_Premultiplied;
        break;
    case ANDROID_BITMAP_FORMAT_A_8:
        format = QImage::Format_Alpha8;
        break;
    default:
        return QImage();
    }

    void *pixels;
    if (AndroidBitmap_lockPixels(env, bitmap.object(), &pixels) != ANDROID_BITMAP_RESULT_SUCCESS)
        return QImage();

    QImage image(info.width, info.height, format);

    if (info.stride == uint32_t(image.bytesPerLine())) {
        memcpy((void*)image.constBits(), pixels, info.stride * info.height);
    } else {
        uchar *bmpPtr = static_cast<uchar *>(pixels);
        const unsigned width = std::min(info.width, (uint)image.width());
        const unsigned height = std::min(info.height, (uint)image.height());
        for (unsigned y = 0; y < height; y++, bmpPtr += info.stride)
            memcpy((void*)image.constScanLine(y), bmpPtr, width);
    }

    if (AndroidBitmap_unlockPixels(env, bitmap.object()) != ANDROID_BITMAP_RESULT_SUCCESS)
        return QImage();

    return image;
}
