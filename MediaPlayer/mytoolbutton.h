#ifndef MYTOOLBUTTON_H
#define MYTOOLBUTTON_H

#include <QToolButton>

class MyToolButton : public QToolButton
{
    Q_OBJECT
public:
    //explicit MyToolButton(QToolButton *parent = nullptr);

    MyToolButton(QString strDefultImg, QString strClickedtImg = "");

signals:

private:
    QString m_strDefultImg;
    QString m_strClickedtImg;

};

#endif // MYTOOLBUTTON_H
