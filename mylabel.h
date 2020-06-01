#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include "playcontrolwidget.h"
#include "mytoolbutton.h"
#include "algorithmconfig.h"

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = nullptr);


    void resizeEvent(QResizeEvent *event);
protected:

    void mouseMoveEvent(QMouseEvent *event);

signals:


private:
    AlgorithmConfig* m_pAlgorithmConfig;

    PlayControlWidget * m_pPlayControl;

    MyToolButton* m_pOpenFileBtn;
};

#endif // MYLABEL_H
