#include "playcontrolwidget.h"
#include "ui_playcontrolwidget.h"

PlayControlWidget::PlayControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayControlWidget)
{
    ui->setupUi(this);

    ui->btnPlayPause->setStyleSheet("background-color:transparent");
    ui->btnPlayPause->setFocusPolicy(Qt::NoFocus);

    ui->btnPrevious->setStyleSheet("background-color:transparent");
    ui->btnPrevious->setFocusPolicy(Qt::NoFocus);

    ui->btnNext->setStyleSheet("background-color:transparent");
    ui->btnNext->setFocusPolicy(Qt::NoFocus);
}

PlayControlWidget::~PlayControlWidget()
{
    delete ui;
}
