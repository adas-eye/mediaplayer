#ifndef PLAYCONTROLWIDGET_H
#define PLAYCONTROLWIDGET_H

#include <QWidget>

namespace Ui {
class PlayControlWidget;
}

class PlayControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlayControlWidget(QWidget *parent = nullptr);
    ~PlayControlWidget();

    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

    void show();


private:
    bool m_isNeedHide;
    Ui::PlayControlWidget *ui;
};

#endif // PLAYCONTROLWIDGET_H
