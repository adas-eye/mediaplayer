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


    /*********************���ſ��ƽ��洦��**********************/
    m_pPlayControl = new PlayControlWidget(this);

    //Ĭ������
    m_pPlayControl->hide();

    /*********************���ļ���ť�����Ժ͹��ܴ���**********************/
    m_pOpenFileBtn = new MyToolButton(":/res/OpenFileLogo.png");

    m_pOpenFileBtn->setParent(this);

    //ֻ��ʾͼƬ
    m_pOpenFileBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);

    //ȥ����ť�߿�
    m_pOpenFileBtn->setStyleSheet("border:none;");

    //��������ļ�����
    connect(m_pOpenFileBtn, &MyToolButton::clicked, [=](){
        QStringList fileNames= QFileDialog::getOpenFileNames(this, tr("���ļ�"), "D:/", tr("��Ƶ�ļ�(*.avi *.mp4)"));
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
