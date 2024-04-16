#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPixmap>
#include "node.h"
#include "line.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix("/home/abdoe/Data2/heart_project/VHM_Matlab_v1.0/EP.JPG");
    // int height = pix.height();
    // int width  = pix.width();


    QTransform rotation;
    rotation.rotate(180);
    QPixmap rotatedImage = pix.transformed(rotation);
    rotation.scale(1, -1);
    rotatedImage = rotatedImage.transformed(rotation);

    // ui->label_pic->setPixmap(rotatedImage.scaled(530, 530));

    Node n1(parent);
    Node n2(parent);
    n1.setColor(Qt::green);
    n2.setColor(Qt::red);
    n1.move(10, 10);
    n2.move(100, 100);
    Line l1(&n1, &n2, parent);
    l1.show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    // QMainWindow::paintEvent(event);

    QPainter painter(this);

    // Set pen for drawing the line
    QPen linePen(Qt::red); // Red color
    linePen.setWidth(5); // Set line width
    painter.setPen(linePen);

    painter.drawEllipse(rect());
    // Draw a line
    // painter.drawLine(20, 20, 100, 100);

    // QPoint points[] = {QPoint{10,10},QPoint{20,20},QPoint{30,30},QPoint{40,40},QPoint{50,50}};
    // // Draw a point
    // painter.drawPoints(&points);
}
