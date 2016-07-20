import QtQuick 2.4
import QtQuick.Controls 1.3
import QtWebView 1.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.2

ApplicationWindow {

    property bool showProgress: webView.loading
                                && Qt.platform.os !== "ios"
                                && Qt.platform.os !== "winphone"
                                && Qt.platform.os !== "winrt"

    property bool showDownloadList : false
    property bool showAppList : false
    property string pkgLocalUri : "/mnt/sdcard/Download/"
    property var pkgName : ""
    property int pkgDownloadNum : 0

    visible: true
    x: initialX
    y: initialY
    width: initialWidth
    height: initialHeight
    title: webView.title

    ExclusiveGroup { id: exclusive }

    toolBar: ToolBar {
        id: navigationBar
        RowLayout {
            anchors.fill: parent
            spacing: 0

            ToolButton {
                id: backButton
                tooltip: qsTr("Back")
                iconSource: "images/left-32.png"
                onClicked: webView.goBack()
                enabled: webView.canGoBack
                Layout.preferredWidth: navigationBar.height
                style: ButtonStyle {
                    background: Rectangle { color: "transparent" }
                }
            }

            ToolButton {
                id: forwardButton
                tooltip: qsTr("Forward")
                iconSource: "images/right-32.png"
                onClicked: webView.goForward()
                enabled: webView.canGoForward
                Layout.preferredWidth: navigationBar.height
                style: ButtonStyle {
                    background: Rectangle { color: "transparent" }
                }
            }

            ToolButton {
                id: reloadButton
                tooltip: webView.loading ? qsTr("Stop"): qsTr("Refresh")
                iconSource: webView.loading ? "images/stop-32.png" : "images/refresh-32.png"
                onClicked: webView.loading ? webView.stop() : webView.reload()
                Layout.preferredWidth: navigationBar.height
                style: ButtonStyle {
                    background: Rectangle { color: "transparent" }
                }
            }

            Item { Layout.preferredWidth: 5 }

            TextField {
                Layout.fillWidth: true
                id: urlField
                inputMethodHints: Qt.ImhUrlCharactersOnly | Qt.ImhPreferLowercase
                text: webView.url


                onAccepted: {

                    if((text.lastIndexOf(".apk")===text.length-4) || (text.lastIndexOf(".APK")===text.length-4) ){

                        text = "http://dl.360safe.com/yunpan/360yunpan_android_6.4.7.apk"
                        console.info("In TextField text = "+text)

                        pkgName = text.substring(text.lastIndexOf("/") + 1, text.length)
                        pkgDownloadNum = pkgDownloadNum +1
                        console.info("In TextField pkgDownloadNum = "+pkgDownloadNum)
                        DownloadManager.startDownload(text)
                        webView.goBack()
                    }else{

                        webView.url = utils.fromUserInput(text)
                    }

                }

                ProgressBar {
                    anchors.centerIn: parent
                    style: LoadProgressStyle { }
                    z: Qt.platform.os === "android" ? -1 : 1
                    visible: showProgress
                    minimumValue: 0
                    maximumValue: 100
                    value: webView.loadProgress > 100 ? 0 : webView.loadProgress
                }
            }

            Item { Layout.preferredWidth: 5 }

            ToolButton {
                id: goButton
                text: qsTr("Go")
                Layout.preferredWidth: navigationBar.height
                onClicked: {
                    Qt.inputMethod.commit()
                    Qt.inputMethod.hide()
                    if((urlField.text.lastIndexOf(".apk")===urlField.text.length-4) || (urlField.text.lastIndexOf(".APK")===urlField.text.length-4) ){

                        urlField.text = "http://dl.360safe.com/yunpan/360yunpan_android_6.4.7.apk"

                        //pkgNames.push(urlField.text.substring(urlField.text.lastIndexOf("/") + 1, urlField.text.length))
                        pkgName = (urlField.text.substring(urlField.text.lastIndexOf("/") + 1, urlField.text.length))

                        pkgDownloadNum = pkgDownloadNum +1
                        DownloadManager.startDownload(urlField.text)
                        webView.goBack()

                    }else{

                        webView.url = utils.fromUserInput(urlField.text)
                    }

                }
                style: ButtonStyle {
                    background: Rectangle { color: "transparent" }
                }
            }
            Item { Layout.preferredWidth: 5 }
            ToolButton {
                id: menuPage
                text: qsTr("Menu")
                Layout.preferredWidth: navigationBar.height
                exclusiveGroup: exclusive
                checked: true
                onClicked: {
                    webwiewRec.visible = true
                    appArray.visible = false
                    downloadList.visible = false

                }
                style: ButtonStyle {
                    background: Rectangle { color: "transparent" }
                }
            }

            Item { Layout.preferredWidth: 5 }
            ToolButton {
                id: applist
                text: qsTr("APP")
                Layout.preferredWidth: navigationBar.height
                onClicked: {
                    webwiewRec.visible = false
                    appArray.visible = true
                    downloadList.visible = false
                }
                style: ButtonStyle {
                    background: Rectangle { color: "transparent" }
                }
            }

            Item { Layout.preferredWidth: 5 }
            ToolButton {
                id: downloadButton
                exclusiveGroup: exclusive
                text: qsTr("DL")
                Layout.preferredWidth: navigationBar.height
                onClicked: {
                    webwiewRec.visible = false
                    appArray.visible = false
                    downloadList.visible = true
                }
                style: ButtonStyle {
                    background: Rectangle { color: "transparent" }
                }
            }
            Item { Layout.preferredWidth: 10 }

        }
    }
    statusBar: StatusBar {
        id: statusBar
        visible: showProgress
        RowLayout {
            anchors.fill: parent
            Label { text: webView.loadProgress == 100 ? qsTr("Done") : qsTr("Loading: ") + webView.loadProgress + "%" }
        }
    }
    Rectangle{
        id : webwiewRec
        visible: true
        width :  parent.width
        height : parent.height
        color: "transparent"
        WebView {
            id: webView
            anchors.fill: parent
            url: initialUrl
        }
    }
    Rectangle{
        id : appArray
        visible: (showAppList || showDownloadList )? false :true
        width :  parent.width
        height : parent.height
        color: "#d3d3d3"

        Button {
             id: getUserApps
             x: 10
             y: 10
             width: 500
             height: 100
             text: qsTr("getUserApps")
             onClicked:
             {
                 PackageManager.getUserApplications();
             }
         }
        Rectangle{
            x: 550
            y: 110
            width: 1000
            height: 2000
            color: "transparent"
            ListView{
                anchors.fill: parent
                clip: true
                model  : 1

                delegate:
                Grid{
                    id: appArrayView
                    anchors.fill: parent
                    spacing: 40
                    columns: 4
                    Repeater {
                    model: 30

                    delegate:
                       Rectangle {
                           width: 200
                           height: 200
                           color: "transparent"
                           Image {
                               id : appIcon
                               anchors.top: parent.top
                               width: 100
                               height: 120
                               source: "qrc:/qtquickplugin/images/template_image.png"

                               Connections
                                {
                                    target:PackageManager
                                    onPackageInfoListUpdated: {
                                        appIcon.source = "image://AppIconProvider/"+index
                                    }
                                }
                           }

                           Label {
                               anchors.bottom:  parent.bottom
                               width: 100
                               height: 75
                               text: PackageManager.getApplicationName(index)
                           }

                           MouseArea {
                                    anchors.fill: parent
                                    onClicked: { PackageManager.launchApp(index)
                                    }
                           }

                       }
                    }
                }
            }
        }
    }
    Rectangle {
        id : downloadList
        visible: (showAppList || showDownloadList )? false : true
        width : parent.width
        height : parent.height
        color: "#d3d3d3"

        ListView {
            anchors.fill: parent
            anchors.margins: 20

            clip: true

            model: 10

            delegate: numberDelegate
            spacing: 10
        }

        Component {
            id: numberDelegate
            Rectangle{
                width: parent.width
                height: 150
                color : "green"
                visible: (pkgDownloadNum > index) ? true : false
                Button{
                    id : apk
                    text :  qsTr(pkgName)
                    anchors.fill: parent
                    onClicked:
                    {   
                        //判断该下载项是否下载完成，下载完成可进行apk的安装，否则Install Button无效
                        if(1){
                            //console.info("Oh no +++++"+pkgName)
                            PackageManager.install((pkgLocalUri+pkgName));
                        }
                    }
                }
            }
        }
    }
}
