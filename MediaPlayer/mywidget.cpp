#include "mywidget.h"
#include <QTimer>
#include <QString>
#include <QtDebug>
#include <QEvent>
#include <QMainWindow>
#include <QFileDialog>


MyWidget::MyWidget(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);

    setAutoFillBackground(true);

    QPalette palette;

    palette.setColor(QPalette::Background, QColor(0,0,0));

    //palette.setBrush(QPalette::Background, QBrush(QPixmap(":/background.png")));

    this->setPalette(palette);


    /*********************播放控制界面处理**********************/
    m_pPlayControl = new PlayControlWidget(this);

    //默认隐藏
    m_pPlayControl->hide();

    /*********************打开文件按钮的属性和功能处理**********************/
    m_pOpenFileBtn = new MyToolButton(":/res/OpenFileLogo.png");

    m_pOpenFileBtn->setParent(this);

    //只显示图片
    m_pOpenFileBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);

    //去掉按钮边框
    m_pOpenFileBtn->setStyleSheet("border:none;");

    //单击后打开文件处理
    connect(m_pOpenFileBtn, &MyToolButton::clicked, [=](){
        QStringList fileNames= QFileDialog::getOpenFileNames(this, tr("打开文件"), "D:/", tr("视频文件(*.avi *.mp4)"));
        qDebug() << fileNames;

        if (!fileNames.empty())
        {
            m_pOpenFileBtn->hide();
        }
    });
}

void MyWidget::resizeEvent(QResizeEvent *event)
{
    m_pOpenFileBtn->setGeometry((this->width() - m_pOpenFileBtn->width()) / 2, (this->height() - m_pOpenFileBtn->height()) / 2 , m_pOpenFileBtn->width(), m_pOpenFileBtn->height());
    QWidget::resizeEvent(event);
}

void MyWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_pPlayControl->isHidden())
    {
        m_pPlayControl->setGeometry(0, this->height() - 30, this->width(),30);
        m_pPlayControl->show();

        QTimer::singleShot(3000, this, [=](){
            m_pPlayControl->hide();
        });
    }
    QWidget::mouseMoveEvent(event);
}

void MyWidget::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
}

void MyWidget::leaveEvent(QEvent *event)
{
    if (!m_pPlayControl->isHidden())
    {
        m_pPlayControl->hide();
    }
    QWidget::leaveEvent(event);
}
