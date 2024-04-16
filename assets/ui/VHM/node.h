// Node.h
#ifndef NODE_H
#define NODE_H

#include <QWidget>

class Node : public QWidget {
    Q_OBJECT
public:
    explicit Node(QWidget *parent = nullptr);
    void setColor(const QColor &color = Qt::green);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QColor m_color;
};

#endif // NODE_H
