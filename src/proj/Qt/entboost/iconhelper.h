#ifndef ICONHELPER_H
#define ICONHELPER_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QtGui>
#include <QtWidgets>
#include <qfont.h>

//IconHelper::Instance()->SetIcon(ui->LeftBtn,QChar(0xf177),12);
//IconHelper::Instance()->SetIcon(ui->RightBtn,QChar(0xf178),12);

class IconHelper : public QObject
{
    Q_OBJECT
public:
    static IconHelper* Instance(void);
    void SetIcon(QLabel* ctl, QChar c, int size=10);
    void SetIcon(QPushButton* ctl, QChar c, int size=10);
    void SetIcon( qint64 subId, QLabel* ctl, int size=10 );
    void SetIcon( qint64 subId, QPushButton* ctl, int size=10 );
    QChar GetIcon( qint64 subId ) const;

signals:

public slots:

private:
    explicit IconHelper(QObject *parent = 0);
    QFont iconFont;
    static IconHelper* _instance;
};

#endif // ICONHELPER_H
