#include "mytoolbutton.h"

//MyToolButton::MyToolButton(QToolButton *parent) : QToolButton(parent)
//{

//}
MyToolButton::MyToolButton(QString strDefultImg, QString strClickedtImg)
{
    this->setFixedSize(90, 90);

    this->m_strDefultImg = strDefultImg;
    this->m_strClickedtImg = strClickedtImg;

    if (!this->m_strDefultImg.isEmpty())
    {
        this->setIcon(QIcon(this->m_strDefultImg));
        this->setIconSize(QSize(this->width(), this->height()));
    }

}
