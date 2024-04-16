// Line.cpp
#include "line.h"
#include <QPainter>

Line::Line(QWidget *startNode, QWidget *endNode, QWidget *parent) : QWidget(parent),
    m_startNode(startNode), m_endNode(endNode) {}

void Line::setColor(const QColor &color) {
    m_color = color;
    update(); // Trigger repaint
}

void Line::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    // painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(m_color, 20)); // Adjust line width as needed
    // painter.drawLine(m_startNode->mapToGlobal(m_startNode->rect().center()),
                     // m_endNode->mapToGlobal(m_endNode->rect().center()));
    painter.drawLine(100, 0, 0, 100);
}
