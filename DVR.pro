TEMPLATE = app
TARGET = DVR
QT += qml quick  core  androidextras gui

winrt: WINRT_MANIFEST.capabilities += internetClient

CONFIG += c++11

SOURCES += main.cpp \
    packageinfo.cpp \
    packagemanager.cpp \
    native.cpp \
    appiconprovider.cpp \
    downloadmanager.cpp \
    videomonitor.cpp \
    fsm.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    packageinfo.h \
    packagemanager.h \
    appiconprovider.h \
    downloadmanager.h \
    videomonitor.h \
    fsm.h

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/RegisterReceiverRunnable.java \
    android/PkgmgrBroadcastReceiver.java \
    android/NativeFunctions.java \
    android/AppInfo.java \
    android/PkgmgrActivity.java \
    android/src/com/eh/ev_01/appstore/AppInfo.java \
    android/src/com/eh/ev_01/appstore/NativeFunctions.java \
    android/src/com/eh/ev_01/appstore/PkgmgrActivity.java \
    android/src/com/eh/ev_01/appstore/PkgmgrBroadcastReceiver.java \
    android/src/com/eh/ev_01/appstore/RegisterReceiverRunnable.java \
    android/src/com/eh/ev_01/appstore/PkgDownloadManager.java

LIBS += -ljnigraphics
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
