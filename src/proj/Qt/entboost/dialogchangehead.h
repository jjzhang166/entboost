#ifndef DIALOGCHANGEHEAD_H
#define DIALOGCHANGEHEAD_H

#include <EbDialogBase.h>
//#include <QListWidgetItem>

namespace Ui {
class DialogChangeHead;
}

class DialogChangeHead : public EbDialogBase
{
    Q_OBJECT

public:
    explicit DialogChangeHead(QWidget *parent = 0);
    ~DialogChangeHead();

    void updateLocaleInfo(void);
    void setHeadResorceFile(const QString & headResourceFile);

signals:
    void onSelectedImage(qint64 resourceId,const QString & resourceFile);

public slots:
    void onClickedPushButtonDefault(void);
    void onClickedPushButtonCustom(void);
    void updateClickedPushButton(const QObject *sender);
    void onClickedPushButtonOpenFile(void);
    void onClickedListWidgetDefaultHeads(const QModelIndex &index);

protected:
    virtual void resizeEvent(QResizeEvent *);
//    virtual void timerEvent(QTimerEvent *);
    virtual void accept();

    void loadDefaultHeadImage(void);

private:
    Ui::DialogChangeHead *ui;
    QLabel * m_labelDefaultIcon;
    QLabel * m_labelCustomIcon;
//    int m_timerIdLoadImage;
    QString m_sHeadResourceFile;
    QString m_sHeadResourceMd5;
    QString m_sNewHeadFile;
};

#endif // DIALOGCHANGEHEAD_H
