import QtQuick 2.0
import QtPositioning 5.6
import QtLocation 5.6

MapQuickItem {
    sourceItem: Text {
        id: cont
        width: 50
        height: 50

        Image {
            id: image

            width: 50
            height: 50
            source: "file:../../forms/marker.png"
            y: 15
        }
        text: "Test"
    }

    anchorPoint.x: cont.width/2
    anchorPoint.y: cont.height
    coordinate: QtPositioning.coordinate(52.4989, 13.3639)
}
