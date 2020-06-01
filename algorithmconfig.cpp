#include "algorithmconfig.h"
#include "ui_algorithmconfig.h"
#include "mediaplay.h"
#include "Singleton.h"

AlgorithmConfig::AlgorithmConfig(QWidget *parent) :
    QWidget(parent),
    m_isNeedHide(true),
    ui(new Ui::AlgorithmConfig)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("background-color: rgb(150,150, 150)");
}

AlgorithmConfig::~AlgorithmConfig()
{
    delete ui;
}

void AlgorithmConfig::enterEvent(QEvent *event)
{
    this->m_isNeedHide = false;
    QWidget::enterEvent(event);
}

void AlgorithmConfig::leaveEvent(QEvent *event)
{
    this->m_isNeedHide = true;
    std::shared_ptr<MediaPlay> media = Singleton<MediaPlay>::GetInstance();
    m_timer.singleShot(2500, this, [=](){
        if (this->m_isNeedHide && !this->isHidden())
        {
            this->hide();
        }
    });

    QWidget::leaveEvent(event);
}

