#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QTimer>
#include <cmath>
#include <iostream>
using std::vector, std::string, std::pair;
#include "../inc/heart_model.hpp"
// 1: Aflutter, 2: EP_AVNRT, 3: Wenckbach NOTE: this is
// overwritten by CMakeLists.txt at line 21 in this commit
#define CASE 1
#include "../inc/data_gen_config.hpp"

int main(int argc, char *argv[]) {
/// QT setup code starts here ///
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);
  /// QT setup code ends here ///

  Heart *heart_model = Heart::getInstance();
  QString color_opt_node[] = {"lime", "red", "yellow"};
  QString color_opt_path[] = {"blue", "lime", "yellow", "black", "red"};

  QVariantList nodeList;
  for (int i = 0; i < Data.node_names.size(); ++i) {
    nodeList.emplace_back(QVariantMap{{"x", Data.node_positions[i].x},
                                      {"y", Data.node_positions[i].y},
                                      {"c", "yellow"}});
  }

  QVariantList pathList;
  for (int i = 0; i < Data.path_names.size(); ++i) {
    const float ent_x =
        Data.node_positions[Data.path_int_parameters[i][1] - 1].x;
    const float ent_y =
        Data.node_positions[Data.path_int_parameters[i][1] - 1].y;
    const float out_x =
        Data.node_positions[Data.path_int_parameters[i][2] - 1].x;
    const float out_y =
        Data.node_positions[Data.path_int_parameters[i][2] - 1].y;
    float degree = 180 / 3.14 * std::atan2(out_y - ent_y, out_x - ent_x);
    // Geometry of the path as the original data for the image and coordiates
    // are flipped around the x-axis
    if (ent_x > out_x) {
      if (ent_y > out_y) {
        degree = std::abs(degree);
      } else {
        degree = -std::abs(degree);
      }
    } else {
      if (ent_y > out_y) {
        degree = std::abs(degree);
      } else {
        degree = 360 - std::abs(degree);
      }
    }
    const float len = std::sqrt(std::pow(out_x - ent_x, 2) +
                                std::pow(out_y - ent_y, 2));
    pathList.emplace_back(QVariantMap{{"x", ent_x},
                                      {"y", ent_y},
                                      {"c", "blue"},
                                      {"l", len},
                                      {"d", degree}});
  }

  QObject *rootObject = engine.rootObjects().first();
  if (rootObject != nullptr) {
    QTimer *timer = new QTimer(rootObject);
    const int delay = 10;
    int index = 0;

    // set pathsNames and nodesNames variables
    QVariantList paths_nodes_names;
    for (int i = 0; i < Data.path_names.size(); ++i) {
      paths_nodes_names.emplace_back(
          QString::fromStdString(Data.path_names[i]));
    }
    rootObject->setProperty("pathsNames",
                            QVariant::fromValue(paths_nodes_names));
    paths_nodes_names.clear();
    for (int i = 0; i < Data.node_names.size(); ++i) {
      paths_nodes_names.emplace_back(
          QString::fromStdString(Data.node_names[i]));
    }
    rootObject->setProperty("nodesNames",
                            QVariant::fromValue(paths_nodes_names));

    QObject::connect(timer, &QTimer::timeout, rootObject, [&] {
      if (index < 10'000'000)  // Data.node_positions.size())
      {
        rootObject->setProperty("timerLabel", index);
        heart_model->heart_automaton();
        for (int i = 0; i < heart_model->getNodeTable().node_table.size();
             ++i) {
          const QVariantMap &originalNode = nodeList.at(i).toMap();
          QVariantMap modifiedElement = originalNode;
          modifiedElement["c"] = color_opt_node[heart_model->getNodeTable()
                                                    .node_table[i]
                                                    .getParameters()
                                                    .node_state_index -
                                                1];
          nodeList.replace(i, modifiedElement);
        }
        for (int i = 0; i < heart_model->getPathTable().path_table.size();
             ++i) {
          const QVariantMap &originalPath = pathList.at(i).toMap();
          QVariantMap modifiedElement = originalPath;
          const int psi = heart_model->getPathTable()
                              .path_table[i]
                              .getParameters()
                              .path_state_index -
                          1;
          modifiedElement["c"] = color_opt_path[psi];
          pathList.replace(i, modifiedElement);
        }
        rootObject->setProperty("pointData", QVariant::fromValue(nodeList));
        rootObject->setProperty("pathData", QVariant::fromValue(pathList));
        index++;
      } else {
        // Stop the timer when all updates are done
        timer->stop();
        std::cout << "END OF 100'000 cycle" << std::endl;
      }
    });
    // Start the timer
    timer->start(delay);
  }

  return app.exec();
}
