import QtQuick 2.12
import QtQuick.Controls 2.12

MouseArea {
    id: viewMouseArea
    width: backgroundRectangle.width
    height: backgroundRectangle.height
    hoverEnabled: true
    property bool isExited: false
    // enum ViewType {
    //     Text,
    //     Kanban
    // }
    // property int currentViewType: ViewChooserButton.ViewType.Text
    signal viewClicked
    signal viewUnclicked
    property string displayFontFamily: "Roboto"
    property string platform: ""
    property string mainFontColor
    property string highlightBackgroundColor
    property string pressedBackgroundColor
    property bool checked: false
    property int pointSizeOffset: -4

    Rectangle {
        id: backgroundRectangle
        width: 107
        height: 80
        color: "transparent"
        radius: 3

        Column {
            anchors.centerIn: parent

            Item {
                id: viewContainer
                Item {
                    id: textViewContainer
                    // visible: viewMouseArea.currentViewType === ViewChooserButton.ViewType.Text
                    width: paperTextRect.width
                    height: paperTextRect.height

                    // Component.onCompleted: {
                    //     if (viewMouseArea.currentViewType === ViewChooserButton.ViewType.Text) {
                    //         viewContainer.width = textViewContainer.width;
                    //         viewContainer.height = textViewContainer.height;
                    //     }
                    // }

                    function updateTextViewContainerColors () {
                        paperTextRect.border.color = viewMouseArea.mainFontColor;
                        paperTextRect2.color = viewMouseArea.mainFontColor;
                        paperTextRect3.color = viewMouseArea.mainFontColor;
                        paperTextRect4.color = viewMouseArea.mainFontColor;
                        paperTextRect5.color = viewMouseArea.mainFontColor;
                        themeText.color = viewMouseArea.mainFontColor;
                    }
                    //
                    // Connections {
                    //     target: viewMouseArea
                    //     function onViewUnclicked () {
                    //         viewMouseArea.checked = false;
                    //         textViewContainer.updateTextViewContainerColors();
                    //     }
                    //     function onThemeChanged () {
                    //         if (!viewMouseArea.checked)
                    //             textViewContainer.updateTextViewContainerColors();
                    //     }
                    // }
                    //
                    // Rectangle {
                    //     id: paperTextRect
                    //     width: 20
                    //     height: 32
                    //     border.width: 1
                    //     radius: 5
                    //     border.color: viewMouseArea.mainFontColor
                    //     color: "transparent"
                    //
                    //     Rectangle {
                    //         id: paperTextRect2
                    //         x: 5
                    //         y: 10
                    //         width: parent.width - 10
                    //         height: 1
                    //         color: viewMouseArea.mainFontColor
                    //     }
                    //
                    //     Rectangle {
                    //         id: paperTextRect3
                    //         x: 5
                    //         y: 14
                    //         width: parent.width - 10
                    //         height: 1
                    //         color: viewMouseArea.mainFontColor
                    //     }
                    //
                    //     Rectangle {
                    //         id: paperTextRect4
                    //         x: 5
                    //         y: 18
                    //         width: parent.width - 10
                    //         height: 1
                    //         color: viewMouseArea.mainFontColor
                    //     }
                    //
                    //     Rectangle {
                    //         id: paperTextRect5
                    //         x: 5
                    //         y: 22
                    //         width: parent.width - 10
                    //         height: 1
                    //         color: viewMouseArea.mainFontColor
                    //     }
                    // }
                }
            }

        }
    }
}
