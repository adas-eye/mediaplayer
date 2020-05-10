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

private:
    Ui::PlayControlWidget *ui;
};

#endif // PLAYCONTROLWIDGET_H
