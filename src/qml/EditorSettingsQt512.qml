import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import nuttyartist.notes 1.0
//import notes

Item {
    id: settingsContainer
    property int extraWidthForQWidgets: platform === "Apple" ? 60 : 0
    property int extraHeightForQWidgets: platform === "Apple" ? 40 : 0
    width: settingsPane.width + extraWidthForQWidgets
    height: settingsPane.height + extraHeightForQWidgets
    property var themeData: {{theme: "Light"}}
    property string displayFontFamily: "Roboto"
    property string platform: ""
    property string categoriesFontColor: settingsContainer.themeData.theme === "Dark"  ? "#868686" : "#7d7c78"
    property string mainFontColor: settingsContainer.themeData.theme === "Dark" ? "#d6d6d6" : "#37352e"
    property string highlightBackgroundColor: settingsContainer.themeData.theme === "Dark" ? "#313131" : "#efefef"
    property string pressedBackgroundColor: settingsContainer.themeData.theme === "Dark" ? "#2c2c2c" : "#dfdfde"
    property string separatorLineColors: settingsContainer.themeData.theme === "Dark" ? "#3a3a3a" : "#ededec"
    property string mainBackgroundColor: settingsContainer.themeData.theme === "Dark" ? "#252525" : "white"
        signal themeChanged
    property int paddingRightLeft: 12
    property var listOfSansSerifFonts: []
    property var listOfSerifFonts: []
    property var listOfMonoFonts: []
    property int chosenSansSerifFontIndex: 0
    property int chosenSerifFontIndex: 0
    property int chosenMonoFontIndex: 0
    property string currentFontTypeface
    property real latestScrollBarPosition: 0.0
    property int pointSizeOffset: -4
    property int qtVersion: 6
    property string currentlySelectedView: "TextView" // "KanbanView"
    property bool isProVersion: false

    Connections {
        target: mainWindow

        function onEditorSettingsShowed (data) {
            var settingsPaneHeightByParentWindow = 0.80 * data.parentWindowHeight; // 80 percent of the parent window's height
            settingsPane.height = scrollViewControl.contentHeight > settingsPaneHeightByParentWindow ? settingsPaneHeightByParentWindow : scrollViewControl.contentHeight;
            revealSettingsAnimation.start();
            settingsContainer.upadteScrollBarPosition();
        }

        function onMainWindowResized (data) {
            var settingsPaneHeightByParentWindow = 0.80 * data.parentWindowHeight; // 80 percent of the parent window's height
            settingsPane.height = scrollViewControl.contentHeight > settingsPaneHeightByParentWindow ? settingsPaneHeightByParentWindow : scrollViewControl.contentHeight;
        }

        function onDisplayFontSet (data) {
            settingsContainer.displayFontFamily = data.displayFont;
        }

        function onPlatformSet (data) {
            settingsContainer.platform = data;
        }

        function onQtVersionSet (data) {
            settingsContainer.qtVersion = data;
        }

        function onThemeChanged (data) {
            settingsContainer.themeData = data;
            themeChanged();
        }

        function onEditorSettingsScrollBarPositionChanged (data) {
            settingsContainer.latestScrollBarPosition = data;
        }

        function onSettingsChanged (data) {
            if (data.currentTheme === "Light") {
                lightThemeChooserButton.themeSelected(true);
                darkThemeChooserButton.themeSelected(false);
                sepiaThemeChooserButton.themeSelected(false);
            } else if (data.currentTheme === "Dark") {
                darkThemeChooserButton.themeSelected(true);
                sepiaThemeChooserButton.themeSelected(false);
                lightThemeChooserButton.themeSelected(false);
            }
        }

        function onFontsChanged (data) {
            settingsContainer.listOfSansSerifFonts = data.listOfSansSerifFonts;
            settingsContainer.chosenSansSerifFontIndex = data.chosenSansSerifFontIndex;
            settingsContainer.currentFontTypeface = data.currentFontTypeface;
        }

        function onProVersionCheck (data) {
            settingsContainer.isProVersion = data;
        }
    }

    // Connections {
    //     target: noteEditorLogic
    //
    //     function onTextShown () {
    //         textView.viewSelected();
    //         kanbanView.viewUnclicked();
    //     }
    //
    //     function onKanbanShown () {
    //         kanbanView.viewSelected();
    //         textView.viewUnclicked();
    //     }
    // }

    FontIconLoader {
        id: fontIconLoader
    }

    PropertyAnimation {
        id: revealSettingsAnimation
        target: settingsContainer
        property: "opacity"
        from: 0.3
        to: 1.0
        duration: 100
        easing.type: Easing.InOutQuad
    }

    function upadteScrollBarPosition () {
        editorSettingsVerticalScrollBar.position = settingsContainer.latestScrollBarPosition;
    }

    Pane {
        id: settingsPane
        visible: true
        padding: 0
        width: 240
        height: 175
        contentWidth: 240
        Material.elevation: 10
        Material.background: settingsContainer.mainBackgroundColor
        x: settingsContainer.extraWidthForQWidgets === 0 ? 0 : 20
        y: settingsContainer.extraHeightForQWidgets === 0 ? 0 : 20

        Rectangle {
            visible: settingsContainer.qtVersion < 6
            anchors.fill: parent
            color: settingsContainer.mainBackgroundColor
        }

        Column {
            Item {
                width: 1
                height: settingsContainer.paddingRightLeft
            }

            Column {
                id: textSettingsGroup
                visible: settingsContainer.currentlySelectedView === "TextView"

                Text {
                    text: qsTr("Text")
                    color: settingsContainer.categoriesFontColor
                    font.pointSize: settingsContainer.platform === "Apple" ? 12 : 12 + settingsContainer.pointSizeOffset
                    font.family: settingsContainer.displayFontFamily
                    x: settingsContainer.paddingRightLeft
                }

                Item {
                    width: 1
                    height: 10
                }

                Row {
                    id: fontSizeRow
                    x: settingsContainer.paddingRightLeft + 10
                    Text {
                        id: fontSizeText
                        x: 20
                        text: qsTr("Font size")
                        color: settingsContainer.mainFontColor
                        font.pointSize: settingsContainer.platform === "Apple" ? 14 : 14 + settingsContainer.pointSizeOffset
                        font.family: settingsContainer.displayFontFamily
                        opacity: settingsContainer.currentlySelectedView === "TextView" ? 1.0 : 0.2
                    }

                    Item {
                        width: settingsPane.contentWidth - fontSizePlusButton.width - fontSizeText.width - fontSizeMinusButton.width - 20 - fontSizeRow.x
                        height: 1
                    }

                    IconButton {
                        id: fontSizeMinusButton
                        icon: fontIconLoader.icons.fa_minus
                        anchors.verticalCenter: fontSizeText.verticalCenter
                        themeData: settingsContainer.themeData
                        platform: settingsContainer.platform
                        iconPointSizeOffset: -4
                        enabled: settingsContainer.currentlySelectedView === "TextView"

                        onClicked: {
                            mainWindow.changeEditorFontSizeFromStyleButtons(FontSizeAction.FontSizeDecrease);
                        }
                    }

                    Item {
                        width: 10
                        height: 1
                    }

                    IconButton {
                        id: fontSizePlusButton
                        icon: fontIconLoader.icons.fa_plus
                        anchors.verticalCenter: fontSizeText.verticalCenter
                        themeData: settingsContainer.themeData
                        platform: settingsContainer.platform
                        iconPointSizeOffset: -4
                        enabled: settingsContainer.currentlySelectedView === "TextView"

                        onClicked: {
                            mainWindow.changeEditorFontSizeFromStyleButtons(FontSizeAction.FontSizeIncrease);
                        }
                    }
                }

                Rectangle {
                    height: 1
                    width: settingsPane.contentWidth
                    color: settingsContainer.separatorLineColors
                }
            }


            Item {
                width: 1
                height: 10
            }

            Text {
                text: qsTr("Theme")
                x: settingsContainer.paddingRightLeft
                color: settingsContainer.categoriesFontColor
                font.pointSize: settingsContainer.platform === "Apple" ? 12 : 12 + settingsContainer.pointSizeOffset
                font.family: settingsContainer.displayFontFamily
            }

            Item {
                width: 1
                height: 5
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                // x: settingsContainer.paddingRightLeft
                ThemeChooserButton {
                    id: lightThemeChooserButton
                    displayFontFamily: settingsContainer.displayFontFamily
                    platform: settingsContainer.platform
                    mainFontColor: settingsContainer.mainFontColor
                    highlightBackgroundColor: settingsContainer.highlightBackgroundColor
                    pressedBackgroundColor: settingsContainer.pressedBackgroundColor
                    themeColor: "#f7f7f7"
                    themeName: "Light"
                    themeData: settingsContainer.themeData
                    qtVersion: settingsContainer.qtVersion

                    onClicked: {
                        mainWindow.setTheme(Theme.Light);
                        darkThemeChooserButton.unclicked();
                        sepiaThemeChooserButton.unclicked();
                    }

                    Connections {
                        target: settingsContainer

                        function onThemeChanged () {
                            lightThemeChooserButton.themeChanged();
                        }
                    }
                }

                ThemeChooserButton {
                    id: darkThemeChooserButton
                    displayFontFamily: settingsContainer.displayFontFamily
                    platform: settingsContainer.platform
                    mainFontColor: settingsContainer.mainFontColor
                    highlightBackgroundColor: settingsContainer.highlightBackgroundColor
                    pressedBackgroundColor: settingsContainer.pressedBackgroundColor
                    themeColor: "#191919"
                    themeName: "Dark"
                    themeData: settingsContainer.themeData
                    qtVersion: settingsContainer.qtVersion

                    onClicked: {
                        mainWindow.setTheme(Theme.Dark);
                        lightThemeChooserButton.unclicked();
                        sepiaThemeChooserButton.unclicked();
                    }

                    Connections {
                        target: settingsContainer

                        function onThemeChanged () {
                            darkThemeChooserButton.themeChanged();
                        }
                    }
                }
            }
        }
    }
}