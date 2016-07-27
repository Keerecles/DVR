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
    core/src/base.cpp \
    core/src/engine.cpp \
    core/src/log.cpp \
    core/src/main.cpp \
    core/src/threadManager.cpp \
    fsm/src/fsm.cpp \
    gstpipeline/src/videomonitor.cpp \
    gstpipeline/src/videomonitorgst.cpp \
    core/src/cJSON.c \
    fsm/src/fsmApi.cpp \
    socket/src/pluginSocket.cpp \
    socket/src/pluginSocketApi.cpp


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
    core/include/base.h \
    core/include/cJSON.h \
    core/include/commonType.h \
    core/include/engine.h \
    core/include/log.h \
    core/include/message.h \
    core/include/threadManager.h \
    fsm/include/fsm.h \
    gstpipeline/include/videomonitor.h \
    socket/include/pluginSocket.h


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
