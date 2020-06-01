#ifndef ALGORITHMSETTING_H
#define ALGORITHMSETTING_H

#include <QWidget>

namespace Ui {
class AlgorithmSetting;
}

class AlgorithmSetting : public QWidget
{
    Q_OBJECT

public:
    explicit AlgorithmSetting(QWidget *parent = nullptr);
    ~AlgorithmSetting();

private:
    Ui::AlgorithmSetting *ui;
};

#endif // ALGORITHMSETTING_H
