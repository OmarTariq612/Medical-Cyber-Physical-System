// Line.h
#ifndef LINE_H
#define LINE_H

#include <QWidget>

class Line : public QWidget {
    Q_OBJECT
public:
    explicit Line(QWidget *startNode, QWidget *endNode, QWidget *parent = nullptr);
    void setColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QWidget *m_startNode;
    QWidget *m_endNode;
    QColor m_color;
};

#endif // LINE_H
