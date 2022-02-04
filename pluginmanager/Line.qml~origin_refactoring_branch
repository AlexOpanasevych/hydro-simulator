import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 1.5
import QtQuick.Layouts 1.15
import KLib 1.0

RowLayout{
    Item{
        Layout.preferredWidth: 50
        Layout.fillHeight: true
        clip: true
        Text {
            anchors.fill: parent
            text: l_ACTIVE
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        
        MouseArea{
            anchors.fill: parent
            onClicked: {
                if (l_ACTIVE !== "Active")
                    l_ACTIVEF(!l_ACTIVE)
            }
        }
    }
    
    Item{
        Layout.fillWidth: true
        Layout.fillHeight: true
        clip: true
        Text {
            anchors.fill: parent
            text: l_NAME
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

        }
    }
    
    Item{
        Layout.fillWidth: true
        Layout.fillHeight: true
        clip: true
        Text {
            anchors.fill: parent
            text: l_DATE
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }
    
    Item{
        Layout.fillWidth: true
        Layout.fillHeight: true
        clip: true
        Text {
            anchors.fill: parent
            text: l_PATH
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        
    }
    
}
