import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons
import Librum.appInformation


Item
{
    id: root
    
    ColumnLayout
    {
        id: layout        
        width: parent.width
        spacing: 0
        
        
        MTitle
        {
            id: title
            Layout.topMargin: 64
            titleText: "Updates"
            descriptionText: "Any new update?"
            titleSize: 25
            descriptionSize: 13.25
        }
        
        Pane
        {
            id: container
            Layout.fillWidth: true
            Layout.topMargin: 32
            horizontalPadding: 40
            topPadding: 32
            bottomPadding: 42
            background: Rectangle
            {
                color: Style.colorBackground
                border.color: Style.colorLightBorder
                radius: 4
                antialiasing: true
            }
            
            
            ColumnLayout
            {
                id: inDetailsLayout
                width: parent.width
                spacing: 0
                
                Label
                {
                    id: newUpdateTitle
                    Layout.fillWidth: true
                    text: "You are up to date!"
                    wrapMode: Text.WordWrap
                    color: Style.colorBaseText
                    font.pointSize: 23
                    font.family: Style.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Label
                {
                    Layout.fillWidth: true
                    Layout.topMargin: 7
                    text: "Make sure to check for updates regularly, so you dont miss out any great features."
                    wrapMode: Text.WordWrap
                    color: Style.colorLightText3
                    font.pointSize: 15
                    font.family: Style.defaultFontFamily
                }
                
                Label
                {
                    Layout.fillWidth: true
                    Layout.topMargin: 26
                    text: "Your current version is:"
                    wrapMode: Text.WordWrap
                    color: Style.colorLightText3
                    font.pointSize: 14.5
                    font.family: Style.defaultFontFamily
                }
                
                Label
                {
                    Layout.fillWidth: true
                    text: AppInformation.newestVersion
                    wrapMode: Text.WordWrap
                    color: Style.colorBasePurple
                    font.pointSize: 14.2
                    font.bold: true
                    font.family: Style.defaultFontFamily
                }
                
                Image
                {
                    id: checkIllustration
                    z: -1
                    Layout.topMargin: 5
                    Layout.leftMargin: -40
                    source: Icons.checkBubbleIllustration
                    sourceSize.width: 320
                    fillMode: Image.PreserveAspectFit
                }
                
                
                Label
                {
                    Layout.fillWidth: true
                    Layout.topMargin: 20
                    text: "See our latest changes at:"
                    wrapMode: Text.WordWrap
                    color: Style.colorLightText3
                    font.pointSize: 13.5
                    font.family: Style.defaultFontFamily
                }
                
                Label
                {
                    Layout.fillWidth: true
                    text: "www.etovex.com/new_updates"
                    wrapMode: Text.WordWrap
                    font.underline: true
                    color: Style.colorBasePurple
                    font.pointSize: 13.5
                    font.family: Style.defaultFontFamily
                }
            }
        }
    }
}