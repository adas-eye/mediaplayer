#include "mylabel.h"
#include "Singleton.h"
#include "mediaplay.h"
#include <QTimer>
#include <QMouseEvent>


MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
  , m_pAlgorithmConfig(new AlgorithmConfig(this))
{
    setMouseTracking(true);

    setAutoFillBackground(true);

    m_pAlgorithmConfig->hide();

    //设置画面拉伸
    setScaledContents(true);

    QPalette palette;

    palette.setColor(QPalette::Background, QColor(0,0,0));

    this->setPalette(palette);

    /*********************播放控制界面处理**********************/
    m_pPlayControl = new PlayControlWidget(this);

    //默认隐藏
    m_pPlayControl->hide();

    //初始化播放类，设置显示label
    std::shared_ptr<MediaPlay> media = Singleton<MediaPlay>::GetInstance();

    media->SetShowLabel(this);

}

void MyLabel::resizeEvent(QResizeEvent *event)
{
    m_pPlayControl->setGeometry(0, this->height() - m_pPlayControl->height(), this->width(), m_pPlayControl->height());
    m_pAlgorithmConfig->setGeometry(0, 0, this->width(), m_pAlgorithmConfig->height());

    QLabel::resizeEvent(event);
}

void MyLabel::mouseMoveEvent(QMouseEvent *event)
{
    if (m_pPlayControl->isHidden())
    {
        m_pPlayControl->show();
    }
    if (event->y() < (m_pAlgorithmConfig->height() + 5))
    {
        if (m_pAlgorithmConfig->isHidden())
        {
            m_pAlgorithmConfig->show();
        }
    }
    QLabel::mouseMoveEvent(event);
}

