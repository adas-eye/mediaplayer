#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include "playcontrolwidget.h"
#include "mytoolbutton.h"

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = nullptr);

signals:

};

#endif // MYWIDGET_H
