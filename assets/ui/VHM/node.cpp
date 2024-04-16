
// Node.cpp
#include "node.h"
#include <QPainter>

Node::Node(QWidget *parent) : QWidget(parent) {
    setFixedSize(20, 20); // Adjust size as needed
}

void Node::setColor(const QColor &color) {
    m_color = color;
    update(); // Trigger repaint
}

void Node::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPen p;
    p.setWidth(10);
    painter.setPen(p);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(m_color);
    painter.drawEllipse(rect());
}
