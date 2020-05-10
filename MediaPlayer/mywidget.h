#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include "playcontrolwidget.h"
#include "mytoolbutton.h"

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = nullptr);

    void resizeEvent(QResizeEvent *event);
protected:

    void mouseMoveEvent(QMouseEvent *event);

    void enterEvent(QEvent *event);

    void leaveEvent(QEvent *event);

signals:

   // void mouseMove(QMouseEvent *event);

private:
    PlayControlWidget * m_pPlayControl;

    MyToolButton* m_pOpenFileBtn;

};

#endif // MYWIDGET_H
