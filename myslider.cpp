#include "myslider.h"
#include "Singleton.h"
#include "mediaplay.h"
#include <QMouseEvent>
#include <QDebug>

MySlider::MySlider(QWidget *parent, Qt::Orientation orientation) : QSlider(orientation, parent)
      ,m_changeable(false)
{
    //this->setRange(0, 1000);

    std::shared_ptr<MediaPlay> media = Singleton<MediaPlay>::GetInstance();


    connect(&m_Timer, &QTimer::timeout, [=](){
        if (media->GetCurrentState() == STOPPED)
        {
            if (this->isEnabled())
            {
                this->setEnabled(false);
            }
            return ;
        }
        if (!this->isEnabled())
        {
            this->setEnabled(true);
        }
        this->setValue(int(media->GetCurrentFrame()));
    });

    connect(this, &MySlider::valueChanged, [=](){
        if (/*m_changeable && */media->GetCurrentState() != STOPPED)
        {
            media->SetPlayPosition(this->value());
        }
    });

    m_Timer.start(1000);
}

void MySlider::mousePressEvent(QMouseEvent *ev)
{
    if (ev->type()==QEvent::MouseButtonPress)     //≈–∂œ¿‡–Õ
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(ev);
        if (mouseEvent->button() == Qt::LeftButton)	//≈–∂œ◊Ûº¸
        {
            int dur = this->maximum() - this->minimum();
            int pos = this->minimum() + dur * ((double)mouseEvent->x() / this->width());
            qDebug() << mouseEvent->x() << " " << this->width()<< " " << pos << dur;
            if(pos != this->sliderPosition())
            {

                std::shared_ptr<MediaPlay> media = Singleton<MediaPlay>::GetInstance();
                media->Pause();
                this->setValue(pos);
            }
        }
    }
    QSlider::mousePressEvent(ev);
}

void MySlider::mouseReleaseEvent(QMouseEvent *ev)
{
    std::shared_ptr<MediaPlay> media = Singleton<MediaPlay>::GetInstance();
    media->Play();
    QSlider::mouseReleaseEvent(ev);
}
