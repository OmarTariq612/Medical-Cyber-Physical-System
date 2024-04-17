#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTimer>
#include <QObject>
#include <iostream>
#include <cmath>
using std::vector, std::string, std::pair;
#include "../inc/heart_model.hpp"
#include "../inc/data.hpp"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    Heart *heart_model = Heart::getInstance();
    QString color_opt_node[]={"lime","red","yellow"};
    QString color_opt_path[]={"blue","lime","yellow","black","red"};

    QVariantList nodeList;
    for (int i = 0; i < Data.node_names.size(); ++i)
        nodeList.emplace_back(QVariantMap{{"x", Data.node_positions[i].x},{"y", Data.node_positions[i].y},{"c", "yellow"}});

    QVariantList pathList;
    for (int i = 0; i < Data.path_names.size(); ++i) {
        float degree = 180.0/3.14*std::atan2(Data.path_float_parameters[i][7], -1);
        float ent_x = Data.node_positions[Data.path_int_parameters[i][1]-1].x;
        float ent_y = Data.node_positions[Data.path_int_parameters[i][1]-1].y;
        float out_x = Data.node_positions[Data.path_int_parameters[i][2]-1].x;
        float out_y = Data.node_positions[Data.path_int_parameters[i][2]-1].y;
        if(ent_x <= out_x) {ent_x = out_x; ent_y = out_y;}
        pathList.emplace_back(QVariantMap{{"x", ent_x},{"y", ent_y},{"c", "blue"},
                                          {"l", Data.path_float_parameters[i][6]},
                                          {"d", degree}});
    }


    QObject *rootObject = engine.rootObjects().first();
    if (rootObject) {
        QTimer *timer = new QTimer(rootObject);
        int delay = 10;
        int index = 0;

        QObject::connect(timer, &QTimer::timeout, rootObject, [&]{
            if (index < 1'000'000)//Data.node_positions.size())
            {
                rootObject->setProperty("timerLabel", index);
                heart_model->heart_automaton();
                for (int i = 0; i < heart_model->getNodeTable().node_table.size(); ++i) {
                    const QVariantMap& originalNode = nodeList.at(i).toMap();
                    QVariantMap modifiedElement = originalNode;
                    modifiedElement["c"] = color_opt_node[heart_model->getNodeTable().node_table[i].getParameters().node_state_index - 1];
                    nodeList.replace(i, modifiedElement);
                }
                for (int i = 0; i < heart_model->getPathTable().path_table.size(); ++i) {
                    const QVariantMap& originalPath = pathList.at(i).toMap();
                    QVariantMap modifiedElement = originalPath;
                    int psi = heart_model->getPathTable().path_table[i].getParameters().path_state_index - 1;
                    // std::cout << psi << std::endl;
                    modifiedElement["c"] = color_opt_path[psi];
                    pathList.replace(i, modifiedElement);
                }
                rootObject->setProperty("pointData", QVariant::fromValue(nodeList));
                rootObject->setProperty("pathData", QVariant::fromValue(pathList));
                index++;
            }
            else {
                // Stop the timer when all updates are done
                timer->stop();
                std::cout << "Else" << std::endl;
            }
        });

        // Start the timer
        timer->start(delay);

    }

    return app.exec();
}
