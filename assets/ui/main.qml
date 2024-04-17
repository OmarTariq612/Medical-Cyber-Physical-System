import QtQuick 2.15
import QtQuick.Window 2.15


Window {
    title: "Heart Model Simulation"
    id:root
    visible: true;
    width: 582; height: 582
    minimumHeight: 582
    maximumHeight: 582
    minimumWidth: 582
    maximumWidth: 582
    property int timerLabel: 0
    // Define a vector of x, y values
    property var pointData: [
            {"x":135.5, "y":295.5, "c":"red"},
    ]
    property var pathData: [
            {"x":135.5, "y":295.5, "d":66, "l":56.8594759033180, "c":"red"},
    ]
    property var nodeStateColor: [{"rest":"lime"},{"ERP":"red"},{"RRP":"yellow"}]
    property var pathStateColor: [{"Idle":"mediumblue"},
        {"Antegrade":"lime"},
        {"Retrograde":"yellow"},
        {"Conflict":"black"},
        {"Double":"red"}]
    Rectangle {
        width:root.width -50 ; height: root.height-50
        x:25;y:25
        color: "#00a3fc"
        Image {
            // x:root.width - 51;
            y:root.height - 51;
            x:1
            // y:1;
            width: root.width-52
            height: root.height-52
            // fillMode: Image.PreserveAspectFit
            source: "../img/EP.JPG"
            // scale: 0.5
            transform: Scale{ yScale: -1}
        }
        // legend
        Rectangle{
            width:90
            height: 160
            x: parent.width-95
            y: 25
            radius: 2
            color: "transparent" // Set the fill color to transparent
            border.color: "#00a3fc" // Set the border color
            Text {
                text: "Automata States"
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                x: 0
                y: -20
                font.pixelSize: 10
                color: "black"
            }
            Repeater {
                model: nodeStateColor
                Rectangle {
                    width: 10
                    height: 10
                    color: Object.values(modelData)[0]
                    x: 10
                    y: index * 20 + 5
                    radius: 5
                    Text {
                        text: Object.keys(modelData)[0]
                        font.bold: true
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        x: 20
                        y: -3
                        font.pixelSize: 10
                        color: "black"
                    }
                }
            }
            Repeater {
                model: pathStateColor
                Rectangle {
                    width: 20
                    height: 5
                    color: Object.values(modelData)[0]
                    x: 5
                    y: (3 + index) * 20 + 5
                    radius: 2
                    Text {
                        text: Object.keys(modelData)[0]
                        font.bold: true
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        x: 25
                        y: -5
                        font.pixelSize:10
                        color: "black"
                    }
                }
            }
        }


        // POINTS
        Repeater {
            Rectangle {
                width: 12
                height: 12
                color: modelData.c
                x: modelData.x - 2 // Adjust position to center the point
                y: root.height - modelData.y - 54 // Adjust position to center the point
                z:2
                radius: 6
            }
            model: pointData
        }
        // LINES
        Repeater {
            Rectangle {
                width: modelData.l
                height: 4
                color: modelData.c
                x: modelData.x + 2
                y: root.height - modelData.y - 46
                z:1
                radius: 3
                transform: Rotation{origin.x:0; origin.y:0; angle:modelData.d}
            }
            model: pathData
        }
        Text {
            text: timerLabel
            x: 10
            font.pixelSize: 12
            color: "black"
            y: root.height - 75
            width: 50
            height: 20
            visible: true
        }
    }

}
