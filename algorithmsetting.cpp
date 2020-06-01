#include "algorithmsetting.h"
#include "ui_algorithmsetting.h"

AlgorithmSetting::AlgorithmSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlgorithmSetting)
{
    ui->setupUi(this);
}

AlgorithmSetting::~AlgorithmSetting()
{
    delete ui;
}
