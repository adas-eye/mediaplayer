#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QSlider>
#include <QTimer>

class MySlider : public QSlider
{
    Q_OBJECT
public:
    explicit MySlider(QWidget *parent = nullptr, Qt::Orientation orientation = Qt::Horizontal);

    void mousePressEvent(QMouseEvent *ev);

    void mouseReleaseEvent(QMouseEvent *ev);

signals:

private:

    bool m_changeable;

    QTimer m_Timer;
};

#endif // MYSLIDER_H
