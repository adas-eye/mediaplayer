#ifndef ALGORITHMCONFIG_H
#define ALGORITHMCONFIG_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class AlgorithmConfig;
}

class AlgorithmConfig : public QWidget
{
    Q_OBJECT

public:
    explicit AlgorithmConfig(QWidget *parent = nullptr);
    ~AlgorithmConfig();

    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private:
    bool m_isNeedHide;
    Ui::AlgorithmConfig *ui;
    QTimer m_timer;
};

#endif // ALGORITHMCONFIG_H
