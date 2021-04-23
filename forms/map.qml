import QtQuick 2.0
import QtPositioning 5.6
import QtLocation 5.6

Item {
    PositionSource {
        active: true
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: Plugin {
            name: "osm"
            PluginParameter {
                name: "osm.mapping.providersrepository.disabled"
                value: "true"
            }
            PluginParameter {
                name: "osm.mapping.providersrepository.address"
                value: "http://maps-redirect.qt.io/osm/5.6/"
            }
        }
        center: getCoordinateFromMaidenheadLocator("JO62QM")
        zoomLevel: 10

    }


    function addQSOMarker(qso){
        var component;
        var sprite;
        component = Qt.createComponent("../forms/marker.qml");
        while(component.status !== Component.Ready){
            console.log("Not Ready!");
        }

        sprite = component.createObject(map, {x: 100, y: 100});
    }

    function getCoordinateFromMaidenheadLocator(locator){
        locator = locator.toUpperCase();

        let lat = 0, lon = 0;
        let x = locator[0].charCodeAt(0) - 'A'.charCodeAt(0);
        let y = locator[1].charCodeAt(0) - 'A'.charCodeAt(0);

        lon += ((360/18) * x) - 180;    // 360 / (A-R)
        lat += ((180/18) * y) - 90;     // 180 / (A-R)

        x = locator[2].charCodeAt(0) - '0'.charCodeAt(0);
        y = locator[3].charCodeAt(0) - '0'.charCodeAt(0);

        lon += (20 / 10) * x; // +20 / (0-9)
        lat += (10 / 10) * y; // +10 / (0-9)

        x = locator[4].charCodeAt(0) - 'A'.charCodeAt(0);
        y = locator[5].charCodeAt(0) - 'A'.charCodeAt(0);

        lon += (2 / 24) * x;
        lat += (1 / 24) * y;

        return QtPositioning.coordinate(lat, lon);
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
