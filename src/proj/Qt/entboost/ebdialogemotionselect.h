#ifndef EBDIALOGEMOTIONSELECT_H
#define EBDIALOGEMOTIONSELECT_H

#include "ebclientapp.h"
#include <ebdialogbase.h>
#include <QListWidgetItem>

namespace Ui {
class EbDialogEmotionSelect;
}

class EbDialogEmotionSelect : public EbDialogBase
{
    Q_OBJECT

public:
    explicit EbDialogEmotionSelect(QWidget *parent = 0);
    ~EbDialogEmotionSelect();

    void updateLocaleInfo(void);
signals:
    void selectedResource(const QIcon& icon,mycp::bigint resourceId,const QString& resourceCmInfo,const QString & resourceFilePath);

public slots:
    void onItemClicked(QListWidgetItem* item);
    void onItemEntered(QListWidgetItem* item);

protected:
    virtual void resizeEvent(QResizeEvent *);
    void checkFocusOut(void);
    virtual void focusOutEvent(QFocusEvent *event);
    virtual bool eventFilter(QObject *obj, QEvent *ev);
    void loadEmotion(void);
    virtual void timerEvent(QTimerEvent * event);

private:
    Ui::EbDialogEmotionSelect *ui;
    int m_timerIdLoadEmotion;
    bool m_hasEmptyFile;
    CLockMap<mycp::bigint,bool> m_loadedResourceIdList;
};

#endif // EBDIALOGEMOTIONSELECT_H
